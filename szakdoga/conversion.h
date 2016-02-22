#ifndef CONVERSION_H_INCLUDED
#define CONVERSION_H_INCLUDED

//
// Keszitette: Toth Mate
// Konverzios es kulso hasznalatra alkalmassa tevo eljarasok, tipusok
//


#include <vector>
#include "glm/glm.hpp"
#include "body.h"

namespace approx{
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef unsigned short Index;


	//minden rajzolasi infot tartalmazo adaszerkezet
	struct BodyList{
		std::vector<Vec3> points, //csucspontok
						  normals; //normal fektorok
		std::vector<Index> indexes; //indexek amik a pontokra es normalokra mutatnak
		std::vector<Index> index_ranges; //az egymast koveto elemek az indexek szamai
	};


	//egyetlen test adatainak kinyerese glm tipusokat hasznalo haromszogelt modellbe kirajzoltatashoz
	//CCW orientalt, OpenGL GL_TRIANGLES modban megjelenitve
	template <class T> BodyList drawinfo(const Body<T>& body){
		BodyList res;
		const std::vector<Vector3<T>>& vs = *body.faces(0).vertex_container();
		const std::vector<Vector3<T>>& ns = *body.faces(0).normal_container();
		res.points.reserve(vs.size());
		for (const Vector3<T>& p : vs){
			res.points.push_back(Vec3( p.x, p.y, p.z ));
		}
		res.normals.reserve(ns.size());
		for (const Vector3<T>& p : ns){
			res.normals.push_back(Vec3(p.x, p.y, p.z ));
		}
		res.index_ranges.push_back(0);
		res.index_ranges.push_back(0);
		for (const Face<T>& f : body){
			for (int i = 2; i < f.size(); ++i){
				res.indexes.push_back(f.indicies(0));
				res.indexes.push_back(f.indicies(i-1));
				res.indexes.push_back(f.indicies(i));
				//minden pontnak azonos normal indexet adok
				Vector3<T> n = f.get_normal();
				Vec3 nrm = Vec3(n.x, n.y, n.z);
				res.normals.push_back(nrm);
				res.normals.push_back(nrm);
				res.normals.push_back(nrm);
			}
		}
		res.index_ranges.push_back(res.index_ranges.size());
		return res;
	}

	//testek kozos adatai egy csomagban, haromszogelve
	template <class BodyIterator,class T> BodyList drawinfo(BodyIterator first, BodyIterator last){
		BodyList res;
		const Body<T>& b = *first;
		const std::vector<Vector3<T>>& vs = *b.faces(0).vertex_container();
		const std::vector<Vector3<T>>& ns = *b.faces(0).normal_container();
		res.points.reserve(vs.size());
		for (const Vector3<T>& p : vs){
			res.points.push_back(Vec3(p.x, p.y, p.z));
		}
		res.normals.reserve(ns.size());
		for (const Vector3<T>& p : ns){
			res.normals.push_back(Vec3(p.x, p.y, p.z));
		}
		res.index_ranges.push_back(0);
		for (; first != last; ++first){
			res.index_ranges.push_back(res.index_ranges.back());
			for (const Face<T>& f : *first){
				for (int i = 2; i < f.size(); ++i){
					res.indexes.push_back(f.indicies(0));
					res.indexes.push_back(f.indicies(i - 1));
					res.indexes.push_back(f.indicies(i));
					//minden pontnak azonos normal indexet adok
					Vector3<T> n = f.get_normal();
					Vec3 nrm = Vec3(n.x, n.y, n.z);
					res.normals.push_back(nrm);
					res.normals.push_back(nrm);
					res.normals.push_back(nrm);
					res.index_ranges.back() += 3;
				}
			}
		}
		return res;
	}

}

#endif