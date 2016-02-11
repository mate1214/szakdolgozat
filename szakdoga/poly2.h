#ifndef POLY2_H_INCLUDED
#define POLY2_H_INCLUDED

#include <vector>
#include <cmath>
#include <algorithm>
#include "vectors.h"
#include "planes.h"

namespace approx{

	template<class T> class Polygon2{
		std::vector<Vector2<T>> pts;
		typedef typename std::vector<Vector2<T>>::const_iterator Iterator;
	public:
		Polygon2(const std::vector<Vector2<T>>& p) : pts(p) {}
		Polygon2(std::vector<Vector2<T>>&& p) : pts(p){}
		Polygon2(const Polygon2&) = default;
		Polygon2(Polygon2&& p) : pts(std::move(p.pts)){}
		template<class Iter> Polygon2(Iter beg, Iter end) : pts(beg, end){}

		Polygon2& operator = (const Polygon2&) = default;
		Polygon2& operator = (Polygon2&& p) { pts.swap(p.pts); }

		Iterator begin() const { return pts.cbegin(); }
		Iterator end() const { return pts.cend(); }

		const std::vector<Vector2<T>>& points() const { return pts; }


		T area() const {
			int n = pts.size()-1;
			T result = 0;
			for (int i = 0; i < n; ++i){
				result += pts[(i + 1)].x*pts[i].y - pts[i].x*pts[(i + 1)].y;
			}
			result += pts.front().x*pts.back().y - pts.back().x* pts.front().y;
			return abs(result/2);
		}



	};

}

#endif