#include <stdio.h>
#include <vector>

//#include "ogldev_math_3d.h"
#include "triangle_list.h"
#include "terrain.h"

// NOTE: The purpose of this code is to abstract a lot of the OpenGL code away
//		 (e.g. all the vertex array/buffer setup) for the terrain related code

TriangleList::TriangleList()
{
}


void TriangleList::CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain)
{
	m_width = Width;
	m_depth = Depth;

	CreateGLState();

	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void TriangleList::CreateGLState()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: left off at 16:06 (step 5) https://youtu.be/xoqESu9iOUE?list=PLA0dXqQjCx0S9qG5dWLsheiCJV-_eLUM0
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int POS_LOC = 0;

	size_t NumFloats = 0;
	
	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
}

// Populates the vertex buffer with the positions of the vertices in the grid (where grid = the terrain)
void TriangleList::PopulateBuffers(const BaseTerrain* pTerrain)
{
	std::vector<Vertex> Vertices;
	Vertices.resize(m_width * m_depth); // width * depth of grid yields the total number of vertices in the grid

	InitVertices(pTerrain, Vertices);

	//std::vector<unsigned int> Indices;
	//int NumQuads = (m_width - 1) * (m_depth - 1);
	//Indices.resize(NumQuads * 6);
	//InitIndices(Indices);

	// Called once vertices is populated with vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}


void TriangleList::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float y = pTerrain->GetHeight(x, z);

	//float WorldScale = pTerrain->GetWorldScale();

	//Pos = Vector3f(x * WorldScale, y, z * WorldScale);
	Pos = glm::vec3(x, y, z);
}


// Performs the actual population of the Vertices vector (c++ vector aka dynamic list) with vertex positions
void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices)
{
	int Index = 0;

	// initializes a vertex for every point in the grid (i.e. every point in the terrain)
	for (int z = 0; z < m_depth; z++) {
		for (int x = 0; x < m_width; x++) {
			assert(Index < Vertices.size());
			Vertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}

    assert(Index == Vertices.size());
}


//void TriangleList::InitIndices(std::vector<unsigned int>& Indices)
//{
//	int Index = 0;
//
//	for (int z = 0; z < m_depth - 1; z++) {
//		for (int x = 0; x < m_width - 1; x++) {
//			unsigned int IndexBottomLeft = z * m_width + x;
//			unsigned int IndexTopLeft = (z + 1) * m_width + x;
//			unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
//			unsigned int IndexBottomRight = z * m_width + x + 1;
//
//			// Add top left triangle
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomLeft;
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexTopLeft;
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexTopRight;
//
//			// Add bottom right triangle
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomLeft;
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexTopRight;
//            assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomRight;
//		}
//	}
//
//    assert(Index == Indices.size());
//}


void TriangleList::Render()
{
	glBindVertexArray(VAO);

	//glDrawElements(GL_TRIANGLES, (m_depth - 1) * (m_width - 1) * 6, GL_UNSIGNED_INT, NULL);
	glDrawArrays(GL_POINTS, 0, m_width * m_depth);

	glBindVertexArray(0);
}
