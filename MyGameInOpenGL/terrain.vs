#version 330

layout (location = 0) in vec3 Position;
//layout (location = 1) in vec3 InColor;

// combination of (V)iew and (P)rojection matrices (our terrain is constructed directly w/ world coordinates, so don't need world matrix)
uniform mat4 gVP;
uniform float gMinHeight; // minimum height of terrain 
uniform float gMaxHeight; // maximum height of terrain

out vec4 Color;

void main()
{
    gl_Position = gVP * vec4(Position, 1.0);

    float DeltaHeight = gMaxHeight - gMinHeight;

    // normalize the y value (between 0.0 and 1.0)
    float HeightRatio = (Position.y - gMinHeight) / DeltaHeight; 

    // don't want to go all the way to 0.0, since it'll be too dark, so we scale it to be between 0.2 and 1.0
    float c = HeightRatio * 0.8 + 0.2;

    Color = vec4(c, c, c, 1.0); // varying color of vertex based on height (y value)
}