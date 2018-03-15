#pragma once
#include <memory>
#include "Projects/Rast/code/ShaderObject.h"
#include "Projects/Rast/code/MeshResource.h"
#include "Projects/Rast/code/TextureResource.h"

class GraphicsNode
{
private:
	std::shared_ptr<ShaderObject> shader;
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;

	matrix4D transformation;

public:

	void setMat(matrix4D mat)
	{
		transformation = mat;
	}

	matrix4D getMat()
	{
		return transformation;
	}

	void setShader(std::shared_ptr<ShaderObject> s)
	{ 
		shader = s; 
	};
	void setMesh(std::shared_ptr<MeshResource> m)
	{ 
		mesh = m; 
	};
	void setTexture(std::shared_ptr<TextureResource> t)
	{
		texture = t; 
	};

	std::shared_ptr<ShaderObject> getShader() { return shader; };
	std::shared_ptr<MeshResource> getMesh() { return mesh; };
	std::shared_ptr<TextureResource> getTexture() { return texture; };

	void setupShaders(char*, char*);
	void draw();
};
