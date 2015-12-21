#include "cPlayer.h"
#include "cSphere.h"
#include "GameConstants.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cPlayer::update(float elapsedTime)
{

	if (m_InputMgr != NULL)
	{
		if (m_InputMgr->isKeyDown(VK_UP))
		{
			playerPos = playerPos - 1;
			if (playerPos < 0)
			{
				playerPos = 0;
			}
			if (playerPos > 2)
			{
				playerPos = 2;
			}
		}
		else if (m_InputMgr->isKeyDown(VK_DOWN))
		{
			playerPos++;
			if (playerPos > 2)
			{
				playerPos = 2;
			}
			if (playerPos < 0)
			{
				playerPos = 0;
			}
		}
		else if (m_InputMgr->isKeyDown(VK_SPACE))
		{
			glm::vec3 sphereGlassDirection;
			sphereGlassDirection.x = -(float)glm::sin(glm::radians(this->getRotation()));
			sphereGlassDirection.y = 0;
			//sphereGlassDirection.z = (float)glm::cos(glm::radians(this->getRotation()));
			sphereGlassDirection.z = 0;
			sphereGlassDirection *= -1;

			cTexture sphereTexture;
			sphereTexture.createTexture("Images/moon.png");

			theGlasses.push_back(new cLaser());
			int numEnemy = theGlasses.size() - 1;
			theGlasses[numEnemy]->setDirection(sphereGlassDirection);
			theGlasses[numEnemy]->setRotation(0.0f);
			theGlasses[numEnemy]->setScale(glm::vec3(0.2, 0.2, 0.2));
			theGlasses[numEnemy]->setSpeed(5.0f);
			theGlasses[numEnemy]->setPosition(this->getPosition());
			theGlasses[numEnemy]->setIsActive(true);
			theGlasses[numEnemy]->update(elapsedTime);

			if (!isSoundMuted)
			{
				m_SoundMgr->getSnd("GlassThrow")->playAudio(AL_TRUE);
			}
			
		}
	}

	/*
	==============================================================
	| Check for collisions between glasses and enemies
	==============================================================
	*/
	for (vector<cLaser*>::iterator glassIterartor = theGlasses.begin(); glassIterartor != theGlasses.end(); ++glassIterartor)
	{
		(*glassIterartor)->update(elapsedTime);
		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->SphereSphereCollision((*glassIterartor)->getPosition(), (*glassIterartor)->getMdlRadius()))
			{
				// if a collision set the bullet and spaceship to false
				(*enemyIterator)->setIsActive(false);
				(*glassIterartor)->setIsActive(false);
				if (!isSoundMuted)
				{	// play the explosion sound.
					m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
					gameScore++;
				}
			}
		}
	}

	//Checks for collisions between enemy and player
	for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
	{
		if ((*enemyIterator)->SphereSphereCollision(this->getPosition(), this->getMdlRadius()))
		{
			// if a collision set the bullet and spaceship to false
			(*enemyIterator)->setIsActive(false);
			this->setIsActive(false);
			gameOverLose = true;
		}
	}

	//removes any glasses in the list that aren't active
	vector<cLaser*>::iterator glassIterartor = theGlasses.begin();
	while (glassIterartor != theGlasses.end())
	{
		if ((*glassIterartor)->isActive() == false)
		{
			glassIterartor = theGlasses.erase(glassIterartor);
		}
		else
		{
			++glassIterartor;
		}
	}

	//removes any enemies in the list that aren't active
	vector<cEnemy*>::iterator enemyIterartor = theEnemy.begin();
	while (enemyIterartor != theEnemy.end())
	{
		if ((*enemyIterartor)->isActive() == false)
		{
			enemyIterartor = theEnemy.erase(enemyIterartor);
		}
		else
		{
			++enemyIterartor;
		}
	}
}

cPlayer::~cPlayer()
{

}