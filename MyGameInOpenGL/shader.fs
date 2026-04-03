#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D material; // 0
uniform sampler2D mask; // 1

void main()
{
    FragColor = mix(texture(material, TexCoord), texture(mask, TexCoord), 0.2);
}