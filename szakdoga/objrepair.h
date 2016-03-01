#ifndef OBJREPAIR_H_INCLUDED
#define OBJREPAIR_H_INCLUDED

#include <vector>
#include <algorithm>
#include <functional>
#include "vectors.h"
#include "indexiterator.h"
//
// Keszitette: Toth Mate
// Ez a header fajl ellatja az egymashoz kozeli pontokbol allo sorozatok javitasat.
// Felhasznalhato faljok betoltesenel illetve barmely algoritmusban mely folyamatosan gyujti a pontokat.
//

namespace approx{

	template <class T> class RepairVector{
		std::vector<Vector3<T>> vecs;
		std::vector<int> ind_map;
		typedef ConstIndexIterator<T> ConstIterator;
		typedef IndexIterator<T> Iterator;
		std::function<T(const Vector3<T>&, const Vector3<T>&)> dist_fun;
		T eps;
		static T def_dist(const Vector3<T>& a, const Vector3<T>& b){
			return (a - b).length();
		}

	public:
		RepairVector(T epsilon) : dist_fun(def_dist),eps(epsilon){}

		int transform_index(int ind) const {
			return ind_map[id];
		}

		std::vector<int> transform_range(const std::vector<int>& inds) const {
			std::vector<int> res;
			res.reserve(inds.size());
			for (int i : inds){
				res.push_back(ind_map[i]);
			}
			return res;
		}

		Vector3<T> operator[](int ind) const {
			return vecs[ind_map[ind]];
		}

		int size() const {
			return ind_map.size();
		}

		ConstIterator begin() const { return ConstIterator(&vecs,&ind_map,0); }
		ConstIterator end()   const { return ConstIterator(&vecs, &ind_map, size()); }
		Iterator begin() { return Iterator(&vecs, &ind_map, 0); }
		Iterator end()   { return Iterator(&vecs, &ind_map, size()); }

		void push_back(const Vector3<T>& v){
			int i = 0,n=size();
			while (i < n && eps < dist_fun(v, vecs[i])){ ++i; }
			ind_map.push_back(i);
			if (i == n) vecs.push_back(v);
		}

		operator std::vector<Vector3<T>>() const {
			return std::vector<Vector3<T>>(begin(), end());
		}

		std::vector<Vector3<T>> needed_vecs() const {
			return vecs;
		}

	};

}

#endif