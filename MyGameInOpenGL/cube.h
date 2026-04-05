#ifndef CUBE_H
#define CUBE_H
#include "material.h"
#include "cube_mesh.h"

class Cube {
	public:
		CubeMesh* cubeMesh;
		Material* material;
		Material* mask;
		Cube(const char* textureMaterialSrc, const char* textureMaskSrc);
		void draw();
	private: 
};

#endif
