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
	return 0;
	
}

void approximator_test() {

	//ez az osztaly tartja szamon az approximaciot
	approx::Approximator<float> app;

	//a megadott fajlnevben levo test a celtest, a kezdo kocka atom 0.5-os kerettel veszi korbe
	if (!app.set_target("test.obj", 0.5f,-1)) {
		std::cout << "HIBA A FAJL BETOLTESENEL!\n";
		exit(1);
	}
	std::cout << "Az elso atom elso lapja:" << std::endl;
	std::cout << app.container().atoms(0).faces(0) << std::endl;
	std::cout << app.container().atoms(0).centroid() << std::endl;

	//A celtest az app.target().body()-ban erheto el, a body.h-ban bovebb info talalhato rola
	//es a metodusairol
	std::cout << "A celtest terfogata: " << app.target().body().volume() << "\n";
	//vagosik
	approx::Plane<float> p = approx::Plane<float>({ 0,0,1 }, {0.0f, 0.0f, 26.0f});

	//az app.container() tartalmazza az atomokat es nyujt lehetoseget az approximacios muveletekre
	//a 0. atomot elvagom az elobb megadott sikkal
	//a cut egy vagasi eredmeny, a vagast vegrehajtom, de az atomok nem kerulnek be a taroloba
	approx::Approximation<float>::CutResult cut = app.container().cut(0, p);
	
	std::cout << "===================================" << std::endl;
	std::cout << "A tarolo vertexei" << std::endl;
	std::cout << "===================================" << std::endl;
		for(auto x : app.container().vertex_container()){
		std::cout << x << "\n"; 
	}
	std::cout << "===================================\n";
	
	//mindenfele vizsgalatok a keletkezett atomokon hogy jok-e, pl terfogat ellenorzes
	//a pozitiv es negativ oldal a metszosikhoz kepesti elhelyezkedest jelenti
	if(cut.negative()->valid()){
		std::cout << "negativ oldali keletkezett atom terfogata: " << cut.negative()->volume() << std::endl;
	}
	else {
		std::cout << "a negativ oldalon nem keletkezett ervenyes atom!" << std::endl;
	}
	if(cut.positive()->valid()){
		std::cout << "pozitiv oldali keletkezett atom terfogata: " << cut.positive()->volume() << std::endl;
	}
	else{
		std::cout << "a pozitiv oldalon nem keletkezett ervenyes atom!" << std::endl;
	}
	if(cut.choose_both()){
		std::cout << "elfogadas sikeres" << std::endl;
	}
	else{
		std::cout << "elfogadas sikertelen" << std::endl;
	}
	std::cout << "a tarolo atomjainak szama elfogadasi kiserlet utan: " << app.container().size() << std::endl;
	int ind = 0;
	app.container().cut(0, approx::Plane<float>({ 1,2,1 }, {15, 30, 30})).choose_both();
	if(app.container().size() > 1){
	app.container().cut(1, approx::Plane<float>({ 3,2,1 }, {15, 30, 30})).choose_positive();
	}
	std::cout << "approximalt test terfogata " << app.container().approximated_body().volume() << std::endl;
	std::cout << "fajlok irasa... " << std::endl;
	approx::ObjectWriter<float>::save_obj("approx.obj", app.container().approximated_body());
	approx::ObjectWriter<float>::save_obj("approx_all.obj", app.container().begin(),app.container().end());
	app.save_approximated_body("asd.obj");
	
	app.restart();


	
}