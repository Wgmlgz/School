#pragma once
#include "Core.hpp"
#include "Items.hpp"
#include "Messages.hpp"

/** @brief Base class for Warehouse, Client, Factory and Trash */
class Building {
  idt id_;
 public:
  Building() { id_ = core.rng(); }
  idt id() { return id_; }

  std::list<std::shared_ptr<Package>> storage;
  virtual void pushPackage(std::shared_ptr<Package> package) {
    storage.push_back(package);
    if (storage.size() >= 64) storage.pop_front();
  }

  virtual std::string name() { return "Building"; }
};

/** @brief Stores warehouse data */
class Warehouse : public Building {
 public:
  Warehouse(const json& j) {
    core.id2str[id()] = name();

    for (auto& [str, _] : PackageInfo::items)
      shelfs[str] = {};

    max_size = j["warehouse"]["max capacity"].get<int>();
    threshold = j["warehouse"]["threshold"].get<int>();
  }

  virtual void pushPackage(std::shared_ptr<Package> package) override {
    shelfs[package->package_info.type_].push_back(package);
    package->house_id_ = id();
  }

  auto& shelf(const std::string& s) { return shelfs[s]; }
  auto& getShelfs() { return shelfs; }
  int& virtualSize(const std::string& s) { return virtual_size[s]; }
  int getThreshold() { return threshold; }
  int getMaxSize() { return max_size; }

  virtual std::string name() override { return "Warehouse"; }

 private:
  std::map<std::string, std::list<std::shared_ptr<Package>>> shelfs;
  std::map<std::string, int> virtual_size;

  int threshold, max_size;
};

/** @brief Stores client data */
class Client : public Building {
 public:
  Client(const json& j) {
    core.id2str[id()] = name();

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

  template <typename T>
  static std::string int2hex(T i) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
    return stream.str();
  }

  virtual std::string name() { return "Client_" + int2hex(id()).substr(0, 4); }

  int packageRng(auto& rng) { return package_rng_(rng); }
  int amountRng(auto& rng) { return amount_rng_(rng); }
  auto getRequestProbability() { return request_probability_; }

 private:
  std::map<std::string, int> probabilities_;
  std::discrete_distribution<> package_rng_;
  std::normal_distribution<> amount_rng_;
  double request_probability_;
};

/** @brief Stores factory data */
class Factory : public Building {
 public:
  Factory(const json& j) {
    core.id2str[id()] = name();
    wait_rng_ = std::normal_distribution<>(
        j["factory"]["wait time"]["mean"].get<double>(),
        j["factory"]["wait time"]["stddev"].get<double>());
  }

  int genWaitTime(auto& rng) { return std::round(wait_rng_(rng)); }
  auto& getRequestQueue() { return request_queue; }
  virtual std::string name() { return "Factory"; }

 private:
  std::normal_distribution<> wait_rng_;
  std::multimap<dayt, Request> request_queue;
};

/** @brief Stores nothing */
struct Trash : public Building {
  Trash() { core.id2str[id()] = name(); }
};

