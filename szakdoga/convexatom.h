#ifndef CONVEXATOM_H_INCLUDED
#define CONVEXATOM_H_INCLUDED

#include <utility>
#include <algorithm>
#include <memory>
#include "body.h"
#include "planes.h"

#include <iostream> //TODO: debug

namespace approx{



	template <class T> class ConvexAtom : public Body < T > {
	public:
		ConvexAtom(std::vector<Face<T>>* f, const std::vector<int>& i): Body<T>(f,i){}
		ConvexAtom(std::vector<Face<T>>* f, std::vector<int>&& i) : Body<T>(f, i){}

		struct CutResult{
			std::shared_ptr<ConvexAtom<T>> positive, negative;
			int points_added;
			int faces_added;
		};


		CutResult cut_by(const Plane<T>& p){
			std::vector<int> pt_ids,pos_faces,neg_faces;
			int faces_added=0,pts_added=0;
			Vector3<T> avg_pt;
			std::vector<Vector3<T>>& vc = *face(0).vertex_container();
			for (int i = 0; i < size();++i){
				Face<T>::CutResult cut = face(i).split_by(p);
				if (cut.pt_inds.size() < cut.positive.size() && cut.pt_inds.size() < cut.negative.size()){ //valodi vagas tortent
					pts_added += cut.points_added;
					faces->push_back(cut.negative);
					faces->push_back(cut.positive);
					faces_added += 2;
					pos_faces.push_back(faces->size() - 1);
					neg_faces.push_back(faces->size() - 2);
					pt_ids.push_back(cut.pt_inds.front());
					pt_ids.push_back(cut.pt_inds.back());
					avg_pt += vc[pt_ids[pt_ids.size() - 1]];
					avg_pt += vc[pt_ids[pt_ids.size() - 2]];
				}
				else{
					//TODO: a pontosan a sikon fekvo lapoknal gaz van, de a konvexitasra hivatkozva kinn a lapszambol ra lehet jonni hogy rossz a vagas
					//TODO: ha az atomon kivuli ellel akarunk vagni akkor fart
					if (cut.pt_inds.size()){//raeso elek
						pt_ids.push_back(cut.pt_inds.front());
						pt_ids.push_back(cut.pt_inds.back());
					}
					if (cut.pt_inds.size() < cut.positive.size()){
						pos_faces.push_back(indicies(i));
					}
					else{
						neg_faces.push_back(indicies(i));
					}
				}
			}
			if (pt_ids.size()){
				avg_pt /= pt_ids.size(); //kozeppont
				Vector3<T> v0 = vc[pt_ids.front()] - avg_pt;
				//TODO: rossz a rendezes
				std::sort(pt_ids.begin(), pt_ids.end(), [&](int a, int b){
					Vector3<T> v1 = vc[a] - avg_pt, v2 = vc[b] - avg_pt;
					T x1 = dot(v1, v0), y1 = cross(v1, v0).length(),
						x2 = dot(v2, v0), y2 = cross(v2, v0).length();
					return atan2(x1, y1) < atan2(x2, y2);
				});

				for (int ix : pt_ids) { cout << vc[ix] << "\n"; }

				vector<int> new_fc;
				for (int i = 0; i < pt_ids.size(); i += 2){
					new_fc.push_back(pt_ids[i]);
				}
				faces->emplace_back(face(0).vertex_container(),new_fc, face(0).normal_container(),p.normal());
				std::reverse(new_fc.begin(),new_fc.end());
				faces->emplace_back(face(0).vertex_container(), std::move(new_fc), face(0).normal_container(),p.normal()*-1);
				faces_added+=2;
				neg_faces.push_back(faces->size() - 2);
				pos_faces.push_back(faces->size() - 1);
				//TODO? garbage collection valamikor valahol a duplazasok miatt
			}
			return{ std::make_shared<ConvexAtom<T>>(faces,std::move(pos_faces)),
					std::make_shared<ConvexAtom<T>>(faces, std::move(neg_faces)),
					pts_added,
					faces_added};
		}

		T intersection_volume(const Body<T>& b){
			//TODO
			return 0;
		}
	};

}

#endif