#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

//uniform sampler2D tex;
uniform vec3 camPos;

void main()
{
	// Ambient Light
	//vec3 ambient = ambientStr * ambientColor;

	// Light Direction
	//vec3 norm = normalize(fragNormal);
	//vec3 lightDir = normalize(fragPos - lightPos);

	// Diffuse Coloring
	//float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	//vec3 diffuse = diffuseStr * lightColor;

	// Specular Highlight
	//vec3 negViewDir = normalize(camPos - fragPos);
	//vec3 halfwayVec = normalize(-lightDir + negViewDir);
	//float spec = pow(max(dot(norm, halfwayVec), 0.0f), shininess);
	//vec3 specular = lightSpecStr * spec * lightColor;

	//color = vec4(ambient + diffuse + specular, 1.0f) * texture(tex, fragTexCoord);

	// Givei it a white color for now
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}