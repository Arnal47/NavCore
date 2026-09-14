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
struct Edge { std::string from; std::string to; double length_m{}; double speed_limit_kph{}; double travel_time_s{}; std::string road_class; bool one_way{true}; std::string osm_way_id; std::string road_name; std::string source_osm_node_id; std::string target_osm_node_id; };
enum class Cost { Distance, Time }; enum class Algorithm { Dijkstra, AStar };
struct RouteOptions { Cost cost{Cost::Distance}; Algorithm algorithm{Algorithm::Dijkstra}; std::unordered_set<std::string> blocked_edges; std::unordered_set<std::string> avoided_classes; std::unordered_map<std::string,double> speed_overrides_kph; };
struct RouteResult { std::vector<std::string> path_nodes; std::vector<Edge> path_edges; double total_distance_m{}; double estimated_travel_time_s{}; std::size_t expanded_nodes{}; double runtime_ms{}; };
class RoadGraph { public: void add_node(Node node); void add_edge(Edge edge); [[nodiscard]] bool has_node(const std::string& id) const; [[nodiscard]] const Node& node(const std::string& id) const; [[nodiscard]] const std::vector<Edge>& outgoing(const std::string& id) const; [[nodiscard]] const std::unordered_map<std::string,Node>& nodes() const noexcept{return nodes_;} [[nodiscard]] std::size_t node_count() const noexcept{return nodes_.size();} private: std::unordered_map<std::string,Node> nodes_; std::unordered_map<std::string,std::vector<Edge>> adjacency_; };
struct GpsPoint { double latitude{}; double longitude{}; std::optional<double> heading_deg; };
struct MatchResult { Edge matched_edge; std::string osm_way_id; std::string road_name; double snapped_latitude{}; double snapped_longitude{}; double lateral_distance_m{}; double heading_difference_deg{}; double score{}; std::size_t candidate_count{}; };
RoadGraph load_csv_map(const std::string& path); RoadGraph load_osm_map(const std::string& path); RouteResult route(const RoadGraph&,const std::string&,const std::string&,const RouteOptions& options = RouteOptions{}); double haversine_m(const Node&,const Node&); std::string edge_key(const Edge&); double default_speed_kph(const std::string& highway); std::vector<GpsPoint> load_gps_trace(const std::string& path); MatchResult match_point(const RoadGraph&,const GpsPoint&); std::vector<MatchResult> match_sequence(const RoadGraph&,const std::vector<GpsPoint>&); RouteResult route_from_matches(const RoadGraph&,const std::vector<MatchResult>&,const RouteOptions& options = RouteOptions{});
}