#ifndef APPROXIMATION_H_INCLUDED
#define APPROXIMATION_H_INCLUDED

#include <vector>
#include <utility>
#include <algorithm>
#include "vectors.h"
#include "face.h"
#include "body.h"
#include "targetbody.h"

namespace approx{

	template <class T, template<class> AtomType> class Approximation{
		const TargetBody<T>* target;
		std::vector<Vector3<T>> vertices, normals;
		std::vector<Face<T>> faces;
		std::vector<AtomType<T>> atoms;
		

		typedef typename std::vector<AtomType<T>>::iterator Iterator;
		typedef typename std::vector<AtomType<T>>::const_iterator ConstIterator;

		void starting_atom(T border){
			Vector3<T> vmin, vmax;
			for (const Face<T>& f : target->body())
				for (const Vector3<T>& v : f){
				vmin.x = std::min(vmin.x, v.x);
				vmin.y = std::min(vmin.y, v.y);
				vmin.z = std::min(vmin.z, v.z);
				vmax.x = std::max(vmax.x, v.x);
				vmax.y = std::max(vmax.y, v.y);
				vmax.z = std::max(vmax.z, v.z);
				}
			//TODO: CW CCW dolog egyeztet
			vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
			vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
			vertices.push_back({ vmax.x + border, vmax.y + border, vmin.z - border });
			vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
			vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
			vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
			vertices.push_back({ vmax.x + border, vmax.y + border, vmax.z + border });
			vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
			normals.push_back({0,0,-1});
			normals.push_back({ 1, 0, 0 });
			normals.push_back({ 0, 0, 1 });
			normals.push_back({ -1, 0, 0 });
			normals.push_back({ 0, 1, 0 });
			normals.push_back({ 0, -1, 0 });
			faces.emplace_back(&vertices, std::vector<int>{0, 1, 2, 3}, &normals, 0);
			faces.emplace_back(&vertices, std::vector<int>{1, 5, 6, 2}, &normals, 1);
			faces.emplace_back(&vertices, std::vector<int>{5, 4, 7, 6}, &normals, 2);
			faces.emplace_back(&vertices, std::vector<int>{4, 0, 3, 7}, &normals, 3);
			faces.emplace_back(&vertices, std::vector<int>{3, 2, 6, 7}, &normals, 4);
			faces.emplace_back(&vertices, std::vector<int>{0, 4, 5, 1}, &normals, 5);
			atoms.emplace_back(&faces, std::vector<int>{0, 1, 2, 3, 4, 5});
		}

	public:
		Approximation(const TargetBody<T>* _target, T _border) : target(_target){
			starting_atom(_border);
		}

		Iterator begin() { return atoms.begin(); }
		Iterator end() { return atoms.end(); }
		ConstIterator begin() const { return atoms.begin(); }
		ConstIterator end() const { return atoms.end(); }

		const AtomType<T>& atom(size_t ind) const {return atoms[ind];}
		const TargetBody<T>& target_body(){return *target;}

		void slice(size_t ind, const Plane<T>& p){
			//TODO
			auto res = atoms[ind].cut_by(p);
		}

		void slice(Iterator pos, const Plane<T>& p){
			slice(pos - atoms.begin(), p);
		}

	};

}

#endif