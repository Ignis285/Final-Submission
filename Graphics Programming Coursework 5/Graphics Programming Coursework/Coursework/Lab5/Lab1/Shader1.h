#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <intrin.h>

#include <GL\glew.h>
#include "transform.h"
#include "Camera.h"

class Shader1
{
public:
	Shader1();

	void BindPlz(); //Set gpu to use our shaders
	void UpdateStuff(const Transform& transform, const Camera& camera);
	void initialize(const std::string& vertFile, const std::string& fragFile);
	GLuint getProgramPlz() { return program1; }

	std::string Shader1::LoadShaderPlz(const std::string& fileName);
	void Shader1::CheckShaderErrorPlz(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shader1::CreateShaderPlz(const std::string& text, unsigned int type);


	// uniform functions
	// ------------------------------------------------------------------------
	void setBoolPlz(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(program1, name.c_str()), (int)value);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setIntPlz(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(program1, name.c_str()), value);
		
		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setFloatPlz(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(program1, name.c_str()), value);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec2Plz(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(program1, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec2Plz(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(program1, name.c_str()), x, y);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec3Plz(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(program1, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec3Plz(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program1, name.c_str()), x, y, z);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setVec4Plz(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(program1, name.c_str()), 1, &value[0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	void setVec4Plz(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(program1, name.c_str()), x, y, z, w);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat2Plz(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(program1, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat3Plz(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(program1, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}
	// ------------------------------------------------------------------------
	void setMat4Plz(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program1, name.c_str()), 1, GL_FALSE, &mat[0][0]);

		if ((glGetUniformLocation(program1, name.c_str()) == -1))
		{
			std::cerr << "Unable to load shader: " << name.c_str() << std::endl;
			__debugbreak();
		}
	}


	~Shader1();


protected:
private:
	static const unsigned int NUM_SHADERS = 3; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint program1; // Track the shader program
	GLuint shaders1[NUM_SHADERS]; //array of shaders
	GLuint uniforms1[NUM_UNIFORMS]; //no of uniform variables
};
