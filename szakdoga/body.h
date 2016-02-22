#ifndef BODY_H_INCLUDED
#define BODY_H_INCLUDED

//
// Keszitette: Toth Mate
// Test tipus mely indexekkel hivatkozik a hatarolo lapjaira, valamint kenyelmi fuggvenyeket nyujt
// 

#include <vector>
#include "face.h"
#include "indexiterator.h"

namespace approx{

	//Parametere a skalar tipus ami felett ertelmezzuk a vektorteret
	template <class T> class Body{
	protected:
		std::vector<Face<T>>* _faces;
		std::vector<int> inds;

		typedef IndexIterator<Face<T>> FaceIterator;
		typedef ConstIndexIterator<Face<T>> ConstFaceIterator;
	public:
		Body(std::vector<Face<T>>* f, const std::vector<int>& i) : _faces(f), inds(i){}
		Body(std::vector<Face<T>>* f, std::vector<int>&& i) : _faces(f), inds(i){}
		Body(const Body&) = default;
		Body(Body&& b) : _faces(b._faces), inds(std::move(b.inds)){}

		Body& operator = (const Body& b) = default;
		Body& operator =(Body&& b){
			inds = std::move(b.inds);
			_faces = b._faces;
			return *this;
		}

		Body migrate_to(std::vector<Face<T>>* fcs){
			return Body(fcs, std::move(inds));
		}

		Body migrate_to(std::vector<Face<T>>* fcs) const {
			return Body(fcs, inds);
		}


		int size() const { return inds.size(); }
	    int indicies(size_t i) const { return inds[i]; }
		const std::vector<int>& indicies() const { return inds; }
		std::vector<int>& indicies() { return inds; }
		Face<T>& faces(size_t i){ return _faces->operator[](i); }
		const Face<T>& faces(size_t i) const { return _faces->operator[](i); }

		FaceIterator begin() { return FaceIterator(_faces, &inds, 0); }
		FaceIterator end() { return FaceIterator(_faces, &inds, inds.size()); }

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