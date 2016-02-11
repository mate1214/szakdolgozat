#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

#include <utility>
#include <algorithm>
#include <vector>
#include "vectors.h"
#include "planes.h"
#include "poly2.h"
#include "indexiterator.h"

namespace approx{

	template <class T> class Face;

	template <class T> class CutResult{
		Face<T> positive_side, negative_side;
		Vector3<T> *cut1, *cut2;
	};

	template <class T> class Face{
		std::vector< Vector3<T> > *vecs, *normals;

		std::vector<int> inds;
		int normal_id;

		void calc_normal(){
			Vector3<T> n = cross(points(0) - points(1), points(2) - points(1));
			n.normalize();
			normal_id = normals->size();
			normals->push_back(n);
		}

		typedef ConstIndexIterator<Vector3<T>> VertexIterator;

	public:
		Face(std::vector< Vector3<T> >* vertices, const std::vector<int>& ids, std::vector< Vector3<T> >* _normals) : vecs(vertices), inds(ids), normals(_normals){ calc_normal(); }
		Face(std::vector< Vector3<T> >* vertices, std::vector<int>&& ids, std::vector< Vector3<T> >* _normals) : vecs(vertices), inds(ids), normals(_normals){ calc_normal(); }
		Face(std::vector< Vector3<T> >* vertices, const std::vector<int>& ids, std::vector< Vector3<T> >* normals, int n_id) : vecs(vertices), inds(ids), normals(normals), normal_id(n_id){}
		Face(std::vector< Vector3<T> >* vertices, std::vector<int>&& ids, std::vector< Vector3<T> >* normals, int n_id) : vecs(vertices), inds(ids), normals(normals), normal_id(n_id){}
		Face(std::vector< Vector3<T> >* vertices, const std::vector<int>& ids, const Vector3<T>& normal, std::vector< Vector3<T> >* _normals)
			: vecs(vertices), inds(ids), normals(_normals), normal_id(normals->size()){
			normals->push_back(normal);
		}
		Face(std::vector< Vector3<T> >* vertices, std::vector<int>&& ids, const Vector3<T>& normal, std::vector< Vector3<T> >* _normals)
			: vecs(vertices), inds(ids), normals(_normals), normal_id(normals->size()){
			normals->push_back(normal);
		}

		Face(const Face&) = default;
		Face(Face&& f) : vecs(f.vecs), normals(f.normals), inds(std::move(f.inds)), normal_id(f.normal_id){}


		Face& operator = (const Face& f) = default;

		Face& operator = (Face&& f){
			inds = std::move(f.inds);
			vecs = f.vecs;
			normals = f.normals;
			normal_id = f.normal_id;
			return *this;
		}

		bool operator == (const Face& other) const{
			return size() == other.size() && std::equal(begin(), end(), other.begin());
		}

		const std::vector<int>& indicies() const { return inds; }
		std::vector<int>& indicies(){ return inds; }
		int normal_index() const { return normal_id; }
		size_t size() const { return inds.size() }


		const Vector3<T>& get_normal() const { return normals->operator[](normal_id); }
		const Vector3<T>& points(size_t ind) const { return vecs->operator[](inds[ind]); }

		Plane<T> to_plane() const { return Plane<T>(get_normal(), points(0)); }

		VertexIterator begin() const { return VertexIterator(vecs, &inds, 0); }
		VertexIterator end() const { return VertexIterator(vecs, &inds, inds.size()); }

		Polygon2<T> to_2d(const Vector3<T>& x, const Vector3<T>& y) const {
			std::vector<Vector2<T>> pts;
			for (const Vector3<T>& v : *this){
				pts.push_back({ dot(v, x), dot(v, y) });
			}
			return Polygon2<T>(std::move(pts));
		}

		Polygon2<T> to_2d() const {
			Vector3<T> y = (points(1) - points(0)).normalized();
			Vector3<T> x = cross(get_normal(), y).normalized();
			return to_2d(x, y);
		}

		std::pair<Face<T>, Face<T>> split_by(const Plane<T>& p) {
			T sign1 = p.classify_point(points(0)), sign2;
			int n = size();
			std::vector<int> pos, neg;
			for (int i = 0; i < n && found < 2; ++i){
				sign2 = p.classify_point(points((i + 1) % n));
				float sign = sign1*sign2;
				if (sign1 < 0){
					neg.push_back(inds[i]);
					if (sign2 > 0){
						T div = abs(sign1 / (abs(sign1) + abs(sign2)));
						neg.push_back(vecs->size());
						pos.push_back(neg.back());
						vecs->push_back(div*points(i) + (1 - div)*points((i + 1) % n));
					}
				}
				else if (sign1 > 0){
					pos.push_back(inds[i]);
					if (sign2 < 0){
						T div = abs(sign1 / (abs(sign1) + abs(sign2)));
						neg.push_back(vecs->size());
						pos.push_back(neg.back());
						vecs->push_back(div*points(i) + (1 - div)*points((i + 1) % n));
					}
				}
				else{
					pos.push_back(inds[i]);
					neg.push_back(inds[i]);
				}
				sign1 = sign2;
			}
			return{ Face<T>(vecs, std::move(pos), normals, normal_id),
				Face<T>(vecs, std::move(neg), normals, normal_id) };
		}


		std::pair<Face<T>, Face<T>> split_by(const Plane<T>& p, std::vector<Vector3<T>>* target_vecs, std::vector<Vector3<T>>* target_normals) {
			
			if (target_vecs == vecs && target_normals == normals) return split_by(p);

			T sign1 = p.classify_point(points(0)), sign2;
			int n = size();
			std::vector<int> pos, neg;
			for (int i = 0; i < n && found < 2; ++i){
				sign2 = p.classify_point(points((i + 1) % n));
				float sign = sign1*sign2;
				target_vecs->push_back(points(i));
				if (sign1 < 0){
					neg.push_back(inds[i]);
					if (sign2 > 0){
						T div = abs(sign1 / (abs(sign1) + abs(sign2)));
						neg.push_back(target_vecs->size());
						pos.push_back(neg.back());
						target_vecs->push_back(div*points(i) + (1 - div)*points((i + 1) % n));
					}
				}
				else if (sign1 > 0){
					pos.push_back(inds[i]);
					if (sign2 < 0){
						T div = abs(sign1 / (abs(sign1) + abs(sign2)));
						neg.push_back(target_vecs->size());
						pos.push_back(neg.back());
						target_vecs->push_back(div*points(i) + (1 - div)*points((i + 1) % n));
					}
				}
				else{
					pos.push_back(inds[i]);
					neg.push_back(inds[i]);
				}
				sign1 = sign2;
			}
			target_normals->push_back(get_normal());
			int n_id = target_normals->size() - 1;
			return{ Face<T>(target_vecs, std::move(pos), target_normals, n_id),
				Face<T>(target_vecs, std::move(neg), target_normals, n_id) };
		}

	};
}




#endif