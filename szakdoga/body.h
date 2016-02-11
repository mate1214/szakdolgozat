#ifndef BODY_H_INCLUDED
#define BODY_H_INCLUDED

#include <vector>
#include "face.h"
#include "indexiterator.h"

namespace approx{

	template <class T> class Body{
		std::vector<Face<T>>* faces;
		std::vector<int> ids;

		typedef IndexIterator<Face<T>> FaceIterator;
		typedef ConstIndexIterator<Face<T>> ConstFaceIterator;
	public:
		Body(std::vector<Face<T>>* f, const std::vector<int>& i) : faces(f), ids(i){}
		Body(std::vector<Face<T>>* f, std::vector<int>&& i) : faces(f), ids(i){}
		Body(const Body&) = default;
		Body(Body&& b) : faces(b.faces), ids(std::move(b.ids));

		Body& operator = (const Body&) = default;
		Body& operator =(Body&& b){
			ids = std::move(b.ids);
			faces = b.faces;
			return *this;

		}

		int size() const { return ids.size(); }
		const std::vector<int>& indicies() const { return ids; }
		std::vector<int>& indicies() { return ids; }
		
		Face& faces(size_t i){ return faces->operator[](i); }
		const Face& faces(size_t i) const { return faces->operator[](i); }


		FaceIterator begin() { return FaceIterator(faces, &inds, 0); }
		FaceIterator end() { return FaceIterator(faces, &inds, inds.size()); }

		ConstFaceIterator begin() const { return ConstFaceIterator(faces, &inds, 0); }
		ConstFaceIterator end() const { return ConstFaceIterator(faces, &inds, inds.size()); }

	};

}

#endif