// szakdoga.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "vectors.h"
#include "face.h"
using namespace std;

template <class T> std::ostream& operator << (std::ostream& o, const approx::Vector2<T>& v){
	o << v.x << ',' << v.y << '\n';
	return o;
}

template <class T> std::ostream& operator << (std::ostream& o, const approx::Vector3<T>& v){
	o << v.x << ',' << v.y << ',' << v.z << '\n';
	return o;
}

template <class T> std::ostream& operator << (std::ostream& o, const approx::Face<T>& f){
	for (auto vert : f){
		cout << vert << '\n';
	}
	return o;
}



int _tmain(int argc, _TCHAR* argv[])
{
	vector<approx::Vector3<float>> verts { { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 2.0f, 0.0f, 0.0f },
											{ 3.0f, 1.0f, 0.0f }, {2.0f,3.0f,0.0f} },
								   normals { {0.0f,0.0f,-1.0f} };
	approx::Face<float> f(&verts, {0,1,2,3,4}, &normals,0);
	/*for (const approx::Vector3<float>& v : f){
		cout << v << '\n';
	}
	auto f2 = f.to_2d();
	for (const approx::Vector2<float>& v : f2){
		cout << v << "\n";
	}
	cout << f2.area();
	approx::Vector3<float> v{ 1.0, 0, 0 };
	cout << '\n'<< v.x << ',' << v.y << ',' << v.z;*/
	approx::Plane<float> p({ 1, 0, 0 }, 2.0f);
	cout << p.classify_point(verts.front()) << " " << p.classify_point(verts.back()) << "\n";
	
	approx::CutResult<float> result = f.split_by(p);
	cout << f << "\n--------------------------\n" << result.negative << "\n--------------------------\n" << result.positive;
	cout << "\n--------------------------\n" << verts.size() << "\n-------------------\n";
	
	cin.get();
	return 0;
}

