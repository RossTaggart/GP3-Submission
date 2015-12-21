#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "GameConstants.h"
#include "cModel.h"
#include "cInputMgr.h"
#include "cTapperGame.h"
#include "cLaser.h"

class cPlayer : public cModel
{
private:
	cInputMgr* m_InputMgr;
public:
	cPlayer();
	void attachInputMgr(cInputMgr* inputMgr);
	virtual void update(float elapsedTime);

	~cPlayer();
};
#endif