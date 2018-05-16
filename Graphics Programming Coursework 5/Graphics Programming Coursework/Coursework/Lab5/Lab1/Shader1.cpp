#include "Shader1.h"



Shader1::Shader1()
{
}

void Shader1::initialize(const std::string& vertFile, const std::string& fragFile)
{
	program1 = glCreateProgram(); // create shader program (openGL saves as ref number)
	shaders1[0] = CreateShaderPlz(LoadShaderPlz(vertFile), GL_VERTEX_SHADER); // create vertex shader
	shaders1[1] = CreateShaderPlz(LoadShaderPlz(fragFile), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(program1, shaders1[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(program1, 0, "VPosition"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(program1, 1, "VTexCoord"); 
	glBindAttribLocation(program1, 2, "VNormal");

	glLinkProgram(program1); //create executables that will run on the GPU shaders
	CheckShaderErrorPlz(program1, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(program1); //check the entire program is valid
	CheckShaderErrorPlz(program1, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms1[TRANSFORM_U] = glGetUniformLocation(program1, "transform"); // associate with the location of uniform variable within a program
}


Shader1::~Shader1()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program1, shaders1[i]); //detach shader from program
		glDeleteShader(shaders1[i]); //delete the sahders
	}
	glDeleteProgram(program1); // delete the program
}

void Shader1::BindPlz()
{
	glUseProgram(program1); //installs the program object specified by program as part of rendering state
}

void Shader1::UpdateStuff(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms1[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint Shader1::CreateShaderPlz(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderErrorPlz(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string Shader1::LoadShaderPlz(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader1::CheckShaderErrorPlz(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

