#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace navcore {
struct Node { std::string id; double latitude{}; double longitude{}; };
struct Edge { std::string from; std::string to; double length_m{}; double speed_limit_kph{}; double travel_time_s{}; std::string road_class; bool one_way{true}; };
enum class Cost { Distance, Time };
enum class Algorithm { Dijkstra, AStar };
struct RouteOptions { Cost cost{Cost::Distance}; Algorithm algorithm{Algorithm::Dijkstra}; std::unordered_set<std::string> blocked_edges; std::unordered_set<std::string> avoided_classes; };
struct RouteResult { std::vector<std::string> path_nodes; std::vector<Edge> path_edges; double total_distance_m{}; double estimated_travel_time_s{}; std::size_t expanded_nodes{}; double runtime_ms{}; };

class RoadGraph {
 public:
  void add_node(Node node);
  void add_edge(Edge edge);
  [[nodiscard]] bool has_node(const std::string& id) const;
  [[nodiscard]] const Node& node(const std::string& id) const;
  [[nodiscard]] const std::vector<Edge>& outgoing(const std::string& id) const;
  [[nodiscard]] const std::unordered_map<std::string, Node>& nodes() const noexcept { return nodes_; }
  [[nodiscard]] std::size_t node_count() const noexcept { return nodes_.size(); }
 private:
  std::unordered_map<std::string, Node> nodes_;
  std::unordered_map<std::string, std::vector<Edge>> adjacency_;
};

RoadGraph load_csv_map(const std::string& path);
RouteResult route(const RoadGraph& graph, const std::string& from, const std::string& to, const RouteOptions& options = {});
double haversine_m(const Node& a, const Node& b);
std::string edge_key(const Edge& edge);
}  // namespace navcore
