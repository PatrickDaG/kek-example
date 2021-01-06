#pragma once

#include <cmath>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <example/external/fmt.hpp>
#include <example/external/json.hpp>
#include <example/external/stb.hpp>

#include <example/objects.hpp>

namespace example {

class Application {
	std::vector<std::unique_ptr<Object>> objects{};
	int width = 0, height = 0;

	Color raytrace(const Ray& ray, int depth) {
		auto is_light = [](auto& obj) {
			if(auto light = dynamic_cast<Sphere*>(obj))
				return light.is_light;
			return false;
		};
		auto kek =
		    [&](auto& ray, auto& pred) {
			    double min = std::numeric_limits<double>::infinity();
			    IntersectInfo info_nearest;
			    Object* hitten = nullptr;
			    for(auto& obj : objects) {
				    if(not pred(obj))
					    continue;
				    auto&& [hit, info] = obj->intersect(ray);
				    if(not hit or info.distance >= min)
					    continue;
				    min = info.distance;
				    hitten = obj.get();
				    info_nearest = info;
			    }
			    return hitten;
		    }

		if(depth > 5) return {};

		auto hitten = kek(ray, std::not_fn(is_light));
		if(not hitten)
			return {0, 0, 0};
		Color res;
		for(auto& obj : objects) {
			if(auto light = dynamic_cast<Sphere*>(obj)) {
				if(light.is_light)
					continue;
				Ray r;
				r.origin = hitten->pos;
				auto temp = light.pos - hitten->pos;
				r.dir = normalize(temp);
				auto len = length(temp);
				if(auto hitten2 = kek(r, std::not_fn(is_light));
				   not hitten2 or length(hitten2->pos - hitten->pos) >= len) {
					re += light.color;
				}
				return hitten->color;
			}

		public:
			Application(const std::string& config_file) {
				auto in = std::ifstream{config_file};
				json j;
				in >> j;
				width = j["width"].get<int>();
				height = j["height"].get<int>();
				for(const auto& obj : j["objects"]) {
					if(obj["type"].get<std::string>() == "Sphere") {
						objects.emplace_back(std::move(std::make_unique<Sphere>(obj)));
					} else {
						fmt::print("Oh no");
					}
				}
			}

			void operator()() {
				Vector3 camera;
				double distance = 2;
				std::vector<uint8_t> data(3 * width * height);
				for(int x = 0; x < width; x++) {
					for(int y = 0; y < height; y++) {
						Vector3 pixel({2 * (x - width / 2.0) / width, -2 * (y - height / 2.0) / width, distance});
						auto dir = normalize(pixel - camera);
						auto color = raytrace({camera, dir});
						data[3 * width * y + 3 * x + 0] = static_cast<uint8_t>(color.r * 255.99);
						data[3 * width * y + 3 * x + 1] = static_cast<uint8_t>(color.g * 255.99);
						data[3 * width * y + 3 * x + 2] = static_cast<uint8_t>(color.b * 255.99);
					}
				}
				stbi_write_png("output.png", width, height, 3, data.data(), width * 3);
			}
		};

	}  // namespace example
