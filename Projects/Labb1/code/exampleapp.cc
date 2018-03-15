//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "engine/config.h"
#include "Projects/Labb1/code/exampleapp.h"
#include <halfedge.h>
#include "Projects/Labb1/code/math.h"
#include "Projects/Labb1/code/TextureResource.h"
#include <cstring>
#include <chrono>

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec2 uv;\n"
"layout(location=2) in vec3 norm;\n"
"layout(location=0) out vec2 UV;\n"
"layout(location=1) out vec3 vertPos;\n"
"layout(location=2) out vec3 NORM;\n"
"uniform mat4 matrix;\n"
"void main()\n"
"{\n"
"	gl_Position = matrix * vec4(pos, 1);\n"
"   UV.x = uv.x;\n"
"   UV.y = 1 - uv.y;\n"
"   vec4 vp4 = matrix * vec4(pos, 1);\n"
"   vertPos = vec3(vp4) /vp4.w;\n"
"   NORM = norm;\n"
//"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec2 uv;\n"
"layout(location=1) in vec3 vertPos;\n"
"layout(location=2) in vec3 NORM;\n"
"out vec3 Color;\n"
"uniform sampler2D texSampler;\n"
"void main()\n"
"{\n"
"	Color = texture(texSampler,uv).rgb;\n"
"}\n";

bool draw = true;

Mesh m;

GLuint VBH;
GLint loc;

matrix4D rot;
matrix4D scale;
int vbl;

TextureResource text;

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
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

struct vbobject
{
    v3 pos;
    v2 uv;
    v3 norm;
};

bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
                                {
                                    //draw = !draw;
                                    if(key == GLFW_KEY_ESCAPE)
                                    {
                                        this->window->Close();
                                    }
                                    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
                                    {
                                        auto t1 = std::chrono::high_resolution_clock::now();
                                        m.subdivide();
                                        auto t2 = std::chrono::high_resolution_clock::now();
                                        double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() * 0.000000001;
                                        printf("Subdiv took: %f seconds\n", duration);

                                        v3* VB = m.getVB();
                                        v2* UVB = m.getUV();
                                        v3* NB = m.getNormals();
        
                                        vbl = m.VBLen();

                                        vbobject *VBO = new vbobject[vbl];
                                        for(int i = 0; i < vbl; i++)
                                        {
                                            VBO[i].pos = VB[i];
                                            VBO[i].uv = UVB[i];
                                            VBO[i].norm = NB[i];
                                        }
                                        // setup vbo
                                        glGenBuffers(1, &VBH);
                                        glBindBuffer(GL_ARRAY_BUFFER, VBH);
                                        glBufferData(GL_ARRAY_BUFFER, (sizeof(v3) + sizeof(v2) + sizeof(v3))*vbl, VBO, GL_STATIC_DRAW);
                                        glBindBuffer(GL_ARRAY_BUFFER, 0);

                                        delete[] VB;
                                        delete[] UVB;
                                        delete[] NB;
                                        delete[] VBO;
                                        
                                    }
                                });
    window->SetMouseMoveFunction([this](float64 ym, float64 xm)
                                  {
                                      matrix4D x;
                                      matrix4D y;
                                      x.setAsRotationX(xm);
                                      y.setAsRotationY(-ym);

                                      rot = x*y;
                                  });

	GLfloat buf[] =
	{
		-0.5f,	-0.5f,	-1,			// pos 0
		1,		0,		0,		1,	// color 0
		0,		0.5f,	-1,			// pos 1
		0,		1,		0,		1,	// color 0
		0.5f,	-0.5f,	-1,			// pos 2
		0,		0,		1,		1	// color 0
	};

	if (this->window->Open())
	{


        scale.setVal(1, 1, 0.3);
        scale.setVal(2, 2, 0.3);
        scale.setVal(3, 3, 0.3);
        
        m.generateHalfEdge((char*)"Projects/Labb1/code/cube_with_hole.obj");
        
        text.generateHandle();
        text.loadImage((char*)"Projects/Labb1/code/besttexture.jpg");
        
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

        v3* VB = m.getVB();
        v2* UVB = m.getUV();
        v3* NB = m.getNormals();
        
        vbl = m.VBLen();
        
        vbobject *VBO = new vbobject[vbl];
        for(int i = 0; i < vbl; i++)
        {
            VBO[i].pos = VB[i];
            VBO[i].uv = UVB[i];
            VBO[i].norm = NB[i];
        }
		// setup vbo
		glGenBuffers(1, &VBH);
		glBindBuffer(GL_ARRAY_BUFFER, VBH);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(v3) + sizeof(v2) + sizeof(v3))*vbl, VBO, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

        delete[] VB;
        delete[] UVB;
        delete[] NB;
        delete[] VBO;
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);
        loc = glGetUniformLocation(this->program, "matrix");
        text.preRender();
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
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

        if(draw)
        {
		// do stuff
        glBindBuffer(GL_ARRAY_BUFFER, VBH);
		glUseProgram(this->program);
        
        glUniformMatrix4fv(loc, 1, 0, (rot*scale).getMat());
        
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 3));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 8, (GLvoid*)(sizeof(float32) * 5));
        if(m.ePerF() == 3)
            glDrawArrays(GL_TRIANGLES, 0, vbl);
        else
            glDrawArrays(GL_QUADS, 0, vbl);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
		this->window->SwapBuffers();
	}
}

} // namespace Example
