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
#include "approximator.h"
using namespace std;
using namespace approx;



struct Less{
	bool operator ()(const approx::Vector3<float>& a, const approx::Vector3<float>& b) const{
		return a.x < b.x ||
			   (a.x==b.x && a.y < b.y) ||
			   (a.x==b.x && a.y == b.y && a.z <b.z);
	}
};

Vector3<float> cut(const Vector3<float>& pt1, const Vector3<float>& pt2, const Plane<float>& p) {
	float sign1 = p.classify_point(pt1), sign2 = p.classify_point(pt2);
	float div = abs(sign1 / (abs(sign1) + abs(sign2)));
	return (1 - div)*pt1 + div*pt2;
}


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

void surf_test() {
	vector<approx::Vector3<float>> vertices{ {1,1,1},{ 1,1,2 }, {1,2,2}, { 1,2,1 } }, normals{ {1,0,0} };
	approx::Face<float> f(&vertices, { 0,1,2,3 }, &normals, 0);

	Plane<float> pl({ 0,0,1 }, 1.5f);
	auto f2 = f.to_2d();
	for (auto e : f2) {
		cout << e << "\n";
	}
	cout << "\n";

	auto cutline = f.to_plane().intersection_line(pl);
	cout << cutline;
	auto cut = f2.cut_by(cutline);
	cout << cut.negative << "\n=============\n" << cut.positive;
}

void poly_clip_test() {
	Polygon2<float> clipper({ {1,3}, {2,2}, {4,3}, {3,7}, {1,6} }),
		toclip({ {3,4}, {6,5}, {8,10}, {2,11}, {1.5f,8} });
	cout << boolalpha << ccw<float>({ 1,3 }, { 2,2 }, { 4,3 }) << "\n";

	auto clipped = clipper.convex_clip(toclip);
	cout << "clipper area: " << clipper.area() << "\ntoclip area: " << toclip.area() << "\nclipped area:" << clipped.area() << "\nclipped:\n";
	cout << clipped;
}


void cut_surface_test() {
	vector<approx::Vector3<float>> vertices, normals;
	vector<approx::Face<float>> faces;
	approx::Plane<float> p({ 1, 0, 0 }, 2.0f);

	approx::Vector3<float> vmin(1, 1, 1), vmax(3, 2, 2);
	float border = 0;
	vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
	vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
	vertices.push_back({ vmax.x + border, vmax.y + border + 2, vmin.z - border });
	vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
	vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
	vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
	vertices.push_back({ vmax.x + border, vmax.y + border + 2, vmax.z + border });
	vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
	normals.push_back({ 0, 0, -1 });
	normals.push_back({ 1, 0, 0 });
	normals.push_back({ 0, 0, 1 });
	normals.push_back({ -1, 0, 0 });
	normals.push_back({ -sqrt(2.0f) / 2, sqrt(2.0f) / 2, 0.0f });
	normals.push_back({ 0, -1, 0 });
	faces.emplace_back(&vertices, std::vector<int>{ 0, 1, 2, 3 }, &normals, 0);
	faces.emplace_back(&vertices, std::vector<int>{ 1, 5, 6, 2 }, &normals, 1);
	faces.emplace_back(&vertices, std::vector<int>{ 5, 4, 7, 6 }, &normals, 2);
	faces.emplace_back(&vertices, std::vector<int>{ 4, 0, 3, 7 }, &normals, 3);
	faces.emplace_back(&vertices, std::vector<int>{ 3, 2, 6, 7 }, &normals, 4);
	faces.emplace_back(&vertices, std::vector<int>{ 0, 4, 5, 1 }, &normals, 5);

	approx::TargetBody<float> tb(vertices, normals, faces);

	/*if (approx::ObjectLoader<float>::load_obj("./test.obj", tb, 0.0f)) {
		cout << "obj ok!\n";
	}*/
	cout << "tbvol: " << tb.body().volume() << "\n";
	approx::Approximation<float> app(&tb, 0.1f);
	cout << "starting atom vol: " << app.atoms(0).volume() << "\n";
	//auto cut = app.cut(0, p);
	//cut.choose_both();

	auto cut = app.cut(0, p);
	cut.choose_both();
	approx::Plane<float> ps({ 0, 0, 1 }, 1.50f);
	app.cut(0, ps).choose_both();

	cout << app.atoms(0).volume() << " -> " << app.atoms(0).intersection_volume() <<"\n";
	cout << app.atoms(1).volume() << " -> " << app.atoms(1).intersection_volume() << "\n";
	cout << app.atoms(2).volume() << " -> " << app.atoms(2).intersection_volume() << "\n";


	for (auto& a : app) {
		for (int i = 0; i < a.size();++i) {
			cout << a.surf_imprints(i) << "\n";
		}
		cout <<  "==================================\n";
	}

	/*cout << "vertex size: " << tb.vertex_container().size() << "\n"
		 << "normal size: " << tb.normal_container().size() << "\n"
		 << "faces size: " << tb.face_container().size() << "\n";

	cout << "distinct normals: " << set<Vector3<float>, Less>(tb.normal_container().begin(), tb.normal_container().end()).size() << "\n";
	cout << "volume:" << tb.body().volume() << "\n";
	for (const auto& f : tb.body()) {
		print_indicies(cout, f);
		cout << f.normal() << "\n";
	}

	cout << "\n=====================================================\n";
	const auto& v = tb.vertex_container();
	vector<Vector3<float>> pts{ cut(v[2], v[6], p), cut(v[0], v[4], p),cut(v[3], v[7], p),cut(v[1], v[5], p) };
	auto base = p.ortho2d();
	cout << "base: " << base.first << "     " << base.second << "\n";
	vector<Vector2<float>> pts2;
	for (auto& e : pts) {
		pts2.push_back({ dot(base.first, e) , dot(base.second, e) });
		cout << pts2.back()  << "\n";
	}
	cout << "\n=====================================================\n";
	auto surf = tb.body().cut_surface(p);
	cout << "\n=====================================================\n";
	for (auto& e : surf) {
		cout << e;
	}*/
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

void approximator_test() {

	//============================================================================================
	//Approximacio pelda
	//============================================================================================

	//ez az osztaly tartja szamon az approximaciot
	approx::Approximator<float> app;

	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
	if (!app.set_target("test.obj", 0.5f)) {
		cout << "HIBA A FAJL BETOLTESENEL!\n";
	}

	//A celtest az app.target().body()-ban erheto el, a body.h-ban bovebb info talalhato rola
	//es a metodusairol
	cout << "A celtest terfogata: " << app.target().body().volume() << "\n";

	//vagosik
	approx::Plane<float> p({ 1,0,0 }, 15.0f);

	//az app.container() tartalmazza az atomokat es nyujt lehetoseget az approximacios muveletekre
	//a 0. atomot elvagom az elobb megadott sikkal
	//a cut egy vagasi eredmeny, a vagast vegrehajtom, de az atomok nem kerulnek be a taroloba
	Approximation<float>::CutResult cut = app.container().cut(0, p);
	
	//mindenfele ugyes okos vizsgalatok a keletkezett atomokon hogy jok-e, pl terfogat ellenorzes
	//a metszet terfogat meg bugol, de a sima terfogat jol mukodik
	//a pozitiv es negativ oldal a metszosikhoz kepesti elhelyezkedest jelenti 
	cout << "negativ oldali keletkezett atom terfogata: " << cut.negative()->volume() << "\n";
	cout << "pozitiv oldali keletkezett atom terfogata: " << cut.positive()->volume() << "\n";

	//mondjuk hogy tetszik az eredmeny, berakjuk az atomokat
	cut.choose_both();
	//mostmar ket atomunk van
	//choose_negative() - choose_positive() ezek csak a negativ vagy csak a pozitiv oldali atomot hagyjak meg
	//ha nem tetszene az eredmeny akkor cut.undo();
	//az undo csak egyszer mukodik, es ha mar elfogadtuk choose_*-al akkor nem undozhatjuk

	//az app.container().atoms(i) az i. atom kozvetlen konstans eleresre, pl teszteli hogy az adott sik atmegy-e rajta
	approx::Plane<float> p2({ 1,0,0 }, 16.0f);
	if (app.container().atoms(1).intersects_plane(p2)) {
		cout << "az 1. indexu atomon atmegy a p2 sik \n";
	}

	//lekerem az atomok rajzolasi adatait
	approx::BodyList data = app.atom_drawinfo();
	//data.points - vertex adatok
	//data.indicies - omlesztve az osszes index
	//data.index_ranges index hatarok:
	//az [index_ranges[i], indes_ranges[i+1]) intervallum az indexekbol egy atom
	//tehat az i. atomnal index_ranges[i] az elso es van index_ranges[i+1]-index_ranges[i] darab
	//GL_TRIANGLES modban mukodnie kell
	for (int i = 0; i < data.index_ranges.size() - 1; ++i) {
		cout << " -------- Atom" << i << " -------- \n";
		for (int j = data.index_ranges[i]; j < data.index_ranges[i + 1]; ++j) {
			cout << data.points[ data.indicies[j] ].x << ", "
				 << data.points[ data.indicies[j] ].y << ", "
				 << data.points[ data.indicies[j] ].z << "\n";
		}
	}

	//hasonlo modon kerheto el a rajzolando celtest is
	approx::BodyList targetdata = app.target_drawinfo();
	app.restart();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//cut_eq_test();
	//poly_graph_test();
	//plane_line_test();
	//poly_partition_test();
	//face_cut_test();
	cut_surface_test();
	//approximator_test();
	//surf_test();
	//poly_clip_test();

	cin.get();

	return 0;
}

