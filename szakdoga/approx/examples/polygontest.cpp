#include <iostream>
#include "poly2.h"
#include "geoios.h"
#include "planes.h"



void poly_cut_test();
void poly_clip_test();
void extreme_clip_test();

int main() {
	std::cout << "==============CUT TEST===============" << std::endl;
	poly_cut_test();
	std::cout << std::endl << "================CLIP TEST==============" << std::endl;
	poly_clip_test();
	std::cout << std::endl << "=============EXTREME CLIP TEST =======" << std::endl;
	extreme_clip_test();
	return 0;
	
}

void extreme_clip_test(){
	approx::Polygon2<float> clipper({ {1,1}, {3,1}, {3,3}, {1,3} }),
			toclip({ {2,1}, {5,1}, {5,3}, {2,3} });
	std::cout << std::boolalpha << approx::ccw<float>({ 1,3 }, { 2,2 }, { 4,3 }) << std::endl;

	auto clipped = clipper.convex_clip(toclip);
	std::cout << "clipper:" << std::endl << clipper << std::endl
			  << "toclip:" << std::endl << toclip << std::endl
			  << "clipper area: " << clipper.area() << std::endl
			  << "toclip area: " << toclip.area() << std::endl
			  << "clipped area:" << clipped.area() << std::endl
			  << "clipped:" << std::endl << clipped;	
}

void poly_cut_test(){
	approx::Polygon2<float> poly({ {1,1}, {3,1}, {3,3}, {1,3} });
	approx::Line<float> line({1,0},2);
	std::cout << "polygon:" << std::endl << poly << std::endl
			  << "line:" << line << std::endl;
	auto cut = poly.cut_by(line);
	std::cout << "negative side:" << std::endl << cut.negative << std::endl
			  << "positive side:" << std::endl << cut.positive << std::endl;
}

void poly_clip_test() {
	approx::Polygon2<float> clipper({ {1,3}, {2,2}, {4,3}, {3,7}, {1,6} }),
			toclip({ {3,4}, {6,5}, {8,10}, {2,11}, {1.5f,8} });
	std::cout << std::boolalpha << approx::ccw<float>({ 1,3 }, { 2,2 }, { 4,3 }) << std::endl;

	auto clipped = clipper.convex_clip(toclip);
	std::cout << "clipper:" << std::endl << clipper << std::endl
			  << "toclip:" << std::endl << toclip << std::endl
			  << "clipper area: " << clipper.area() << std::endl
			  << "toclip area: " << toclip.area() << std::endl
			  << "clipped area:" << clipped.area() << std::endl
			  << "clipped:" << std::endl << clipped;
}