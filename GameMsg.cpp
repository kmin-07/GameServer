#include "GameMsg.h"
#include <google/protobuf/message.h>
#include "msg.pb.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg) :enMsgType(_type), pMsg(_pMsg)
{
	
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _stream): enMsgType(_type)
{
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		pMsg = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_POSTION:
		pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	pMsg->ParseFromString(_stream);
}

std::string GameMsg::serialize()
{
	std::string ret;

	pMsg->SerializeToString(&ret);
	
	return ret;
}

GameMsg::~GameMsg()
{
	//delete pMsg;
}

MultiMsg::~MultiMsg()
{
	/*list<GameMsg*>::iterator ite;
	
	for (ite=m_Msgs.begin();ite!=m_Msgs.end();++ite)
	{
		delete (*ite);
		ite = m_Msgs.erase(ite);
	}*/
}
