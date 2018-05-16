#include "MainGame1.h"
#include "Camera.h"
#include <iostream>
#include <string>


//Transforms for each mesh
Transform Mesh1Transform;
Transform Mesh2Transform;
Transform Mesh3Transform;
Transform Mesh4Transform;
Transform Mesh5Transform;

MainGame1::MainGame1()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* Cat();	//Cat Mesh
	Mesh* Tree();	//Tree Mesh
	Mesh* Rock();	//Rock Mesh
	Mesh* Bird();	//Bird Mesh
	Mesh* Monkey();	//Monkey Mesh
	Shader1* shaderBlur();	//Blur Shader
	Shader1* shaderRim();	//Rim Shader
	Shader1* shaderToonRim();	//ToonRim Shader
	Shader1* shaderFog();	//Fog Shader
	Shader1* blinnPhongShader();	//Blinn Phong Shader
	Shader1* shaderPhong();	//Phong Shader
}

MainGame1::~MainGame1() 
{
}

void MainGame1::run() // Runs Method on start up
{
	initializeSystems();
	gameLoop();
}

void MainGame1::initializeSystems()	//initializes the game
{
	_gameDisplay.initDisplay(); 
	//Initializes all used shaders
	shaderBlur.initialize("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	shaderRim.initialize("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	toonRimShader.initialize("..\\res\\shaderToonRim.vert", "..\\res\\shaderToonRim.frag");
	shaderFog.initialize("..\\res\\ShaderFog.vert", "..\\res\\ShaderFog.frag");
	blinnPhongShader.initialize("..\\res\\BlinnPhong1.vert", "..\\res\\BlinnPhong1.frag");
	shaderPhong.initialize("..\\res\\Phong.vert", "..\\res\\Phong.frag");

	//Initializes all used models
	Cat.loadModel("..\\res\\cat.obj");
	Tree.loadModel("..\\res\\Tree.obj");
	Rock.loadModel("..\\res\\Rocky.obj");
	Bird.loadModel("..\\res\\twitter.obj");
	Monkey.loadModel("..\\res\\monkey3.obj");
	
	//Initializes  camera and sets its position, size and direction
	theCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	transformCounter = 1.0f;	//sets Counter for later use
	transformCounter1 = 0.0f;	//sets second counter for later use
	rgb = glm::vec3(0.6, 0.2, 0.2);	//Sets color to be used for fog shader
	Blinn = false;	//Sets wheither or not Blinn fog is in use to false
}

void MainGame1::gameLoop()	//Runs while game us runing
{
	while (_gameState != GameState::EXIT)
	{
		processInputs();	//Runs all used inputes, constantly checking if they are being used
		drawGame();	//Constantly running the drawgame method
	}
}

void MainGame1::processInputs()	//Used for all user inputs to game
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
		case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_ESCAPE:	//Closes game when "escape" is pressed
					_gameState = GameState::EXIT;
					break;
				case SDLK_3:	//Changes fog colour to blue when "3" is pressed
					rgb = glm::vec3(0.2, 0.2, 0.6);
					break;
				case SDLK_1:	//Changes fog colour to red when "1" is pressed
					rgb = glm::vec3(0.6, 0.2, 0.2);
					break;
				case SDLK_2:	//Changes fog colour to blue when "2" is pressed
					rgb = glm::vec3(0.2, 0.6, 0.2);
					break;
				case SDLK_9:	//Changes monkey model to phong shader
					Blinn = true;
					break;
				case SDLK_0:	//Changes monkey model to blinn-phong shader
					Blinn = false;
					break;
				}
		}
	}
	
}

void MainGame1::setRimShader()	//Sets values for Rim shader
{
	shaderRim.setMat4Plz("u_vm", theCamera.GetView());
	shaderRim.setMat4Plz("u_pm", theCamera.GetProjection());
}

void MainGame1::setToonRimShader()	//Sets values for toon Rim shader
{
	toonRimShader.setVec3Plz("lightDir", glm::vec3(0.5, 0.5, 0.5));	
	toonRimShader.setMat4Plz("u_vm", theCamera.GetView());
	toonRimShader.setMat4Plz("u_pm", theCamera.GetProjection());
	toonRimShader.setMat4Plz("v_pos", Mesh3Transform.GetModel());
}

void MainGame1::setFogShader(float Z_pos)	//Sets values for fog shader
{
	shaderFog.setVec3Plz("lightDir", glm::vec3(0.5, 0.5, 0.5));

	shaderFog.setMat4Plz("u_vm", theCamera.GetView());
	shaderFog.setMat4Plz("u_pm", theCamera.GetProjection());

	shaderFog.setVec3Plz("fogColor", rgb);
	shaderFog.setFloatPlz("minDist", -5.0f);
	shaderFog.setFloatPlz("maxDist", 5.0f);

	shaderFog.setFloatPlz("z_pos", Z_pos);
}

void MainGame1::blobEffect()	//Sets values for blur shader
{
	GLuint blockIndex = glGetUniformBlockIndex(shaderBlur.getProgramPlz(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(shaderBlur.getProgramPlz(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(shaderBlur.getProgramPlz(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(shaderBlur.getProgramPlz(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame1::setBlinnPhong()	//Sets values for Blinn Phong shader
{
	blinnPhongShader.setVec3Plz("viewPos", theCamera.getPos());
	blinnPhongShader.setVec3Plz("lightPos", Tree.getSpherePos());
	blinnPhongShader.setBoolPlz("blinn", true);
}

void MainGame1::setPhong()	//Sets values for phong shader
{
	shaderPhong.setVec3Plz("viewPos", theCamera.getPos());
	shaderPhong.setVec3Plz("lightPos", Tree.getSpherePos());
	shaderPhong.setVec3Plz("lightColor", glm::vec3(1.0, 1.0, 1.0));
	shaderPhong.setVec3Plz("objectColor", glm::vec3(0.0, 0.0, 1.0));
}

void MainGame1::drawGame()	//Draw game method is constantly updating as game is running
{	
	transformCounter1 = transformCounter1 + 0.1f;
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//Calls methods to draw each model and bind shaders
	Mesh1Draw();
	Mesh2Draw();
	Mesh3Draw();
	Mesh4Draw();
	Mesh5Draw();

	transformCounter = transformCounter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 

void MainGame1::Mesh1Draw()
{
	//Sets method positions 
	Mesh1Transform.SetPos(glm::vec3(4.0, -4.0, -(sinf(transformCounter))));
	Mesh1Transform.SetRot(glm::vec3(0.0, transformCounter * 2, 0.0));
	Mesh1Transform.SetScale(glm::vec3(2.0, 2.0, 2.0));

	//Binds fog shader and sets shader up
	shaderFog.BindPlz();
	setFogShader(Cat.getSpherePos().z + 4.0);
	shaderFog.UpdateStuff(Mesh1Transform, theCamera);

	//Draws Model and updates sphere postion
	Cat.draw();
	Cat.updateSphereData(*Mesh1Transform.GetPos(), 0.62f);
}

void MainGame1::Mesh2Draw()
{
	//Sets method positions 
	Mesh2Transform.SetPos(glm::vec3(0, -4.0, 0));
	Mesh2Transform.SetRot(glm::vec3(0.0, transformCounter * 0.5, 0.0));
	Mesh2Transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	//Binds rim shader
	shaderRim.BindPlz();
	setRimShader();
	shaderRim.UpdateStuff(Mesh2Transform, theCamera);

	//Draws model and updates sphere positon
	Tree.draw();
	Tree.updateSphereData(*Mesh2Transform.GetPos(), 0.62f);
}

void MainGame1::Mesh3Draw()
{
	//Sets method positions 
	Mesh3Transform.SetPos(glm::vec3(-4.0, -3.5, 0.0));
	Mesh3Transform.SetRot(glm::vec3(transformCounter * 3, transformCounter * 3, 0));
	Mesh3Transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	//Binds rim shader
	toonRimShader.BindPlz();
	setToonRimShader();
	toonRimShader.UpdateStuff(Mesh3Transform, theCamera);

	//Draws model and updates sphere positon
	Rock.draw();
	Rock.updateSphereData(*Mesh3Transform.GetPos(), 0.62f);
}

void MainGame1::Mesh4Draw()
{
	//Sets method positions 
	Mesh4Transform.SetPos(glm::vec3(-4.0, 2.0, 0.0));
	Mesh4Transform.SetRot(glm::vec3(0, transformCounter * 3, 0));
	Mesh4Transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	//Binds blur shader
	shaderBlur.BindPlz();
	blobEffect();
	shaderBlur.UpdateStuff(Mesh4Transform, theCamera);

	//Draws model and updates sphere positon
	Bird.draw();
	Bird.updateSphereData(*Mesh4Transform.GetPos(), 0.62f);
}

void MainGame1::Mesh5Draw()
{  
	//Sets method positions 
	Mesh5Transform.SetPos(glm::vec3(4.0, 2.0, -(sinf(transformCounter * 2))));
	Mesh5Transform.SetRot(glm::vec3(0, transformCounter * 6, 0));
	Mesh5Transform.SetScale(glm::vec3(1, 1, 1));
	//Binds either phong or blinn-phong shader
	if (Blinn == false)
	{
		blinnPhongShader.BindPlz();
		setBlinnPhong();
		blinnPhongShader.UpdateStuff(Mesh5Transform, theCamera);
	}
	else if (Blinn)
	{
		shaderPhong.BindPlz();
		setPhong();
		shaderPhong.UpdateStuff(Mesh5Transform, theCamera);
	}

	//Draws model and updates sphere positon
	Monkey.draw();
	Monkey.updateSphereData(*Mesh5Transform.GetPos(), 0.62f);
}