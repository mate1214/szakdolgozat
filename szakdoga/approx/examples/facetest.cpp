#include <iostream>
#include <vector>
#include "face.h"
#include "geoios.h"


//============================================================================================
//Lapokat tesztelo pelda
//
//Bemutatja hogy kell lapokat felvenni es hozzakotni egy tarolohoz, elvagni.
//============================================================================================

void cut(const approx::Face<float>& f, const approx::Plane<float>& p);
void face_cut_test();
void face_centroid_test();

int main(){
	std::cout << "=========== FACE CUT TEST =============" << std::endl;
	face_cut_test();
	std::cout << "========== FACE CENTROID TEST =========" << std::endl;
	face_centroid_test();
	return 0;
	
}

void cut(const approx::Face<float>& f, const approx::Plane<float>& p){
	auto cut = f.cut_by(p);
	std::cout << "positive side:" << std::endl
			  << cut.positive << std::endl 
	          << "--------------------------" << std::endl
			  << "negative side:"<< cut.negative << std::endl;	
}


void face_cut_test(){
	std::vector<approx::Vector3<float>> vertices{ { 0.0f, 1.0f, 3.0f }, { 1.0f, 0.0f, 3.0f }, { 2.0f, 0.0f, 3.0f }, { 3.0f, 1.0f, 3.0f }, { 2.0f, 3.0f, 3.0f } },
		normals{ { 0.0f, 0.0f, -1.0f } };
	approx::Face<float> f(&vertices, { 0, 1, 2, 3, 4 }, &normals, 0);
	approx::Plane<float> p({ 0, 0, -1.0f }, -3.0f);
	approx::Plane<float> p2({ 1, 0, 0 }, 2.0f);
	std::cout << "coplanar cut" <<std::endl;
	cut(f,p);
	std::cout << "average cut" << std::endl;
	cut(f,p2);
	std::vector<approx::Vector3<float>> tmpv, tmpn;
	auto cut2 = f.cut_by(p, &tmpv, &tmpn);
	std::cout << "new container cut done" << std::endl;
	f = cut2.negative;
	auto cut3 = f.cut_by(p, &tmpv, &tmpn);
	std::cout << cut3.negative << std::endl << "--------------------------" << std::endl << cut3.positive << std::endl;
	std::cout << cut3.negative.cut_by(p).positive.cut_by(p).negative;
}

void face_centroid_test() {
	std::vector<approx::Vector3<float>> vertices{ { 0.0f, 1.0f, 3.0f },{ 1.0f, 0.0f, 3.0f },{ 2.0f, 0.0f, 3.0f },{ 3.0f, 1.0f, 3.0f },{ 2.0f, 3.0f, 3.0f } },
		normals{ { 0.0f, 0.0f, -1.0f } };
	std::reverse(vertices.begin(), vertices.end());
	approx::Face<float> f(&vertices, { 0, 1, 2, 3, 4 }, &normals, 0);
	std::cout << f.centroid() << std::endl;
}