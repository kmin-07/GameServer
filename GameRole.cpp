#include "GameRole.h"
#include <iostream>
#include "GameMsg.h"
#include <zinx.h>
#include "GameChannel.h"
#include "GameProtocol.h"
#include "msg.pb.h"
#include "AOIWorld.h"
#include "RandomName.h"
#include "ZinxTimer.h"
#include <algorithm>
using namespace std;

static AOIWorld world(0,400, 0, 400, 20, 20);

GameMsg* GameRole::CreateIDNameLogin()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	return pRet;
}

GameMsg* GameRole::CreateSrdPlayers()
{
	pb::SyncPlayers* pMsg = new pb::SyncPlayers();
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single:srd_list)
	{
		auto pPlayer = pMsg->add_ps();
		auto pRole = dynamic_cast<GameRole*>(single);
		pPlayer->set_pid(pRole->iPid);
		pPlayer->set_username(pRole->szName);

		auto pPostion = pPlayer->mutable_p();
		pPostion->set_x(pRole->x);
		pPostion->set_y(pRole->y);
		pPostion->set_z(pRole->z);
		pPostion->set_v(pRole->v);

	}
	GameMsg* ptr = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pMsg);
	return ptr;
}

GameMsg* GameRole::CreatSelfPostion()
{
	pb::BroadCast* pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(2);

	auto pPostion = pMsg->mutable_p();
	pPostion->set_x(x);
	pPostion->set_y(y);
	pPostion->set_z(z);
	pPostion->set_v(v);

	GameMsg* ptr = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return ptr;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
	return pRet;
}

GameMsg* GameRole::CreateTalkBroadCast(string text)
{
	pb::BroadCast* pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(1);
	pMsg->set_content(text);

	GameMsg* ptr = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return ptr;
}

static default_random_engine random_engine(time(NULL));
GameRole::GameRole()
{
	szName = "TOM";
	x = 100+random_engine()%50;
	z = 100+random_engine()%50;C
	


}

GameRole::~GameRole()
{
}
class ExitTimer :public TimerOutProc
{
	virtual void Proc() override
	{
		ZinxKernel::Zinx_Exit();
	}
	virtual int GetTimeSec() override
	{
		return 20;
	}
};
static ExitTimer g_exit_timer;
bool GameRole::Init()
{
	bool bRet = false;

	iPid = m_proto->m_channel->GetFd();

	bRet = world.AddPlayer(this);

	if (true==bRet)
	{
		auto pmsg = CreateIDNameLogin();

		ZinxKernel::Zinx_SendOut(*pmsg, *m_proto);

		pmsg = CreateSrdPlayers();

		ZinxKernel::Zinx_SendOut(*pmsg, *m_proto);

		auto srd_list = world.GetSrdPlayers(this);
		for (auto single:srd_list )
		{
			pmsg = CreatSelfPostion();
			auto pRole = dynamic_cast<GameRole*>(single);
			ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_proto));
		}
	}
	return bRet;
}
void GameRole::ProcTalkMsg(string _content)
{
	auto role_list = ZinxKernel::Zinx_GetAllRole();
	for (auto pRole:role_list )
	{
		auto pGameRole = dynamic_cast<GameRole*>(pRole);
		auto pmsg = CreateTalkBroadCast(_content);
		ZinxKernel::Zinx_SendOut(*pmsg, *(pGameRole->m_proto));
	}
}

void GameRole::ProcMoveMsg(float _x, float _y, float _z, float _v)
{
	auto s1 = world.GetSrdPlayers(this);
	world.DelPlayer(this);
	x = _x;
	y = _y;
	z = _z;
	v = _v;
	world.AddPlayer(this);
	auto s2 = world.GetSrdPlayers(this);
	for (auto single_player:s2)
	{
		if (s1.end()==find(s1.begin(),s1.end(),single_player))
		{
			ViewAppear(dynamic_cast<GameRole*>(single_player));
		}
	}

	for (auto single_player:s1)
	{
		if (s2.end()==find(s2.begin(),s2.end(),single_player))
		{
			ViewLost(dynamic_cast<GameRole*>(single_player));
		}
	}
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single:srd_list)
	{
		pb::BroadCast* pmsg = new pb::BroadCast();
		auto pPos = pmsg->mutable_p();
		pPos->set_x(_x);
		pPos->set_y(_y);
		pPos->set_z(_z);
		pPos->set_v(_v);
		pmsg->set_pid(iPid);
		pmsg->set_tp(4);
		pmsg->set_username(szName);
		auto pRole = dynamic_cast<GameRole*>(single);
		ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pmsg)), *(pRole->m_proto));
	}

}

void GameRole::ViewAppear(GameRole* _pRole)
{
	auto pmsg = _pRole->CreatSelfPostion();
	ZinxKernel::Zinx_SendOut(*pmsg, *m_proto);

	pmsg = CreatSelfPostion();
	ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_proto));
}

void GameRole::ViewLost(GameRole* _pRole)
{
	auto pmsg = _pRole->CreateIDNameLogoff();
	ZinxKernel::Zinx_SendOut(*pmsg, *m_proto);
	pmsg = CreateIDNameLogoff();
	ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_proto));

}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
	GET_REF2DATA(MultiMsg, input, _poUserData);

	for (auto single : input.m_Msgs)
	{
		cout << "type is" << single->enMsgType << endl;
		cout << single->pMsg->Utf8DebugString() << endl;
		
		
	
		auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);
		switch(single->enMsgType)
		{
		case GameMsg::MSG_TYPE_CHAT_CONTENT:		
			ProcTalkMsg(dynamic_cast<pb::Talk*>(single->pMsg)->content());
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
			ProcMoveMsg(NewPos->x(), NewPos->y(), NewPos->z(), NewPos->v());
			break;
		}	
	}
	return nullptr;
}

void GameRole::Fini()
{
	auto pmsg= CreateIDNameLogoff();
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		//pmsg = CreatSelfPostion();
		auto pRole = dynamic_cast<GameRole*>(single);
		ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_proto));
	}
	
	world.DelPlayer(this);

	
	if (ZinxKernel::Zinx_GetAllRole().size()<=1)
	{
		TimerOutMng::GetInstance().AddTask(&g_exit_timer);
	}
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
