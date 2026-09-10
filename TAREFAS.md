# Tarefas dos slides

As cinco tarefas estão nas aulas 07, 08 e 09. A aula 10 não tem
enunciados intitulados "Tarefa". Os outros espaços incompletos da biblioteca
pertencem a aulas fora deste conjunto e não foram alterados.

## 1. Curva fechada — aula 07, slide 60

O arquivo `petala.txt` contém duas curvas de Bézier cúbicas conectadas,
com sete pontos de controle. O último ponto repete o primeiro para fechar
a pétala. O formato é o mesmo de `borboleta.txt` e `curvaC.txt`:
quantidade de pontos na primeira linha, seguida dos pares x e y.

`tarefa07.cpp` segue os exemplos dos slides 55 e 59: carrega os pontos com
`loadCurve`, amostra a spline com `sample_bezier_spline<3>` e desenha
o contorno usando `LineStrip`. Para preencher a região, usa
`triangulate_polygon` e `Elements<Triangles>` da biblioteca existente.

Saídas: `output_tarefa07_curva.png` e `output_tarefa07_preenchida.png`.

## 2. Composição com transformações — aula 08, slide 80

`tarefa08.cpp` reutiliza `petala.txt` para desenhar três flores e suas folhas.
Cada flor tem oito cópias da pétala, combinando `translate`, `rotate_2d` e
`scale`, como nos exemplos `cg2d11g.cpp` e `cg2d16.cpp`.

A imagem tem 900 × 600 pixels e a janela vai de −3 a 3 em x e de −2 a 2
em y. Ambas têm razão de aspecto 3/2, portanto a janela não distorce o
desenho. As pétalas das flores usam escala uniforme; as folhas recebem
uma escala não uniforme intencional para mudar sua forma.

Saída: `output_tarefa08.png`.

## 3. Bézier quadrática e circunferência — aula 08, slide 84

Os pontos de controle do slide 83, também presentes em `cg2d09a.cpp`, são
P₀ = (1, 0, 1), P₁ = (1, 1, 1) e P₂ = (0, 2, 2).

Substituindo na expressão da curva quadrática, para 0 ≤ t ≤ 1:

```text
C(t) = (1 − t)² P₀ + 2t(1 − t) P₁ + t² P₂

X(t) = (1 − t)² + 2t(1 − t) = 1 − t²
Y(t) = 2t(1 − t) + 2t²     = 2t
W(t) = (1 − t)² + 2t(1 − t) + 2t² = 1 + t²
```

Logo, C(t) = (1 − t², 2t, 1 + t²). Como são coordenadas homogêneas,
dividimos por W(t), que é sempre positivo, para obter o ponto no plano:

```text
x(t) = (1 − t²)/(1 + t²)
y(t) = 2t/(1 + t²)
```

A distância desse ponto à origem satisfaz:

```text
d² = x(t)² + y(t)²
   = [(1 − t²)² + 4t²]/(1 + t²)²
   = [1 − 2t² + t⁴ + 4t²]/(1 + t²)²
   = [1 + 2t² + t⁴]/(1 + t²)²
   = (1 + t²)²/(1 + t²)²
   = 1

d = √1 = 1.
```

Portanto, todos os pontos estão sobre a circunferência unitária.
Para t de 0 a 1, o arco vai de (1, 0) a (0, 1), no primeiro quadrante.
A normalização corresponde à função `get2DPosition(vec3)` já existente.
As duas representações do slide são equivalentes em coordenadas
homogêneas; não são vetores numericamente iguais antes da divisão por W.

## 4. Interpolação de atributos — aula 09, slide 21

Em `Render2D.h`, `draw(Triangle<Varying>)` transforma os vértices para
coordenadas de tela e usa `rasterizeTriangle`. Para cada pixel, calcula
`barycentric_coords`, interpola os atributos com `mix_triangle` e chama
`paint`, como solicitado. Triângulos sem área são descartados para evitar
divisão por zero. O exemplo `cg2d12.cpp` já desenha triângulos com cores
por vértice e permite conferir a interpolação.

## 5. Recorte de linhas — aula 09, slide 39

Em `Clip2D.h`, `line_clip` segue o algoritmo de Cyrus–Beck do slide 37:
começa com `maxIn = 0` e `minOut = 1`, percorre os semiplanos de
`clip_polygon` e atualiza os parâmetros de entrada e saída.
Descarta o segmento se ambos os extremos estão fora de um semiplano
ou se `maxIn > minOut`. Ao final, usa `mix_line` sobre uma cópia dos
vértices originais para preservar a interpolação dos atributos.

## Compilação e execução

Execute na raiz do repositório, com compilador C++17 e os cabeçalhos STB
disponíveis em `stb/stb_image.h` e `stb/stb_image_write.h`.
Cada `.cpp` com `main` deve ser compilado separadamente.

```sh
mkdir -p bin
g++ -std=c++17 tarefa07.cpp Image.cpp -o bin/tarefa07
g++ -std=c++17 tarefa08.cpp Image.cpp -o bin/tarefa08
./bin/tarefa07
./bin/tarefa08

g++ -std=c++17 cg2d12.cpp Image.cpp -o bin/cg2d12
./bin/cg2d12

g++ -std=c++17 test_case04.cpp -o bin/test_case04
./bin/test_case04

g++ -std=c++17 testRender2D.cpp Image.cpp -o bin/testRender2D
./bin/testRender2D
```

`cg2d12` salva `output.png`, como os demais exemplos originais.
`testClipping01.cpp` e `testClipping02.cpp` também podem ser compilados
com `Image.cpp` para visualizar os recortes de linhas e triângulos.

Validação realizada: os três grupos de testes de `test_case04` passaram.
Os três testes de `testRender2D` também passaram, verificando cores no
interior do triângulo, interpolação após recorte e triângulos sem área.
Os dois programas das tarefas e os três exemplos visuais acima foram
compilados e executados; as imagens foram geradas.

Neste ambiente, o compilador e os cabeçalhos foram extraídos em `/tmp`,
sem instalação no sistema. Enquanto essa pasta existir, os comandos
acima podem ser usados substituindo `g++` por:

```sh
/tmp/cg-toolchain/usr/bin/g++ -B/tmp/cg-toolchain/usr/bin/ -B/tmp/cg-toolchain/usr/lib64/ -isystem /tmp/cg-toolchain/usr/include
```
