#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cTapperGame.h"
#include "cSoundMgr.h"
#include "cLaser.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;


    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

    //The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	LPCSTR gameSounds[5] = { "Audio/Laser_Shoot3.wav", "Audio/jazz-chords-piano.wav", "Audio/Explosion.wav", "Audio/victory.wav", "Audio/Sad_Trombone.wav"};

	theSoundMgr->add("GlassThrow", gameSounds[0]);
	theSoundMgr->add("Background", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);
	theSoundMgr->add("Win", gameSounds[3]);
	theSoundMgr->add("Lose", gameSounds[4]);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }


	// Create Texture map
	cTexture tardisTexture;
	//tardisTexture.createTexture("Models/Tardis.png");
	tardisTexture.createTexture("Models/Spearman.png");
	cTexture starTexture;
	starTexture.createTexture("Images/star.png");
	//starTexture.createTexture("Images/canvas.png");

	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//More lights
	cLight sunLight2(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(5, 10, 10, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//another light
	cLight sunLight3(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(9, 10, 10, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//a 4th light
	cLight sunLight4(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(13, 10, 10, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//a 5th light
	cLight sunLight5(GL_LIGHT4, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(17, 10, 10, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);

	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[3] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/doctor_who.ttf" };

	//add fonts
	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	theFontMgr->addFont("DrWho", gameFonts[2], 48);

	// Create a camera
	cCamera theCamera;
	theCamera.attachInputMgr(theInputMgr);
	theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 10.0f));
	theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera.update();

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Model
	cModelLoader playerMdl;
	playerMdl.loadModel("Models/Ribak_rigged.obj");

	//Enemy model
	cModelLoader enemyMdl;
	enemyMdl.loadModel("Models/Jack_Skellington.obj");

	cModelLoader enemyMdl2;
	enemyMdl2.loadModel("Models/lofy.obj");

	cModelLoader enemyMdl3;
	enemyMdl3.loadModel("Models/Spearman.obj");

	//Player
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(9.5, 5, 10), 270.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0.0), 5.0f, true);
	thePlayer.setMdlDimensions(playerMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);

	//Enemy
	cEnemy testEnemy;
	testEnemy.initialise(glm::vec3(-9.5, 0, 10), 90.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	testEnemy.setMdlDimensions(enemyMdl.getModelDimensions());

	//Another enemy object - glass
	cModelLoader glassMdl;
	glassMdl.loadModel("Models/Mug.obj");

	cLaser testGlass;
	testGlass.initialise(glm::vec3(0, 5, 10), 0.0f, glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, 0, 0), 5.0f, true);
	testEnemy.setMdlDimensions(glassMdl.getModelDimensions());

	//set up first person camera
	cCamera firstPersonCamera;
	firstPersonCamera.attachInputMgr(theInputMgr);
	glm::vec3 firstPersonCameraPos = glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y + 1, thePlayer.getPosition().z);
	firstPersonCamera.setTheCameraPos(firstPersonCameraPos);
	firstPersonCamera.setTheCameraLookAt(glm::vec3(0.0f, 5.0f, 0.0f));
	firstPersonCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	firstPersonCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	firstPersonCamera.setTheProjectionMatrix(60.0f, firstPersonCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	firstPersonCamera.update();

	float tCount = 0.0f;
	string outputMsg;

	//player position
	playerPos = -1;

	//plays background music
	theSoundMgr->getSnd("Background")->playAudio(AL_LOOPING);

	//random number of enemies that will spawn in each lane.
	//will be different each execution of game.
	srand(time(NULL));
	int noOfEnemiesPerLane = rand() % 40 + 15;

	//lods in the enemies that will be in the game
	//based on the results of the random generator
	for (int i = 0; i < noOfEnemiesPerLane; i++)
	{
		theEnemy.push_back(new cEnemy());
		theEnemy[i]->initialise(glm::vec3(-9.5, 0, 10), 90.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
		int randEnemyModel = rand() % 3 + 1;
		//sets model dimensions based on which model the enemy will be assigned
		switch (randEnemyModel)
		{
		case 1:
			theEnemy[i]->setMdlDimensions(enemyMdl.getModelDimensions());
			break;
		case 2:
			theEnemy[i]->setMdlDimensions(enemyMdl2.getModelDimensions());
			break;
		case 3:
			theEnemy[i]->setMdlDimensions(enemyMdl3.getModelDimensions());
			break;
		}
		theEnemy[i]->setModelNumber(randEnemyModel);
		theEnemy[i]->setIsActive(true);
		theEnemy[i]->randomisePosition();
		theEnemy[i]->randomiseSpeed();
		theEnemy[i]->attachSoundMgr(theSoundMgr);
	}

   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		//toggles whether or not sound is muted
		if (theInputMgr->isKeyDown(VK_DELETE))
		{
			isSoundMuted = !isSoundMuted;
		}
		//toggles the camera
		else if (theInputMgr->isKeyDown(VK_INSERT))
		{
			firstPersonCameraActive = !firstPersonCameraActive;

		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth,windowHeight);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//switches cameras based on boolean value
		if (firstPersonCameraActive == false)
		{
			glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());
		}
		else
		{
			glLoadMatrixf((GLfloat*)&firstPersonCamera.getTheViewMatrix());
		}
	
		theStarField.render(0.0f);
		sunMaterial.useMaterial();
		//turn on lights
		sunLight.lightOn();
		sunLight2.lightOn();
		sunLight3.lightOn();
		sunLight4.lightOn();
		sunLight5.lightOn();

		//MAIN GAME LOOP
		if (!mainMenu && !gameOverLose && !gameOverWin)
		{

			//mutes or plays looping game sound
			if (isSoundMuted)
			{
				theSoundMgr->getSnd("Background")->stopAudio();
			}
			else
			{
				if (theSoundMgr->getSnd("Background")->getSoundPlaying() == true)
				{
					//do nothing
				}
				else if (theSoundMgr->getSnd("Background")->getSoundPlaying() == false)
				{
					theSoundMgr->getSnd("Background")->playAudio(AL_LOOPING);
				}

			}

			//Polls players position, used to switch between different positions in game
			//Controls sun at this point in time, no models loaded in yet
			switch (playerPos)
			{
			case 0:
				thePlayer.setPosition(glm::vec3(playerPosX, playerPosY[0], 10));
				firstPersonCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y + 1, thePlayer.getPosition().z));
				firstPersonCamera.setTheCameraLookAt(glm::vec3(0.0f, 5.0f, 0.0f));
				firstPersonCamera.update();
				break;
			case 1:
				thePlayer.setPosition(glm::vec3(playerPosX, playerPosY[1], 10));
				firstPersonCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y + 1, thePlayer.getPosition().z));
				firstPersonCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
				firstPersonCamera.update();
				break;
			case 2:
				thePlayer.setPosition(glm::vec3(playerPosX, playerPosY[2], 10));
				firstPersonCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y + 1, thePlayer.getPosition().z));
				firstPersonCamera.setTheCameraLookAt(glm::vec3(0.0f, -5.0f, 0.0f));
				firstPersonCamera.update();
				break;
			}

			//render + update player
			if (thePlayer.isActive())
			{
				playerMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
				thePlayer.update(elapsedTime);
			}

			//render and update enemies
			for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->isActive())
				{
					//renders enemy model based on the model number set within the enemy object
					switch ((*enemyIterator)->getModelNumber())
					{
					case 1:
						enemyMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
						break;
					case 2:
						enemyMdl2.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
						break;
					case 3:
						enemyMdl3.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
						break;
					}
					enemyMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
					(*enemyIterator)->update(elapsedTime);
				}
			}

			//render and update glasses
			for (vector<cLaser*>::iterator glassIterator = theGlasses.begin(); glassIterator != theGlasses.end(); ++glassIterator)
			{
				if ((*glassIterator)->isActive())
				{
					glassMdl.renderMdl((*glassIterator)->getPosition(), (*glassIterator)->getRotation(), (*glassIterator)->getScale());
					(*glassIterator)->update(elapsedTime);
				}
			}

			//if no enemies are left in the game
			if (theEnemy.empty())
			{
				gameOverWin = true;
			}

			//outputMsg = to_string(floorf(tCount)); // convert float to string
			outputMsg = to_string(gameScore);

			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			//displays name of the game and the score
			theFontMgr->getFont("DrWho")->printText("Tapper!", FTPoint(10, 55, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText(outputMsg.c_str(), FTPoint(850, 55, 0.0f), colour3f(255.0f, 255.0f, 0.0f)); // uses c_str to convert string to LPCSTR
			glPopMatrix();
		}
		//shows main menu
		if (mainMenu && !gameOverLose && !gameOverWin)
		{
			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			theFontMgr->getFont("DrWho")->printText("Welcome To Tapper!", FTPoint(40, 300, 0.0f), colour3f(0.0f, 0.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("Press The 1 key On The", FTPoint(40, 350, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("Numpad To Continue!", FTPoint(40, 400, 0.0f), colour3f(255.0f, 0.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("Controls:", FTPoint(590, 300, 0.0f), colour3f(0.0f, 255.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("Up/Down: Moves Player", FTPoint(590, 350, 0.0f), colour3f(255.0f, 255.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("Space: Fire Glass", FTPoint(590, 400, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("Delete: Mutes Sound", FTPoint(590, 450, 0.0f), colour3f(0.0f, 191.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("Insert: Change Camera", FTPoint(590, 500, 0.0f), colour3f(255.0f, 140.0f, 0.0f));

			glPopMatrix();

			//pressing 1 on numpad toggles menu 
			if (theInputMgr->isKeyDown(VK_NUMPAD1))
			{
				mainMenu = !mainMenu;
			}

		}
		//shows game over screen
		if (gameOverLose && !mainMenu && !gameOverWin)
		{
			
			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			if (!isSoundMuted)
			{
				theSoundMgr->getSnd("Lose")->playAudio(AL_TRUE);
			}
			theFontMgr->getFont("DrWho")->printText("GAME OVER!", FTPoint(400, 200, 0.0f), colour3f(0.0f, 0.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("You Failed.", FTPoint(400, 250, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("To Play Again Simply", FTPoint(400, 350, 0.0f), colour3f(255.0f, 0.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("Close And Restart The", FTPoint(400, 400, 0.0f), colour3f(0.0f, 255.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText("Game!", FTPoint(400, 450, 0.0f), colour3f(255.0f, 255.0f, 0.0f));

			glPopMatrix();

		}
		//shows winning screen
		if (gameOverWin && !mainMenu && !gameOverLose)
		{
			theSoundMgr->getSnd("Background")->stopAudio();
			if (!isSoundMuted)
			{
				theSoundMgr->getSnd("Win")->playAudio(AL_TRUE);
			}
			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			theFontMgr->getFont("DrWho")->printText("Congrats!", FTPoint(400, 300, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("You Won The Game!", FTPoint(400, 350, 0.0f), colour3f(255.0f, 0.0f, 0.0f));
			theFontMgr->getFont("DrWho")->printText("Your score was ", FTPoint(400, 400, 0.0f), colour3f(0.0f, 0.0f, 255.0f));
			theFontMgr->getFont("DrWho")->printText((to_string(gameScore)).c_str(), FTPoint(400, 450, 0.0f), colour3f(255.0f, 255.0f, 0.0f));
			glPopMatrix();
		}

		pgmWNDMgr->swapBuffers();

		tCount += elapsedTime;

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
