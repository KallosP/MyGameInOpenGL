#version 330

layout (location = 0) in vec3 Position;

// combination of (V)iew and (P)rojection matrices (our terrain is constructed directly w/ world coordinates, so don't need world matrix)
uniform mat4 gVP;

out vec4 Color;

void main()
{
    gl_Position = gVP * vec4(Position, 1.0);

    Color = vec4(Position.y / 200.0); // varying color of vertex based on height (y value)
}