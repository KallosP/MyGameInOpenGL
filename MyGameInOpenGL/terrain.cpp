#include "terrain.h"

void BaseTerrain::InitTerrain(float w) {
	worldScale = w;
	terrainShader = Shader("terrain.vs", "terrain.fs");
	m_VPLoc = glGetUniformLocation(terrainShader.ID, "gVP");
}

void BaseTerrain::LoadFromFile(const char* pFilename) {
	LoadHeightMapFile(pFilename);
	triangleList.CreateTriangleList(terrainSize, terrainSize, this);
}

void BaseTerrain::LoadHeightMapFile(const char* pFilename) {
	int FileSize = 0;
	unsigned char* p = (unsigned char*) ReadBinaryFile(pFilename, FileSize);

	// ensure the file has a whole number of floating point values
	assert(FileSize % sizeof(float) == 0); 

	// heightmap is square, so width = depth = sqrt(total number of floats)
	// need to divide by sizeof(float) to get the number of floats in the file
	terrainSize = (int)sqrt((float)FileSize / (float)sizeof(float));

	heightMap.InitArray2D(terrainSize, terrainSize, p);
}

void BaseTerrain::Render(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
	glm::mat4 VP = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, camera.RenderDistance) * camera.GetViewMatrix();
	terrainShader.use();
	terrainShader.setMat4("gVP", VP);

	triangleList.Render();
}
