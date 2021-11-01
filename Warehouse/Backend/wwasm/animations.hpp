#pragma once
#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <string>
#include <iostream>

namespace wwasm {
struct prp {
  float f_;
  float real_f_;
  prp(float f = 0) : f_(f) {}
  operator float() const{ return f_; }
};

namespace literals {
prp operator"" _cnt(long double val) { return prp(float(val)); }
prp operator"" _cnt(unsigned long long val) { return prp(float(val)); }
}  // namespace literals

struct frm {
  std::map<std::string, prp> props;
  frm() {}
  frm(const std::string& name, const prp& prop) { props.insert({name, prop}); }
  const prp operator[](const std::string& str) const {
    return props.at(str);
  }
  prp& operator[](const std::string& str) { return props.at(str); }
};

std::ostream& operator<<(std::ostream& os, const frm& fr) {
  for (auto& [id, p] : fr.props) {
    os << id << ": "<< p.f_ << std::endl;
  }
  return os;
}
frm operator|(const frm& lhs, const frm& rhs) {
  auto res = lhs;
  for (const auto& [id, prop] : rhs.props) {
    res.props.insert({id, prop});
  }
  return res;
}

float lerp(float a, float b, float f) {
  f = (3.0 * f * f) - (2.0 * f * f * f);
  return (a * (1.0 - f)) + (b * f);
}

frm interp(const frm& lhs, const frm& rhs, float t) {
  auto check = [](auto const& lhs, auto const& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(),
      [](auto a, auto b) { return a.first == b.first; });
  };
  if (!check(rhs.props, rhs.props)) throw std::runtime_error("keys arn't equal");

  auto res = lhs;
  for (const auto& [id, _] : lhs.props) {
    res[id] = lerp(lhs[id], rhs[id], t);
  }
  return res;
}


struct anim {
  std::vector<std::pair<float, frm>> frames_;
  int frame_id_ = 1;
  bool loop = true;   
  std::chrono::time_point<std::chrono::system_clock> start_, last_;
  std::function<void(anim&)> on_fin_;

  int cur_, next_;
  anim(const std::vector<std::pair<float, frm>>& frames,
      std::function<void(anim&)> on_fin = [](anim& a) {})
      : frames_(frames), on_fin_(on_fin) {
    if (frames.size() < 2) throw std::runtime_error("need 2 or more frames");
    replay();
  }

  void replay() {
    frame_id_ = 1;
    last_ = start_ = std::chrono::system_clock::now();
    cur_ = 0;
    next_ = 1;
  }

  bool nextFrame() {
    ++frame_id_;
    if (frame_id_ >= frames_.size()) {
      return true;
    } else {
      ++next_;
      ++cur_;
    }

    return false;
  }

  frm get() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> dt = now - last_;
    std::chrono::duration<float> ts = now - start_;

    bool call_fin = false;
    if (ts.count() > frames_[next_].first) {
      call_fin = nextFrame();
      if (!call_fin) last_ = now;
    }

    dt = now - last_;
    ts = now - last_;
    auto t = dt.count() / (frames_[next_].first - frames_[cur_].first);
    t = std::min<float>(std::max<float>(0, t), 1);
    auto res = interp(frames_[cur_].second, frames_[next_].second, t);
    if (call_fin) on_fin_(*this);
    return res;
  }
};
}
