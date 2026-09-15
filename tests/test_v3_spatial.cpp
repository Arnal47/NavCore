#include "navcore/spatial_index.hpp"
#include <iostream>
#include <stdexcept>
int main(){try{auto g=navcore::make_grid_network(100,100);navcore::SpatialIndex idx(g);navcore::SpatialQueryStats s;auto v=idx.query(30.05,120.05,200,&s);if(v.empty()||s.total_indexed_segments<20000||s.filtered_candidates>=s.total_indexed_segments)throw std::runtime_error("spatial index query");navcore::SpatialQueryStats off;if(!idx.query(0,0,10,&off).empty())throw std::runtime_error("off road");std::cout<<"v3 tests passed\n";}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}