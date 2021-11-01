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
  virtual std::string name() {
    return "Client";
  }
};

struct Factory : public Building {
  std::map<dayt, Request> request_queue;
  virtual std::string name() {
    return "Factory";
  }
};

struct Trash : public Building {
  
};

