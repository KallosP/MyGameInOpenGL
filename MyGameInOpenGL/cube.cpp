#include "cube.h"

// constructor
Cube::Cube(const char* textureMaterialSrc, const char* textureMaskSrc) {
	cubeMesh = new CubeMesh();
	// why do we need setInt? -> assigns our data we decalred here in this program (e.g. material and mask) to the corresponding variables we declared on the GPU in the fragment shader
	// why do we need to call use() (i.e. glActiveTexture/glBindTexture) for every texture in the render loop? -> because we have to continuously redraw those textures every frame. And the only way to draw a texture is if they are bound 
	// what does it mean to bind a texture? -> means we're telling opengl that's the one we're currently working with
	material = new Material(textureMaterialSrc);
	mask = new Material(textureMaskSrc);
}

void Cube::draw() {
	cubeMesh->draw();
}
