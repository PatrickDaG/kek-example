#pragma once

#include <optional>
#include <string>
#include <utility>

#include <example/external/json.hpp>

#include <example/math.hpp>
#include <utility>

namespace example {

struct IntersectInfo {
	double distance = 0.0;
	Vector3 pos{0, 0, 0};
	Vector3 normal{0, 0, 0};
	Color color{};
	double metallic = 0, roughness = 0;
};

class Object {
public:
	Color color;
	Vector3 pos;
	std::string type;
	double metallic = 0, roughness = 0;

public:
	Object(const json& j)
	    : color{j["color"]["r"], j["color"]["g"], j["color"]["b"]}
	    , pos{j["pos"]["x"], j["pos"]["y"], j["pos"]["z"]}
	    , type{j["type"]}
	    , metallic{j["metallic"]}
	    , roughness{j["roughness"]} {}

	virtual ~Object() = default;

	virtual std::optional<IntersectInfo> intersect(const Ray& ray) = 0;
};

class Sphere : public Object {
public:
	double radius = 0;
	bool is_light = false;

	Sphere(const json& j) : Object(j), radius(j["radius"]), is_light(j["is_light"]) {}

	virtual ~Sphere() = default;

	std::optional<IntersectInfo> intersect(const Ray& ray) override {
		auto new_pos = ray.origin - pos;
		auto sphere_to_ray = 2 * dot(new_pos, ray.dir);
		auto ray_to_ray = dot(ray.dir, ray.dir);  // should be always 1
		auto sphere_to_radius = dot(new_pos, new_pos) - radius * radius;
		auto disk = sphere_to_ray * sphere_to_ray - 4 * ray_to_ray * sphere_to_radius;
		if(disk < 0)
			return std::nullopt;
		auto dist = (-sphere_to_ray - std::sqrt(disk)) / (2 * ray_to_ray);
		if(dist < 0.0001) {
			dist = (-sphere_to_ray + std::sqrt(disk)) / (2 * ray_to_ray);
			if(dist < 0.0001) {
				return {};
			}
		}
		auto hit_pos = (ray.dir * dist + ray.origin);
		auto normal = normalize(hit_pos - pos);
		return IntersectInfo{dist, hit_pos, normal, color, metallic, roughness};
	}
};
}  // namespace example
