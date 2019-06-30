#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"
#include <zinx.h>
GameChannel::GameChannel(int _fd):ZinxTcpData(_fd){}

GameChannel::~GameChannel()
{
	//delete m_protocol;
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return m_protocol;
}



ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
	
	auto pChannel = new GameChannel(_fd);

	auto peotocol = new GameProtocol();

	auto prole = new GameRole();

	pChannel->m_protocol = peotocol;

	peotocol->m_channel = pChannel;

	peotocol->m_role = prole;

	prole->m_proto = peotocol;
	

	ZinxKernel::Zinx_Add_Proto(*peotocol);
	
	ZinxKernel::Zinx_Add_Role(*prole);
	
	return pChannel;
}
