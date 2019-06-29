#include <cstdio>
#include <zinx.h>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
int main()
{
	ZinxKernel::ZinxKernelInit();
	/*��Ӽ���ͨ��*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();

    return 0;
}