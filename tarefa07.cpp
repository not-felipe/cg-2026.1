#include "Render2D_v1.h"
#include "bezier.h"
#include "polygon_triangulation.h"

int main(){
	std::vector<vec2> P = loadCurve("petala.txt");
	if(P.empty()){
		std::cerr << "Nao foi possivel carregar petala.txt\n";
		return 1;
	}
	std::vector<vec2> Q = sample_bezier_spline<3>(P, 30);
	std::vector<unsigned int> indices = triangulate_polygon(Q);

	LineStrip L{Q.size()};
	Elements<Triangles> T{indices};

	ImageRGB G(600, 600);
	G.fill(white);
	render2d(Q, L, magenta, G);
	G.save("output_tarefa07_curva.png");

	G.fill(white);
	render2d(Q, T, magenta, G);
	G.save("output_tarefa07_preenchida.png");
}
