#define APPROX_NO_CONVERSION

#include <iostream>
#include "approximator.h"
#include "geoios.h"
#include "objio.h"

//============================================================================================
//Approximacio pelda
//
//Bemutatja hogy kell betolteni egy fajlt, vagni az atomokat, es megvizsgalni az eredmenyt.
//============================================================================================

void approximator_test();

int main() {

	approximator_test();

	std::cin.get();

	return 0;

}

void approximator_test() {

	//ez az osztaly tartja szamon az approximaciot
	approx::Approximator<float> app;

	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
	if (!app.set_target("test.obj", 0.5f, -1)) {
		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
		exit(1);
	}
	std::cout << "Az elso atom elso lapja:" << std::endl;
	std::cout << app.container().atoms(0).faces(0) << std::endl;
	std::cout << app.container().atoms(0).centroid() << std::endl;

	//A celtest az app.target().body()-ban erheto el, a body.h-ban bovebb info talalhato rola
	//es a metodusairol
	std::cout << "A celtest terfogata: " << app.target().body().volume() << "\n";


	approx::ObjectWriter<float>::save_obj("testout.obj", app.target().body());



	//vagosik
	approx::Plane<float> p = approx::Plane<float>({ 0,0,1 }, { 0.0f, 0.0f, 26.0f });

	//az app.container() tartalmazza az atomokat es nyujt lehetoseget az approximacios muveletekre
	//a 0. atomot elvagom az elobb megadott sikkal
	//a cut egy vagasi eredmeny, a vagast vegrehajtom, de az atomok nem kerulnek be a taroloba
	approx::Approximation<float>::CutResult cut = app.container().cut(0, p);
	//mindenfele vizsgalatok a keletkezett atomokon hogy jok-e, pl terfogat ellenorzes
	//a pozitiv es negativ oldal a metszosikhoz kepesti elhelyezkedest jelenti
	if (cut.negative()->valid()) {
		std::cout << "negativ oldali keletkezett atom terfogata: " << cut.negative()->volume() << std::endl;
	}
	else {
		std::cout << "a negativ oldalon nem keletkezett ervenyes atom!" << std::endl;
	}
	if (cut.positive()->valid()) {
		std::cout << "pozitiv oldali keletkezett atom terfogata: " << cut.positive()->volume() << std::endl;
	}
	else {
		std::cout << "a pozitiv oldalon nem keletkezett ervenyes atom!" << std::endl;
	}
	if (cut.choose_both()) {
		std::cout << "elfogadas sikeres" << std::endl;
	}
	else {
		std::cout << "elfogadas sikertelen" << std::endl;
	}
	std::cout << "a tarolo atomjainak szama elfogadasi kiserlet utan: " << app.container().size() << std::endl;
	int ind = 0;
	app.container().cut(0, approx::Plane<float>({ 1,2,1 }, { 15, 30, 30 })).choose_both();
	if (app.container().size() > 1) {
		app.container().cut(1, approx::Plane<float>({ 3,2,1 }, { 15, 30, 30 })).choose_positive();
	}
	std::cout << "approximalt test terfogata " << app.container().approximated_body().volume() << std::endl;
	std::cout << "fajlok irasa... " << std::endl;
	approx::ObjectWriter<float>::save_obj("approx.obj", app.container().approximated_body());
	approx::ObjectWriter<float>::save_obj("approx_all.obj", app.container().begin(), app.container().end());
	std::cout << "approx.obj es approx_all.obj elkeszult";

	std::cout << "approx.obj es approx_all.obj elkeszult";
	std::cout << "===================================\n";
	for (auto x : app.container().vertex_container()) {
		std::cout << x << "\n";
	}
	std::cout << "===================================\n";
	app.container().final_transform();


}



//// szakdoga.cpp : Defines the entry point for the console application.
////
//
//#include "stdafx.h"
//
//
//
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include "vectors.h"
//#include "face.h"
//#include "convexatom.h"
//#include "targetbody.h"
//#include "approximation.h"
//#include "conversion.h"
//#include "polygraph.h"
//#include "geoios.h"
//#include "objio.h"
//#include "approximator.h"
//
////using namespace std;
//using namespace approx;
//
//
//
//struct Less{
//	bool operator ()(const approx::Vector3<float>& a, const approx::Vector3<float>& b) const{
//		return a.x < b.x ||
//			   (a.x==b.x && a.y < b.y) ||
//			   (a.x==b.x && a.y == b.y && a.z <b.z);
//	}
//};
//
//Vector3<float> cut(const Vector3<float>& pt1, const Vector3<float>& pt2, const Plane<float>& p) {
//	float sign1 = p.classify_point(pt1), sign2 = p.classify_point(pt2);
//	float div = abs(sign1 / (abs(sign1) + abs(sign2)));
//	return (1 - div)*pt1 + div*pt2;
//}
//
//
//void cut_eq_test(){
//	std::vector<approx::Vector3<float>>
//		vertices{ { 1.0000456f, 0.0f, 0.0f },
//				  { sqrt(2.0f), sqrt(3.0f), sqrt(11.0f) },
//				  {(sqrt(2.0f)+sqrt(3.17f))/2.0f,sqrt(3.0f)+sqrt(8.767f) ,sqrt(10.65f)+sqrt(1.5678f)},
//				  {sqrt(6.56f),sqrt(3.0f)+sqrt(6.789f),-0.564f} },
//		normals{ cross(vertices[1] - vertices[0], vertices[2] - vertices[0]).normalized(),
//				 cross(vertices[3] - vertices[1], vertices[2] - vertices[1]).normalized() };
//	approx::Face<float>
//		face1(&vertices, {0,1,2},&normals,0),
//		face2(&vertices, {1,3,2},&normals,1);
//	approx::Plane<float> plane({0,1,0},sqrt(3.0f)+sqrt(5.456f));
//	auto cut1 = face1.cut_by(plane);
//	auto cut2 = face2.cut_by(plane);
//	std::cout << vertices[cut1.pt_inds.front()] << "\n"
//		<< vertices[cut1.pt_inds.back()] << "\n"
//		<< vertices[cut2.pt_inds.front()] << "\n"
//		<< vertices[cut2.pt_inds.back()] << "\n";
//	std::cout << std::boolalpha << (vertices[cut1.pt_inds.front()] == vertices[cut2.pt_inds.back()]);
//}
//
//void poly_graph_test(){
//	Graph<float> gr;
//	gr[{1, 1}].push_back({2,1.043f});
//	gr[{ 2, 1.043f }].push_back({1,1});
//
//	gr[{ 2.5f, 2.5f }].push_back({ 2, 1.043f });
//	gr[{ 2, 1.043f }].push_back({ 2.5f, 2.5f });
//
//	gr[{ 2.5f, 2.5f }].push_back({ 3,2 });
//	gr[{ 3, 2 }].push_back({ 2.5f,2.5f });
//	
//	gr[{ 2.5f, 2.5f }].push_back({ 1,5});
//	gr[{ 1, 5 }].push_back({ 2.5f, 2.5f });
//	
//	gr[{ 1, 5 }].push_back({ 7, 12 });
//	gr[{ 7, 12 }].push_back({ 1, 5 });
//
//	gr[{ 7, 12 }].push_back({ 5, 16 });
//	gr[{ 5, 16 }].push_back({ 7, 12 });
//
//	gr[{ 5, 16 }].push_back({ 1, 5 });
//	gr[{ 1, 5 }].push_back({ 5, 16 });
//
//	gr[{ 1, 5 }].push_back({ 1, 1 });
//	gr[{ 1, 1 }].push_back({ 1, 5 });
//
//
//	gr[{ 1, 5 }].push_back({ -1.5f, 4 });
//	gr[{ -1.5f, 4 }].push_back({ 1, 5 });
//
//	gr[{ -1.5f, 4 }].push_back({ -2, 8 });
//	gr[{ -2, 8 }].push_back({ -1.5f, 4 });
//
//	gr[{ 1, 5 }].push_back({ -2, 8 });
//	gr[{ -2, 8 }].push_back({ 1, 5 });
//
//	gr[{ 10, 10 }].push_back({ 15, 15 });
//	gr[{ 15, 15 }].push_back({ 10, 10 });
//
//	gr[{ 10, 10 }].push_back({ 15, 10 });
//	gr[{ 15, 10 }].push_back({ 10, 10 });
//
//	gr[{ 15, 10 }].push_back({ 15, 15 });
//	gr[{ 15, 15 }].push_back({ 15, 10 });
//
//
//	auto ls = get_polys(gr);
//
//	for (const auto& p : ls){
//		std::cout << p;
//	}
//
//}
//
//void plane_line_test(){
//	Plane<float> p1({0,0,-1},-2);
//	auto base = p1.ortho2d();
//	std::cout << "Plane1: " << p1 << "\nBase:\n" << base.first << "\n" << base.second << "\n";
//	Plane<float> p2({ sqrt(2.0f) / 2, 0, sqrt(2.0f) / 2 }, 1);
//	std::cout << "Plane2: " << p2 << "\n";
//	Line<float> l = p1.intersection_line(p2);
//	std::cout << l;
//}
//
//void surf_test() {
//	std::vector<approx::Vector3<float>> vertices{ {1,1,1},{ 1,1,2 }, {1,2,2}, { 1,2,1 } }, normals{ {1,0,0} };
//	approx::Face<float> f(&vertices, { 0,1,2,3 }, &normals, 0);
//
//	Plane<float> pl({ 0,0,1 }, 1.5f);
//	auto f2 = f.to_2d();
//	for (auto e : f2) {
//		std::cout << e << "\n";
//	}
//	std::cout << "\n";
//
//	auto cutline = f.to_plane().intersection_line(pl);
//	std::cout << cutline;
//	auto cut = f2.cut_by(cutline);
//	std::cout << cut.negative << "\n=============\n" << cut.positive;
//}
//
//void poly_clip_test() {
//	Polygon2<float> clipper({ {1,3}, {2,2}, {4,3}, {3,7}, {1,6} }),
//		toclip({ {3,4}, {6,5}, {8,10}, {2,11}, {1.5f,8} });
//	std::cout << std::boolalpha << ccw<float>({ 1,3 }, { 2,2 }, { 4,3 }) << "\n";
//
//	auto clipped = clipper.convex_clip(toclip);
//	std::cout << "clipper area: " << clipper.area() << "\ntoclip area: " << toclip.area() << "\nclipped area:" << clipped.area() << "\nclipped:\n" << clipped;
//}
//
//
//void cut_surface_test() {
//	std::vector<approx::Vector3<float>> vertices, normals;
//	std::vector<approx::Face<float>> faces;
//	approx::Plane<float> p({ 1, 0, 0 }, 2.0f);
//
//	approx::Vector3<float> vmin(1, 1, 1), vmax(3, 2, 2);
//	float border = 0;
//	vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
//	vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
//	vertices.push_back({ vmax.x + border, vmax.y + border + 2, vmin.z - border });
//	vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
//	vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
//	vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
//	vertices.push_back({ vmax.x + border, vmax.y + border + 2, vmax.z + border });
//	vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
//	normals.push_back({ 0, 0, -1 });
//	normals.push_back({ 1, 0, 0 });
//	normals.push_back({ 0, 0, 1 });
//	normals.push_back({ -1, 0, 0 });
//	normals.push_back({ -sqrt(2.0f) / 2, sqrt(2.0f) / 2, 0.0f });
//	normals.push_back({ 0, -1, 0 });
//	faces.emplace_back(&vertices, std::vector<int>{ 0, 1, 2, 3 }, &normals, 0);
//	faces.emplace_back(&vertices, std::vector<int>{ 1, 5, 6, 2 }, &normals, 1);
//	faces.emplace_back(&vertices, std::vector<int>{ 5, 4, 7, 6 }, &normals, 2);
//	faces.emplace_back(&vertices, std::vector<int>{ 4, 0, 3, 7 }, &normals, 3);
//	faces.emplace_back(&vertices, std::vector<int>{ 3, 2, 6, 7 }, &normals, 4);
//	faces.emplace_back(&vertices, std::vector<int>{ 0, 4, 5, 1 }, &normals, 5);
//
//	approx::TargetBody<float> tb(vertices, normals, faces);
//
//	/*if (approx::ObjectLoader<float>::load_obj("./test.obj", tb, 0.0f)) {
//		cout << "obj ok!\n";
//	}*/
//	std::cout << "tbvol: " << tb.body().volume() << "\n";
//	approx::Approximation<float> app(&tb, 0.1f);
//	std::cout << "starting atom vol: " << app.atoms(0).volume() << "\n";
//	//auto cut = app.cut(0, p);
//	//cut.choose_both();
//
//	auto cut = app.cut(0, p);
//	cut.choose_both();
//	approx::Plane<float> ps({ 0, 0, 1 }, 1.50f);
//	app.cut(0, ps).choose_both();
//
//	std::cout << app.atoms(0).volume() << " -> " << app.atoms(0).intersection_volume() <<"\n"
//			<< app.atoms(1).volume() << " -> " << app.atoms(1).intersection_volume() << "\n"
//			<< app.atoms(2).volume() << " -> " << app.atoms(2).intersection_volume() << "\n";
//
//
//	for (auto& a : app) {
//		for (int i = 0; i < a.size();++i) {
//			std::cout << a.surf_imprints(i) << "\n";
//		}
//		std::cout <<  "==================================\n";
//	}
//
//	/*cout << "vertex size: " << tb.vertex_container().size() << "\n"
//		 << "normal size: " << tb.normal_container().size() << "\n"
//		 << "faces size: " << tb.face_container().size() << "\n";
//
//	cout << "distinct normals: " << set<Vector3<float>, Less>(tb.normal_container().begin(), tb.normal_container().end()).size() << "\n";
//	cout << "volume:" << tb.body().volume() << "\n";
//	for (const auto& f : tb.body()) {
//		print_indicies(cout, f);
//		cout << f.normal() << "\n";
//	}
//
//	cout << "\n=====================================================\n";
//	const auto& v = tb.vertex_container();
//	vector<Vector3<float>> pts{ cut(v[2], v[6], p), cut(v[0], v[4], p),cut(v[3], v[7], p),cut(v[1], v[5], p) };
//	auto base = p.ortho2d();
//	cout << "base: " << base.first << "     " << base.second << "\n";
//	vector<Vector2<float>> pts2;
//	for (auto& e : pts) {
//		pts2.push_back({ dot(base.first, e) , dot(base.second, e) });
//		cout << pts2.back()  << "\n";
//	}
//	cout << "\n=====================================================\n";
//	auto surf = tb.body().cut_surface(p);
//	cout << "\n=====================================================\n";
//	for (auto& e : surf) {
//		cout << e;
//	}*/
//}
//
////TODO: kivett feature
////void poly_partition_test(){
////	Polygon2<float> convex_poly({ { 1, 1 }, { 2, 1 }, { 3, 2 }, { 5, 3 }, {3,10} });
////	cout << "CCW: " << convex_poly.is_ccw() << "\n";
////	auto polys = convex_poly.convex_partitions();
////	for (auto& e : polys){
////		cout << e << '\n';
////	}
////}
//
//void face_cut_test(){
//	std::vector<approx::Vector3<float>> vertices{ { 0.0f, 1.0f, 3.0f }, { 1.0f, 0.0f, 3.0f }, { 2.0f, 0.0f, 3.0f }, { 3.0f, 1.0f, 3.0f }, { 2.0f, 3.0f, 3.0f } },
//		normals{ { 0.0f, 0.0f, -1.0f } };
//	approx::Face<float> f(&vertices, { 0, 1, 2, 3, 4 }, &normals, 0);
//	approx::Plane<float> p({ 0, 0, -1.0f }, -3.0f);
//	auto cut = f.cut_by(p);
//	std::cout << "coplanar cut done\n";
//	std::cout << cut.positive << "\n--------------------------\n" << cut.negative;
//	std::vector<approx::Vector3<float>> tmpv, tmpn;
//	auto cut2 = f.cut_by(p, &tmpv, &tmpn);
//	std::cout << "new container cut done\n";
//	f = cut2.negative;
//	auto cut3 = f.cut_by(p, &tmpv, &tmpn);
//	std::cout << cut3.negative << "\n--------------------------\n" << cut3.positive;
//	std::cout << cut3.negative.cut_by(p).positive.cut_by(p).negative;
//}
//
//void conversion_test() {
//
//	std::vector<approx::Vector3<float>> vertices, normals;
//	std::vector<approx::Face<float>> faces;
//	approx::Plane<float> p({ 1, 0, 0 }, 1.50f);
//
//	approx::Vector3<float> vmin(1, 1, 1), vmax(3, 2, 2);
//	float border = 0;
//	vertices.push_back({ vmin.x - border, vmin.y - border, vmin.z - border });
//	vertices.push_back({ vmax.x + border, vmin.y - border, vmin.z - border });
//	vertices.push_back({ vmax.x + border, vmax.y + border, vmin.z - border });
//	vertices.push_back({ vmin.x - border, vmax.y + border, vmin.z - border });
//	vertices.push_back({ vmin.x - border, vmin.y - border, vmax.z + border });
//	vertices.push_back({ vmax.x + border, vmin.y - border, vmax.z + border });
//	vertices.push_back({ vmax.x + border, vmax.y + border, vmax.z + border });
//	vertices.push_back({ vmin.x - border, vmax.y + border, vmax.z + border });
//	normals.push_back({ 0, 0, -1 });
//	normals.push_back({ 1, 0, 0 });
//	normals.push_back({ 0, 0, 1 });
//	normals.push_back({ -1, 0, 0 });
//	normals.push_back({ 0, 1, 0.0f });
//	normals.push_back({ 0, -1, 0 });
//	faces.emplace_back(&vertices, std::vector<int>{ 0, 1, 2, 3 }, &normals, 0);
//	faces.emplace_back(&vertices, std::vector<int>{ 1, 5, 6, 2 }, &normals, 1);
//	faces.emplace_back(&vertices, std::vector<int>{ 5, 4, 7, 6 }, &normals, 2);
//	faces.emplace_back(&vertices, std::vector<int>{ 4, 0, 3, 7 }, &normals, 3);
//	faces.emplace_back(&vertices, std::vector<int>{ 3, 2, 6, 7 }, &normals, 4);
//	faces.emplace_back(&vertices, std::vector<int>{ 0, 4, 5, 1 }, &normals, 5);
//
//	approx::TargetBody<float> tb(vertices, normals, faces);
//
//	/*if (approx::ObjectLoader<float>::load_obj("./test.obj", tb, 0.0f)) {
//	cout << "obj ok!\n";
//	}*/
//	std::cout << "tbvol: " << tb.body().volume() << "\n";
//	approx::Approximator<float> app(std::move(std::make_unique<TargetBody<float>>(tb)), 0.0f);
//	//approx::Approximator<float> app("test.obj", 0.0f);
//
//
//	app.container().cut(0, p);
//
//	ConvexAtom<float> neg = *app.container().last_cut_result().negative();
//	ConvexAtom<float> pos = *app.container().last_cut_result().positive();
//
//	app.container().last_cut_result().choose_both();
//
//	
//
//	std::cout << std::boolalpha << (std::equal(neg.begin(),neg.end(),app.container().atoms(0).begin())) << "\n";
//
//	BodyList data = app.atom_drawinfo();
//
//	int i = 0;
//	for (const ConvexAtom<float>& a : app.container()) {
//		std::cout << "==========ATOM" << i++ << "================\n";
//		for (const Face<float>& f : a) {
//			for (int ind : f.indicies()) {
//				std::cout << ind << ",";
//			}
//			std::cout << "\n";
//		}
//	}
//
//	data = drawinfo(app.container().atoms(1));
//
//	for (int i = 0; i < (int)data.index_ranges.size() - 1; ++i) {
//		std::cout << " -------- DrawnAtom" << i << " -------- \n";
//		std::cout << data.index_ranges[i] << " - " << data.index_ranges[i + 1] << "\n";
//		for (int j = data.index_ranges[i]; j < data.index_ranges[i + 1]; j+=3) {
//			std::cout << data.indicies[j] << ',' << data.indicies[j+1] << ',' << data.indicies[j+2] << " => ";
//			std::cout << "(" << data.points[data.indicies[j]].x << ", "
//				<< data.points[data.indicies[j]].y << ", "
//				<< data.points[data.indicies[j]].z << ") ; ("
//				<< data.points[data.indicies[j+1]].x << ", "
//				<< data.points[data.indicies[j+1]].y << ", "
//				<< data.points[data.indicies[j+1]].z << ") ; ("
//				<< data.points[data.indicies[j+2]].x << ", "
//				<< data.points[data.indicies[j+2]].y << ", "
//				<< data.points[data.indicies[j+2]].z << ")\n";
//		}
//	}
//	
//}
//
//
//void targetbody_ccw_test() {
//
//	//ez az osztaly tartja szamon az approximaciot
//	approx::Approximator<float> app;
//
//	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
//	if (!app.set_target("gummybear.obj", 1.0f)) {
//		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
//	}
//	approx::ObjectWriter<float>::save_obj("testout.obj", app.target().body());
//	std::cout << "\n\n";
//	for (auto& f : app.target().body()) {
//		std::cout << dot(f.normal(),cross(f.points(2) - f.points(1),f.points(0)-f.points(1)).normalized())<< "\n";
//	}
//
//
//	/*for (const Face<float>& f : app.target().body()) {
//		std::cout << f;
//		auto n = cross(f.points(2) - f.points(1), f.points(0) - f.points(1)).normalized();
//		std::cout << n << "  ~  " << f.normal() << "\n";
//	}
//
//	approx::BodyList data = app.target_drawinfo();
//
//	/*for (int i = 0; i < data.index_ranges.size() - 1; ++i) {
//		std::cout << " -------- Atom" << i << " -------- \n";
//		for (int j = data.index_ranges[i]; j < data.index_ranges[i + 1]; ++j) {
//			std::cout << data.points[ data.indicies[j] ].x << ", "
//				 << data.points[ data.indicies[j] ].y << ", "
//				 << data.points[ data.indicies[j] ].z << "\n";
//		}
//	}*/
//
//	//std::cout << app.target().body().volume() << "\n";
//	//std::cout << app.target().body().centroid() << "\n";
//}
//
//
//void ccw_test(){
//	Polygon2<float> convex_poly({ { 1, 1 }, { 2, 1 }, { 3, 2 }, {2,6} });
//	std::cout << "CCW: " << convex_poly.is_ccw() << "\n";
//}
//
//void add(Graph<float>& gr, Vector2<float> a, Vector2<float> b) {
//	gr[a].push_back(b);
//	gr[b].push_back(a);
//}
//
//void sausage_chain_test() {
//	Graph<float> gr;
//	std::vector<std::pair<Vector2<float>, Vector2<float>>> pairs{
//		{{2,2},{3,3}},
//		{{3,3},{3,4}},
//		{ {3,4},{2,5} },
//		{ {2,5},{1,4} },
//		{ {1,4},{2,2} },
//		{ {2,5},{3,5} },
//		{ {3,5},{4,6} },
//		{ {4,6},{4,7} },
//		{ {4,7},{2,5} },
//		{ {4,7},{5,6} },
//		{ {5,6},{6,6} },
//		{ {6,6},{7,7} },
//		{ {7,7},{6,9} },
//		{ {6,9},{4,7} },
//		{ {6,9},{7,11} },
//		{ {7,11},{6,13} },
//		{ {6,13},{5,11} },
//		{ {5,11},{6,9} },
//		{ {7,7},{6,4} },
//		{ {6,4},{7,3} },
//		{ {7,3},{8,5} },
//		{ {8,5},{7,7} },
//		{ {7,3},{5,3} },
//		{ {5,3},{5,2} },
//		{ {5,2},{7,3} },
//		{ {5,2},{4,3} },
//		{ {4,3},{2,2} },
//		{ {5,2},{4,1} },
//		{ {4,1},{3,1} },
//		{ {3,1},{2,2} },
//	};
//	std::random_shuffle(pairs.begin(), pairs.end());
//	for (auto p : pairs) {
//		add(gr, p.first, p.second);
//	}
//
//	auto ls = get_polys(gr);
//
//	for (const auto& p : ls) {
//		std::cout << p;
//	}
//}
//
//void line_test() {
//	approx::Line<float> l({0,1}, {1,1});
//	for (auto p : std::vector<Vector2<float>>{ {0,0}, {4,0}, {4,4}, {0,4} }) {
//		std::cout << p << "   " << l.classify_point(p) << "\n";
//	}
//}
//
//
//void coplanar_cut_test(){
//	std::vector<approx::Vector3<float>>
//		vertices{ {1,1,2.5f},{ 2,1,2.5f },{ 3,2,2.5f } },
//		normals{ {0,0,1} };
//	approx::Face<float>
//		face1(&vertices, { 0,1,2 }, &normals, 0);
//	approx::Plane<float> plane({ 0,0,1 }, 2.5f);
//	auto cut1 = face1.cut_by(plane);
//	std::cout << cut1.positive << "\n" << cut1.negative << "\n" << cut1.points_added << "\n" << cut1.pt_inds.size();
//}
//
//void donut_test() {
//	Graph<float> gr;
//	std::vector<std::pair<Vector2<float>, Vector2<float>>> pairs{
//		{{0,0},{4,0}},
//		{ { 0,0 },{ 0,4 } },
//		//{ { 4,4 },{ 4,0 } },
//		{ { 0,4 },{ 4,4 } },
//		{{4,0},{5,1}},
//		{{4,4},{5,1}},
//
//		{ { 1,1 },{ 3,1 } },
//		{ { 1,1 },{ 1,3 } },
//		{ { 3,3 },{ 3,1 } },
//		{ { 3,3 },{ 1,3 } },
//	};
//
//	std::random_shuffle(pairs.begin(), pairs.end());
//	for (auto p : pairs) {
//		add(gr, p.first, p.second);
//	}
//
//	auto ls = get_polys(gr);
//
//	for (const auto& p : ls) {
//		std::cout << p;
//	}
//	//std::cout << ls[0].contains(ls[1]) << " " << ls[0].contains(ls[1].points(0)) << '\n';
//	for (int i = 0; i < ls[1].size(); ++i) {
//		std::cout << ls[0].contains(ls[1].points(i)) << "  ";
//	}
//}
//
//void real_donut_test(){
//	approx::Approximator<float> app;
//
//	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
//	if (!app.set_target("torus2.obj", 0.2f)) {
//		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
//	}
//	std::cout << "target vol: " << app.target().body().volume() << " starting atom vol: " << app.container().atoms(0).volume() <<"\n";
//	app.container().cut(0, approx::Plane<float>({ 0,1,0 }, 0.0f)).choose_both();
//
//	auto a = app.container().atoms(1);
//	std::cout << a.intersection_volume() << "\n";
//	for (auto f : a) {
//		std::cout << f << "\n---->normal: " << f.normal() << "\n";
//	}
//
//
//	//for (const auto& a : app.container()) {
//	//	std::cout << "volume " << a.volume() << "  " << a.intersection_volume() << "\n";
//	//	for (int i = 0; i < (int)a.size(); ++i) {
//	//		std::cout << "\tface area: " << a.surf_imprints(i)->area() << " ---> " << a.faces(i).normal() << "\n";
//	//		/*for (auto& f : a.surf_imprints(i)->poly) {
//	//		std::cout <<"pos: " << f.second << "\n"<< f.first << "\n";
//	//		}*/
//	//	}
//	//}
//
//	//std::cout << "=========================================================================\n";
//
//	//app.container().cut(0, approx::Plane<float>({ 1,0,0 }, 0.0f)).choose_both();
//	//for (const auto& a : app.container()) {
//	//	std::cout << "volume " << a.volume() << "  " << a.intersection_volume() << "\n";
//	//	for (int i = 0; i < (int)a.size(); ++i) {
//	//		std::cout << "\tface area: " << a.surf_imprints(i)->area() << " ---> " << a.faces(i).normal() << "\n";
//	//		/*for (auto& f : a.surf_imprints(i)->poly) {
//	//			std::cout <<"pos: " << f.second << "\n"<< f.first << "\n";
//	//		}*/
//	//	}
//	//}
//
//	//std::cout << "volume " << app.container().atoms(1).volume() << "  " << app.container().atoms(1).intersection_volume() << "\n";
//
//	/*for (int i = 0; i < (int)app.container().atoms(0).size(); ++i) {
//		for (auto& f : app.container().atoms(0).surf_imprints(i)->poly) {
//			for (auto& f2 : app.container().atoms(0).surf_imprints(i)->poly) {
//				std::cout << (f == f2) << "\n";
//			}
//		}
//	}*/
//
//	//auto slice = app.container().target_body().body().cut_surface(approx::Plane<float>({ 0,1,0 }, 0));
//	//std::cout << slice[1].first.contains(slice[0].first) << "\n";
//}
//
//void donut_cut_test() {
//	approx::Approximator<float> app;
//
//	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
//	if (!app.set_target("torus2.obj", 0.2f)) {
//		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
//	}
//
//	auto list = app.target().body().cut_surface(approx::Plane<float>({ 0,1,0 }, 0.0f));
//
//	auto p1 = approx::Plane<float>({ 0,1,0 }, 0.0f);
//	auto p = approx::Plane<float>({ 1,0,0 }, 0.0f);
//	auto lin = p1.intersection_line(p);
//	for (auto e : list) {
//		std::cout << "POLY: "<< e.first.area() << "\n";
//		auto cut = e.first.cut_by(lin);
//		std::cout << "first: "  << cut.negative.area() << "\n" << cut.negative 
//				  << "second: " << cut.positive.area() << "\n" << cut.positive << "\n";
//	}
//}
//
//void monster_test() {
//
//	approx::Approximator<float> app;
//
//	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
//	if (!app.set_target("test.obj", 0.5f)) {
//		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
//	}
//
//	ObjectWriter<float>::save_obj("atomtest.obj", app.container().atoms(0));
//
//	app.container().cut(0, approx::Plane<float>({ 0,0,1 }, {0,0,26})).choose_both();
//	std::cout << "cut ->" << app.container().size() << "\n";
//	app.container().cut(1, approx::Plane<float>({ 1,1,0 }, { 25,15,26 })).choose_both();
//	std::cout << "cut ->" << app.container().size() << "\n";
//	app.container().cut(1, approx::Plane<float>({ 1,0,1 }, { 25,15,26 })).choose_both();
//	std::cout << "cut ->" << app.container().size() << "\n";
//	app.container().cut(3, approx::Plane<float>({ 1,1,1 }, { 25,15,26 })).choose_both();
//	std::cout << "cut ->" << app.container().size() << "\n";
//	int i = 0;
//	for (auto& a : app.container()) {
//		std::cout << a.volume() << "\n";
//		if (a.volume() < 0) {
//			std::cout << "\n=========================\n";
//			std::cout << a.bad_normal_ind() << "\n";
//			for (auto& f : a) {
//				std::cout << f;
//				std::cout << std::set<Vector3<float>,Less>(f.begin(), f.end()).size() << "\n";
//			}
//			std::cout << "\n=========================\n";
//			
//		}
//		else ++i;
//		std::cout << a.faces_inside().size() << " " << a.face_indicies_inside().size() << "\n";
//	}
//	//if(i<app.container().size())
//	ObjectWriter<float>::save_obj("randomout.obj", app.container().begin() + app.container().size()-1, app.container().end());
//
//}
//
//
///*
//void volume_test() {
//	std::string fn = "atomtest.obj";
//	approx::TargetBody<float> tb;
//	if (!approx::ObjectLoader<float>::load_obj(fn, tb)) {
//		std::cout << "HIBA A BETOLTESNEL\n";
//	}
//	for (const auto& f : tb.body()) {
//		std::cout << f.to_2d().area() << "  *  " << f.to_plane().signed_distance() << " " << dot(f.normal(),f.points(0))
//			<< "\nclnormal: " << cross(f.points(2)-f.points(1),f.points(0)-f.points(1)).normalized() << " normal: " << f.normal() << " + \n";
//	}
//	std::cout << "=========================================\n";
//	if (!approx::ObjectLoader<float>::load_obj(fn, tb)) {
//		std::cout << "HIBA A BETOLTESNEL\n";
//	}
//	Vector3<float> vec = -1.0f * tb.body().centroid();
//	tb.move_by(vec);
//	std::cout << tb.body().volume() << "\n";
//	for (const auto& f : tb.body()) {
//		std::cout << f.to_2d().area() << "  *  " << f.to_plane().signed_distance() << " normal: " << f.normal() <<" + \n";
//	}
//
//	for (float t = 0.1f; t < 1.1f; t += 0.1f) {
//		if (!approx::ObjectLoader<float>::load_obj(fn, tb)) {
//			std::cout << "HIBA A BETOLTESNEL\n";
//		}
//		tb.move_by(t*vec);
//		std::cout << tb.body().volume()<< "\n";
//	}
//	ObjectWriter<float>::save_obj(fn+"buggy_out.obj",tb.body());
//}*/
//
//
//void plane_test() {
//	float x=70, y=-23, z=47;
//	approx::Plane<float> p1({x,y,z},5),
//						 p2({ -x,-y,-z }, -5);
//	auto base1 = p1.ortho2d(),
//		 base2 = p2.ortho2d();
//	std::cout << base1.first << "     " << base1.second << "\n" << base2.first << "     " << base2.second << "\n";
//}
//
//void drawinfo2d_test() {
//
//}
//
//void centroid_test() {
//	std::vector<approx::Vector3<float>> vertices{ { 0.0f, 1.0f, 3.0f },{ 1.0f, 0.0f, 3.0f },{ 2.0f, 0.0f, 3.0f },{ 3.0f, 1.0f, 3.0f },{ 2.0f, 3.0f, 3.0f } },
//		normals{ { 0.0f, 0.0f, -1.0f } };
//	std::reverse(vertices.begin(), vertices.end());
//	approx::Face<float> f(&vertices, { 0, 1, 2, 3, 4 }, &normals, 0);
//	std::cout << f.centroid();
//}
//
//
//void ortho_test() {
//	approx::Plane<float> p({ 0,0,1 }, { 1,1,1 });
//	auto base = p.ortho2d();
//	std::cout << dot(cross(base.first, base.second), p.normal());
//}
//
//void dokucode() {
//	approx::Vector3<float> v1(1.0f, 2.0f, 3.0f); //az (1,2,3) vektor
//	approx::Vector3<float> nullvect; //default inicializacioval nullvektor
//	approx::Vector3<float> v2(-0.7f, 2.43f, 3.3f);
//	approx::Vector3<float> v3 = v1 + v2, v4 = v1 - v2, v5 = -5.0f * v2;
//	approx::Vector3<float> v6 = cross(v1, v2);
//	v3 += v5; v4 -= v3;
//	float skalarszorzat = dot(v1, v2);
//
//}
//
///*void coplanar_cut_test() {
//	std::vector<Vector3<float>> pts{ {1,1,1}, {1,2,1},{2,sqrtf(2.0f),1} },
//		normals{ {0,0,1} };
//	approx::Face<float> fc(&pts, { 0,1,2 }, &normals, 0);
//	approx::Plane<float> p({ 0,0,1 }, 1);
//	auto cut = fc.cut_by(p);
//	std::cout << cut.points_added << " " << cut.pt_inds.size() << "\n";
//}*/
//
//
//void approximator_test() {
//
//	//ez az osztaly tartja szamon az approximaciot
//	approx::Approximator<float> app;
//
//	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
//	if (!app.set_target("test.obj", 0.5f, -1, false)) {
//		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
//		exit(1);
//	}
//	std::cout << "Az elso atom elso lapja:" << std::endl;
//	std::cout << app.container().atoms(0).faces(0) << std::endl;
//	std::cout << app.container().atoms(0).centroid() << std::endl;
//
//	//A celtest az app.target().body()-ban erheto el, a body.h-ban bovebb info talalhato rola
//	//es a metodusairol
//	std::cout << "A celtest terfogata: " << app.target().body().volume() << "\n";
//
//
//	approx::ObjectWriter<float>::save_obj("testout.obj", app.target().body());
//
//
//
//	//vagosik
//	approx::Plane<float> p = approx::Plane<float>({ 0,0,1 }, { 0.0f, 0.0f, 26.0f });
//
//	//az app.container() tartalmazza az atomokat es nyujt lehetoseget az approximacios muveletekre
//	//a 0. atomot elvagom az elobb megadott sikkal
//	//a cut egy vagasi eredmeny, a vagast vegrehajtom, de az atomok nem kerulnek be a taroloba
//	approx::Approximation<float>::CutResult cut = app.container().cut(0, p);
//	//mindenfele vizsgalatok a keletkezett atomokon hogy jok-e, pl terfogat ellenorzes
//	//a pozitiv es negativ oldal a metszosikhoz kepesti elhelyezkedest jelenti
//	if (cut.negative()->valid()) {
//		std::cout << "negativ oldali keletkezett atom terfogata: " << cut.negative()->volume() << std::endl;
//	}
//	else {
//		std::cout << "a negativ oldalon nem keletkezett ervenyes atom!" << std::endl;
//	}
//	if (cut.positive()->valid()) {
//		std::cout << "pozitiv oldali keletkezett atom terfogata: " << cut.positive()->volume() << std::endl;
//	}
//	else {
//		std::cout << "a pozitiv oldalon nem keletkezett ervenyes atom!" << std::endl;
//	}
//	if (cut.choose_both()) {
//		std::cout << "elfogadas sikeres" << std::endl;
//	}
//	else {
//		std::cout << "elfogadas sikertelen" << std::endl;
//	}
//	std::cout << "a tarolo atomjainak szama elfogadasi kiserlet utan: " << app.container().size() << std::endl;
//	int ind = 0;
//	app.container().cut(0, approx::Plane<float>({ 1,2,1 }, { 15, 30, 30 })).choose_both();
//	if (app.container().size() > 1) {
//		app.container().cut(1, approx::Plane<float>({ 3,2,1 }, { 15, 30, 30 })).choose_positive();
//	}
//	std::cout << "approximalt test terfogata " << app.container().approximated_body().volume() << std::endl;
//	std::cout << "fajlok irasa... " << std::endl;
//	approx::ObjectWriter<float>::save_obj("approx.obj", app.container().approximated_body());
//	approx::ObjectWriter<float>::save_obj("approx_all.obj", app.container().begin(), app.container().end());
//	std::cout << "approx.obj es approx_all.obj elkeszult";
//	app.restart();
//
//
//
//}
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	//cut_eq_test();
//	//poly_graph_test();
//	//plane_line_test();
//	//poly_partition_test();
//	//face_cut_test();
//	//cut_surface_test();
//	//approximator_test();
//	//surf_test();
//	//poly_clip_test();
//	//conversion_test();
//	targetbody_ccw_test();
//	//ccw_test();
//	//donut_test();
//	//line_test();
//	//real_donut_test();
//	//donut_cut_test();
//	//monster_test();
//	//volume_test();
//	//plane_test();
//	//centroid_test();
//	//ortho_test();
//	//dokucode();
//	//std::cout << (-1 % 5);
//	//coplanar_cut_test();
//	std::cin.get();
//
//	return 0;
//}
//
