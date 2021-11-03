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
  std::map<std::string, int> virtual_size;

  int threshold;
  int max_size;

  virtual std::string name() override {
    return "Warehouse";
  }

  Warehouse(const json& j) {
    core.id2str[id_] = name();
    for (auto& [str, _] : PackageInfo::items) {
      shelfs[str] = {};
    }
    max_size = j["warehouse"]["max capacity"].get<int>();
    threshold = j["warehouse"]["threshold"].get<int>();
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

  Client(const json& j) {
    core.id2str[id_] = name();

    std::vector<double> probabilities(j["packages"]["n"].get<int>());
    for (auto& i : probabilities)
      i = j["clients"]["type"]["base"].get<int>() +
          core.rng() % j["clients"]["type"]["diff"].get<int>();

    package_rng_ = std::discrete_distribution<>(probabilities.begin(), probabilities.end());
    amount_rng_ = std::normal_distribution<>(
        j["clients"]["amount"]["mean"].get<double>(),
        j["clients"]["amount"]["stddev"].get<double>());

    request_probability_ = std::normal_distribution<>(
        j["clients"]["request propability"]["mean"].get<double>(),
        j["clients"]["request propability"]["stddev"].get<double>())(core.rng);
  }
};

struct Factory : public Building {
  std::normal_distribution<> wait_rng_;
  std::multimap<dayt, Request> request_queue;
  virtual std::string name() {
    return "Factory";
  }
  Factory(const json& j) {
    core.id2str[id_] = name();
    wait_rng_ = std::normal_distribution<>(
        j["factory"]["wait time"]["mean"].get<double>(),
        j["factory"]["wait time"]["stddev"].get<double>());
  }
};

struct Trash : public Building {
  Trash() { core.id2str[id_] = name(); }
};

