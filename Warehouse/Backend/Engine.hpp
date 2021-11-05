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

/** @brief Controls main simulation */
class Engine {
 public:
  std::vector<std::string> packages_by_n;

  std::map<idt, std::shared_ptr<Package>> packages_list_;
  std::map<idt, Building*> buildings_list_;

  Trash trash_;
  Factory factory_;
  Warehouse warehouse_;
  std::vector<Client> clients;

  // std::map<idt, std::list<Contract>> contracts_;
  std::map<idt, std::list<Request>> requests_;

  std::function<void(const Request&)> on_push_request = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&)> on_package_created = [](auto a){};
  std::function<void(const std::shared_ptr<Package>&, idt)> on_package_moved = [](auto a, auto b){};

  json warehouse_json;
  json settings;

  Engine(const json& j) : settings(j), factory_(Factory(j)), warehouse_(Warehouse(j)) {
    buildings_list_.insert({trash_.id(), &trash_});
    buildings_list_.insert({factory_.id(), &factory_});
    buildings_list_.insert({warehouse_.id(), &warehouse_});

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
      buildings_list_.insert({i.id(), &i});
    }
  }

  void pushOrder(const Order& order) {
    for (auto& i : order.content_) {
      buildings_list_.at(order.to_)->pushPackage(i);
    }
  }

  void pushRequest(const Request& request) {
    requests_[request.to_].push_back(request);
    on_push_request(request);
  }

  void updateClient(Client& self) {
    wlog("Client update");

    /** Request generation */
    if (core.rngd() < self.getRequestProbability()) {
      PackageInfo::items.size();
      std::string package_type = packages_by_n[self.packageRng(core.rng)];
      int amount = self.amountRng(core.rng);

      pushRequest(Request(self.id(), warehouse_.id(),
                          PackageInfo::items[package_type], amount, core.day));
    }
  }

  void updateWarehouse(Warehouse& self, std::list<Request>& requests) {
    wlog("Warehouse update");
    warehouse_json.clear();

    warehouse_json["in requests"] = requests;

    /** Remove old packages */
    std::vector<std::shared_ptr<Package>> remove_contract_content;
    for (const auto& package_type : packages_by_n) {
      while (self.shelf(package_type).size() and
             self.shelf(package_type).front()->production_time_ +
                     PackageInfo::items[package_type].expiration_time_ <
                 core.day) {
        auto t = self.shelf(package_type).front();
        self.shelf(package_type).pop_front();
        --self.virtualSize(package_type);
        /* Need to order less */
        --self.getScore(package_type);
        remove_contract_content.push_back(t);
      }
    }

    if (remove_contract_content.size()) {
      pushOrder(Order(self.id(), trash_.id(), remove_contract_content, core.day));
    }

    /** Process requsts */
    std::vector<Request> new_requests;

    /** amount, clients */
    std::map<std::string, std::pair<int, int>> want;

    for (const auto& request : requests) {
      auto package_type = request.package_.type_;
      want[package_type].first += request.amount_;
      want[package_type].second += 1;
    }

    while (requests.size()) {
      auto request = requests.front();
      requests.pop_front();
      
      /** Spread evenly */
      auto package_type = request.package_.type_;
      auto old = request.amount_;
      auto t = std::ceil((want[package_type].first) / want[package_type].second);
      if (request.amount_ > t) request.amount_ = t;
      t = self.shelf(package_type).size();
      if (request.amount_ > t) request.amount_ = t;

      /* Need to order more */
      self.getScore(package_type) += old - request.amount_;

      if (request.amount_ <= 0) continue;

      /** Resolve request */
      std::vector<std::shared_ptr<Package>> order_content;
      for (auto j = 0; j < request.amount_; ++j) {
        auto package = self.shelf(package_type).front();
        self.shelf(package_type).pop_front();
        order_content.push_back(package);
        --self.virtualSize(package_type);
      }
      pushOrder(Order(self.id(), request.from_, order_content, core.day));
    }


    /**
     * ######.........
     * threshold^
     *       #########
     * buy this^
     */
    for (const auto& package_type : packages_by_n) {
      if (self.virtualSize(package_type) < self.getThreshold(package_type)) {
        auto factory_request =
          Request(self.id(), factory_.id(), PackageInfo::items[package_type],
            self.getMaxVirtualSize(package_type) - self.virtualSize(package_type),
            core.day);
        new_requests.push_back(factory_request);
        pushRequest(factory_request);
        self.virtualSize(package_type) = self.getMaxVirtualSize(package_type);
      }
    }

    /** Change threshold and max virtual size based on score */
    for (const auto& package_type : packages_by_n) {
      auto& t = self.getMaxVirtualSize(package_type);
      auto& score = self.getScore(package_type);
      if (score < 0) --t, ++score;
      else ++t, --score;
      if (t < 0) t = 0;
      if (t > self.getMaxSize()) t = self.getMaxSize();
    }
    warehouse_json["score"] = self.score();
    warehouse_json["max virtual size"] = self.maxVirtualSize();
    warehouse_json["out requests"] = new_requests;
  }

  void updateFactory(Factory& self, std::list<Request>& requests) {
    wlog("Factory update");
    /* Process requests */
    for (auto i = requests.begin(); i != requests.end();) {
      dayt done_day = core.day + self.genWaitTime(core.rng);

      self.getRequestQueue().insert({done_day, *i});

      auto t = i;
      ++i;
      requests.erase(t);
    }
    while (self.getRequestQueue().size() and
           self.getRequestQueue().begin()->first <= core.day) {
      auto request = self.getRequestQueue().begin()->second;
      self.getRequestQueue().erase(self.getRequestQueue().begin());

      std::vector<std::shared_ptr<Package>> contract_content;
      for (int j = 0; j < request.amount_; ++j) {
        contract_content.push_back(createPackage(request.package_.type_));
      }
      pushOrder(Order(self.id(), request.from_, contract_content, core.day));
    }
  }

  void update() {
    updateWarehouse(warehouse_, requests_[warehouse_.id()]);
    for(auto& client : clients) updateClient(client);
    updateFactory(factory_, requests_[factory_.id()]);
  }

  std::shared_ptr<Package> createPackage(const std::string& type) {
    auto package = std::shared_ptr<Package>(new Package(type, core.day, factory_.id()));
    packages_list_.insert({package->id_, package});
    on_package_created(package);
    return package;
  }
};

Engine engine(default_settings);

