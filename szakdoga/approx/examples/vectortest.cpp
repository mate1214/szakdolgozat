#include <iostream>
#include "vectors.h"
#include "geoios.h"
using namespace approx;
using namespace std;


int main(){
	
	Vector3<float> a(1,2,3),
				   b(0,10,0);
	cout << "a = " << a << std::endl
		 << "b = " << b << std::endl
		 << "|a| = " << a.length() << std::endl
		 << "|b| = " << b.length() << std::endl
	     << "a + b = " << (a+b) << std::endl
		 << "a - b = " << (a-b) << std::endl
		 << "a * b = " << dot(a,b) << std::endl
		 << "a x b = " << cross(a,b) << std::endl;
	return 0;
	
}