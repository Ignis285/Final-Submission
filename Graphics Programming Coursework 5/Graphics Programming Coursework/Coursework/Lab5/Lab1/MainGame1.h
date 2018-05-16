#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader1.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame1
{
public:
	MainGame1();
	~MainGame1();

	void run();

private:

	void initializeSystems();
	void processInputs();
	void gameLoop();
	void drawGame();
	void blobEffect();
	void setRimShader();
	void setToonRimShader();
	void setBlinnPhong();
	void setFogShader(float Z_pos);
	void setPhong();
	void Mesh1Draw();
	void Mesh2Draw();
	void Mesh3Draw();
	void Mesh4Draw();
	void Mesh5Draw();

	Display _gameDisplay;
	GameState _gameState;
	Mesh Cat;
	Mesh Tree;
	Mesh Rock;
	Mesh Bird;
	Mesh Monkey;
	Shader1 shaderBlur;
	Shader1 shaderRim;
	Shader1 toonRimShader;
	Shader1 shaderFog;
	Shader1 blinnPhongShader;
	Shader1 shaderPhong;

	glm::mat4 modelView1;

	Camera theCamera;

	float transformCounter;
	float transformCounter1;
	glm::vec3 rgb;
	bool Blinn;
};

