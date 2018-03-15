#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "engine/core/app.h"
#include "engine/render/window.h"
#include "Projects/Rast/code/GraphicsNode.h"
#include "Projects/Rast/code/math.h"
#include "Projects/Rast/code/LightNode.h"
#include "Projects/Rast/code/Render.h"

namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();
private:
	LightNode light;
	GraphicsNode gfx;

	float intensity = 1;

	vector4D lightPos;

	float xAcc = 0;
	float yAcc = 0;

	int keysPressedx = 0;
	int keysPressedy = 0;
	matrix4D move;
	matrix4D rot;
	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
};
} // namespace Example
