// szakdoga.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "vectors.h"
#include "face.h"
#include "convexatom.h"
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
	o << "\n----------------------------\n";
	for (auto vert : f){
		o << vert << '\n';
	}
	return o;
}

struct Less{
	bool operator ()(const approx::Vector3<float>& a, const approx::Vector3<float>& b) const{
		return a.x < b.x ||
			   (a.x==b.x && a.y < b.y) ||
			   (a.x==b.x && a.y == b.y && a.z <b.z);
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	vector<approx::Vector3<float>> vertices { { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 2.0f, 0.0f, 0.0f },
	{ 3.0f, 1.0f, 0.0f }, {2.0f,3.0f,0.0f} },
								   normals { {0.0f,0.0f,-1.0f} };
	vector<approx::Face<float>> faces;
	/*approx::Face<float> f(&vertices, {0,1,2,3,4}, &normals,0);
	for (const approx::Vector3<float>& v : f){
		cout << v << '\n';
	}
	auto f2 = f.to_2d();
	for (const approx::Vector2<float>& v : f2){
		cout << v << "\n";
	}
	cout << f2.area();
	approx::Vector3<float> v{ 1.0, 0, 0 };
	cout << '\n'<< v.x << ',' << v.y << ',' << v.z;
	approx::Plane<float> p({ 1, 0, 0 }, 2.0f);
	cout << p.classify_point(vertices.front()) << " " << p.classify_point(vertices.back()) << "\n";
	
	approx::Face<float>::CutResult result = f.split_by(p);
	cout << f << "\n--------------------------\n" << result.negative << "\n--------------------------\n" << result.positive;
	cout << "\n--------------------------\n" << vertices.size() << "\n-------------------\n";
	cout << result.positive.size();*/
	vertices.clear();
	normals.clear();

	approx::Vector3<float> vmin(1,1,1), vmax(3,2,2);
	float border = 0;
	//TODO: CW CCW dolog egyeztet
	vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
	vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
	vertices.push_back({ vmax.x + border, vmax.y + border+2, vmin.z - border });
	vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
	vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
	vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
	vertices.push_back({ vmax.x + border, vmax.y + border+2, vmax.z + border });
	vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
	normals.push_back({ 0, 0, -1 });
	normals.push_back({ 1, 0, 0 });
	normals.push_back({ 0, 0, 1 });
	normals.push_back({ -1, 0, 0 });
	//normals.push_back({ 0, 1, 0 });
	normals.push_back({ -sqrt(2.0f) / 2, sqrt(2.0f) / 2, 0.0f });
	normals.push_back({ 0, -1, 0 });
	faces.emplace_back(&vertices, std::vector<int>{ 0, 1, 2, 3 }, &normals, 0);
	faces.emplace_back(&vertices, std::vector<int>{ 1, 5, 6, 2 }, &normals, 1);
	faces.emplace_back(&vertices, std::vector<int>{ 5, 4, 7, 6 }, &normals, 2);
	faces.emplace_back(&vertices, std::vector<int>{ 4, 0, 3, 7 }, &normals, 3);
	faces.emplace_back(&vertices, std::vector<int>{ 3, 2, 6, 7 }, &normals, 4);
	faces.emplace_back(&vertices, std::vector<int>{ 0, 4, 5, 1 }, &normals, 5);
	approx::ConvexAtom<float> atom(&faces, std::vector < int > {0, 1, 2, 3, 4, 5});
	auto cut = atom.cut_by(approx::Plane<float>({1,0,0},2.0f));
/*	for (auto& f : *cut.negative.get()){
		cout << f;
	}
	cout << "=====================================\n";
	for (auto& f : *cut.positive.get()){
		cout << f;
	}
	cout << "==========================" << faces.size()<<"\n";
	for (auto x : vertices){ cout << x << "\n"; }*/
	cout << atom.volume() << " " << cut.positive->volume() << " " << cut.negative->volume();
	cout << "\n" << vertices.size();
	cin.get();
	return 0;
}

