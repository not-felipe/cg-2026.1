#include "acutest.h"
#include "Render2D.h"

struct Vec2Col{
	vec2 position;
	vec3 color;
};

struct Shader{
	using Varying = Vec2Col;

	void vertexShader(Vec2Col input, Varying& output){
		output = input;
	}

	void fragmentShader(Varying v, RGB& color){
		color = v.color;
	}
};

void test_triangle_colors(){
	// Em uma imagem 9x9, os vértices ficam em (1,1), (7,1), (1,7).
	Vec2Col P[] = {
		{{-2.0f/3, -2.0f/3}, red},
		{{ 2.0f/3, -2.0f/3}, green},
		{{-2.0f/3,  2.0f/3}, blue}
	};
	ImageRGB G(9, 9);
	G.fill(white);
	Shader S;
	Render2D(P, Triangles{3}, S, G);

	// O centroide (3,3) recebe um terço de cada cor.
	RGB centro = G(3, 3);
	TEST_CHECK(centro.data[0] == 85);
	TEST_CHECK(centro.data[1] == 85);
	TEST_CHECK(centro.data[2] == 85);
	TEST_CHECK(norm(vec3(G(2, 2)) - vec3{2.0f/3, 1.0f/6, 1.0f/6}) < 0.01);
	TEST_CHECK(norm(vec3(G(7, 7)) - white) == 0);
}

void test_triangle_clipped(){
	// A cor varia de vermelho em x=-2 a verde em x=2.
	Vec2Col P[] = {
		{{-2, -2}, red},
		{{ 2, -2}, green},
		{{ 0,  2}, 0.5*red + 0.5*green}
	};
	ImageRGB G(9, 9);
	G.fill(white);
	Shader S;
	Render2D(P, Triangles{3}, S, G);

	TEST_CHECK(norm(vec3(G(4, 4)) - vec3{0.5, 0.5, 0}) < 0.01);
	TEST_CHECK(norm(vec3(G(1, 1)) - vec3{2.0f/3, 1.0f/3, 0}) < 0.01);
	TEST_CHECK(norm(vec3(G(0, 8)) - white) == 0);
}

void test_triangle_degenerate(){
	Vec2Col P[] = {
		{{-0.5, 0}, red}, {{0, 0}, green}, {{0.5, 0}, blue}
	};
	ImageRGB G(9, 9);
	G.fill(white);
	Shader S;
	Render2D(P, Triangles{3}, S, G);

	for(int y = 0; y < G.height(); y++)
		for(int x = 0; x < G.width(); x++)
			TEST_CHECK(norm(vec3(G(x, y)) - white) == 0);
}

TEST_LIST = {
	{"triangle colors", test_triangle_colors},
	{"triangle clipped", test_triangle_clipped},
	{"triangle degenerate", test_triangle_degenerate},
	{NULL, NULL}
};
