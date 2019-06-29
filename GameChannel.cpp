#include "GameChannel.h"
#include "GameProtocol.h"
#include <zinx.h>
GameChannel::GameChannel(int _fd):ZinxTcpData(_fd){}

GameChannel::~GameChannel(){}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return m_protocol;
}



ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
	
	auto pChannel = new GameChannel(_fd);

	auto peotocol = new GameProtocol();

	pChannel->m_protocol = peotocol;

	ZinxKernel::Zinx_Add_Proto(*peotocol);
	
	return pChannel;
}
