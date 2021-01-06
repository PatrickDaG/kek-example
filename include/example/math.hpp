#pragma once

#include <cmath>

namespace example {

struct Color {
	double r = 0, g = 0, b = 0;
};

struct Vector3 {
	double x = 0, y = 0, z = 0;
};

struct Ray {
	Vector3 origin, dir;
};

Vector3 operator/(const Vector3& l, double r) { return {l.x / r, l.y / r, l.z / r}; }

Vector3 operator-(const Vector3& l, const Vector3& r) { return {l.x - r.x, l.y - r.y, l.z - r.z}; }

Vector3 operator+(const Vector3& l, const Vector3& r) { return {l.x + r.x, l.y + r.y, l.z + r.z}; }

Vector3 operator*(const Vector3& l, double r) { return {l.x * r, l.y * r, l.z * r}; }

double dot(const Vector3& l, const Vector3& r) { return l.x * r.x + l.y * r.y + l.z * r.z; }

Vector3 normalize(const Vector3& v) {
	double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return v / l;
}

double length(const Vector3& v) {
	double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return l;
}
}
