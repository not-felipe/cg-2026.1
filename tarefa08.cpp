#include "Render2D_v1.h"
#include "bezier.h"
#include "polygon_triangulation.h"
#include "transforms2d.h"

int main(){
	std::vector<vec2> P = loadCurve("petala.txt");
	if(P.empty()){
		std::cerr << "Nao foi possivel carregar petala.txt\n";
		return 1;
	}
	std::vector<vec2> Q = sample_bezier_spline<3>(P, 30);
	std::vector<unsigned int> indices = triangulate_polygon(Q);
	Elements<Triangles> T{indices};
	LineStrip L{Q.size()};

	ImageRGB G(900, 600);
	G.fill(RGB(0xfff8ee));
	// A janela e a imagem têm a mesma razão de aspecto: 3/2.
	mat3 W = window(-3, 3, -2, 2);

	vec2 centros[] = {{-1.8, 0.6}, {0, 0.8}, {1.8, 0.4}};
	float tamanhos[] = {0.8, 1.0, 0.7};
	vec3 cores[] = {RGB(0xe87583), RGB(0xeeb447), RGB(0xb38bcb)};
	vec3 verde = RGB(0x477852);
	std::vector<vec2> caule = {{0, -1.7}, {0, 0}};
	Lines LC{2};

	for(int i = 0; i < 3; i++){
		mat3 C = translate(centros[i]);
		render2d(W*C*caule, LC, verde, G);

		// A mesma pétala também forma as folhas dos caules.
		mat3 F = C*translate(0, -1.0)*rotate_2d(-0.9)*scale(0.45, 0.65);
		render2d(W*F*Q, T, verde, G);
		F = C*translate(0, -1.3)*rotate_2d(0.9)*scale(0.35, 0.55);
		render2d(W*F*Q, T, verde, G);

		for(int j = 0; j < 8; j++){
			float angulo = 2*M_PI*j/8;
			mat3 M = C*rotate_2d(angulo)*scale(tamanhos[i], tamanhos[i]);
			render2d(W*M*Q, T, cores[i], G);
			render2d(W*M*Q, L, 0.8*cores[i], G);
		}
	}
	G.save("output_tarefa08.png");
}
