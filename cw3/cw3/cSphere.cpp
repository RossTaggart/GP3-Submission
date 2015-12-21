/*
==================================================================================
cSphere.cpp
==================================================================================
*/

#include "cSphere.h"


cSphere::cSphere()
{
	setRotAngle(0.0f);
}

cSphere::cSphere(GLdouble dRadius, GLint dSlices, GLint dStacks)
{
	setRotAngle(0.0f);
	sRadius = dRadius;
	sSlices = dSlices;
	sStacks = dStacks;
}

glm::vec3 cSphere::getDirection()
{
	return m_sphDirection;
}

void cSphere::setDirection(glm::vec3 sphDirection)
{
	m_sphDirection = sphDirection;
}

void cSphere::render(float rotAngle)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate Sphere to take account of gluSphere caps
	glRotatef(getRotAngle(), 0, 0, 1);	// Rotation is now on z rather than Y to make sphere spin

	gluSphere(quad, sRadius, sSlices, sStacks);

	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void cSphere::initialise(cTexture theTexture)
{
	quad = gluNewQuadric();

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, theTexture.getTexture());

}

void cSphere::initialise(GLint theTextureID, glm::vec3 thePosition, glm::vec3 theTranslation)
{
	quad = gluNewQuadric();
	m_TextureID = theTextureID;
	m_Position = thePosition;
	m_Translation = theTranslation;

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluQuadricTexture(quad, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

}

void cSphere::setPosition(glm::vec3 sphPosition)
{
	m_Position = sphPosition;
}

void cSphere::update(float elapsedTime)
{
	m_Position += m_sphDirection * 5.0f * elapsedTime;

	/*float distance = sqrt(pow(19.0f - m_Position.x, 2) + pow(4.0f - m_Position.y, 2));
	float startX = m_Position.x;
	float startY = m_Position.y;
	float directionX = (-19.0f + startX) / distance;
	float directionY = (-4.0f + startY) / distance;
	bool moving = true;

	if (moving)
	{
		m_Position = glm::vec3(m_Position.x + (directionX * 1 * 0.01f), m_Position.y, m_Position.z);

		if (sqrt(pow(m_Position.x - startX, 2) + pow(m_Position.y - startY, 2)) >= distance)
		{
			m_Position = glm::vec3(9.0f, 4.0f, m_Position.z);
		}
	}*/
}