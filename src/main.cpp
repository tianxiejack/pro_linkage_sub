


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

string gun_ip;
string gun_username;
string gun_password;
string ball_ip;
string ball_username;
string ball_password;

int ReadOnvifConfigFile()
{	
	string cfgAvtFile;
	char cfg_avt[30] = "cfg_";
	cfgAvtFile = "onvif.yml";
	FILE *fp = fopen(cfgAvtFile.c_str(), "rt");

	if(fp != NULL){
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		fclose(fp);
		if(len < 10)
			return  -1;
		else
		{
			FileStorage fr(cfgAvtFile, FileStorage::READ);
			if(fr.isOpened())
			{
				sprintf(cfg_avt, "cfg_gun_ip_1");
				gun_ip = (string)fr[cfg_avt];
				sprintf(cfg_avt, "cfg_gun_name_1");
				gun_username = (string)fr[cfg_avt];
				sprintf(cfg_avt, "cfg_gun_password_1");
				gun_password= (string)fr[cfg_avt];
				sprintf(cfg_avt, "cfg_ball_ip_1");
				ball_ip = (string)fr[cfg_avt];
				sprintf(cfg_avt, "cfg_ball_name_1");
				ball_username = (string)fr[cfg_avt];
				sprintf(cfg_avt, "cfg_ball_password_1");
				ball_password = (string)fr[cfg_avt];
				fr.release();
 			}
			else
			{
				printf("[get params]open YML failed\n");
				exit(-1);
			}
		}
	}
	else
	{
		printf("[get params] Can not find YML. Please put this file into the folder of execute file\n");
		exit (-1);
	}
	return 0;
}



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

	ReadOnvifConfigFile();

	Capture* rtp0 = RTSPCapture_Create();	//qiang
	string rtspAddress;
	rtspAddress.clear();
	rtspAddress = "rtsp://"+gun_username+":"+gun_password+"@"+gun_ip+":554/h264/ch0/main/av_stream";

	rtp0->init(rtspAddress.c_str(),1,1920,1080,CVideoProcess::processFrame);

	Capture* rtp1 = RTSPCapture_Create();	//qiu
	rtspAddress = "rtsp://"+ball_username+":"+ball_password+"@"+ball_ip+":554/h264/ch0/main/av_stream";
	rtp1->init(rtspAddress.c_str(),0,1920,1080,CVideoProcess::processFrame);

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

