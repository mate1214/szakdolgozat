#ifndef CONVEXATOM_H_INCLUDED
#define CONVEXATOM_H_INCLUDED

//
// Keszitette: Toth Mate
// Konvex testeket reprezentalo adatszerkezet melyet az approximacio atomjaihoz hasznalunk.
// Kepes a sajat terfogatanak es fontosabb geometriai attributumainak kiszamolasara,
// valamint eretelmezve van rajta a sikkal vagas muvelete
//

#include <utility>
#include <algorithm>
#include <memory>
#include <map>
#include <set>
#include "body.h"
#include "planes.h"

namespace approx{


	//parameterezheto barmely Vector3-al kompatibilis skalar tipussal, melyre direkt,
	//vagy implicit konverzioval ertelmezve van az atan2 fuggveny
	template <class T> class ConvexAtom : public Body < T > {

		struct Less{ //rendezesi muvelet a map tipussal valo hasznalathoz
			bool operator ()(const Vector3<T>& a,const Vector3<T>& b) const{
				return a.x < b.x ||
					(a.x == b.x && a.y < b.y) ||
					(a.x == b.x && a.y == b.y && a.z <b.z);
			}
		};
	public:
		ConvexAtom(std::vector<Face<T>>* f, const std::vector<int>& i): Body<T>(f,i){}
		ConvexAtom(std::vector<Face<T>>* f, std::vector<int>&& i) : Body<T>(f, i){}

		struct CutResult{
			std::shared_ptr<ConvexAtom<T>> positive, negative;
			int points_added;
			int faces_added;
		};

		// az atom elvagasa adott sikkal
		// a muveletigeny linearis a lapok szamaban valamint valodi vagasnal linearitmikus a vagasi pontok szamaban
		CutResult cut_by(const Plane<T>& p) {
			std::vector<int> pt_ids,pos_faces,neg_faces;
			int faces_added=0,pts_added=0;
			Vector3<T> avg_pt;
			std::vector<Vector3<T>>& vc = *faces(0).vertex_container();
			std::map<Vector3<T>, int,Less> ptbuffer;
			for (int i = 0; i < size();++i){ //vegegiteralok minden lapon es elvagom oket a sikkal, kezelve a hamis vagasokat
				Face<T>::CutResult cut = faces(i).cut_by(p,ptbuffer);
				if (cut.pt_inds.size() < cut.positive.size() && cut.pt_inds.size() < cut.negative.size()){ //valodi vagas tortent, mindket oldalon valid sokszog all
					pts_added += cut.points_added; //osszegzem a hozzaadott uj pontokat
					_faces->push_back(cut.negative); //a kapott lapokat szortirozom az uj sokszogekbe
					_faces->push_back(cut.positive); 
					faces_added += 2; //2 uj lap kerult a taroloba
					pos_faces.push_back(_faces->size() - 1);
					neg_faces.push_back(_faces->size() - 2);
					pt_ids.push_back(cut.pt_inds.front());
					pt_ids.push_back(cut.pt_inds.back());
					avg_pt += vc[pt_ids[pt_ids.size() - 1]];
					avg_pt += vc[pt_ids[pt_ids.size() - 2]];
				}
				else{//a vagas nem krealt uj lapokat, de lehet hogy egy el vagy pont raesik
					if (cut.pt_inds.size()){//raeso pont vagy pontok
						pt_ids.push_back(cut.pt_inds.front());
						pt_ids.push_back(cut.pt_inds.back());
					}
					if (cut.pt_inds.size() < cut.positive.size()){ //a pozitiv oldalra kerul az egesz lap
						pos_faces.push_back(indicies(i));
					}
					else{ //a negativ oldalra kerul az egesz lap
						neg_faces.push_back(indicies(i));
					}
				}
			}
			if (pt_ids.size()){ //a vagasnak volt hatasa, keletkezett lap
				avg_pt /= pt_ids.size(); //kozeppont szamitasa
				// a kapott pontokat a sulypont korul a lap sikjan forgasszog szerint rendezem
				//a kapott lap konvex, tehat az ismetlodesek kiszurese utan megkapom a megfelelp sokszoget
				Vector3<T> vx = vc[pt_ids.front()] - avg_pt, //x vektor a sikon
						   vy = cross(p.normal(),vx); //y vektor a sikon
				//rendezes a tangensbol visszanyert szog alapjan
				std::sort(pt_ids.begin(), pt_ids.end(), [&](int a, int b){
					Vector3<T> v1 = vc[a] - avg_pt, v2 = vc[b] - avg_pt;
					T x1 = dot(v1, vx), y1 = dot(v1,vy),
						x2 = dot(v2, vx), y2 = dot(v2,vy);
					return atan2(x1, y1) < atan2(x2, y2);
				});
				//a rendezett pontokbol minden masodik egyedi bekerul a sokszogre
				vector<int> new_fc{pt_ids[0]};
				for (int i = 2; i < pt_ids.size(); i += 2){
					if(vc[pt_ids[i]] != vc[new_fc.back()]) new_fc.push_back(pt_ids[i]); //egy csucsnal lehet hogy tobb el osszefut, nem akarunk egymas utan ugyanolyan pontokat
				}
				_faces->emplace_back(faces(0).vertex_container(),new_fc, faces(0).normal_container(),p.normal());
				std::reverse(new_fc.begin(),new_fc.end());
				_faces->emplace_back(faces(0).vertex_container(), std::move(new_fc), faces(0).normal_container(),p.normal()*-1);
				faces_added+=2;
				neg_faces.push_back(_faces->size() - 2);
				pos_faces.push_back(_faces->size() - 1);
			}
			return{ std::make_shared<ConvexAtom<T>>(_faces,std::move(pos_faces)),
					std::make_shared<ConvexAtom<T>>(_faces,std::move(neg_faces)),
					pts_added,
					faces_added};
		}

		bool point_inside(const Vector3<T>& pt) const {
			for (const Face<T>& f : *this){
				if (f.to_plane().classify_point(pt) > 0) return false;
			}
			return true;
		}

		int vertex_count(const Body<T>& b) const{
			int cnt = 0;
			std::set<Vector3<T>> pts;
			for (const Face<T>& face : b){
				for (const Vector3<T>& p : face){
					if (point_inside(p)) pts.insert(p);
				}
			}
			return pts.size();
		}

		//metszetterfogat szamitas a 
		T intersection_volume(const Body<T>& b) const { //TODO: ez nagyon nincs megirva
			/*std::vector<Vector3<T>> tmp_points,tmp_normals;
			std::vector<Face<T>> faces1, faces2;
			std::vector<std::pair<int, int>> edges;
			//vegigmegyek ennek az atomnak az elein, mindegyikkel elmetszem a testet
			Body<T>::ConstFaceIterator fiter = begin();
			Plane<T> p = fiter->to_plane();
			for (const Face<T>& f : b){
				Face<T>::CutResult cut = f.cut_into();
			}
			return 0;*/
		}
	};

}

#endif