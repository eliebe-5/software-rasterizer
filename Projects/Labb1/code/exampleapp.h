#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "engine/config.h"
#include "engine/core/app.h"
#include "engine/render/window.h"

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

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
};
} // namespace Example
