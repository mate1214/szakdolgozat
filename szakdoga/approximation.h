#ifndef APPROXIMATION_H_INCLUDED
#define APPROXIMATION_H_INCLUDED

#include <vector>
#include <utility>
#include <algorithm>
#include "vectors.h"
#include "face.h"
#include "body.h"
#include "targetbody.h"

namespace approx{

	//approximacios tarolo mely az osszes atomot tarolja
	//parameterei:
	// T: az approximacio skalar tipusa, kompatibilisnek kell lennie a Vector3 sablonnal, valamint az atan2 fuggvennyel mukodnie kell
	// AtomType: az atomtipus melyet az approximacio hasznal, publikus interfeszenek meg kell egyeznie a ConvexAtom tipussal,
	//			 az approximacio soran ez a tipus lesz minden atom tipusa
	template <class T, template<class> AtomType> class Approximation{
		const TargetBody<T>* target; //a cel test
		std::vector<Vector3<T>> _vertices, _normals; //pontok es normalisok
		std::vector<Face<T>> _faces; //lapok
		std::vector<AtomType<T>> _atoms; //atomok
		
		int last_cut; //az utolso vagasi muvelet atomja
		AtomType<T>::CutResult cut_res; //az utolso vagas eredmenye

		//iterator tipusok a konnyu bejaras erdekeben
		typedef typename std::vector<AtomType<T>>::iterator Iterator;
		typedef typename std::vector<AtomType<T>>::const_iterator ConstIterator;

		//a kezdoatom egy tengelyek menten felhuzott kocka mely minden iranyban megadott hatarral a test korul helyezkedik el
		//kiszamitasanak ideje linearis a test pontjainak szamaban
		void starting_atom(T border){
			Vector3<T> vmin, vmax;
			for (const Face<T>& f : target->body())
				for (const Vector3<T>& v : f){
				vmin.x = std::min(vmin.x, v.x);
				vmin.y = std::min(vmin.y, v.y);
				vmin.z = std::min(vmin.z, v.z);
				vmax.x = std::max(vmax.x, v.x);
				vmax.y = std::max(vmax.y, v.y);
				vmax.z = std::max(vmax.z, v.z);
				}
			_vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
			_vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
			_vertices.push_back({ vmax.x + border, vmax.y + border, vmin.z - border });
			_vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
			_vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
			_vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
			_vertices.push_back({ vmax.x + border, vmax.y + border, vmax.z + border });
			_vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
			_normals.push_back({ 0,  0,-1});
			_normals.push_back({ 1,  0, 0 });
			_normals.push_back({ 0,  0, 1 });
			_normals.push_back({ -1, 0, 0 });
			_normals.push_back({ 0,  1, 0 });
			_normals.push_back({ 0, -1, 0 });
			_faces.emplace_back(&_vertices, std::vector<int>{0, 1, 2, 3}, &_normals, 0);
			_faces.emplace_back(&_vertices, std::vector<int>{1, 5, 6, 2}, &_normals, 1);
			_faces.emplace_back(&_vertices, std::vector<int>{5, 4, 7, 6}, &_normals, 2);
			_faces.emplace_back(&_vertices, std::vector<int>{4, 0, 3, 7}, &_normals, 3);
			_faces.emplace_back(&_vertices, std::vector<int>{3, 2, 6, 7}, &_normals, 4);
			_faces.emplace_back(&_vertices, std::vector<int>{0, 4, 5, 1}, &_normals, 5);
			_atoms.emplace_back(&_faces, std::vector<int>{0, 1, 2, 3, 4, 5});
		}
		friend class CutResult;
	public:
		class CutResult
		{
			Approximation *app;
		public:
			CutResult(Approximation *a) : app(a){}

			//pozitiv oldali atom
			const AtomType* positive(){
				return static_cast<AtomType<T>*>(a->cut_res.positive.get())
			}

			//negativ oldali atom
			const AtomType* negative(){
				return static_cast<AtomType<T>*>(a->cut_res.negative.get())
			}

			//az eredeti atomot toroljuk beszurjuk az ujakat
			void choose_both(){
				if (a->last_cut != -1){
					a->_atoms.erase(a->_atoms.begin() + a->last_cut);
					a->_atoms.push_back(*static_cast<AtomType<T>*>(a->cut_res.positive.get()));
					a->_atoms.push_back(*static_cast<AtomType<T>*>(a->cut_res.negative.get()));
					a->cut_res.positive.reset();
					a->cut_res.negative.reset();
					a->last_cut = -1;
				}
			}

			//a negativ atomot megtartjuk a pozitivat es az eredetit eldobjuk
			void choose_negative(){
				if (a->last_cut != -1){
					a->_atoms.erase(a->_atoms.begin() + a->last_cut);
					a->_atoms.push_back(*static_cast<AtomType<T>*>(a->cut_res.negative.get()));
					a->cut_res.positive.reset();
					a->cut_res.negative.reset();
					a->last_cut = -1;
				}
			}

			//a pozitiv atomot megtartjuk a negativot es az eredetit eldobjuk
			void choose_positive(){
				if (a->last_cut != -1){
					a->_atoms.erase(a->_atoms.begin() + a->last_cut);
					a->_atoms.push_back(*static_cast<AtomType<T>*>(a->cut_res.positive.get()));
					a->cut_res.positive.reset();
					a->cut_res.negative.reset();
					a->last_cut = -1;
				}
			}

			//a vagast visszavonjuk, az eredeti atom megmarad, a beszurt lapok es pontok torlodnek
			void undo(){
				if (a->last_cut != -1){
					a->cut_res.positive.reset();
					a->cut_res.negative.reset();
					a->_faces.erase(a->_faces.end() - a->cut_res.faces_added, a->_faces.end());
					a->_vertices.erase(a->_vertices.end() - a->cut_res.points_added, a->_vertices.end());
					a->_normals.pop_back();
					a->_normals.pop_back();
					a->last_cut = -1;
				}
			}
		};

		//konstruktor mely az approximalando testre mutato pointert es a kezdo kocka lapjainak kozelseget varja
		//futasideje linearis a celtest pontjainak szamaban
		Approximation(const TargetBody<T>* _target, T _border) : target(_target), last_cut(-1){
			starting_atom(_border);
		}
		
		//masolo ertekadas, gondoskodik a koltoztetesnel fellepo pointer valtasrol
		Approximation& operator = (const Approximation& app){
			target = app.target;
			_vertices = app._vertices;
			_normals = app._normals;
			_faces.clear();
			_atoms.clear();
			for (const Face<T>& f : app._faces){
				_faces.push_back(f.migrate_to(&_vertices, &_normals));
			}
			for (const AtomType<T>& a : app._atoms){
				_atoms.push_back(a.migrate_to(&_faces));
			}
			return *this;
		}

		//mozgato ertekadas, gondoskodnia kell a koltoztetesnel fellepo pointer atallitasrol,
		//ezert bar gyorsabb a masolasnal, megis linearis muveletigenye van
		Approximation& operator = (Approximation&& app){
			target = app.target;
			_vertices = std::move(app._vertices);
			_normals = std::move(app._normals);
			_faces.clear();
			_atoms.clear();
			for (Face<T>&& f : app._faces){
				_faces.push_back(f.migrate_to(&_vertices, &_normals));
			}
			for (AtomType<T>&& a : app._atoms){
				_atoms.push_back(a.migrate_to(&_faces));
			}
			return *this;
		}

		//kezdo atomra mutato iterator
		Iterator begin() { return _atoms.begin(); }
		//utolso utani atomra mutato iterator
		Iterator end() { return _atoms.end(); }
		//kezdo atomra mutato konstans iterator
		ConstIterator begin() const { return _atoms.begin(); }
		//utolso utani atomra mutato konstans iterator
		ConstIterator end() const { return _atoms.end(); }

		//atomok elerese index alapjan
		const AtomType<T>& atoms(size_t ind) const {return _atoms[ind];}
		//pontokat tartalmazo vektor
		const std::vector<Vector3<T>>& vertices() const { return _vertices; }
		//normalvektorokat tartalmazo vektor
		const std::vector<Vector3<T>>& normals() const { return _normals; }
		//konstans eleres a celtestre
		const TargetBody<T>& target_body(){return *target;}
		//az atomszam lekerese
		int size() const { return _atoms.size(); }

		//az adott indexu atom elvagasa megadott sikkal
		//az eredmenykent keletkezett atomok, lapok es pontok bekerulnek a taroloba
		CutResult cut(size_t ind, const Plane<T>& p){
			last_cut = i;
			cut_res = _atoms[ind].cut_by(p);
			return CutResult(this);
		}

		//az adott iterator altal mutatott atom elvagasa megadott sikkal
		AtomType<T>::CutResult cut(Iterator pos, const Plane<T>& p){
			return cut(pos - _atoms.begin(), p);
		}

		

	};

}

#endif