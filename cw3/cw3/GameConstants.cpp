/*
==================================================================================
GameConstants.cpp
==================================================================================
*/

#include "GameConstants.h"

int drawMode = 0;
float rotationAngle = 0;
float translationZ = 0;
bool fire = false;
float playerPosX = 9.5f;
float playerPosY[] = { 5.0f, 0.0f, -5.0f };
float customerStartPosX = -9.5f;
float customerStartPosY[] = { 5.0f, 0.0f, -5.0f };
int playerPos;
bool isSoundMuted = false;
bool firstPersonCameraActive = false;

bool gameOverLose = false;
bool mainMenu = true;
bool gameOverWin = false;
