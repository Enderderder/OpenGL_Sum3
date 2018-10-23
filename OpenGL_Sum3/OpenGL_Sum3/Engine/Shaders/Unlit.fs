#version 450 core

//in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = vec4(0.0, 0.8, 1.0, 1.0);  //texture(tex, fragTexCoord);
}