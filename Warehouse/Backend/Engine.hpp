#pragma once
#include "Core.hpp"
#include "Items.hpp"
#include "Buildings.hpp"
#include "Messages.hpp"
#include "WJson.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct Engine {
  int K, M;

  std::vector<std::string> packages_by_n;

  std::map<idt, std::shared_ptr<Package>> packages_list_;
  std::map<idt, Building*> buildings_list_;

  Trash trash_;
  Factory factory_;
  Warehouse warehouse_;
  std::vector<Client> clients;

  std::map<idt, std::list<Contract>> contracts_;
  std::map<idt, std::list<Request>> requests_;

  std::function<void(const Contract&)> on_push_contract = [](auto a){};
  std::function<void(const Request&)> on_push_request = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&)> on_package_created = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&, idt)> on_package_moved = [](auto a, auto b){};

  json warehouse_json;

  json j = {{"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {{"everything", 42}}},
            {"list", {1, 0, 2}},
            {"object", {{"currency", "USD"}, {"value", 42.99}}}};

  Engine(int k, int m)
      : K(k),
       M(m) {
    buildings_list_.insert({trash_.id_, &trash_});
    buildings_list_.insert({factory_.id_, &factory_});
    buildings_list_.insert({warehouse_.id_, &warehouse_});

    auto it = PackageInfo::items.begin();
    for (int i = 0; i < K; ++i) {
      packages_by_n.push_back(it->first);
      ++it;
    }
    for (int i = 0; i < M; ++i) {
      clients.emplace_back(K);
    }
    for (auto& i : clients) {
      buildings_list_.insert({i.id_, &i});
    }
    wlog(j);
  }

  void pushContract(const Contract& contract) {
    wlog("Pushed contract: " +  contract.json());
    on_push_contract(contract);
    for (auto& i : contract.content_) {
      buildings_list_.at(contract.to_)->pushPackage(i);
    }
  }

  void pushRequest(const Request& request) {
    requests_[request.to_].push_back(request);
    on_push_request(request);
  }

  void updateClient(Client& self) {
    wlog("Client update");

    /* request generation */
    if (core.rngd() < self.request_probability_) {
      PackageInfo::items.size();
      std::string package_type = packages_by_n[self.package_rng_(core.rng)];

      pushRequest(Request(self.id_, warehouse_.id_, PackageInfo::items[package_type], 3, core.day));
    }
    if (core.rng() % 5 == 0) pushRequest(Request(self.id_, warehouse_.id_, PackageInfo::items["Milk"], 3, core.day));
    
    /* Apply contacts */
  }

  void updateWarehouse(Warehouse& self, std::list<Request>& requests) {
    wlog("Warehouse update");
    warehouse_json.clear();

    warehouse_json["in requests"] = requests;
    // warehouse_json += range2arr("in requests", requests, [](const Request& req){return req.json();});
    // warehouse_json += ",";

    /* Process requsts */
    std::vector<Request> new_requests;
    // std::vector<Contract> new_requests;

    for (auto i = requests.begin(); i != requests.end();) {
      if (i->amount_ > self.shelfs[i->package_.type_].size()) {
        // TODO remove this
        auto request = Request(self.id_, factory_.id_, i->package_, i->amount_, core.day);
        new_requests.push_back(request);
        pushRequest(request);

        auto t = i;
        ++i;
        requests.erase(t);
        continue;
      }

      std::vector<std::shared_ptr<Package>> contract_content;
      for (auto j = 0; j < i->amount_; ++j) {
        auto package = self.shelfs[i->package_.type_].front();
        self.shelfs[i->package_.type_].pop_front();
        contract_content.push_back(package);
        // self.storage[i->package_.type_].pop_front();
      }
      pushContract(Contract(self.id_, i->from_, contract_content, core.day));
      auto t = i;
      ++i;
      requests.erase(t);
    }

    // TODO threshold stuff

    warehouse_json["out requests"] = new_requests;
    // warehouse_json += range2arr("out requests", new_requests, [](const Request& req){return req.json();});
    // warehouse_json += "}";
    // warehouse_json = warehouse_json;
  }

  //* DONE
  void updateTrash(Trash& self, std::list<Request>& requests) {
    wlog("Trash update");
  }

  //* DONE
  void updateFactory(Factory& self, std::list<Request>& requests) {
    wlog("Factory update");
    /* Process requests */
    for (auto i = requests.begin(); i != requests.end();) {
      dayt done_day = core.day + 2;

      self.request_queue.insert({done_day, *i});

      auto t = i;
      ++i;
      requests.erase(t);
    }
    while (self.request_queue.size() and
           self.request_queue.begin()->first <= core.day) {
      wlog(std::to_string(self.request_queue.begin()->first) + " " + std::to_string(core.day));
      auto request = self.request_queue.begin()->second;
      self.request_queue.erase(self.request_queue.begin());

      std::vector<std::shared_ptr<Package>> contract_content;
      for (int j = 0; j < request.amount_; ++j) {
        contract_content.push_back(createPackage(request.package_.type_));
      }
      pushContract(Contract(self.id_, request.from_, contract_content, core.day));
    }
  }

  // void logPackages() {
  //   wlog("Packages: ");
  //   for (auto& [id, p] : packages_list_) {
  //     wlog("Package from list: " + std::to_string(id) + " " +
  //          p->package_info.type_ + " " +
  //          (&buildings_list_.at(p->house_id_))->name());
  //   }
  // }
  void update() {
    for(auto& client : clients) updateClient(client);
    updateWarehouse(warehouse_, requests_[warehouse_.id_]);
    updateFactory(factory_, requests_[factory_.id_]);
    updateTrash(trash_, requests_[trash_.id_]);
  }

  std::shared_ptr<Package> createPackage(const std::string& type) {
    auto package = std::shared_ptr<Package>(new Package(type, core.day, factory_.id_));
    packages_list_.insert({package->id_, package});
    on_package_created(package);
    return package;
  }
};

Engine engine(6, 4);
