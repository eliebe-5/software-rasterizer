#pragma once
#include "Projects/Rast/code/GraphicsNode.h"
#include <functional>
#include <vector>

struct color
{
	color(int re, int gr, int bl) { r = re; g = gr; b = bl; };
	int r, g, b;
};

class Render
{
public:
	Render();
	void setVB(vertex* v) { VB = v; }
	void setIB(int* i) { IB = i; }
	void init();
	std::vector<vertex> bresenham(vertex, vertex);
	void rasterize(vertex, vertex, vertex);
	void drawBuffer(vertex*, int*, int, int, unsigned char*, int, int, matrix4D, matrix4D, vector4D);
	void colorPixel(vertex);
	unsigned char* getTex(unsigned char*);
	/*void fillBotFlat(vertex, vertex, vertex);
	void fillTopFlat(vertex, vertex, vertex);*/
	void filler(vertex, vertex, vertex);
private:
	float * zBuffer;
	const int screenWidth = 1024;
	const int screenHeight = 768;
	std::function<color(vertex)> pixel;
	vertex * VB;
	int * IB;
	matrix4D MVP;
	matrix4D viewM;
	matrix4D projectionM;
	char* ImgR;
	char* ImgG;
	char* ImgB;

	vector4D vertPos;
	vector4D lightPos;
	
	
};
