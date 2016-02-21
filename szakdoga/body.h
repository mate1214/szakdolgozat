#ifndef BODY_H_INCLUDED
#define BODY_H_INCLUDED

//
// Keszitette: Toth Mate
// Test tipus mely indexekkel hivatkozik a lapjaira
// 

#include <vector>
#include "face.h"
#include "indexiterator.h"

namespace approx{

	//Parametere a skalar tipus ami felett ertelmezzuk a vektorteret
	template <class T> class Body{
	protected:
		std::vector<Face<T>>* _faces;
		std::vector<int> ids;

		typedef IndexIterator<Face<T>> FaceIterator;
		typedef ConstIndexIterator<Face<T>> ConstFaceIterator;
	public:
		Body(std::vector<Face<T>>* f, const std::vector<int>& i) : _faces(f), ids(i){}
		Body(std::vector<Face<T>>* f, std::vector<int>&& i) : _faces(f), ids(i){}
		Body(const Body&) = default;
		Body(Body&& b) : _faces(b._faces), ids(std::move(b.ids)){}

		Body& operator = (const Body& b) = default;
		Body& operator =(Body&& b){
			ids = std::move(b.ids);
			_faces = b._faces;
			return *this;
		}

		Body migrate_to(std::vector<Face<T>>* fcs){
			return Body(fcs, std::move(ids));
		}

		Body migrate_to(std::vector<Face<T>>* fcs) const {
			return Body(fcs, ids);
		}


		int size() const { return ids.size(); }
	    int indicies(size_t i) const { return ids[i]; }
		const std::vector<int>& indicies() const { return ids; }
		Face<T>& faces(size_t i){ return _faces->operator[](i); }
		const Face<T>& faces(size_t i) const { return _faces->operator[](i); }

		FaceIterator begin() { return FaceIterator(_faces, &ids, 0); }
		FaceIterator end() { return FaceIterator(_faces, &ids, ids.size()); }

		ConstFaceIterator begin() const { return ConstFaceIterator(_faces, &inds, 0); }
		ConstFaceIterator end() const { return ConstFaceIterator(_faces, &inds, inds.size()); }

		T volume(){
			T sum = 0;
			for (const Face<T>& f : *this){
				sum += f.to_2d().area() * dot(f.points(0), f.get_normal());
			}
			sum /= 3;
			return sum;
		}

		Vector3<T> centroid() const{
			int cnt = 0;
			Vector3<T> center;
			for (const Face<T>& f : *this){
				center += f.center();
			}
			center /= size();
			return center;
		}

	};

}

#endif