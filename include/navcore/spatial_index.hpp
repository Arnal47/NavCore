#pragma once
#include "navcore/navcore.hpp"
#include <chrono>
#include <unordered_map>
namespace navcore { struct SpatialQueryStats { std::size_t total_indexed_segments{},queried_cells{},raw_candidates{},filtered_candidates{}; double latency_us{}; }; class SpatialIndex { public: explicit SpatialIndex(const RoadGraph&,double cell_degrees=.005); std::vector<Edge> query(double latitude,double longitude,double radius_m,SpatialQueryStats* stats=nullptr) const; private: double cell_; std::unordered_map<std::string,std::vector<Edge>> cells_; std::size_t total_{}; }; RoadGraph make_grid_network(int width,int height); }