#ifndef POLYGRAPH_H_INCLUDED
#define POLYGRAPH_H_INCLUDED


//
// Keszitette: Toth Mate
// Az itt talalhato muveletek ketto dimenzios pontokbol allo grafokat alakitanak at sokszogekke.
//

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include "poly2.h"


namespace approx{

	template <class T> struct Less2d{ //rendezesi muvelet a map tipussal valo hasznalathoz
		bool operator ()(const Vector2<T>& a, const Vector2<T>& b) const{
			return a.x < b.x || (a.x == b.x && a.y < b.y);
		}
	};

	template <class T> using Graph = std::map < Vector2<T>, std::vector<Vector2<T>>, Less2d<T> > ;

	template <class T> void depth_first_search(const Graph<T>& graph, std::set<Vector2<T>,Less2d<T>>& visited, std::vector<Vector2<T>>& path, std::vector<Polygon2<T>>& result){
		visited.insert(path.back());
		for (const Vector2<T>& pt : graph.at(path.back())){
			if (!visited.count(pt)){//arra a pontra meg nem mentem
				path.push_back(pt);
				visited.insert(pt);
				depth_first_search(graph, visited, path, result);
				path.pop_back();
			}
			else{
				typename std::vector<Vector2<T>>::iterator it = find(path.begin(), path.end()-1,pt);
				//korre talaltam
				if (it != path.end()-1 && it != path.end()-2){ 
					Polygon2<T> tmp(it, path.end());
					//a kesobbi alkalmazasok miatt fix ccw sorrendbe rakom
					if (!tmp.is_ccw()) std::reverse(tmp.begin(), tmp.end());
					result.emplace_back(std::move(tmp));
				}
			}
		}
	}

	template<class T> bool graph_neighbours(const Graph<T>& graph,const Vector2<T>& a, const Vector2<T>& b) {
		auto avec = graph.at(a);
		auto bvec = graph.at(b);
		return
			(std::find(avec.begin(), avec.end(), b) != avec.end() &&
				std::find(bvec.begin(), bvec.end(), a) != bvec.end());
	
	}



	template <class T> std::vector<Polygon2<T>> get_polys(const Graph<T>& graph){
		std::vector<Polygon2<T>> result;
		std::set<Vector2<T>,Less2d<T>> visited;
		std::vector<Vector2<T>> path;
		for (typename Graph<T>::const_iterator it = graph.begin(); it != graph.end(); ++it){
			if (!visited.count(it->first)){
				path.push_back(it->first);
				depth_first_search(graph, visited, path, result);
				path.pop_back();
			}
		}
		return result;
	}


}

#endif