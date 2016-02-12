#ifndef CONVEXATOM_H_INCLUDED
#define CONVEXATOM_H_INCLUDED

#include <utility>
#include "body.h"
#include "planes.h"


namespace approx{
	
	template <class T> class ConvexAtom : public Body < T > ;
	
	template <class T> class CutResult{};


	template <class T> class ConvexAtom : public Body < T > {
	public:
		ConvexAtom(std::vector<Face<T>>* f, const std::vector<int>& i): Body<T>(f,i){}
		ConvexAtom(std::vector<Face<T>>* f, std::vector<int>&& i) : Body<T>(f, i){}

		CutResult<T> cut_by(const Plane<T>& p){
			//TODO
		}

		T intersection_volume(const Body<T>& b){
			//TODO
		}
	};

}

#endif