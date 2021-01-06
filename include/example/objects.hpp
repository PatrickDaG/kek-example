#pragma once

#include <string>
#include <utility>

#include <example/external/json.hpp>

#include <example/math.hpp>
#include <utility>

namespace example {

struct IntersectInfo {
	double distance = 0.0;
	Vector3 pos{0, 0, 0};
};

class Object {
public:
	Color color;
	Vector3 pos;
	std::string type;

public:
	Object(const json& j)
	    : color{j["color"]["r"], j["color"]["g"], j["color"]["b"]}
	    , pos{j["pos"]["x"], j["pos"]["y"], j["pos"]["z"]}
	    , type{j["type"]} {}

	virtual ~Object() = default;

	virtual std::pair<bool, IntersectInfo> intersect(const Ray& ray) = 0;
};

class Sphere : public Object {
	double radius = 0;
	bool is_light = false;

public:
	Sphere(const json& j) : Object(j), radius(j["radius"]), is_light(j["is_light"]) {}

	virtual ~Sphere() = default;

	std::pair<bool, IntersectInfo> intersect(const Ray& ray) override {
		auto new_pos = ray.origin - pos;
		auto sphere_to_ray = 2 * dot(new_pos, ray.dir);
		auto ray_to_ray = dot(ray.dir, ray.dir);  // should be always 1
		auto sphere_to_radius = dot(pos, pos) - radius * radius;
		auto disk = sphere_to_ray * sphere_to_ray - 4 * ray_to_ray * sphere_to_radius;
		if(disk < 0)
			return {false, {}};
		auto dist = (-sphere_to_ray - std::sqrt(disk)) / (2 * ray_to_ray);
		if(dist < 0) return {false, {}};
		return {true, {dist, (ray.dir * dist + ray.origin)}};
	}
};
}  // namespace example
