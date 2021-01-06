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
#include <random>
#include <ranges>

#include <example/external/fmt.hpp>
#include <example/external/json.hpp>
#include <example/external/stb.hpp>

#include <example/objects.hpp>
#include "math.hpp"

namespace example {

using std::views::iota;

constexpr const auto samples = 5;

class Application {
	std::random_device rd{"default"};
	std::default_random_engine eng{rd()};
	std::uniform_real_distribution<double> rnd{0.0, 1.0};

	Vector3 random_unit_vector() {
		while(true) {
			Vector3 v = {rnd(eng), rnd(eng), rnd(eng)};
			auto len = length(v);
			if(len > 0.0 and len <= 1.0)
				return normalize(v);
		}
	}

	std::vector<std::unique_ptr<Object>> objects{};
	int width = 0, height = 0;

	Color raytrace(const Ray& ray, int depth) {
		// auto is_light = [](auto& obj) noexcept {
		//     if(auto light = dynamic_cast<Sphere*>(std::addressof(obj)))
		//         return light->is_light;
		//     return false;
		// };
		auto kek = [&](auto& ray, auto&& pred) {
			double min = std::numeric_limits<double>::infinity();
			std::optional<IntersectInfo> info_nearest;
			for(auto& obj : objects) {
				if(not pred(*obj))
					continue;
				auto info = obj->intersect(ray);
				if(not info or info->distance >= min)
					continue;
				min = info.value().distance;
				info_nearest = info;
			}
			return info_nearest;
		};

		if(depth > 3)
			return {0.4, 0.4, 0.4};

		auto info = kek(ray, [](auto&) { return true; });

		if(not info)
			return {0.4, 0.4, 0.4};

		if(info->is_light)
			return info->color;
		Color res;
		// for(auto& obj : objects) {
		//	if(auto light = dynamic_cast<Sphere*>(obj.get())) {
		//		if(not light->is_light)
		//			continue;
		//		Ray r = {};
		//		r.origin = info->pos;
		//		auto temp = light->pos - info->pos;
		//		r.dir = normalize(temp);
		//		auto len = length(temp);
		//		if(auto info2 = kek(r, std::not_fn(is_light)); not info2 or length(info2->pos - info->pos) >= len) {
		//		}
		//	}
		//}
		if(info->metallic > 0.5) {
			auto perfect_reflected = reflect(-ray.dir, info->normal);
			for(int i = 0; i < samples; i++) {
				Ray scattered = {info->pos, normalize(perfect_reflected + random_unit_vector() * info->roughness)};
				res += info->color * raytrace(scattered, depth + 1);
			}
		} else {
			for(int i : iota(0, samples)) {
				Ray scattered = {info->pos, normalize(info->normal + random_unit_vector() * info->roughness)};
				res += info->color * raytrace(scattered, depth + 1);
			}
		}
		return res / samples;
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
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				Vector3 pixel({2 * (x - width / 2.0) / width, -2 * (y - height / 2.0) / width, distance});
				auto dir = normalize(pixel - camera);
				auto color = raytrace({camera, dir}, 0);
				data[3 * width * y + 3 * x + 0] = static_cast<uint8_t>(color.r * 255.99);
				data[3 * width * y + 3 * x + 1] = static_cast<uint8_t>(color.g * 255.99);
				data[3 * width * y + 3 * x + 2] = static_cast<uint8_t>(color.b * 255.99);
				fmt::print("{}/{}\r", width * y + x, width * height);
			}
		}
		stbi_write_png("output.png", width, height, 3, data.data(), width * 3);
	}
};

}  // namespace example
