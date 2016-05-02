#include <iostream>
#include "approximation.h"
#include "approximator.h"
#include "geoios.h"

void targetbody_test();

int main(){
	
	targetbody_test();
	return 0;
	
}

void targetbody_test() {

	//ez az osztaly tartja szamon az approximaciot
	approx::Approximator<float> app;

	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
	if (!app.set_target("gummybear.obj", 1.0f)) {
		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
		exit(1);
	}

	std::cout << "volume: " << app.target().body().volume() << std::endl;
	std::cout << "centroid: " << app.target().body().centroid() << std::endl;
}
