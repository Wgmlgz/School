#pragma once
#include "Core.hpp"
#include "Items.hpp"
#include "Buildings.hpp"

struct Request {
  idt from_, to_;
  PackageInfo& package_;
  uint32_t amount_;
  TimePoint time_;
  idt id_;

  Request(idt from, idt to, PackageInfo& package, uint32_t amount, TimePoint time) :
    from_(from), to_(to), package_(package), amount_(amount), time_(time), id_(core.rng()) {}
};

struct Contract {
  idt from_, to_;
  std::vector<std::shared_ptr<Package>> content_;
  TimePoint time_;
  idt id_;
};

struct Engine {
  std::unordered_multiset<std::shared_ptr<Package>> packages_list_;
  std::unordered_multiset<std::shared_ptr<Building>> buildings_list_;

  Trash trash_;
  Factory factory_;
  Warehouse warehouse_;
  std::vector<Client> clients = {Client(), Client(), Client(), Client()};

  std::map<idt, std::list<Contract>> contracts_;
  std::map<idt, std::list<Request>> requests_;

  std::function<void(const Contract& contract)> on_push_contract;
  std::function<void(const Request& contract)> on_push_request;

  void pushContract(const Contract& contract) {
    contracts_[contract.to_].push_back(contract);
    on_push_contract(contract);
  }

  void pushRequest(const Request& request) {
    requests_[request.to_].push_back(request);
    on_push_request(request);
  }

  void updateClient(Client& self, std::list<Contract>& contracts, std::list<Request>& requests) {
    if (core.clock > self.next_time) {
      self.last_day += duration_cast<seconds>(core.day);
      self.next_time = self.last_day + duration_cast<seconds>(core.rngDayTime());

      std::cout <<( core.clock - self.next_time).count() << std::endl;
      /* Send request to warehouse */
      // TODO make request generaion
      pushRequest(Request(self.id_, warehouse_.id_, PackageInfo::items["DEFAULT"], 3, core.clock));
    }
    
    /* Process contracts */
    while (contracts.size()) {
      auto& contract = contracts.front();
      for (auto i : contract.content_) {
        self.pushPackage(i);
      }
      contracts.pop_front();
    }
  }

  void updateWarehouse(Warehouse& self, std::list<Contract>& contracts, std::list<Request>& requests) {

  }

  void updateTrash(Trash& self, std::list<Contract>& contracts, std::list<Request>& requests) {

  }

  void updateFactory(Factory& self, std::list<Contract>& contracts,
                     std::list<Request>& requests) {}

  void update(dur dt) {
    for(auto& client : clients) updateClient(client, contracts_[client.id_], requests_[client.id_]);
    updateWarehouse(warehouse_, contracts_[warehouse_.id_], requests_[warehouse_.id_]);
    updateFactory(factory_, contracts_[factory_.id_], requests_[factory_.id_]);
    updateTrash(trash_, contracts_[trash_.id_], requests_[trash_.id_]);
  }
};

Engine engine;
