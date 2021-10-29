#pragma once
#include "Core.hpp"
#include "Items.hpp"

struct Building {
  idt id_;
  std::map<std::string, std::list<std::shared_ptr<Package>>> shelfs;

  Building() {
    id_ = core.rng();
  }

  void pushPackage(const std::shared_ptr<Package>& package) {
    shelfs[package->package_info.type_].push_back(package);
  }
};

struct Warehouse : public Building {
  // 
};

struct Client : public Building {
  TimePoint last_day;
  TimePoint next_time;
  Client() {
    last_day = core.clock;
    next_time = core.clock;
  }
};

struct Factory : public Building {
  // std::uniform_real_distribution<dur> delay_rng();

  // std::priority_queue<std::pair<TimePoint, std::shared_ptr<Package>>> queue;
};

struct Trash : public Building {
  
};

