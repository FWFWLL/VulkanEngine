#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <cstddef>
#include <iostream>

class Vector3 {
public:
	double c[3];

	Vector3() : c{0, 0, 0} {}
	Vector3(double x, double y, double z) : c{x, y, z} {}

	double x() const {return c[0];}
	double y() const {return c[1];}
	double z() const {return c[2];}

	Vector3 operator -() const {return Vector3(-c[0], -c[1], -c[2]);}

	double operator [](size_t i) const {return c[i];}
	double& operator [](size_t i) {return c[i];}

	Vector3& operator +=(const Vector3 &v) {
		c[0] += v.c[0];
		c[1] += v.c[1];
		c[2] += v.c[2];

		return *this;
	}

	Vector3& operator *=(double s) {
		c[0] *= s;
		c[1] *= s;
		c[2] *= s;

		return *this;
	}

	Vector3& operator /=(double s) {
		return *this *= 1/s;
	}

	double length() const {
		return sqrt(length_squared());
	}

	double length_squared() const {
		return c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
	}
};

using Point3 = Vector3;

inline std::ostream& operator <<(std::ostream &out, const Vector3 &v) {
	return out << v[0] << ", " << v[1] << ", " << v[2];
}

inline Vector3 operator +(const Vector3 &v, const Vector3 &u) {
	return Vector3(v[0] + u[0], v[1] + u[1], v[2] + u[2]);
}

inline Vector3 operator -(const Vector3 &v, const Vector3 &u) {
	return Vector3(v[0] - u[0], v[1] - u[1], v[2] - u[2]);
}

inline Vector3 operator *(const Vector3 &v, const Vector3 &u) {
	return Vector3(v[0] * u[0], v[1] * u[1], v[2] * u[2]);
}

inline Vector3 operator *(const double s, const Vector3 &v) {
	return Vector3(s * v[0], s * v[1], s * v[2]);
}

inline Vector3 operator *(const Vector3 &v, const double s) {
	return s * v;
}

inline Vector3 operator /(const Vector3 &v, const double s) {
	return (1/s) * v;
}

inline double dot(const Vector3 &v, const Vector3 &u) {
	return v[0] * u[0] + v[1] * u[1] + v[2] * v[2];
}

inline Vector3 cross(const Vector3 &v, const Vector3 &u) {
	return Vector3(v[1] * u[2] - v[2] * u[1], v[2] * u[0] - v[0] * u[2], v[0] * u[1] - v[1] * u[0]);
}

inline Vector3 unit(const Vector3 &v) {
	return v / v.length();
}

#endif // !VECTOR_HPP

