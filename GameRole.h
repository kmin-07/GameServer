#pragma once
#include <zinx.h>
#include "AOIWorld.h"
#include "GameMsg.h"
class GameProtocol;
class GameRole;
class GameRole:public Irole,public Player
{
	float x = 0;
	float y = 0;//h
	float z = 0;
	float v = 0;
	int iPid = 0;
	std::string szName;
	void ProcTalkMsg(string _content);
	void ProcMoveMsg(float _x, float _y, float _z, float _v);

	void ViewAppear(GameRole* _pRole);
	void ViewLost(GameRole* _pRole);
public:
	GameMsg* CreateIDNameLogin();
	GameMsg* CreateSrdPlayers();
	GameMsg* CreatSelfPostion();
	GameMsg* CreateIDNameLogoff();
	GameMsg* CreateTalkBroadCast(string text);
public:
	GameRole();
	virtual~GameRole();
	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual UserData* ProcMsg(UserData& _poUserData) override;
	virtual void Fini() override;

	GameProtocol* m_proto = NULL;

	// ͨ�� Player �̳�
	virtual int GetX() override;
	virtual int GetY() override;
};

