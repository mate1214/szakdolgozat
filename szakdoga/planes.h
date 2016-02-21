#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

//
// Keszitette Toth Mate
// 2 dimenzios vonalak es 3 dimenzios sikok reprezentelesara kepes sablonok.
// A sikok feladata az adatszerkezetekben a ter ketteosztasa, valamint a pontok osztalyozasa.
// Mivel a 2 es 3 dimenzios sikok ugyanazokat a feladatokat latjak el, kozos sablonon, a hipersikon alapulnak.
//


#include "vectors.h"

namespace approx{

	//Hipersik tetszoleges dimenziohoz 
	template <class Vector,class T> class HyperPlane{
		Vector n; //normalvektor
		T dist; //elojeles tavolsag az origotol a normalvektor iranyaban
	public:
		//sik egy normalissal es egy ponttal megadva
		HyperPlane(const Vector& _normal, const Vector& _point) : n(_normal.normalized()),dist(dot(n,_point)){}
		//sik egy normalissal es elojeles tavolsaggal megadva
		HyperPlane(const Vector& _normal, T distance) : n(_normal.normalized()), dist(distance){}
		//masolo konstruktor
		HyperPlane(const HyperPlane&) = default;

		HyperPlane& operator = (const HyperPlane&) = default;

		//elojeles tavolsag az origotol a normalvektor iranyaban
		T signed_distance() const { return dist; }
		
		//normalvektor lekerdezes
		Vector& normal() { return normal; }
		Vector normal() const{ return n; }

		//egy pont amely a sikon helyezkedik el
		Vector example_point() const{
			return n*dist;
		}

		//a pont elojeles tavolsaga a siktol, pozitiv ha a normalvektor iranyaban a sikon tul esik es negativ kulonben
		T classify_point(const Vector& p) const {
			return dot(p, n) - dist;
		}

	};


	template <class T> using Plane = HyperPlane < Vector3<T>,T >;
	template <class T> using Line = HyperPlane < Vector2<T>, T > ;
}

#endif