#include "navcore/navcore.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace navcore {
namespace { std::vector<std::string> split(const std::string& s) { std::vector<std::string> r; std::stringstream ss(s); std::string x; while (std::getline(ss,x,',')) r.push_back(x); return r; } bool boolean(const std::string& x) { if (x=="1" || x=="true") return true; if (x=="0" || x=="false") return false; throw std::runtime_error("invalid boolean: " + x); } }
RoadGraph load_csv_map(const std::string& path) { std::ifstream in(path); if (!in) throw std::runtime_error("cannot open map: " + path); RoadGraph g; std::string line; std::size_t n=0; while (std::getline(in,line)) { ++n; if (line.empty() || line[0]=='#') continue; auto f=split(line); try { if (f[0]=="N" && f.size()==4) g.add_node({f[1],std::stod(f[2]),std::stod(f[3])}); else if (f[0]=="E" && f.size()==8) g.add_edge({f[1],f[2],std::stod(f[3]),std::stod(f[4]),std::stod(f[5]),f[6],boolean(f[7]),"","","",""}); else throw std::runtime_error("malformed row"); } catch (const std::exception& e) { throw std::runtime_error("CSV row " + std::to_string(n) + ": " + e.what()); } } return g; }
}  // namespace navcore
