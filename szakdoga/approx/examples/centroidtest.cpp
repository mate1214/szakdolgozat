#include <iostream>
#include "body.h"
#include "geoios.h"
#include "objio.h"

void centroid_test();

int main() {

	centroid_test();

	return 0;

}

void centroid_test() {

	std::vector<approx::Vector3<float>> vecs{
		{4.5f,2,2.3f},
		{6.3f,2,0.4f},
		{3.5f ,5,1.7f },
		{1.7f ,2,0.9f }
	}, normals;

	for (auto& v : vecs) {
		v *= 0.01;
	}

	std::vector<approx::Face<float>> faces{
		approx::Face<float>(&vecs,std::vector<int>{0,1,2},&normals),
		approx::Face<float>(&vecs,std::vector<int>{3,0,2},&normals),
		approx::Face<float>(&vecs,std::vector<int>{1,3,2},&normals),
		approx::Face<float>(&vecs,std::vector<int>{0,3,1},&normals)
	};

	approx::Body<float> bdy(&faces, std::vector<int>{0, 1, 2, 3});

	std::cout << "centroid: " << bdy.centroid() << "\n";

}