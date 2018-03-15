#pragma once
#include "Projects/Rast/code/GraphicsNode.h"
#include <assert.h>

void GraphicsNode::draw()
{
	assert(!(shader == nullptr || mesh == nullptr || texture == nullptr));
	

	texture->preRender();
	shader->apply();
	mesh->draw();

	shader->setUniMat("matrix", transformation);
}

void GraphicsNode::setupShaders(char* fs, char* vs)
{
	shader->loadFragmentShader(fs);
	shader->loadVertexShader(vs);
	shader->loadProgram();
}
