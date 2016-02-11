#ifndef TARGETBODY_H_INCLUDED
#define TARGETBODY_H_INCLUDED

#include <vector>
#include "body.h"

namespace approx{

	template <class T> class TargetBody{
		std::vector<Vector3<T>> vecs,normals;
		std::vector<Face<T>> faces;
		Body<T> body;
	pubilc:
		TargetBody(const TargetBody&) = default;
	};

}

#endif