#include "polygraph.h"
#include "geoios.h"

//============================================================================================
//Polygon epito pelda
//
//Bemutatja a graf alapu polygonepites mukodeset
//============================================================================================


void poly_graph_test();

int main() {
	
	poly_graph_test();
	return 0;
	
}

void poly_graph_test(){
	approx::Graph<float> gr;
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
		std::cout << p;
	}

}