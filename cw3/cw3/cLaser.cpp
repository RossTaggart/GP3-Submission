#include "cLaser.h"

cLaser::cLaser() : cModel()
{

}

void cLaser::move()
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
		cModel::m_mdlPosition = glm::vec3(cModel::m_mdlPosition.x + (directionX * 1 * 0.01f), cModel::m_mdlPosition.y, cModel::m_mdlPosition.z);

		if (sqrt(pow(cModel::m_mdlPosition.x - startX, 2) + pow(cModel::m_mdlPosition.y - startY, 2)) >= distance)
		{
			cModel::m_mdlPosition = glm::vec3(19.0f, 4.0f, cModel::m_mdlPosition.z);
		}
	}
}

void cLaser::update(float elapsedTime)
{
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	if (cModel::m_mdlPosition.x > PLAYFIELDX ||
		cModel::m_mdlPosition.x < -PLAYFIELDX ||
		cModel::m_mdlPosition.z > PLAYFIELDZ ||
		cModel::m_mdlPosition.z < -PLAYFIELDZ)
		cModel::m_IsActive = false;

}

cLaser::~cLaser()
{

}
