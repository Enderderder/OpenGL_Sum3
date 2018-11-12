#version 450 core

in GS_FS_VERTEX
{
	vec2 texCoord;
} fs_in;

uniform sampler2D texture;
out vec4 color;

void main()
{
	color = texture(texture, vec2(fs_in.texCoord.x, fs_in.texCoord.y)) *
			vec4(123.0f/255.0f, 173.0f/255.0f, 203.0f/255.0f, 1.0f);
}