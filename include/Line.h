#pragma once
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum LineAlgorithm
{
	Equation,
	DDA,
	Midpoint,
	MidpointA,
	Bresenham,
	BresenhamA
};

void lineEquation(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image);
void lineDDA(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image);
void lineMidpoint(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image);
void lineBresenham(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image);

void drawLine(glm::vec2 p1, glm::vec2 p2, TGAColor color, LineAlgorithm a, TGAImage& image);

void line(glm::vec2 p1, glm::vec2 p2, TGAImage& image);
void line(glm::vec2 p1, glm::vec2 p2, TGAColor color, TGAImage& image);