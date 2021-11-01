#pragma once
#include "Core.hpp"
#include "Items.hpp"

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

struct Contract {
  idt from_, to_;
  std::vector<std::shared_ptr<Package>> content_;
  dayt time_;
  idt id_;

  Contract(idt from, idt to, std::vector<std::shared_ptr<Package>>& content,
           dayt time_)
      : from_(from),
        to_(to),
        content_(content),
        time_(time_),
        id_(core.rng()) {}

  std::string json() const {
    std::string res = "{";

    res += R"("from_":)" + std::to_string(from_) + ',';
    res += R"("to_":)" + std::to_string(to_) + ',';
    res += R"("time_":)" + std::to_string(time_) + ',';
    res += R"("id_":)" + std::to_string(id_) + ',';
    res += "}";
    return res;
  }
};

void to_json(json& j, const Contract& co) {
  j = json{
      {"from", core.id2str[co.from_]},
      {"to", core.id2str[co.to_]},
      {"time", co.time_},
  };
}