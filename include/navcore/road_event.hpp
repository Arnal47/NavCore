#pragma once
#include "navcore/navcore.hpp"
namespace navcore { class RoadEventOverlay { public: void close(const std::string& edge); void override_speed(const std::string& edge,double kph); bool closed(const Edge&) const; double speed(const Edge&) const; void apply(RouteOptions&) const; private: std::unordered_set<std::string> closed_; std::unordered_map<std::string,double> speeds_; }; }