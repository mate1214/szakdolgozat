#include <vector>
#include <iostream>
#include "objrepair.h"
#include "geoios.h"

//A javitovektorok tesztelese feher es feketedoboz modszerekkel

void blackbox();
void whitebox();
void convert_with_repairvector(const std::vector<approx::Vector3<float>>&, float);
void convert_with_nullrepair(const std::vector<approx::Vector3<float>>&);


int main() {
	std::cout << "BLACK BOX TEST" << std::endl;
	blackbox();
	std::cout << "WHITE BOX TEST" << std::endl;
	whitebox();
	std::cout << "TEST ENDED" << std::endl;

	std::cin.get();
	return 0;

}



void convert_with_repairvector(const std::vector<approx::Vector3<float>>& vec, float eps) {
	approx::RepairVector<float> rvec(eps);
	std::cout << "epsilon: " << eps << std::endl;
	std::cout << "original: " << vec << std::endl;
	for (approx::Vector3<float> f : vec) {
		rvec.push_back(f);
	}

	//std::cout << "repaired: " << (std::vector<approx::Vector3<float>>)rvec << std::endl;
	std::cout << "repaired: [ ";
	for (int i = 0; i<rvec.size(); ++i) {

		std::cout << rvec[i] << " ";

	}
	std::cout << "]";


	std::cout << "needed: " << rvec.needed_vecs() << std::endl;
}
void convert_with_nullrepair(const std::vector<approx::Vector3<float>>& vec) {
	approx::NullRepair<float> rvec;
	std::cout << "original: " << vec << std::endl;
	for (approx::Vector3<float> f : vec) {
		rvec.push_back(f);
	}
	std::cout << "repaired: " << (std::vector<approx::Vector3<float>>)rvec << std::endl;
	std::cout << "needed: " << rvec.needed_vecs() << std::endl;
}

void blackbox() {
	std::cout << "Testing: RepairVector<float>" << std::endl;
	std::vector<approx::Vector3<float>> in1{ { 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 1,2,3 },{ 6,5,1 } }; //van ismetlodes
	std::vector<approx::Vector3<float>> in2{}; //ures bemenet
	std::vector<approx::Vector3<float>> in3{ { 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 6,5,1 } }; //nincs ismetlodes
	std::vector<approx::Vector3<float>> in4{ { 1,2,3 },{ 4,3,2 },{ 4,3,1.7f },{ 6,5,1 } }; //epszilonos ismetlodes van
	convert_with_repairvector(in1, 0.0f);
	std::cout << std::endl;
	convert_with_repairvector(in1, 0.4f);
	std::cout << std::endl;
	convert_with_repairvector(in2, 0.4f);
	std::cout << std::endl;
	convert_with_repairvector(in3, 0.2f);
	std::cout << std::endl;
	convert_with_repairvector(in3, 10.0f);
	std::cout << std::endl;
	convert_with_repairvector(in4, 0.5f);
	std::cout << std::endl;
	convert_with_repairvector(in4, 0.1f);
	std::cout << std::endl;
	std::cout << "Testing: NullRepair<float>" << std::endl;
	convert_with_nullrepair(in1);
	std::cout << std::endl;
	convert_with_nullrepair(in2);
	std::cout << std::endl;
	convert_with_nullrepair(in3);
	std::cout << std::endl;
	convert_with_nullrepair(in4);
	std::cout << std::endl;
}

void whitebox() {

	std::cout << "Testing: RepairVector<float>" << std::endl;
	std::vector<approx::Vector3<float>> in1{ { 1,2,3 },{ 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 6,5,1 } }; //van pontos ismetlodes az elejen
	std::vector<approx::Vector3<float>> in2{ { 4,3,2 },{ 6,5,1 },{ 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 6,5,1 },{ 4,3,2 }, }; //tobbszoros pontos ismetlodes
	std::vector<approx::Vector3<float>> in3{ { 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 6,5,1 },{ 6,5,1 } }; // pontos imsetlodes a vegen
	std::vector<approx::Vector3<float>> in4{ { 6,5,1.05f },{ 1,2,3 },{ 4,3,2 },{ 4,3,1 },{ 6,5,1.1f },{ 6,5,1 } }; // epszilonos imsetlodes tobbszorosen
	convert_with_repairvector(in1, 0.0f);
	convert_with_repairvector(in1, 0.2f);
	convert_with_repairvector(in2, 0.0f);
	convert_with_repairvector(in2, 0.2f);
	convert_with_repairvector(in3, 0.0f);
	convert_with_repairvector(in3, 0.2f);
	convert_with_repairvector(in4, 0.0f);
	convert_with_repairvector(in4, 0.1f);
	convert_with_repairvector(in4, 1.0f);
	std::cout << "Testing: NullRepair<float>" << std::endl;
	convert_with_nullrepair(in1);
	convert_with_nullrepair(in2);
	convert_with_nullrepair(in3);
	convert_with_nullrepair(in4);
}
