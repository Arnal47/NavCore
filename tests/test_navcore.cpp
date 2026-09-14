#include "navcore/navcore.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>

static void expect(bool v, const char* m){if(!v)throw std::runtime_error(m);} static void throws(const std::function<void()>& f){try{f();}catch(const std::exception&){return;}throw std::runtime_error("expected exception");}
int main(){try{ auto g=navcore::load_csv_map("data/sample_map.csv"); for(auto a:{navcore::Algorithm::Dijkstra,navcore::Algorithm::AStar}) { navcore::RouteOptions o;o.algorithm=a; auto d=navcore::route(g,"A","F",o); expect(d.total_distance_m==3400,"shortest distance"); o.cost=navcore::Cost::Time; auto t=navcore::route(g,"A","F",o); expect(t.estimated_travel_time_s==227,"fastest time"); } navcore::RouteOptions o; throws([&]{navcore::route(g,"F","A",o);}); o.blocked_edges.insert("A->D"); expect(navcore::route(g,"A","F",o).path_nodes.at(1)=="B","blocked road detour"); o={};o.avoided_classes.insert("motorway"); expect(navcore::route(g,"A","F",o).total_distance_m==3400,"avoid motorway"); expect(navcore::route(g,"A","A",o).path_nodes.size()==1,"same point"); navcore::RoadGraph bad;bad.add_node({"X",0,0});throws([&]{bad.add_node({"X",0,0});});throws([&]{bad.add_edge({"X","Y",1,1,1,"x",true});}); std::cout<<"10 tests passed\n";}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
