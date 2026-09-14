#include "navcore/navcore.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

static void expect(bool v, const char* m){if(!v)throw std::runtime_error(m);} static void throws(const std::function<void()>& f){try{f();}catch(const std::exception&){return;}throw std::runtime_error("expected exception");}
static void expect_path(const navcore::RouteResult& r,const std::vector<std::string>& p,const char* m){expect(r.path_nodes==p,m);}
int main(){try{
  auto g=navcore::load_csv_map("data/sample_map.csv");
  for(auto a:{navcore::Algorithm::Dijkstra,navcore::Algorithm::AStar}) { navcore::RouteOptions o;o.algorithm=a; auto d=navcore::route(g,"A","F",o); expect(d.total_distance_m==2700,"shortest distance"); expect_path(d,{"A","D","E","F"},"shortest-distance path"); o.cost=navcore::Cost::Time; auto t=navcore::route(g,"A","F",o); expect(t.estimated_travel_time_s==180,"fastest time"); expect_path(t,{"A","B","C","F"},"fastest-time path"); }
  navcore::RouteOptions normal; normal.algorithm=navcore::Algorithm::AStar; normal.cost=navcore::Cost::Time; auto motorway=navcore::route(g,"A","F",normal); expect(motorway.path_edges.back().road_class=="motorway","motorway selected without avoidance"); normal.avoided_classes.insert("motorway"); auto avoided=navcore::route(g,"A","F",normal); expect_path(avoided,{"A","D","E","F"},"avoid motorway path"); for(const auto& e:avoided.path_edges) expect(e.road_class!="motorway","avoid motorway edges");
  navcore::RouteOptions o; throws([&]{navcore::route(g,"F","A",o);}); o.blocked_edges.insert("A->D"); expect_path(navcore::route(g,"A","F",o),{"A","B","C","F"},"blocked road detour"); o={}; expect(navcore::route(g,"A","A",o).path_nodes.size()==1,"same point"); navcore::RoadGraph bad;bad.add_node({"X",0,0});throws([&]{bad.add_node({"X",0,0});});throws([&]{bad.add_edge({"X","Y",1,1,1,"x",true});});
  // Deliberately under-reported geographic edge costs make raw Haversine overestimate.
  navcore::RoadGraph adversarial; adversarial.add_node({"S",0,0}); adversarial.add_node({"M",0,1}); adversarial.add_node({"G",0,2}); adversarial.add_edge({"S","G",10,50,10,"test",true}); adversarial.add_edge({"S","M",1,50,1,"test",true}); adversarial.add_edge({"M","G",1,50,1,"test",true}); navcore::RouteOptions dopt; auto d=navcore::route(adversarial,"S","G",dopt); dopt.algorithm=navcore::Algorithm::AStar; auto a=navcore::route(adversarial,"S","G",dopt); expect(d.total_distance_m==a.total_distance_m,"A* admissible distance cost"); expect_path(a,{"S","M","G"},"A* adversarial path");
  std::cout<<"all NavCore tests passed\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}