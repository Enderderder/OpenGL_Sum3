#pragma once

// Global Include
#include "Utility.h"

class ShaderLoader
{
	private:

		static std::string ReadShader(const char *filename);
		static GLuint CreateShader(GLenum shaderType,
			std::string source,
			const char* shaderName);

	public:

		ShaderLoader(void);
		~ShaderLoader(void);
		static GLuint CreateProgram(const char* VertexShaderFilename,
			const char* FragmentShaderFilename);

};
