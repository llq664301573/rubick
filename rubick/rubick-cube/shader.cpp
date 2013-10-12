#include "shader.hpp"

GLuint loadShaders(int n, ...)
{
	GLint result = GL_FALSE;
	int infoLogLength;
	GLuint ProgramID = glCreateProgram();
	
	va_list ap;
	va_start(ap, n);
	for (int i=0;i<n;i++)
	{
		string path = va_arg(ap, const char*);
		// Read the Shader code from the file
		string shaderCode;
		ifstream shaderStream(path.c_str(), ios::in);
		
		if(shaderStream.is_open())
		{
			string Line;
			while(getline(shaderStream, Line))
				shaderCode += Line + "\n";
			shaderStream.close();
		}
		else
		{
			printf("Impossible to open %s. Are you in the right directory ?\n", path.c_str());
			return 0;
		}


		GLenum type = path.find(".vertexshader") != string.npos ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		GLuint shaderID = glCreateShader(type);

		// Compile Shader
		printf("Compiling shader : %s\n", path.c_str());
		char const * sourcePointer = shaderCode.c_str();
		glShaderSource(shaderID, 1, &sourcePointer, NULL);
		glCompileShader(shaderID);

		// Check Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if ( infoLogLength > 0 )
		{
			vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
			printf("%s\n", &shaderErrorMessage[0]);
		}

		glAttachShader(ProgramID, shaderID);

		glDeleteShader(shaderID);
	}

	va_end(ap);

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 )
	{
		vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	return ProgramID;
}