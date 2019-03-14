#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoord;

out vec3 fragPos;
out vec3 fragNormal;
//out vec2 fragTexCoord;

uniform mat4 model;
uniform mat3 normalMat;
uniform mat4 vp;

void main()
{
	gl_Position = vp * model * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0));
	//fragTexCoord = texCoord;
	fragNormal = normalMat * normal;
}