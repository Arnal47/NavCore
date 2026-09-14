#include "navcore/navcore.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
  try { std::string map, from, to, algo="dijkstra", cost="distance"; navcore::RouteOptions o;
    for(int i=1;i<argc;i+=2) { if(i+1>=argc) throw std::runtime_error("missing value for " + std::string(argv[i])); std::string k=argv[i],v=argv[i+1]; if(k=="--map") map=v; else if(k=="--from") from=v; else if(k=="--to") to=v; else if(k=="--algo") algo=v; else if(k=="--cost") cost=v; else if(k=="--avoid") o.avoided_classes.insert(v); else if(k=="--block") o.blocked_edges.insert(v); else throw std::runtime_error("unknown option: "+k); }
    if(map.empty()||from.empty()||to.empty()) throw std::runtime_error("usage: navcore_cli --map FILE --from ID --to ID --algo dijkstra|astar --cost distance|time [--avoid CLASS] [--block FROM->TO]");
    if(algo=="astar") o.algorithm=navcore::Algorithm::AStar; else if(algo!="dijkstra") throw std::runtime_error("--algo must be dijkstra or astar"); if(cost=="time") o.cost=navcore::Cost::Time; else if(cost!="distance") throw std::runtime_error("--cost must be distance or time");
    auto r=navcore::route(navcore::load_csv_map(map),from,to,o); std::cout<<"route: "; for(std::size_t i=0;i<r.path_nodes.size();++i) std::cout<<(i?" -> ":"")<<r.path_nodes[i]; std::cout<<"\ntotal distance: "<<r.total_distance_m<<" m\nETA: "<<r.estimated_travel_time_s<<" s\nvisited nodes: "<<r.expanded_nodes<<"\nruntime: "<<r.runtime_ms<<" ms\n"; return 0;
  } catch(const std::exception& e) { std::cerr<<"error: "<<e.what()<<'\n'; return 2; }
}
