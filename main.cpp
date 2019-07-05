#include <cstdio>
#include <zinx.h>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "ZinxTimer.h"
#include "RandomName.h"
#include "AOIWorld.h"

extern RandomName random_name; 

void daemonlize()
{
	int pid=fork();
	if (0>pid)
	{
		exit(-1);
	}
	if (0<pid)
	{
		exit(0);
	}

	setsid();

	//子进程 设置执行路径

	
	//子进程 重定向3个文件描述符到/dev/null
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd>=0)
	{
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);
		close(nullfd);
	}

	while (1)
	{
		int ppid = fork();
		if (0>pid)
		{
			exit(-1);
		}
		if (pid>0)
		{
			int iStatus = 0;
			wait(&iStatus);
			if (0==iStatus)
			{
				exit(0);
			}
		}
		else
		{
			break;
		}

	}
	
}
int main()
{
	random_name.LoadFile();
	//daemonlize();
	ZinxKernel::ZinxKernelInit();
	/*添加监听通道*/
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new GameConnFact())));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();

    return 0;
}