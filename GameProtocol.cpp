#include "GameProtocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "GameMsg.h"
#include <zinx.h>
#include <ZinxTCP.h>

using namespace std;
GameProtocol::GameProtocol(){}

GameProtocol::~GameProtocol()
{
	/*delete m_role;
	delete m_channel;*/
}

UserData* GameProtocol::raw2request(std::string _szInput)
{
	MultiMsg* pRet = new MultiMsg();
	szlast.append(_szInput);
	while (1)
	{
		
		if (szlast.size() < 8)
		{
			break;
		}
		int ilength = 0;
		ilength |= szlast[0] << 0;
		ilength |= szlast[1] << 8;
		ilength |= szlast[2] << 16;
		ilength |= szlast[3] << 24;

		int id = 0;
		id |= szlast[4] << 0;
		id |= szlast[5] << 8;
		id |= szlast[6] << 16;
		id |= szlast[7] << 24;

		if (szlast.size() - 8 < ilength)
		{
			break;
		}

		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szlast.substr(8, ilength));

		pRet->m_Msgs.push_back(pMsg);

		szlast.erase(0, 8 + ilength);
	}
	
	for (auto single : pRet->m_Msgs)
	{
		cout << single->pMsg->Utf8DebugString() << endl;
	}

	return pRet;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	GET_REF2DATA(GameMsg,gmsg,_oUserData);
	int id = 0;
	int length = 0;
	string MsgContent;
	id = gmsg.enMsgType;

	MsgContent = gmsg.serialize();

	length = MsgContent.size();

	string* pret = new string();

	pret->push_back((length >> 0) & 0xff);
	pret->push_back((length >> 8) & 0xff);
	pret->push_back((length >> 16) & 0xff);
	pret->push_back((length >> 24) & 0xff);
	pret->push_back((id >> 0) & 0xff);
	pret->push_back((id >> 8) & 0xff);
	pret->push_back((id >> 16) & 0xff);
	pret->push_back((id >> 24) & 0xff);
	pret->append(MsgContent);

	return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return m_role;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return m_channel;
}
