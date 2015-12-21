#ifndef _CENEMY_H
#define _CENEMY_H

#include "GameConstants.h"
#include "cModel.h"

class cEnemy : public cModel
{
public:
	cEnemy();
	virtual void update(float elapsedTime);
	void move();
	void randomiseSpeed();
	void randomisePosition();
	void setSpeed(float speed);
	void setModelNumber(int modelNo);
	int getModelNumber();

	~cEnemy();
private:
	const float m_EnemyMinSpeed = 3.0f;
	const float m_EnemyMaxSpeed = 10.0f;
	const float m_EnemySpeedAdjustment = 2.5f;
	float speedOfModel;
	int modelNumber;
};

#endif