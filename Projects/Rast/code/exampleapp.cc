//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#pragma once
#include "engine/config.h"
#include "Projects/Rast/code/exampleapp.h"
#include <cstring>
//#include <windows.h>

float sc = 0.05f;

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	lightPos.setVector(-1.0f, -1.0f, -1.0f);
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{

		if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			intensity++;
		}

		if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			intensity--;
			if (intensity < 0)
				intensity = 0;
		}
		
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if(action == GLFW_PRESS)
				keysPressedy++;
			if (yAcc < 0)
				yAcc = -yAcc;

			if (yAcc != 0)
				yAcc += 10 / yAcc;
			else
				yAcc += 10;
		}
		else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if (action == GLFW_PRESS)
				keysPressedx++;
			if (xAcc > 0)
				xAcc = -xAcc;
			if (xAcc != 0)
				xAcc -= 10 / -xAcc;
			else
				xAcc -= 10;
		}
		else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if (action == GLFW_PRESS)
				keysPressedy++;
			if (yAcc > 0)
				yAcc = -yAcc;

			if (yAcc != 0)
				yAcc -= 10 / -yAcc;
			else
				yAcc -= 10;
		}
		else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			if (action == GLFW_PRESS)
				keysPressedx++;
			if (xAcc < 0)
				xAcc = -xAcc;

			if (xAcc != 0)
				xAcc += 10 / xAcc;
			else
				xAcc += 10;
		}
		else if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			this->window->Close();
		}
		else if (action == GLFW_RELEASE)
		{
			if(key == GLFW_KEY_D || key == GLFW_KEY_A)
				keysPressedx--;
			if (key == GLFW_KEY_S || key == GLFW_KEY_W)
				keysPressedy--;

			if (keysPressedy == 0)
			{
				yAcc = 0;
			}
			if (keysPressedx == 0)
			{
				xAcc = 0;
			}
		}
	});

	window->SetMousePressFunction([this](int32 button, int32 pressed, int32)
	{
		if (pressed && button == GLFW_MOUSE_BUTTON_LEFT)
		{
			window->SetMouseMoveFunction([this](float64 ym, float64 xm)
			{
				matrix4D x;
				matrix4D y;
				x.setAsRotationX(xm);
				y.setAsRotationY(ym);

				rot = x*y;
			});
		}
		else if (pressed && button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			window->SetMouseMoveFunction([this](float64 xm, float64 ym)
			{
				vector4D x;
				x.setX((xm - 384)/768);
				x.setY(-((ym - 512)/1024));
				x.setZ(lightPos.getZ());

				window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
				{
					if (key == GLFW_KEY_LEFT_SHIFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
					{

						window->SetMouseMoveFunction([this](float64 xm, float64 ym)
						{
							vector4D x;
							x.setX(lightPos.getX());
							x.setY(lightPos.getY());
							x.setZ(-((ym - 512) / 1024));

							lightPos = x;
						});

					}
				});

				cout << x << endl;

				lightPos = x;
			});
		}
		else
		{
			window->SetMouseMoveFunction([this](float64, float64)
			{
			
			});
		}
		
	});

	window->SetMouseScrollFunction([this](float64, float64 val)
	{
		sc += 0.005f*val;
		if (sc < 0)
		{
			sc = 0;
		}
	});


	if (this->window->Open())
	{
		char* p = "Projects/Rast/code/quad.obj";
		
		auto sh = shared_ptr<ShaderObject>(new ShaderObject);

		auto me = shared_ptr<MeshResource>(MeshResource::loadMesh(p));

		auto te = shared_ptr<TextureResource>(new TextureResource);
		
		gfx.setMesh(me);
		gfx.setShader(sh);
		gfx.setTexture(te);

		light.setGfxRef(gfx.getShader());

		char* path1 = "Projects/Rast/code/fs.txt";
		char* path2 = "Projects/Rast/code/vs.txt";
		gfx.setupShaders(path1, path2);
		gfx.getShader()->getUniLoc("matrix");
		gfx.getShader()->getUniLoc("normalMat");

		gfx.getShader()->getUniLoc("lightPos");
		gfx.getShader()->getUniLoc("specColor");

		//gfx.getTexture()->loadImage(path);

		//glEnable(GL_CULL_FACE);
	
		// set clear color to gray
		glClearColor(0.4f, 0.0f, 0.0f, 1.0f);

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	Render r;
	r.init();

	vertex * VB = new vertex[8];
	int * IB = new int[36];

	vertex v1(-0.1f, -0.1f, 0);
	vertex v2(0.1f, -0.1f, 0);
	vertex v3(0.1f, 0.1f, 0);
	vertex v4(-0.1f, 0.1f, 0);

	vector4D uv1(0, 0, 0);
	vector4D uv2(1, 0, 0);
	vector4D uv3(1, 1, 0);
	vector4D uv4(0, 1, 0);


	VB[0] = v1;
	VB[1] = v2;
	VB[2] = v3;
	VB[3] = v4;

	VB[0].UV = uv1;
	VB[1].UV = uv2;
	VB[2].UV = uv3;
	VB[3].UV = uv4;

	VB[0].norm.setZ(-1);
	VB[1].norm.setZ(-1);
	VB[2].norm.setZ(-1);
	VB[3].norm.setZ(-1);

	IB[0] = 0;
	IB[1] = 1;
	IB[2] = 2;
	IB[3] = 2;
	IB[4] = 3;
	IB[5] = 0;

	char* path = "Projects/Rast/code/besttexture.jpg";
	unsigned char* image = gfx.getTexture()->getImage(path);

	
	gfx.getTexture()->generateHandle();

	matrix4D cam;

	cam.setPosition(0, 0, 10);

	matrix4D scale;

	matrix4D d;
	d.setAsRotationZ(90);
	while (this->window->IsOpen())
	{

		light.setPosition(lightPos);
		light.setColor(vector4D(0, 0, 0));
		light.setIntensity(0);

		move.setPosition(move.getVal(1, 4) + (0.001f*xAcc), move.getVal(2, 4) + (0.001f*yAcc), move.getVal(3, 4));
		scale = scale.makeScaling(sc);
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		gfx.setMat(d * scale);

		r.drawBuffer(VB, IB, 4, 6, image, 184, 184, move*rot, rot, lightPos);
		unsigned char* tex = new unsigned char[1024 * 768 * 3];
		tex = r.getTex(tex);
		gfx.getTexture()->loadImageTwo(tex, 1024, 768);
		
		delete[] tex;
		gfx.draw();
		gfx.getShader()->setUniMat("normalMat", cam);
		light.applyMat();

		this->window->SwapBuffers();
	}
}

} // namespace Example
