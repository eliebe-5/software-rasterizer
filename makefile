CC=g++
CFLAGS=-I. -lGLEW -lGL -lglfw -pthread
DBFLAG=-g
ENGINE= engine/config.cc engine/core/app.cc engine/render/window.cc

rast:
	$(CC) -std=c++0x -o Rasterizer Projects/Rast/code/main.cc Projects/Rast/code/Render.cc Projects/Rast/code/LightNode.cc Projects/Rast/code/exampleapp.cc Projects/Rast/code/GraphicsNode.cc Projects/Rast/code/MeshResource.cc Projects/Rast/code/ShaderObject.cc Projects/Rast/code/TextureResource.cc $(ENGINE) $(CFLAGS) 
rast-db:
	$(CC) -std=c++0x -o Rasterizer Projects/Rast/code/main.cc Projects/Rast/code/Render.cc Projects/Rast/code/LightNode.cc Projects/Rast/code/exampleapp.cc Projects/Rast/code/GraphicsNode.cc Projects/Rast/code/MeshResource.cc Projects/Rast/code/ShaderObject.cc Projects/Rast/code/TextureResource.cc $(ENGINE) $(CFLAGS) $(DBFLAG)

labb1:
	$(CC) -std=c++0x -o Labb1 Projects/Labb1/code/main.cc Projects/Labb1/code/exampleapp.cc /usr/include/halfedge.cc Projects/Labb1/code/TextureResource.cc $(ENGINE) $(CFLAGS)

labb1-db:
	$(CC) -std=c++0x -o Labb1 Projects/Labb1/code/main.cc Projects/Labb1/code/exampleapp.cc /usr/include/halfedge.cc Projects/Labb1/code/TextureResource.cc $(ENGINE) $(CFLAGS) $(DBFLAG)

