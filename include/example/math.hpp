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

Vector3& operator+=(Vector3& l, const Vector3& r) { return l = l + r; }

Vector3 operator*(const Vector3& l, double r) { return {l.x * r, l.y * r, l.z * r}; }

Vector3 operator-(const Vector3& v) { return {-v.x, -v.y, -v.z}; }

Color operator/(const Color& l, double r) { return {l.r / r, l.g / r, l.b / r}; }

Color operator-(const Color& l, const Color& r) { return {l.r - r.r, l.g - r.g, l.b - r.b}; }

Color operator+(const Color& l, const Color& r) { return {l.r + r.r, l.g + r.g, l.b + r.b}; }

Color& operator+=(Color& l, const Color& r) { return l = l + r; }

Color operator*(const Color& l, double r) { return {l.r * r, l.g * r, l.b * r}; }

Color operator*(const Color& l, const Color& r) { return {l.r * r.r, l.g * r.g, l.b * r.b}; }

double dot(const Vector3& l, const Vector3& r) { return l.x * r.x + l.y * r.y + l.z * r.z; }

Vector3 normalize(const Vector3& v) {
	double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return v / l;
}

Vector3 reflect(const Vector3& dir, const Vector3& norm) { return dir - norm * 2 * dot(dir, norm); }

double length(const Vector3& v) {
	double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return l;
}
}  // namespace example
