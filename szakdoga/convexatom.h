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
		CutResult<T> cut_by(const Plane<T>& p){
			//TODO
		}
	};

}

#endif