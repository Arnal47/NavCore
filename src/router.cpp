#include "navcore/navcore.hpp"
#include <algorithm>
#include <functional>
#include <queue>
#include <stdexcept>

namespace navcore {
RouteResult route(const RoadGraph& g, const std::string& from, const std::string& to, const RouteOptions& opt) {
  if (!g.has_node(from) || !g.has_node(to)) throw std::runtime_error("origin or destination does not exist");
  const auto started=std::chrono::steady_clock::now(); if (from==to) return {{from},{},0,0,1,0};
  struct Q { double f; std::string id; bool operator>(const Q& o) const { return f>o.f; } }; std::priority_queue<Q,std::vector<Q>,std::greater<Q>> q;
  std::unordered_map<std::string,double> d; std::unordered_map<std::string,Edge> prev; d[from]=0; q.push({0,from}); std::size_t expanded=0;
  constexpr double max_speed_mps=130.0/3.6;
  auto h=[&](const std::string& x) { if (opt.algorithm==Algorithm::Dijkstra) return 0.0; const double meters=haversine_m(g.node(x),g.node(to)); return opt.cost==Cost::Distance ? meters : meters/max_speed_mps; };
  while(!q.empty()) { auto cur=q.top(); q.pop(); if (cur.f > d[cur.id]+h(cur.id)+1e-9) continue; ++expanded; if(cur.id==to) break; for(const auto& e:g.outgoing(cur.id)) { if(opt.blocked_edges.count(edge_key(e)) || opt.avoided_classes.count(e.road_class)) continue; const double w=opt.cost==Cost::Distance?e.length_m:e.travel_time_s; const double nd=d[cur.id]+w; if(!d.count(e.to)||nd<d[e.to]) { d[e.to]=nd; prev[e.to]=e; q.push({nd+h(e.to),e.to}); } } }
  if(!d.count(to)) throw std::runtime_error("destination is unreachable under requested constraints"); RouteResult r; r.expanded_nodes=expanded; for(std::string x=to;x!=from;) { const auto& e=prev.at(x); r.path_edges.push_back(e); r.path_nodes.push_back(x); r.total_distance_m+=e.length_m; r.estimated_travel_time_s+=e.travel_time_s; x=e.from; } r.path_nodes.push_back(from); std::reverse(r.path_nodes.begin(),r.path_nodes.end()); std::reverse(r.path_edges.begin(),r.path_edges.end()); r.runtime_ms=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-started).count(); return r;
}
}  // namespace navcore
