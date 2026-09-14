#include "navcore/navcore.hpp"

#include <cmath>
#include <stdexcept>

namespace navcore {
void RoadGraph::add_node(Node node) {
  if (node.id.empty() || nodes_.count(node.id) != 0U) throw std::runtime_error("duplicate or empty node id: " + node.id);
  nodes_.emplace(node.id, std::move(node));
}
void RoadGraph::add_edge(Edge edge) {
  if (!has_node(edge.from) || !has_node(edge.to)) throw std::runtime_error("edge references nonexistent node");
  if (!(edge.length_m > 0.0) || !(edge.speed_limit_kph > 0.0) || !(edge.travel_time_s > 0.0)) throw std::runtime_error("edge has invalid distance or speed");
  adjacency_[edge.from].push_back(edge);
  if (!edge.one_way) { std::swap(edge.from, edge.to); adjacency_[edge.from].push_back(std::move(edge)); }
}
bool RoadGraph::has_node(const std::string& id) const { return nodes_.count(id) != 0U; }
const Node& RoadGraph::node(const std::string& id) const { auto it = nodes_.find(id); if (it == nodes_.end()) throw std::runtime_error("unknown node: " + id); return it->second; }
const std::vector<Edge>& RoadGraph::outgoing(const std::string& id) const { static const std::vector<Edge> empty; auto it = adjacency_.find(id); return it == adjacency_.end() ? empty : it->second; }
double haversine_m(const Node& a, const Node& b) { constexpr double pi = 3.14159265358979323846, r = 6371000.0; const auto rad = [](double d) { return d * pi / 180.0; }; const double dlat = rad(b.latitude-a.latitude), dlon = rad(b.longitude-a.longitude); const double h = std::sin(dlat/2)*std::sin(dlat/2)+std::cos(rad(a.latitude))*std::cos(rad(b.latitude))*std::sin(dlon/2)*std::sin(dlon/2); return 2*r*std::atan2(std::sqrt(h), std::sqrt(1-h)); }
std::string edge_key(const Edge& edge) { return edge.from + "->" + edge.to; }
}  // namespace navcore
