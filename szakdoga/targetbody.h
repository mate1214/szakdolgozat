#ifndef TARGETBODY_H_INCLUDED
#define TARGETBODY_H_INCLUDED

#include <vector>
#include "body.h"

namespace approx{

	template <class T> class TargetBody{
		std::vector<Vector3<T>> vecs,normals;
		std::vector<Face<T>> faces;
		Body<T> bdy;

		static std::vector<int> range(size_t n){
			std::vector<int> res;
			res.reserve(n);
			for (size_t i = 0; i < n; ++i){
				res.push_back(i);
			}
			return res;
		}

	pubilc:
		TargetBody(const TargetBody&) = default;
		TargetBody(TargetBody&&) = default;
		TargetBody(const std::vector<Vector3<T>>& vec, const std::vector<Vector3<T>>& nrm, const std::vector<Face<T>>& fac)
			: vecs(vec), normals(nrm), faces(fac), bdy(&faces, std::move(range(faces.size()))){}
		TargetBody(std::vector<Vector3<T>>&& vec, std::vector<Vector3<T>>&& nrm, std::vector<Face<T>>&& fac)
			: vecs(vec), normals(nrm), faces(fac), bdy(&faces, std::move(range(faces.size()))){}

		const Body<T>& body() const{
			return bdy;
		}
	};

}

#endif