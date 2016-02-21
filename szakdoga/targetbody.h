#ifndef TARGETBODY_H_INCLUDED
#define TARGETBODY_H_INCLUDED

//
// Keszitette: Toth Mate
// Osztaly mely kezeli a kozelitendo testet minden adataval egyutt.
// A masolasa es a mozgatasa is linearis mert a lapok mutatoit felul kell irni
//

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "body.h"

namespace approx{

	//Parametere tetszoleges Vector3 kompatibilis skalar
	template <class T> class TargetBody{
		std::vector<Vector3<T>> vecs,normals; //pont es normalvektor tarolok
		std::vector<Face<T>> faces; //lapok
		Body<T> bdy; //test

		static std::vector<int> range(size_t n){ //segedfuggveny az indexek eloallitasahoz
			std::vector<int> res;
			res.reserve(n);
			for (size_t i = 0; i < n; ++i){
				res.push_back(i);
			}
			return res;
		}

	public:
		//masolo es mozgato konstruktorok
		TargetBody(const TargetBody& t) : vecs(t.vecs), normals(t.normals), bdy(t.bdy.migrate_to(&faces)){
			faces.reserve(t.faces.size());
			for (const Face<T>& f : t.faces) faces.push_back(f.migrate_to(&vecs, &normals));
		}
		//a koltoztetesi pointer atallitas miatt a mozgato konstruktor is linearis ideju
		TargetBody(TargetBody&&) vecs(std::move(t.vecs)), normals(std::move(t.normals)), bdy(std::move(t.bdy.migrate_to(&faces))){
			faces.reserve(t.faces.size());
			for (Face<T>&& f : t.faces) faces.push_back(f.migrate_to(&vecs, &normals));
		}

		//inicializalas pont, normalvektor es lap vektorokkal, a lapok kozul mindengyiket hasznalja
		TargetBody(const std::vector<Vector3<T>>& vec, const std::vector<Vector3<T>>& nrm, const std::vector<Face<T>>& fac)
			: vecs(vec), normals(nrm), faces(fac), bdy(&faces, std::move(range(faces.size()))){}
		TargetBody(std::vector<Vector3<T>>&& vec, std::vector<Vector3<T>>&& nrm, std::vector<Face<T>>&& fac)
			: vecs(vec), normals(nrm), faces(fac), bdy(&faces, std::move(range(faces.size()))){}

		//a konkret test kinyerese
		const Body<T>& body() const{
			return bdy;
		}

		const std::vector<Vector3<T>>& points() const { return vecs; }
		const std::vector<Vector3<T>>& normals() const { return normals; }
		//masolo ertekadas
		TargetBody& operator = (const TargetBody& t) {
			vecs = t.vecs;
			normals = t.normals;
			faces.clear();
			faces.reserve(t.faces.size());
			for (const Face<T>& f : t.faces){
				faces.push_back(f.migrate_to(&vecs, &normals));
			}
			bdy = t.bdy.migrate_to(&faces);
			return *this;
		}


		//mozgato ertekadas, linearis idoben a lapokmutatok atalaitasa miatt
		TargetBody& operator = (TargetBody&& t){
			vecs = std::move(t.vecs);
			normals = std::move(t.normals);
			faces.clear();
			faces.reserve(t.faces.size());
			for (Face<T>&& f : t.faces){
				faces.push_back(f.migrate_to(&vecs, &normals));
			}
			bdy = std::move(t.bdy.migrate_to(&faces));
			return *this;
		}

		// .obj formatumu fajl betoltese
		void load_obj(const std::string& filename){
			vecs.clear();
			normals.clear();
			faces.clear();
			std::ifstream f(filename);
			std::string line;
			bool vt = false;
			while (std::getline(f, line)){
				std::string::size_type ind = line.find('#');
				if (ind != std::string::npos){
					line.erase(ind);
				}
				if (line.size()){
					std::stringstream stream(line);
					std::string beg;
					T x, y, z;
					int ind1, ind2, ind3;
					char sep;
					line >> beg;
					if (beg == "v"){
						stream >> x >> y >> z;
						vertices.push_back({ x, y, z });
					}
					else if (beg == "vn"){
						//TODO: kesobb esetleg kellhet olyan amiben a normalok jol allnak
					}
					else if (beg == "vt"){
						vt = true;
					}
					else if (beg == "f"){
						std::vector<int> inds;
						if (vt){
							while (stream >> ind1 >> sep >> ind2 >> sep >> ind3){
								inds.push_back(ind1);
							}
							normals.push_back(cross(vecs[inds[2]] - vecs[inds[1]], vecs[inds[1]] - vecs[inds[0]]).normalized());
							faces.emplace_back(&vecs, std::move(inds), &normals, normals.size() - 1);
						}
						else{
							while (stream >> ind1 >> sep >> sep >> ind3){
								inds.push_back(ind1);
							}
							normals.push_back(cross(vecs[inds[2]] - vecs[inds[1]], vecs[inds[1]] - vecs[inds[0]]).normalized());
							faces.emplace_back(&vecs, std::move(inds), &normals, normals.size() - 1);
						}
					}
				}
			}
			bdy = Body<T>(&faces, std::move(range(faces.size()-1)));
		}


	};

}

#endif