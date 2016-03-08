// szakdoga.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vectors.h"
#include "face.h"
#include "convexatom.h"
#include "targetbody.h"
#include "approximation.h"
#include "conversion.h"
#include "polygraph.h"
#include "geoios.h"
#include "objio.h"

using namespace std;
using namespace approx;



struct Less{
	bool operator ()(const approx::Vector3<float>& a, const approx::Vector3<float>& b) const{
		return a.x < b.x ||
			   (a.x==b.x && a.y < b.y) ||
			   (a.x==b.x && a.y == b.y && a.z <b.z);
	}
};


void cut_eq_test(){
	vector<approx::Vector3<float>>
		vertices{ { 1.0000456f, 0.0f, 0.0f },
				  { sqrt(2.0f), sqrt(3.0f), sqrt(11.0f) },
				  {(sqrt(2.0f)+sqrt(3.17f))/2.0f,sqrt(3.0f)+sqrt(8.767f) ,sqrt(10.65f)+sqrt(1.5678f)},
				  {sqrt(6.56f),sqrt(3.0f)+sqrt(6.789f),-0.564f} },
		normals{ cross(vertices[1] - vertices[0], vertices[2] - vertices[0]).normalized(),
				 cross(vertices[3] - vertices[1], vertices[2] - vertices[1]).normalized() };
	approx::Face<float>
		face1(&vertices, {0,1,2},&normals,0),
		face2(&vertices, {1,3,2},&normals,1);
	approx::Plane<float> plane({0,1,0},sqrt(3.0f)+sqrt(5.456f));
	auto cut1 = face1.cut_by(plane);
	auto cut2 = face2.cut_by(plane);
	cout << vertices[cut1.pt_inds.front()] << "\n"
		<< vertices[cut1.pt_inds.back()] << "\n"
		<< vertices[cut2.pt_inds.front()] << "\n"
		<< vertices[cut2.pt_inds.back()] << "\n";
	cout << boolalpha << (vertices[cut1.pt_inds.front()] == vertices[cut2.pt_inds.back()]);
}

void poly_graph_test(){
	Graph<float> gr;
	gr[{1, 1}].push_back({2,1.043f});
	gr[{ 2, 1.043f }].push_back({1,1});

	gr[{ 2.5f, 2.5f }].push_back({ 2, 1.043f });
	gr[{ 2, 1.043f }].push_back({ 2.5f, 2.5f });

	gr[{ 2.5f, 2.5f }].push_back({ 3,2 });
	gr[{ 3, 2 }].push_back({ 2.5f,2.5f });
	
	gr[{ 2.5f, 2.5f }].push_back({ 1,5});
	gr[{ 1, 5 }].push_back({ 2.5f, 2.5f });
	
	gr[{ 1, 5 }].push_back({ 7, 12 });
	gr[{ 7, 12 }].push_back({ 1, 5 });

	gr[{ 7, 12 }].push_back({ 5, 16 });
	gr[{ 5, 16 }].push_back({ 7, 12 });

	gr[{ 5, 16 }].push_back({ 1, 5 });
	gr[{ 1, 5 }].push_back({ 5, 16 });

	gr[{ 1, 5 }].push_back({ 1, 1 });
	gr[{ 1, 1 }].push_back({ 1, 5 });


	gr[{ 1, 5 }].push_back({ -1.5f, 4 });
	gr[{ -1.5f, 4 }].push_back({ 1, 5 });

	gr[{ -1.5f, 4 }].push_back({ -2, 8 });
	gr[{ -2, 8 }].push_back({ -1.5f, 4 });

	gr[{ 1, 5 }].push_back({ -2, 8 });
	gr[{ -2, 8 }].push_back({ 1, 5 });

	gr[{ 10, 10 }].push_back({ 15, 15 });
	gr[{ 15, 15 }].push_back({ 10, 10 });

	gr[{ 10, 10 }].push_back({ 15, 10 });
	gr[{ 15, 10 }].push_back({ 10, 10 });

	gr[{ 15, 10 }].push_back({ 15, 15 });
	gr[{ 15, 15 }].push_back({ 15, 10 });


	auto ls = get_polys(gr);

	for (const auto& p : ls){
		cout << p;
	}

}

void plane_line_test(){
	Plane<float> p1({0,0,-1},-2);
	auto base = p1.ortho2d();
	cout << "Plane1: " << p1 << "\nBase:\n" << base.first << "\n" << base.second << "\n";
	Plane<float> p2({ sqrt(2.0f) / 2, 0, sqrt(2.0f) / 2 }, 1);
	cout << "Plane2: " << p2 << "\n";
	Line<float> l = p1.intersection_line(p2);
	cout << l;
}

//TODO: kivett feature
//void poly_partition_test(){
//	Polygon2<float> convex_poly({ { 1, 1 }, { 2, 1 }, { 3, 2 }, { 5, 3 }, {3,10} });
//	cout << "CCW: " << convex_poly.is_ccw() << "\n";
//	auto polys = convex_poly.convex_partitions();
//	for (auto& e : polys){
//		cout << e << '\n';
//	}
//}

void face_cut_test(){
	vector<approx::Vector3<float>> vertices{ { 0.0f, 1.0f, 3.0f }, { 1.0f, 0.0f, 3.0f }, { 2.0f, 0.0f, 3.0f }, { 3.0f, 1.0f, 3.0f }, { 2.0f, 3.0f, 3.0f } },
		normals{ { 0.0f, 0.0f, -1.0f } };
	approx::Face<float> f(&vertices, { 0, 1, 2, 3, 4 }, &normals, 0);
	approx::Plane<float> p({ 0, 0, -1.0f }, -3.0f);
	auto cut = f.cut_by(p);
	cout << "coplanar cut done\n";
	cout << cut.positive << "\n--------------------------\n" << cut.negative;
	vector<approx::Vector3<float>> tmpv, tmpn;
	auto cut2 = f.cut_by(p, &tmpv, &tmpn);
	cout << "new container cut done\n";
	f = cut2.negative;
	auto cut3 = f.cut_by(p, &tmpv, &tmpn);
	cout << cut3.negative << "\n--------------------------\n" << cut3.positive;
	cout << cut3.negative.cut_by(p).positive.cut_by(p).negative;
}



int _tmain(int argc, _TCHAR* argv[])
{
	//cut_eq_test();
	//poly_graph_test();
	//plane_line_test();
	//poly_partition_test();
	//face_cut_test();
	//cin.get();
	
	
	vector<approx::Vector3<float>> vertices { { 0.0f, 1.0f, 3.0f }, { 1.0f, 0.0f, 3.0f }, { 2.0f, 0.0f, 3.0f }, { 3.0f, 1.0f, 3.0f }, {2.0f,3.0f,3.0f} },
								   normals { {0.0f,0.0f,-1.0f} };
	vector<approx::Face<float>> faces;
	approx::Plane<float> p({ 1, 0, 0 }, 6.0f);

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
	
	cout << p.classify_point(vertices.front()) << " " << p.classify_point(vertices.back()) << "\n";
	
	approx::Face<float>::CutResult result = f.cut_by(p);
	cout << f << "\n--------------------------\n" << result.negative << "\n--------------------------\n" << result.positive;
	cout << "\n--------------------------\n" << vertices.size() << "\n-------------------\n";
	cout << result.positive.size();
	vertices.clear();
	normals.clear();*/
	
	approx::Vector3<float> vmin(1,1,1), vmax(3,2,2);
	float border = 0;
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

	//approx::TargetBody<float> tb(vertices,normals,faces);
	//cout << tb.body().size() <<"\n";
	//cout << (-0.0f == 0.0f) << (0.0f*-0.0f < 0);
	approx::TargetBody<float> tb;
	if (approx::ObjectLoader<float>::load_obj("./test.obj", tb,0.1f)){
		cout << "obj done\n";
	}
	else{
		cout << "obj error!";
		cin.get();
		return 0;
	}

	/*for (const auto& f : tb.body()){
		cout << f << "   " << f.normal() << "\n";
	}*/

	
	//for (auto& f : tb.body()) cout << f;
	approx::Approximation<approx::ConvexAtom<float>> app(&tb,0.0001f);
	//cout << "\n" << app.begin()->volume() << "\n";
	auto cut = app.cut(0,p);
	//cout << "cut done\n";
	cut.choose_both();
	app.garbage_collection();
	for (auto& b : app){
		cout << /*b.volume() <<*/ " "<< b.intersection_volume() <<"\n";
	}
	//approx::ObjectWriter<float>::save_obj("test_out.obj", app);
	approx::BodyList rajzol = approx::drawinfo<decltype(app.begin()),float>(app.begin(),app.end());

	cout << "--------------------------------pts\n";
	for (auto e : rajzol.points){
		cout << e.x << "," << e.y << "," << e.z << "\n";
	}
	cout << "--------------------------------inds\n";
	for (int i : rajzol.indicies){
		cout << i << " ";
	}
	cout << "\n--------------------------------ind_ranges\n";
	for (int i : rajzol.index_ranges){
		cout << i << " ";
	}
	cin.get();
	
	//for(auto x : app.vertex_container()) cout << x << "\n";
	//for (auto x : app.normal_container()) cout << x << "\n";
	//for (auto x : app.face_container()) cout << x << "\n";
	
	//for (auto f : *(app.begin()+1)) cout << f << "\n";
	//cout << app.size() << "\n";
	//cin.get();

	/*approx::ConvexAtom<float> atom(&faces, std::vector < int > {0, 1, 2, 3, 4, 5});
	auto cut = atom.cut_by(approx::Plane<float>({1,0,0},2.0f));
	for (auto& f : *cut.negative.get()){
		cout << f;
	}
	cout << "=====================================\n";
	for (auto& f : *cut.positive.get()){
		cout << f;
	}
	cout << "==========================" << faces.size()<<"\n";
	for (auto x : vertices){ cout << x << "\n"; }
	cout << atom.volume() << " " << cut.positive->volume() << " " << cut.negative->volume();
	cout << "\n" << vertices.size();
	cin.get();*/





	return 0;
}

