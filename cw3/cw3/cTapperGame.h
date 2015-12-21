#ifndef _CTAPPERGAME_H
#define _CTAPPERGAME_H

#include "cEnemy.h"
#include "cSphere.h"
#include "cLaser.h"

#include <vector>

extern std::vector<cEnemy*> theEnemy;
extern std::vector<cLaser*> theGlasses;
extern int gameScore;

#endif