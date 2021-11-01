#pragma once
#include "Core.hpp"
#include "Items.hpp"
#include "Messages.hpp"


struct Building {
  idt id_;

  Building() {
    id_ = core.rng();
  }

  std::list<std::shared_ptr<Package>> storage;
  virtual void pushPackage(std::shared_ptr<Package> package) {
    storage.push_back(package);
    if (storage.size() >= 64) storage.pop_front();
  }

  virtual std::string name() {
    return "Building";
  }
};

struct Warehouse : public Building {
  std::map<std::string, std::list<std::shared_ptr<Package>>> shelfs;

  virtual std::string name() override {
    return "Warehouse";
  }

  Warehouse() {
    core.id2str[id_] = name();
    for (auto& [str, _] : PackageInfo::items) {
      shelfs[str] = {};
    }
  }

  virtual void pushPackage(std::shared_ptr<Package> package) override {
    shelfs[package->package_info.type_].push_back(package);
    package->house_id_ = id_;
  }
};

struct Client : public Building {
  std::map<std::string, int> probabilities_;
  std::discrete_distribution<> package_rng_;
  std::normal_distribution<> amount_rng_;
  double request_probability_;

  virtual std::string name() {
    return "Client";
  }

  Client(int n) {
    core.id2str[id_] = name();

    // std::initializer_list<double> probabilities(n);
    // std::initializer_list<double> probabilities(n);
    std::vector<double> probabilities(n);
    for (auto& i : probabilities) i = core.rngd();
    // amount_rng_ = {core.rng() % };
    // std::initializer_list<double> l = probabilities;
    package_rng_ = std::discrete_distribution<>(probabilities.begin(), probabilities.end());
  }

  // void generateRequest() {
  //   std::string type = 
  // }
};

struct Factory : public Building {
  std::map<dayt, Request> request_queue;
  virtual std::string name() {
    return "Factory";
  }
  Factory() { core.id2str[id_] = name(); }
};

struct Trash : public Building {
  Trash() { core.id2str[id_] = name(); }
};

