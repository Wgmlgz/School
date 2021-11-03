#pragma once
#include "Core.hpp"
#include "Items.hpp"
#include "Buildings.hpp"
#include "Messages.hpp"
#include "WJson.hpp"
#include "json.hpp"

using json = nlohmann::json;

json default_settings = json::parse(R"({
  "packages": {
    "n": 4
  },
  "clients": {
    "n": 4,
    "amount": {
      "mean": 4.0,
      "stddev": 2.0
    },
    "type": {
      "base": 10,
      "diff": 10
    },
    "request propability": {
      "mean": 0.5,
      "stddev": 0.2
    }
  },
  "factory": {
    "wait time": {
      "mean": 4,
      "stddev": 1
    }
  },
  "warehouse": {
    "max capacity": 30,
    "threshold": 25
  }
})");

struct Engine {
  std::vector<std::string> packages_by_n;

  std::map<idt, std::shared_ptr<Package>> packages_list_;
  std::map<idt, Building*> buildings_list_;

  Trash trash_;
  Factory factory_;
  Warehouse warehouse_;
  std::vector<Client> clients;

  std::map<idt, std::list<Contract>> contracts_;
  std::map<idt, std::list<Request>> requests_;

  std::function<void(const Request&)> on_push_request = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&)> on_package_created = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&, idt)> on_package_moved = [](auto a, auto b){};

  json warehouse_json;
  json settings;

  Engine(const json& j) : settings(j), factory_(Factory(j)), warehouse_(Warehouse(j)) {
    buildings_list_.insert({trash_.id_, &trash_});
    buildings_list_.insert({factory_.id_, &factory_});
    buildings_list_.insert({warehouse_.id_, &warehouse_});

    auto it = PackageInfo::items.begin();

    for (auto& [key, _] : PackageInfo::items)
      packages_by_n.push_back(key);
    
    std::shuffle(packages_by_n.begin(), packages_by_n.end(), core.rng);
    
    auto n_to_erase = packages_by_n.size() - settings["packages"]["n"].get<int>();
    for (int i = 0; i < n_to_erase; ++i)
      packages_by_n.pop_back();

    for (int i = 0; i < settings["clients"]["n"].get<int>(); ++i) {
      clients.push_back(Client(settings));
    }
    for (auto& i : clients) {
      buildings_list_.insert({i.id_, &i});
    }
  }

  void pushContract(const Contract& contract) {
    wlog("Pushed contract: " +  contract.json());
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
      int amount = self.amount_rng_(core.rng);

      pushRequest(Request(self.id_, warehouse_.id_,
                          PackageInfo::items[package_type], amount, core.day));
    }
  }

  void updateWarehouse(Warehouse& self, std::list<Request>& requests) {
    wlog("Warehouse update");
    warehouse_json.clear();

    warehouse_json["in requests"] = requests;

    /* Remove old packages */
    std::vector<std::shared_ptr<Package>> remove_contract_content;
    for (const auto& package_type : packages_by_n) {
      while (self.shelfs[package_type].size() and
             self.shelfs[package_type].front()->production_time_ +
                     PackageInfo::items[package_type].expiration_time_ <
                 core.day) {
        auto t = self.shelfs[package_type].front();
        self.shelfs[package_type].pop_front();
        --self.virtual_size[package_type];
        remove_contract_content.push_back(t);
      }
    }

    if (remove_contract_content.size()) {
      pushContract(
          Contract(self.id_, trash_.id_, remove_contract_content, core.day));
    }

    /* Process requsts */
    std::vector<Request> new_requests;
    while (requests.size()) {
      auto request = requests.front();
      requests.pop_front();
      
      auto package_type = request.package_.type_;
      if (request.amount_ > self.shelfs[package_type].size()) {
        continue;
      }

      /* Resolve request */
      std::vector<std::shared_ptr<Package>> contract_content;
      for (auto j = 0; j < request.amount_; ++j) {
        auto package = self.shelfs[package_type].front();
        self.shelfs[package_type].pop_front();
        contract_content.push_back(package);
        --self.virtual_size[package_type];
      }
      pushContract(Contract(self.id_, request.from_, contract_content, core.day));
    }


    /**
     * ######.........
     * threshold^
     *       #########
     * buy this^
     *
     */
    for (const auto& package_type : packages_by_n) {
      if (self.virtual_size[package_type] < self.threshold) {

        auto factory_request =
            Request(self.id_, factory_.id_, PackageInfo::items[package_type],
                    self.max_size - self.virtual_size[package_type], core.day);
        new_requests.push_back(factory_request);
        pushRequest(factory_request);
        self.virtual_size[package_type] = self.max_size;
      }
    }
    warehouse_json["out requests"] = new_requests;
  }

  //* DONE
  void updateFactory(Factory& self, std::list<Request>& requests) {
    wlog("Factory update");
    /* Process requests */
    for (auto i = requests.begin(); i != requests.end();) {
      dayt done_day = core.day + std::round(self.wait_rng_(core.rng));

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
    updateWarehouse(warehouse_, requests_[warehouse_.id_]);
    for(auto& client : clients) updateClient(client);
    updateFactory(factory_, requests_[factory_.id_]);
  }

  std::shared_ptr<Package> createPackage(const std::string& type) {
    auto package = std::shared_ptr<Package>(new Package(type, core.day, factory_.id_));
    packages_list_.insert({package->id_, package});
    on_package_created(package);
    return package;
  }
};

Engine engine(default_settings);

