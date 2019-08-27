


#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <glut.h>
#include <sys/time.h>


#include "process51.hpp"
#include "Ipc.hpp"
#include "msgDriv.h"
#include "hahaha.hpp"



#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>



using namespace std;
using namespace cv;

extern int startEnable, ipcDbgOn;


int main(int argc, char **argv)
{
	struct timeval tv;
	int testMode=0;

	if(argc > 1)
	{
		if(strcmp(argv[1], "test") == 0)
		{
			testMode = 1;
			ipcDbgOn = 1;
			printf("====== self test mode enter ======\n");
		}
	}

#if defined(__linux__)
	setenv ("DISPLAY", ":0", 0);
	printf("\n setenv DISPLAY=%s\n",getenv("DISPLAY"));
#endif

	MSGDRIV_create();
#ifdef __IPC__
	Ipc_pthread_start();
#endif
	CProcess proc;
	if(testMode)
	{
		//proc.SaveTestConfig();
		proc.ReadTestConfig();
	}
	while(false == startEnable)
	{
		tv.tv_sec = 0;
		tv.tv_usec = 50000;
		select( 0, NULL, NULL, NULL, &tv );
	};
	proc.loadIPCParam();
	proc.creat();
	proc.init();
	proc.run();

	Capture* rtp0 = RTSPCapture_Create();	//qiang
	rtp0->init("rtsp://admin:admin%2018@192.168.0.66:554/h264/ch0/main/av_stream",0,1920,1080,CVideoProcess::processFrame);

	Capture* rtp1 = RTSPCapture_Create();	//qiu
	rtp1->init("rtsp://admin:admin$2018@192.168.0.64:554/h264/ch0/main/av_stream",1,1920,1080,CVideoProcess::processFrame);

	if(testMode)
		glutKeyboardFunc(keyboard_event);
	glutMainLoop();
	proc.destroy();
#ifdef __IPC__
	Ipc_pthread_stop();
#endif
    	return 0;
}

//__IPC__
//__MOVE_DETECT__
//__TRACK__

//---------------------------
//__MMT__
//__BLOCK__

