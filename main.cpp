#include <cstdio>
#include <zinx.h>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "ZinxTimer.h"
int main()
{
	ZinxKernel::ZinxKernelInit();
	/*Ìí¼Ó¼àÌýÍ¨µÀ*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();

    return 0;
}