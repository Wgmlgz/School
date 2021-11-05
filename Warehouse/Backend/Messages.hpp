#pragma once
#include "Core.hpp"
#include "Items.hpp"

/** POD for store request data */
struct Request {
  idt from_, to_;
  PackageInfo& package_;
  uint32_t amount_;
  dayt day_;
  idt id_;

  Request(idt from, idt to, PackageInfo& package, uint32_t amount, dayt day)
      : from_(from),
        to_(to),
        package_(package),
        amount_(amount),
        day_(day),
        id_(core.rng()) {}

  std::string json() const {
    std::string res = "{";
    res += R"("from_":)" + std::to_string(from_) + ',';
    res += R"("to_":)" + std::to_string(to_) + ',';
    res += R"("package":)" + package_.json() + ',';
    res += R"("day_":)" + std::to_string(day_) + ',';
    res += R"("id_":)" + std::to_string(id_);
    res += "}";
    return res;
  }
};

void to_json(json& j, const Request& re) {
  j = json{
      {"from", core.id2str[re.from_]},
      {"amount", re.amount_},
      {"to", core.id2str[re.to_]},
      {"package", re.package_.type_},
      {"day", re.day_},
  };
}

/** POD for store order data */
struct Order {
  idt from_, to_;
  std::vector<std::shared_ptr<Package>> content_;
  dayt day_;
  idt id_;

  Order(idt from, idt to, std::vector<std::shared_ptr<Package>>& content,
           dayt day)
      : from_(from),
        to_(to),
        content_(content),
        day_(day),
        id_(core.rng()) {}
};

void to_json(json& j, const Order& co) {
  j = json{
      {"from", core.id2str[co.from_]},
      {"to", core.id2str[co.to_]},
      {"day", co.day_},
      {"content type", co.content_.front()->package_info.type_},
      {"amount", co.content_.size()},
  };
}