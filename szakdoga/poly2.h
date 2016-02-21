#ifndef POLY2_H_INCLUDED
#define POLY2_H_INCLUDED

//
// Keszitette Toth Mate
// Kettodimenzios sokszog sablon. A haromdimenzios lappal ellentetben minden pontjat helyben tarolja.
// Feladata a teruletenek kiszamitasa, valamint a vagas implementalasa.

#include <vector>
#include <cmath>
#include <algorithm>
#include "vectors.h"
#include "planes.h"

namespace approx{

	// Sokszog tipus 2 dimenzioban.
	// Tetszoleges skalar tipussal paramezerezheto ami megfelel a Vector2 elvarasainak.
	template<class T> class Polygon2{
		std::vector<Vector2<T>> pts;
		typedef typename std::vector<Vector2<T>>::const_iterator ConstIterator;
		typedef typename std::vector<Vector2<T>>::iterator Iterator;
	public:
		//vektorral megadott konstruktorok
		Polygon2(const std::vector<Vector2<T>>& p) : pts(p) {}
		Polygon2(std::vector<Vector2<T>>&& p) : pts(p){}
		//masolo, mozgato valamint iteratorbol masolo konstruktorok
		Polygon2(const Polygon2&) = default;
		Polygon2(Polygon2&& p) : pts(std::move(p.pts)){}
		template<class Iter> Polygon2(Iter beg, Iter end) : pts(beg, end){}

		//ertekadas masolasra es mozgatasra
		Polygon2& operator = (const Polygon2&) = default;
		Polygon2& operator = (Polygon2&& p) { pts.swap(p.pts); }

		//csak olvashato iteratorok a pontok bejarasara
		ConstIterator begin() const { return pts.cbegin(); }
		ConstIterator end() const { return pts.cend(); }
		//irhato es olvashato iteratorok a pontokon
		Iterator begin() { return pts.begin(); }
		Iterator end() { return pts.end(); }

		//pontok szama
		int size() const { return pts.size(); }
		//pontokat tartalmazo vektor konstans elerese
		const std::vector<Vector2<T>>& points() const { return pts; }
		//i. pont elerese
		const Vector2<T>& points(size_t i) const { return pts[i]; }
		Vector2<T>& points(size_t i) { return pts[i]; }
		
		
		//teruletet kiszamito metodus, a pontossaga a megadott skalartol fugg
		//kepes kezelni CW es CCW sorrendben megadott alakzatot is
		T area() const {
			int n = pts.size()-1;
			T result = 0;
			for (int i = 0; i < n; ++i){
				result += pts[(i + 1)].x*pts[i].y - pts[i].x*pts[(i + 1)].y;
			}
			result += pts.front().x*pts.back().y - pts.back().x* pts.front().y;
			return abs(result / static_cast<T>(2));
		}

		//vagasi eredmeny tipus
		struct CutResult{
			Polygon2 negative, //a vago egyenes negativ oldalara eso resz
					 positive; //a vago egyenes pozitiv oldalara eso resz
		};

		//a megadott vonallal elvagva kapott sokszogek szamitasa
		//feltetelezzuk, hogy a sokszog konvex, ebbol kovetkezoen az eredmeny is az
		//muveletigeny linearis a csucspontok szamaban
		CutResult split_by(const Line<T> l) const {
			std::vector<Vector2<T>> pos, neg;
			T sign1 = l.classify_point(pts[0]); //az aktualisan vizsgalt pont elhelyezkedese a sikhoz kepest
			const int n = size();
			for (int i = 0; i < n;++i){ //vegigiteralok a pontok kozott
				T sign2 = l.classify_point(pts[(i + 1) % n]); //a kovetkezo pont betajolasa
				if (sign1 < 0){//a pont a negativ oldalra esik
					neg.push_back(pts[i]);
					if (sign2 > 0){//a kovetkezo pont pozitiv, a ketto kozott vagas keletkezik
						T div = abs(sign1 / (abs(sign1)+abs(sign2)));
						Vector2<T> cpt = (1 - div)*pts[i] + div*pts[(i + 1) % n];
						pos.push_back(cpt);
						neg.push_back(cpt);
					}
				}
				else if (sign1 > 0){//a pont a pozitiv oldalra esik
					pos.push_back(pts[i]);
					if (sign2 < 0){//a kovetkezo pont negativ, a ketto kozott vagas keletkezik
						T div = abs(sign1 / (abs(sign1) + abs(sign2)));
						Vector2<T> cpt = (1 - div)*pts[i] + div*pts[(i + 1) % n];
						pos.push_back(cpt);
						neg.push_back(cpt);
					}
				}
				else{ //a pont a vonalon van, ekkor mindket oldalhoz tartozik
					pos.push_back(pts[i]);
					neg.push_back(pts[i]);
				}
				sign1 = sign2; //a kovetkezo pont elojelet mar kiszamoltuk
			}
			return{ Polygon2<T>(std::move(neg)), Polygon2<T>(std::move(pos))};
		}

	};

}

#endif