#pragma once

#include "Projects/Rast/code/Render.h"

void SwitchToOctantZeroFrom(int octant, vertex A, vertex* B)
{
	float temp;
	switch (octant)
	{
	case 0:
		break;
	case 1:
		B->pos.setX(B->pos.getX() - A.pos.getX());
		B->pos.setY(B->pos.getY() - A.pos.getY());
		temp = B->pos.getX();
		B->pos.setX(B->pos.getY());
		B->pos.setY(temp);
		B->pos.setX(B->pos.getX() + A.pos.getX());
		B->pos.setY(B->pos.getY() + A.pos.getY());
		break;
	case 6:
		B->pos.setX(B->pos.getX() - A.pos.getX());
		B->pos.setY(B->pos.getY() - A.pos.getY());
		temp = B->pos.getY();
		B->pos.setY( -B->pos.getX());
		B->pos.setX(temp);
		B->pos.setX(B->pos.getX() + A.pos.getX());
		B->pos.setY(B->pos.getY() + A.pos.getY());
		break;
	case 7:
		B->pos.setX(B->pos.getX() - A.pos.getX());
		B->pos.setY(B->pos.getY() - A.pos.getY());
		B->pos.setY(-(B->pos.getY()));
		B->pos.setX(B->pos.getX() + A.pos.getX());
		B->pos.setY(B->pos.getY() + A.pos.getY());
		break;
	}
}

int SwitchFromOctantZeroTo(vertex* A, vertex* B)
{
	if (B->pos.getX() >= A->pos.getX())
		if (B->pos.getY() > A->pos.getY())
			if ((B->pos.getY() - A->pos.getY()) < (B->pos.getX() - A->pos.getX()))
				return 0;
			else
			{
				B->pos.setX(B->pos.getX() - A->pos.getX());
				B->pos.setY(B->pos.getY() - A->pos.getY());
				float temp = B->pos.getX();
				B->pos.setX(B->pos.getY());
				B->pos.setY(temp);
				B->pos.setX(B->pos.getX() + A->pos.getX());
				B->pos.setY(B->pos.getY() + A->pos.getY());
				return 1;
			}
		else
			if ((A->pos.getY() - B->pos.getY()) < (B->pos.getX() - A->pos.getX()))
			{
				B->pos.setX(B->pos.getX() - A->pos.getX());
				B->pos.setY(B->pos.getY() - A->pos.getY());
				B->pos.setY(-B->pos.getY());
				B->pos.setX(B->pos.getX() + A->pos.getX());
				B->pos.setY(B->pos.getY() + A->pos.getY());
				return 7;
			}
			else
			{
				B->pos.setX(B->pos.getX() - A->pos.getX());
				B->pos.setY(B->pos.getY() - A->pos.getY());
				float temp = B->pos.getX();
				B->pos.setX(-B->pos.getY());
				B->pos.setY(temp);
				B->pos.setX(B->pos.getX() + A->pos.getX());
				B->pos.setY(B->pos.getY() + A->pos.getY());
				return 6;
			}
	return -1;
}

Render::Render()
{
	ImgR = new char[screenWidth * screenHeight];
	ImgG = new char[screenWidth * screenHeight];
	ImgB = new char[screenWidth * screenHeight];
	zBuffer = new float[screenWidth * screenHeight];
	for (int i = 0; i < screenWidth*screenHeight; i++)
	{
		ImgR[i] = 128;
		ImgG[i] = 128;
		ImgB[i] = 128;
		zBuffer[i] = -5;
	}

	vertPos.setVector(0, 0, 0);
	lightPos.setVector(0, 0, -5);
}

void Render::init()
{
	vector4D eye, target, up;
	eye.setVector(0, 0, -5);
	target.setVector(0, 0, 0);
	up.setVector(0, 1, 0);

	vector4D temp = target - eye;

	vector4D zaxis = temp.norm();
	temp = up.cross(zaxis);
	vector4D xaxis = temp.norm();
	vector4D yaxis = zaxis.cross(xaxis);

	matrix4D view;
	view.setVal(1, 1, xaxis.getX());
	view.setVal(2, 1, xaxis.getY());
	view.setVal(3, 1, xaxis.getZ());
	view.setVal(4, 1, -xaxis.dot(eye));

	view.setVal(1, 2, yaxis.getX());
	view.setVal(2, 2, yaxis.getY());
	view.setVal(3, 2, yaxis.getZ());
	view.setVal(4, 2, -yaxis.dot(eye));

	view.setVal(1, 3, zaxis.getX());
	view.setVal(2, 3, zaxis.getY());
	view.setVal(3, 3, zaxis.getZ());
	view.setVal(4, 3, -zaxis.dot(eye));

	view.setVal(1, 4, 0);
	view.setVal(2, 4, 0);
	view.setVal(3, 4, 0);
	view.setVal(4, 4, 1);

	viewM = view;

	float fov = 90;
	float near = 1.0;
	float far = 10.0;

	float scale = 1 / tan((fov * 0.5f) * (MATH_PI / 180));

	matrix4D proj;
	proj.setVal(1, 1, scale);
	proj.setVal(2, 2, scale);
	proj.setVal(3, 3, far / (far - near));
	proj.setVal(4, 3, -far * near / (far - near));
	proj.setVal(3, 4, 1);
	proj.setVal(4, 4, 0);

	projectionM = proj;
}

vertex bary(vertex toInterpolate, vertex a, vertex b, vertex c)
{
	float areaTot = abs(( a.pos.getX()*(b.pos.getY() - c.pos.getY())
					+ b.pos.getX()*(c.pos.getY() - a.pos.getY())
					+ c.pos.getX()*(a.pos.getY() - b.pos.getY())
					) / 2);
	float areaAB = abs((a.pos.getX()*(b.pos.getY() - toInterpolate.pos.getY())
					+ b.pos.getX()*(toInterpolate.pos.getY() - a.pos.getY())
					+ toInterpolate.pos.getX()*(a.pos.getY() - b.pos.getY())
					) / 2);
	float areaAC = abs((a.pos.getX()*(toInterpolate.pos.getY() - c.pos.getY())
					+ toInterpolate.pos.getX()*(c.pos.getY() - a.pos.getY())
					+ c.pos.getX()*(a.pos.getY() - toInterpolate.pos.getY())
					) / 2);
	float areaBC = abs((toInterpolate.pos.getX()*(b.pos.getY() - c.pos.getY())
					+ b.pos.getX()*(c.pos.getY() - toInterpolate.pos.getY())
					+ c.pos.getX()*(toInterpolate.pos.getY() - b.pos.getY())
					) / 2);

	float pOfA = areaBC / areaTot;
	float pOfB = areaAC / areaTot;
	float pOfC = areaAB / areaTot;

	if(pOfA + pOfB + pOfC > 1)
		return toInterpolate;

	toInterpolate.norm = a.norm * pOfA + b.norm * pOfB + c.norm * pOfC;
	toInterpolate.UV = a.UV * pOfA + b.UV * pOfB + c.UV * pOfC;
	toInterpolate.pos.setZ(a.pos.getZ() * pOfA + b.pos.getZ() * pOfB + c.pos.getZ() * pOfC);

	return toInterpolate;
}

std::vector<vertex> Render::bresenham(vertex p1, vertex p2)
{
	int rem = SwitchFromOctantZeroTo(&p1, &p2);

	std::vector<vertex> points;

	float dVar = 2 * (p2.pos.getY() - p1.pos.getY()) - (p2.pos.getX() - p1.pos.getX());

	int x = (int)p1.pos.getX();
	int y = (int)p1.pos.getY();

	for (;x < p2.pos.getX(); x++)
	{
		if (dVar < 0)
			dVar = dVar + 2 * (p2.pos.getY() - p1.pos.getY());
		else
		{
			y++;
			dVar = dVar + 2 * ((p2.pos.getY() - p1.pos.getY()) - (p2.pos.getX() - p1.pos.getX()));
		}
		vertex edgePoint = vertex(x, y, 0.0f);
		SwitchToOctantZeroFrom(rem, p1, &edgePoint);
		points.push_back(edgePoint);
	}

	return points;
}

void Render::filler(vertex a, vertex b, vertex c)
{
	std::vector<vertex> lineA;
	std::vector<vertex> lineB;
	std::vector<vertex> lineC;

	std::vector<vertex> fill;

	lineA = bresenham(b, a);
	lineB = bresenham(c, b);
	lineC = bresenham(c, a);

	for (unsigned int i = 0; i < lineB.size(); i++)
	{
		fill = bresenham(a, lineB[i]);
		for (unsigned int j = 0; j < fill.size(); j++)
		{
			fill[j] = bary(fill[j], a, b, c);

			colorPixel(fill[j]);
		}
	}
}

void Render::rasterize(vertex a, vertex b, vertex c)
{

	if ((a.pos[0] == b.pos[0] && a.pos[1] == b.pos[1]) || (a.pos[0] == c.pos[0] && a.pos[1] == c.pos[1]) || (b.pos[0] == c.pos[0] && b.pos[1] == c.pos[1]))
		return;

	std::vector<vertex> lineA;
	std::vector<vertex> lineB;
	std::vector<vertex> lineC;

	std::vector<vertex> fill;

	if(a.pos.getX() < b.pos.getX())
		lineA = bresenham(a, b);
	else
		lineA = bresenham(b, a);

	if(b.pos.getX() < c.pos.getX())
		lineB = bresenham(b, c);
	else
		lineB = bresenham(c, b);

	if(a.pos.getX() < c.pos.getX())
		lineC = bresenham(a, c);
	else
		lineC = bresenham(c, a);

	for (unsigned int i = 1; i < lineA.size(); i++)
	{
		lineA[i] = bary(lineA[i], a, b, c);
		colorPixel(lineA[i]);
	}
	for (unsigned int i = 1; i < lineB.size(); i++)
	{
		lineB[i] = bary(lineB[i], a, b, c);
		colorPixel(lineB[i]);
	}
	for (unsigned int i = 1; i < lineC.size(); i++)
	{
		lineC[i] = bary(lineC[i], a, b, c);
		colorPixel(lineC[i]);
	}

	filler(a, b, c);
	filler(b, a, c);
}

void Render::drawBuffer(vertex* vertexBuffer, int* indexBuffer, int vbl, int ibl, unsigned char* texture, int textW, int textH, matrix4D modelM, matrix4D normalMR, vector4D lightP)
{

	lightPos = lightP;
	for (int i = 0; i < screenWidth*screenHeight; i++)
	{
		ImgR[i] = 128;
		ImgG[i] = 128;
		ImgB[i] = 128;
		zBuffer[i] = -5;
	}

	MVP = projectionM * viewM * modelM;

	VB = new vertex[vbl];
	IB = new int[ibl];

	for (int i = 0; i < vbl; i++)
	{
		VB[i] = vertexBuffer[i];
	}

	for (int i = 0; i < ibl; i++)
	{
		IB[i] = indexBuffer[i];
	}

	for (int i = 0; i < vbl; i++)
	{
		VB[i].pos = MVP * VB[i].pos;
		VB[i].norm = normalMR * VB[i].norm;
		VB[i].pos.divideByW();
		VB[i].pos.setX((float)(int)(VB[i].pos.getX() * screenWidth));
		VB[i].pos.setY((float)(int)(VB[i].pos.getY() * screenHeight));
	}

	pixel = [texture, textH, textW, lightP, normalMR](vertex v)->color
	{

	  if(v.UV.getX() >= 1) v.UV.setX(0.99999f);
	  if(v.UV.getY() >= 1) v.UV.setY(0.99999f);
	  if(v.UV.getX() < 0) v.UV.setX(0.0f);
	  if(v.UV.getY() < 0) v.UV.setY(0.0f);
		
		float shininess = 16.0f;
		float screenGamma = 2.2f;
		vector4D ambientColor = vector4D(0.0, 0.0, 0.0);

		vector4D specColor(0.5, 0.5, 0.5);

		matrix4D cam;
		cam.setPosition(0, 0, 0);

		color c(0,0,0);

		vector4D temp = lightP;
		temp.norm();
		vector4D worldPoint = v.pos;

		matrix4D temp2 = normalMR;

		vector4D normal = (temp2 * v.norm).norm();
		vector4D lightDir = (temp - worldPoint.norm()).norm();

		int n = ((int)(v.UV.getY() * textH) * textW + (int)(v.UV.getX() * textW)) * 3;

		if (n < 0)
			n = 0;

		float diffuseColor_array[4] = { abs((int)texture[n]) , abs((int)texture[n + 1]), abs((int)texture[n + 2]), 1.0f };

		vector4D diffuseColor = vector4D(diffuseColor_array[0] * (1.0f / 255.0f), diffuseColor_array[1] * (1.0f / 255.0f), diffuseColor_array[2] * (1.0f / 255.0f));

		float lambertian = lightDir.dot(normal) > 0.0f ? lightDir.dot(normal) : 0.0f;
		float specular = 0.2f;

		if (lambertian > 0.0f)
		{
			vector4D viewDir = (cam.getPosition().norm() - worldPoint.norm()).norm();
			vector4D halfDir = (lightDir + viewDir).norm();
			float specAngle = halfDir.dot(normal) > 0.0f ? halfDir.dot(normal) : 0.0f;
			specular = pow(specAngle, shininess);

		}

		vector4D colorLinear = ambientColor + diffuseColor * lambertian + specColor * specular;
		vector4D colorGammaCorrected;
		colorGammaCorrected[0] = pow(colorLinear[0], screenGamma) * 255 < 255 ? pow(colorLinear[0], screenGamma) * 255 : 255;
		colorGammaCorrected[1] = pow(colorLinear[1], screenGamma) * 255 < 255 ? pow(colorLinear[1], screenGamma) * 255 : 255;
		colorGammaCorrected[2] = pow(colorLinear[2], screenGamma) * 255 < 255 ? pow(colorLinear[2], screenGamma) * 255 : 255;

		c.r = colorGammaCorrected[0];
		c.g = colorGammaCorrected[1];
		c.b = colorGammaCorrected[2];

		return c;
	};

	for (int i = 0; i < ibl; i+=3)
	{
		rasterize(VB[IB[i]], VB[IB[i + 1]], VB[IB[i + 2]]);
	}

	delete[] VB;
	delete[] IB;
}

void Render::colorPixel(vertex v)
{
	color c = pixel(v);

	if (v.pos.getX() > 0 && v.pos.getX() * v.pos.getY() > 0 && v.pos.getX() * v.pos.getY() < screenHeight * screenWidth && v.pos.getX() < screenWidth && v.pos.getY() < screenHeight)
	{
		if (zBuffer[(int)(v.pos.getY() * screenWidth + v.pos.getX())] <= v.pos.getZ())
		{
			zBuffer[(int)(v.pos.getY() * screenWidth + v.pos.getX())] = v.pos.getZ();
			int n = (int)(v.pos.getY() * screenWidth + v.pos.getX());

			ImgR[n] = (char)(c.r);
			ImgG[n] = (char)(c.g);
			ImgB[n] = (char)(c.b);
		}
	}
}

unsigned char* Render::getTex(unsigned char* tex)
{
	int j = 0;
	for (int i = 0; i < screenWidth * 3 * screenHeight; i+=3)
	{
		tex[i] = ImgR[j];
		tex[i + 1] = ImgG[j];
		tex[i + 2] = ImgB[j];
		j++;
	}
	return tex;
}
