#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 vp;
uniform vec3 quadVec_1, quadVec_2;

out GS_FS_VERTEX
{
	vec2 texCoord;
}gs_out;

void buildQuad(float _size, mat4 _vp)
{
	vec3 p1 = gl_in[0].gl_Position.xyz +(-quadVec_1 - quadVec_2) * _size;
	gl_Position = _vp * vec4(p1, 1.0f);
	gs_out.texCoord = vec2(0.0f, 0.0f); 
	EmitVertex();
	
	vec3 p2 = gl_in[0].gl_Position.xyz + (-quadVec_1 + quadVec_2) * _size;
	gl_Position = _vp * vec4(p2, 1.0f);
	gs_out.texCoord = vec2(0.0f, 1.0f);
	EmitVertex();
	
	vec3 p3 = gl_in[0].gl_Position.xyz + (quadVec_1 - quadVec_2) * _size;
	gl_Position = _vp * vec4(p3, 1.0f);
	gs_out.texCoord = vec2(1.0f, 0.0f); 
	EmitVertex();
	
	vec3 p4 = gl_in[0].gl_Position.xyz + (quadVec_1 + quadVec_2) * _size;
	gl_Position = _vp * vec4(p4, 1.0f);
	gs_out.texCoord = vec2(1.0f, 1.0f); 
	EmitVertex();
	
	EndPrimitive();
}

void main()
{
	buildQuad(0.5, vp);
}