#ifndef TERRAIN_H
#define TERRAIN_H

#include "read_files.h"
#include "config.h"
#include "array_2D.h"
#include "triangle_list.h"
#include "camera.h"
#include "shader.h"
// stores/renders the heightmap

class BaseTerrain {
	public:
		BaseTerrain() {};

		void InitTerrain();

		void Render(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
		 
		void LoadFromFile(const char* pFilename); // loads a heightmap from a file

		// fetches the corresponding height value (y) from the provided x,z coordinates
		float GetHeight(int x, int z) const { return heightMap.Get(x, z); }
	protected:
		void LoadHeightMapFile(const char* pFilename);

		// width * depth of the height map = terrain size
		int terrainSize = 0;
		// attribute for the heightmap itself
		Array2D<float> heightMap;
		TriangleList triangleList;

		// location of the "VP" uniform variable declared in the GLSL vertex shader code (VP = view-projection matrix)
		unsigned int m_VPLoc = -1;
		Shader terrainShader;
};


#endif 

