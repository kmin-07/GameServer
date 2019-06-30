#include "GameRole.h"
#include <iostream>
#include "GameMsg.h"
#include <zinx.h>
#include "GameChannel.h"
#include "GameProtocol.h"
#include "msg.pb.h"
#include "AOIWorld.h"
using namespace std;

static AOIWorld world(0,400, 0, 400, 20, 20);

GameMsg* GameRole::CreateIDNameLogin()
{
	pb::SyncPid* pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
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

	GameMsg* ptr = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pMsg);
	return ptr;
}


GameRole::GameRole()
{
	szName = "TOM";
	x = 100;
	z = 100;
	


}

GameRole::~GameRole()
{
}

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


UserData* GameRole::ProcMsg(UserData& _poUserData)
{
	GET_REF2DATA(MultiMsg, input, _poUserData);

	for (auto single : input.m_Msgs)
	{
		cout << "type is" << single->enMsgType << endl;
		cout << single->pMsg->Utf8DebugString() << endl;
		
		auto srd_list = world.GetSrdPlayers(this);
		auto text = dynamic_cast<pb::Talk*>(single->pMsg)->content();
		
		switch(single->enMsgType)
		{
		case GameMsg::MSG_TYPE_CHAT_CONTENT:
			
			for (auto role : srd_list)
			{
				auto msg = GameRole::CreateTalkBroadCast(text);
				auto pRole = dynamic_cast<GameRole*>(role);
				ZinxKernel::Zinx_SendOut(*msg, *(pRole->m_proto));
			}
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
			auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);

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
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
