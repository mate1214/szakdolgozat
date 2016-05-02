#include "planes.h"
#include "geoios.h"


//============================================================================================
//Sik pelda
//
//Bemutatja a sikok es vonalak hasznalatat.
//============================================================================================


void plane_test();
void line_test();
void plane_line_test();

int main() {
	std::cout << std::endl << "================    Plane base test     ================" << std::endl << std::endl;
	plane_test();
	std::cout << std::endl << "================ Plane intersection est ================" << std::endl << std::endl;
	plane_line_test();
	std::cout << std::endl << "================       Line test        ================" << std::endl << std::endl;
	line_test();
	return 0;
	
}

void plane_test() {
	float x=70, y=-23, z=47;
	approx::Plane<float> p1({x,y,z},5),
						 p2({ -x,-y,-z }, -5);
	auto base1 = p1.ortho2d(),
		 base2 = p2.ortho2d();
	std::cout << "Base1 : " << base1.first << " ; " << base1.second << std::endl
	          << "Base2 : " << base2.first << " ; " << base2.second << std::endl;
}

void line_test() {
	approx::Line<float> l({0,1}, {1,1});
	std::cout << "Line : " << l << std::endl;
	std::cout << "=======================" << std::endl;
	std::cout << "point | signed distance" << std::endl;
	std::cout << "=======================" << std::endl;
	for (auto p : std::vector<approx::Vector2<float>>{ {0,0}, {4,0}, {4,4}, {0,4} }) {
		std::cout << p << " | " << l.classify_point(p) << std::endl;
	}
}

void plane_line_test(){
	approx::Plane<float> p1({0,0,-1},-2);
	auto base = p1.ortho2d();
	std::cout << "Plane1: " << p1 << std::endl << "Base:" << std::endl << base.first << " " << base.second << std::endl;
	approx::Plane<float> p2({ sqrt(2.0f) / 2, 0, sqrt(2.0f) / 2 }, 1);
	std::cout << "Plane2: " << p2 << std::endl;
	approx::Line<float> l = p1.intersection_line(p2);
	std::cout << "Intersection line: " << l << std::endl;
}