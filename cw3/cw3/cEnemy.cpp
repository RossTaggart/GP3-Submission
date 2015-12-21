#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{

}

//randomises the speed of the model
void cEnemy::randomiseSpeed()
{
	int random = rand() % 7 + 1;
	switch (random)
	{
	case 1:
		speedOfModel = 0.0045f;
		break;
	case 2:
		speedOfModel = 0.007f;
		break;
	case 3:
		speedOfModel = 0.01f;
		break;
	case 4:
		speedOfModel = 0.02f;
		break;
	case 5:
		speedOfModel = 0.04f;
		break;
	case 6:
		speedOfModel = 0.06f;
		break;
	case 7:
		speedOfModel = 0.08f;
		break;
	}
}

//randomises in which lane the enemy will spawn in
void cEnemy::randomisePosition()
{
	int lanePos = rand() % 3 + 1;
	switch (lanePos)
	{
		case 1:
			setPosition(glm::vec3(customerStartPosX, customerStartPosY[0], 10));
			break;
		case 2:
			setPosition(glm::vec3(customerStartPosX, customerStartPosY[1], 10));
			break;
		case 3:
			setPosition(glm::vec3(customerStartPosX, customerStartPosY[2], 10));
			break;
	}

	update(0.0f);
}

//updates the enemy and makes sure it doesn't stray out of bounds
//without the game ending
void cEnemy::update(float elapsedTime)
{

	move();
	if (cModel::m_mdlPosition.x > 10.5 || cModel::m_mdlPosition.x < -10.5)
	{
		setIsActive(false);
		gameOverLose = true;
	}
	
}

void cEnemy::setSpeed(float speed)
{
	speedOfModel = speed;
}

void cEnemy::setModelNumber(int modelNo)
{
	modelNumber = modelNo;
}

int cEnemy::getModelNumber()
{
	return modelNumber;
}

void cEnemy::move()
{
	//do movement here
	//Test loop for moving customer towards player
	float distance = sqrt(pow(19.0f - cModel::m_mdlPosition.x, 2) + pow(4.0f - cModel::m_mdlPosition.y, 2));
	float startX = cModel::m_mdlPosition.x;
	float startY = cModel::m_mdlPosition.y;
	float directionX = (19.0f - startX) / distance;
	float directionY = (4.0f - startY) / distance;
	bool moving = true;

	if (moving)
	{
		cModel::m_mdlPosition = glm::vec3(cModel::m_mdlPosition.x + (directionX * 1 * speedOfModel), cModel::m_mdlPosition.y, cModel::m_mdlPosition.z);

		if (sqrt(pow(cModel::m_mdlPosition.x - startX, 2) + pow(cModel::m_mdlPosition.y - startY, 2)) >= distance)
		{
			cModel::m_mdlPosition = glm::vec3(19.0f, 4.0f, cModel::m_mdlPosition.z);
		}
	}
}

cEnemy::~cEnemy()
{

}