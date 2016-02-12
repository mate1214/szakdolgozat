#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "vectors.h"

namespace approx{

	template <class Vector,class T> class HyperPlane{
		Vector n;
		T dist;
	public:
		HyperPlane(const Vector& _normal, const Vector& _point) : n(_normal.normalized()),dist(dot(n,_point)){}
		HyperPlane(const Vector& _normal, T distance) : n(_normal.normalized()), dist(distance){}
		HyperPlane(const HyperPlane&) = default;

		HyperPlane& operator = (const HyperPlane&) = default;

		T signed_distance() const { return dist; }
		Vector& normal() { return normal; }
		const Vector normal() const{ return normal; }
		Vector example_point() const{
			return n*dist;
		}
		T classify_point(const Vector& p) const {
			return dot(p, n) - dist;
		}

	};


	template <class T> using Plane = HyperPlane < Vector3<T>,T >;
	template <class T> using Line = HyperPlane < Vector2<T>, T > ;
}

#endif