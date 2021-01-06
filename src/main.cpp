#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <example/external/fmt.hpp>
#include <example/external/json.hpp>
#include <example/external/stb.hpp>

#include <example/math.hpp>
#include <example/objects.hpp>
#include <example/application.hpp>

namespace example {

void generate_example_config() {
	json j;
	j["width"] = 1024;
	j["height"] = 1024;
	j["objects"] = {{{"type", "Sphere"},
	                 {"color", {{"r", 1.0}, {"g", 1.0}, {"b", 1.0}}},
	                 {"pos", {{"x", 1.0}, {"y", -1.0}, {"z", 10.0}}},
					 {"radius",2.0},{"is_light", false}}
   				 ,{{"type", "Sphere"},
	                 {"color", {{"r", 0.5}, {"g", 1.0}, {"b", 0.5}}},
	                 {"pos", {{"x", 0.0}, {"y", -50.0}, {"z", 0.0}}},
					 {"radius",49.0}, {"is_light" , false}}
					 ,{{"type", "Sphere"},
	                 {"color", {{"r", 0.5}, {"g", 1.0}, {"b", 0.5}}},
	                 {"pos", {{"x", 0.0}, {"y", -50.0}, {"z", 0.0}}},
					 {"radius",49.0}, {"is_light", true}}};

	auto f = std::ofstream{"config.json"};
	f << std::setw(4) << j << std::endl;
}
}

int main() {
	example::generate_example_config();
	example::Application{"config.json"}();
	return EXIT_SUCCESS;
}
