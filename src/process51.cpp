
#include <glut.h>
#include "process51.hpp"
#include "vmath.h"
#include "msgDriv.h"
#include "app_ctrl.h"
#include "osd_cv.h"
#include "app_status.h"
#include "configtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <algorithm>
#include <iostream>
#include "Ipc.hpp"
#include "encTrans.hpp"


extern int ScalerLarge,ScalerMid,ScalerSmall;
extern LinkagePos_t linkagePos; 
extern OSDSTATUS gSYS_Osd;
ENCSTATUS gSYS_Enc = {0};
ALG_CONFIG_Trk gCFG_Trk = {0};
ALG_CONFIG_Mtd gCFG_Mtd = {0};
ALG_CONFIG_Enh gCFG_Enh = {0};
ALG_CONFIG_Mmt gCFG_Mmt = {0};
CProcess * CProcess::sThis = NULL;
CProcess* plat = NULL;

OSA_SemHndl g_linkage_getPos;

#define SPECIAL_KEY_UP			101
#define SPECIAL_KEY_DOWN 		103
#define SPECIAL_KEY_LEFT 		100
#define SPECIAL_KEY_RIGHT 		102

#define SPECIAL_KEY_PAGEUP 		104
#define SPECIAL_KEY_PAGEDOWN 	105


void inputtmp(unsigned char cmdid)
{
	plat->OnKeyDwn(cmdid);
}

void getMmtTg(unsigned char index,int *x,int *y)
{
	*x = (int)plat->m_mtd[0]->tg[index].cur_x%vdisWH[plat->extInCtrl->SensorStat][0];
	*y = (int)plat->m_mtd[0]->tg[index].cur_y%vdisWH[plat->extInCtrl->SensorStat][1];
}

#if __MOVE_DETECT__
void getMtdxy(int &x,int &y,int &w,int &h)
{
	x = y = w = h = -1;
	if(plat->chooseDetect >= 10)
	{
		//printf(" getMtdxy error chooseDetect %d\n", plat->chooseDetect);
		return ;
	}
	if(plat->validMtdRecord[plat->chooseDetect])
	{
		for(int i = 0 ;i<plat->mvList.size();i++)
		{
			if(plat->chooseDetect == plat->mvList[i].number)
			{
				x = plat->mvList[i].trkobj.targetRect.x + plat->mvList[i].trkobj.targetRect.width/2;
				y = plat->mvList[i].trkobj.targetRect.y + plat->mvList[i].trkobj.targetRect.height/2;
				w = plat->mvList[i].trkobj.targetRect.width;
				h  = plat->mvList[i].trkobj.targetRect.height;
			}
		}	
	}
}
#endif

CProcess::CProcess():m_bDrawPolyRoi(false),m_bDrawPolyUnRoi(false),m_bDrawEdgeUnRoi(false),
	m_bDrawFullEdgeUnRoi(false)

{	
	extInCtrl = (CMD_EXT*)OSA_memAlloc(sizeof(CMD_EXT));
	memset(extInCtrl,0,sizeof(CMD_EXT));
	msgextInCtrl = extInCtrl;
	sThis = this;
	plat = this;
}

CProcess::~CProcess()
{
	if(extInCtrl != NULL)
	{
		OSA_memFree(extInCtrl);
		extInCtrl = NULL;
	}

	sThis=NULL;
}

int CProcess::SaveTestConfig()
{
	int *testConfig = (int *)OSA_memAlloc( 4*CFGID_FEILD_MAX*CFGID_BLOCK_MAX );
	cfg_dbg_setDefault(testConfig);

	printf("-----------save config------\n");
	string cfgAvtFile, str;
	int configId_Max = CFGID_FEILD_MAX*CFGID_BLOCK_MAX;
	char  cfg_avt[30] = "cfg_avt_";
	cfgAvtFile = "/home/ubuntu/nsight_project/Profile_init.yml";

	FileStorage fr(cfgAvtFile, FileStorage::WRITE);
	if(fr.isOpened())
	{
		for(int i=0; i<configId_Max; i++)
		{
			int blkId = (i/CFGID_FEILD_MAX);
			int feildId = (i%CFGID_FEILD_MAX);
			int usrosdId = -1;
			sprintf(cfg_avt, "cfg_avt_%03d_%02d", blkId, feildId);
			if(blkId >= CFGID_TRK_BKID && blkId < (CFGID_TRK_BKID+3))
			{
				float value = (float)fr[cfg_avt];
				memcpy(&value, &testConfig[i], 4);
				fr<< cfg_avt << value;
				//cout<<"read i="<<i<<"!!data="<<value<<endl;
			}
			else
			{
				int value = testConfig[i];
				fr<< cfg_avt << value;
			}
		}
	}
	else
	{
		printf("-----------save failed-----------\n");
	}
	OSA_memFree(testConfig);
	return 0;
}

int CProcess::ReadTestConfig()
{
	int *testConfig = (int *)OSA_memAlloc( 4*CFGID_FEILD_MAX*CFGID_BLOCK_MAX );
	unsigned char *testUser = (unsigned char *)OSA_memAlloc( USEROSD_LENGTH*CFGID_USEROSD_MAX );

	string cfgAvtFile, str;
	int configId_Max = CFGID_FEILD_MAX*CFGID_BLOCK_MAX;
	char cfg_avt[30] = "cfg_avt_";
	cfgAvtFile = "/home/ubuntu/nsight_project/Profile_init.yml";

	memset(testConfig, 0, 4*CFGID_FEILD_MAX*CFGID_BLOCK_MAX);
	memset(testUser, 0, USEROSD_LENGTH*CFGID_USEROSD_MAX);
	FileStorage fr(cfgAvtFile, FileStorage::READ);
	if(fr.isOpened())
	{
		for(int i=0; i<configId_Max; i++)
		{
			int blkId = (i/CFGID_FEILD_MAX);
			int feildId = (i%CFGID_FEILD_MAX);
			int usrosdId = -1;
			sprintf(cfg_avt, "cfg_avt_%03d_%02d", blkId, feildId);
			if((blkId >= CFGID_OSD_BKID) && (blkId <= CFGID_OSD_BKID + 15))
				usrosdId = blkId - CFGID_OSD_BKID;
			else if((blkId >= CFGID_OSD2_BKID) && (blkId <= CFGID_OSD2_BKID + 15))
				usrosdId = blkId - CFGID_OSD2_BKID + 16;
			if((usrosdId >= 0) && (i == CFGID_OSD_CONTENT(usrosdId) || i == CFGID_OSD2_CONTENT(usrosdId)))
			{
				str = (string)fr[cfg_avt];
				//cout<<"read i="<<i<<"!!str="<<str<<endl;
				str.copy((char *)(testUser+usrosdId*USEROSD_LENGTH), str.length()<USEROSD_LENGTH?str.length():USEROSD_LENGTH, 0);
			}
			else if(blkId < CFGID_TRK_BKID || blkId >= CFGID_PID_BKID)
			{
				continue;	// no need read
			}
			else if(blkId >= CFGID_TRK_BKID && blkId < (CFGID_TRK_BKID+3))
			{
				float value = (float)fr[cfg_avt];
				memcpy(&testConfig[i], &value, 4);
				//cout<<"read i="<<i<<"!!data="<<value<<endl;
			}
			else if(i == CFGID_ENCOUT_rmip)
			{
				str = (string)fr[cfg_avt];
				unsigned int intip = stringip2int(str);
				memcpy(&testConfig[i], &intip, 4);
			}
			else
			{
				int value = (int)fr[cfg_avt];
				testConfig[i] = value;
			}
		}
		fr.release();
	}
	else
	{
		printf("[get params]open YML failed\n");
	}
	cfg_dbg_getDefault(testConfig, testUser);
	OSA_memFree(testConfig);
	OSA_memFree(testUser);
	return 0;
}

void CProcess::loadIPCParam()
{
	CMD_EXT *pIStuts = extInCtrl;
	
	memset(rcTrackBak, 0, sizeof(rcTrackBak));
	memset(tgBak, 0, sizeof(tgBak));
	memset(&extOutAck, 0, sizeof(ACK_EXT));
	prisensorstatus=0;//tv
	m_castTm=0;
	m_bCast=false;
	rememflag=false;
	rememtime=0;
	// default cmd value
	
	for(int i = 0; i < MAX_CHAN; i++)
	{
		pIStuts->opticAxisPosX[i] = vdisWH[i][0]/2;
		pIStuts->opticAxisPosY[i] = vdisWH[i][1]/2;
	}
	
	pIStuts->unitAimW 		= 	AIM_WIDTH;
	pIStuts->unitAimH 		= 	AIM_HEIGHT;
	pIStuts->unitAimX		=	vdisWH[video_pal][0]/2;
	pIStuts->unitAimY		=	vdisWH[video_pal][1]/2;

	cfg_ctrl_mainchReset(pIStuts);
	//m_curChId = pIStuts->SensorStat;
	pIStuts->SensorStatpri  =   pIStuts->SensorStat;
	pIStuts->PicpSensorStatpri	=	pIStuts->PicpSensorStat = 0xFF;
	
	pIStuts->changeSensorFlag = 0;
	
	pIStuts->AvtTrkAimSize= AVT_TRK_AIM_SIZE;

	for(int i = 0; i < MAX_CHAN; i++)
	{
		pIStuts->AvtPosX[i] = pIStuts->AxisPosX[i] = pIStuts->opticAxisPosX[i];
		pIStuts->AvtPosY[i] = pIStuts->AxisPosY[i] = pIStuts->opticAxisPosY[i];
		crossBak[i].x = pIStuts->opticAxisPosX[i];
		crossBak[i].y = pIStuts->opticAxisPosY[i];
	}
	
	pIStuts->PicpPosStat = 0;
	pIStuts->FovStat=1;

	pIStuts->FrCollimation=2;
	pIStuts->PicpSensorStatpri=2;
	pIStuts->axisMoveStepX = 0;
	pIStuts->axisMoveStepY = 0;

	memset(secBak,0,sizeof(secBak));
	memset(Osdflag,0,sizeof(Osdflag));
	
	Mmtsendtime=0;

	rendpos[0].x=vdisWH[0][0]*2/3;
	rendpos[0].y=vdisWH[0][1]*2/3;
	rendpos[0].w=vdisWH[0][0]/3;
	rendpos[0].h=vdisWH[0][1]/3;

	rendpos[1].x=vdisWH[0][0]*2/3;
	rendpos[1].y=0;
	rendpos[1].w=vdisWH[0][0]/3;
	rendpos[1].h=vdisWH[0][1]/3;

	rendpos[2].x=0;
	rendpos[2].y=0;
	rendpos[2].w=vdisWH[0][0]/3;
	rendpos[2].h=vdisWH[0][1]/3;

	rendpos[3].x=0;
	rendpos[3].y=vdisWH[0][1]*2/3;
	rendpos[3].w=vdisWH[0][0]/3;
	rendpos[3].h=vdisWH[0][1]/3;

	cfg_ctrl_acqReset(pIStuts);
	update_param_osd();

	pIStuts->SceneAvtTrkStat = 0;

	m_tempXbak = m_tempYbak = 0;
	memset(m_rectnbak, 0, sizeof(m_rectnbak));
	memset(mRectbak, 0, sizeof(mRectbak));
	memset(timearr, 0, sizeof(timearr));
	memset(timearrbak, 0, sizeof(timearrbak));
	timexbak = timeybak = 0;
#if __MOVE_DETECT__
	chooseDetect = 10;
#endif

	memset(validMtdRecord,0,10);
	
	OSA_semCreate(&g_linkage_getPos, 1, 0);

	//m_display.linkage.menuLoadIpcParam(getSysconfig());

	return ;
}


int  CProcess::WindowstoPiexlx(int x,int channel)
{
	int ret=0;
	ret= cvRound(x*1.0/vdisWH[0][0]*vcapWH[channel][0]);
	 if(ret<0)
	 	{
			ret=0;
	 	}
	 else if(ret>=vcapWH[channel][0])
	 	{
			ret=vcapWH[channel][0];
	 	}


	  return ret;
}


int  CProcess::WindowstoPiexly(int y,int channel)
{
	 int ret=0;
	 ret= cvRound(y*1.0/vdisWH[0][1]*vcapWH[channel][1]);

	  if(ret<0)
	 	{
			ret=0;
	 	}
	 else if(ret>=vcapWH[channel][1])
	 	{
			ret=vcapWH[channel][1];
	 	}
	return  ret;
}



float  CProcess::PiexltoWindowsx(int x,int channel)
{
	 float ret=0;
	 ret= cvRound(x*1.0/vcapWH[channel][0]*vdisWH[channel][0]);
	 if(ret<0)
 	 {
		ret=0;
 	 }
	 else if(ret>=vdisWH[channel][0])
 	 {
		ret=vdisWH[channel][0];
 	 }
	 if(extInCtrl->ImgMmtshow[extInCtrl->SensorStat])
 	 {
		ret =ret*2/3;
 	 }

	 return ret;
}

float  CProcess::PiexltoWindowsy(int y,int channel)
{
	 float ret=0;
	 ret= cvRound(y*1.0/vcapWH[channel][1]*vdisWH[channel][1]);

	 if(ret<0)
 	 {
		ret=0;
 	 }
	 else if(ret>=vdisWH[channel][1])
 	 {
		ret=vdisWH[channel][1];
 	 }

	  if(extInCtrl->ImgMmtshow[extInCtrl->SensorStat])
 	  {
		ret =ret*2/3;
 	  }
	
	return  ret;
}

float  CProcess::PiexltoWindowsxf(float x,int channel)
{
	float ret=0;
	 ret= (x*1.0/vcapWH[channel][0]*vdisWH[channel][0]);
	 if(ret<0)
 	{
		ret=0;
 	}
	 else if(ret>=vdisWH[channel][0])
 	{
		ret=vdisWH[channel][0];
 	}

	  return ret;
}


int  CProcess::PiexltoWindowsxzoom(int x,int channel)
{
	int ret=0;
	 ret= cvRound(x*1.0/vcapWH[channel][0]*vdisWH[channel][0]);
	 if(ret<0)
 	{
		ret=0;
 	}
	 else if(ret>=vdisWH[channel][0])
 	{
		ret=vdisWH[channel][0];
 	}

	if(extInCtrl->ImgMmtshow[extInCtrl->SensorStat])
 	{
		ret =ret*2/3;
 	}

	if(extInCtrl->FovCtrl==5&&extInCtrl->SensorStat==0)
 	{
 		ret=ret-320;
		ret=2*ret;
 	}
	return ret;
}

int  CProcess::PiexltoWindowsyzoom(int y,int channel)
{
	 int ret=0;
	 ret= cvRound(y*1.0/vcapWH[channel][1]*vdisWH[channel][1]);

	  if(ret<0)
 	{
		ret=0;
 	}
	 else if(ret>=vdisWH[channel][1])
 	{
		ret=vdisWH[channel][1];
 	}

	  if(extInCtrl->ImgMmtshow[extInCtrl->SensorStat])
 	{
		ret =ret*2/3;
 	}

	 if(extInCtrl->FovCtrl==5&&extInCtrl->SensorStat==0)
 	{
 		ret=ret-256;
		ret=2*ret;
 	}
	return  ret;
}

int  CProcess::PiexltoWindowsxzoom_TrkRect(int x,int channel)
{
	int ret=0;

	ret= cvRound(x*1.0/vcapWH[channel][0]*vdisWH[channel][0]);
	
	if(ret<0)
	{
		ret=0;
	}
	else if(ret>=vdisWH[channel][0])
	{
		ret=vdisWH[channel][0];
	}

	//result to even
	if((ret%2)==0)
		ret = ret;
	else
		ret = ret+1;
	
	return ret;
}

int  CProcess::PiexltoWindowsyzoom_TrkRect(int y,int channel)
{
	 int ret=0;

	 ret= cvRound(y*1.0/vcapWH[channel][1]*vdisWH[channel][1]);

	if(ret<0)
 	{
		ret=0;
 	}
	 else if(ret>=vdisWH[channel][1])
 	{
		ret=vdisWH[channel][1];
 	}

	if((ret%2)==0)
		ret = ret;
	else
		ret = ret+1;

	return  ret;
}

void CProcess::OnCreate()
{
	MSGAPI_initial();
}

void CProcess::OnDestroy(){};
void CProcess::OnInit()
{
	m_stateManger = new StateManger();
	m_stateManger->init(m_display.chinese_osd, m_display.changeDisplayMode ,m_pMovDetector);
	
	return ;
}
void CProcess::OnConfig(){};
void CProcess::OnRun()
{
	update_param_alg();
	msgdriv_event(MSGID_EXT_INPUT_SENSOR, NULL);

#ifdef ENCTRANS_ON
	if(gSYS_Enc.srcType == 0)// ENCTRANS_SCREEN
	{
		cv::Size imgSize[1];
		imgSize[0] = cv::Size(1920, 1080);
		EncTrans_create(1, imgSize);
		msgdriv_event(MSGID_EXT_INPUT_GSTCTRL, 0);
	}
	else if(gSYS_Enc.srcType == 1)	// ENCTRANS_APPCAP
	{
		int encId=0;
		cv::Size imgSize[ENT_CHN_MAX];
		for(int i=0; i<MAX_CHAN; i++)
		{
			if(gSYS_Enc.vinChMask & (1<<i))
			{
				gSYS_Enc.vinEncId[i] = encId;
				imgSize[encId] = cv::Size(vcapWH[i][0], vcapWH[i][1]);
				encId++;
				if(encId >= ENT_CHN_MAX)
					break;
			}
		}
		EncTrans_create(encId, imgSize);
		msgdriv_event(MSGID_EXT_INPUT_GSTCTRL, 0);
	}
#endif
};
void CProcess::OnStop()
{
#ifdef ENCTRANS_ON
	EncTrans_destroy();
#endif
};
void CProcess::Ontimer(){

	//msgdriv_event(MSGID_EXT_INPUT_VIDEOEN,NULL);
};
bool CProcess::OnPreProcess(int chId, Mat &frame)
{
	if(m_bCast){
		Uint32 curTm = OSA_getCurTimeInMsec();
		Uint32 elapse = curTm - m_castTm;

		if(elapse < 2000){
			return false;
		}
		else
		{
			m_bCast=false;
		}
	}
	return true;
}


int onece=0;

void CProcess::osd_mtd_show(TARGET tg[], bool bShow)
{
	int i;
	int frcolor= gSYS_Osd.osdDrawColor;
	unsigned char Alpha = (bShow) ? frcolor : 0;
	CvScalar colour=GetcvColour(Alpha);

	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{
		if(tg[i].valid)
		{
			cv::Rect result;
			result.width = 32;
			result.height = 32;
			result.x = ((int)tg[i].cur_x) % vdisWH[extInCtrl->SensorStat][0];
			result.y = ((int)tg[i].cur_y ) % vdisWH[extInCtrl->SensorStat][1];
			result.x = result.x - result.width/2;
			result.y = result.y - result.height/2;
			rectangle(m_display.m_imgOsd[extInCtrl->SensorStat],
				Point( result.x, result.y ),
				Point( result.x+result.width, result.y+result.height),
				colour, 1, 8);
		}
	}
}


void CProcess::DrawCross(cv::Rect rec,int fcolour ,int sensor,bool bShow /*= true*/)
{
	unsigned char colour = (bShow) ?fcolour : 0;
	Line_Param_fb lineparm;
	lineparm.x		=	rec.x;
	lineparm.y		=	rec.y;
	lineparm.width	=	rec.width;
	lineparm.height	=	rec.height;
	lineparm.frcolor	=	colour;
	if(sensor>=MAX_CHAN)
		sensor = 0;
	Drawcvcrossaim(m_display.m_imgOsd[sensor],&lineparm);
}

void CProcess::DrawAcqRect(cv::Mat frame,cv::Rect rec,int frcolor,bool bshow)
{
	int color = (bshow)?frcolor:0;
	int leftBottomx 	= rec.x;
	int leftBottomy 	= rec.y;
	int leftTopx 		= leftBottomx ;
	int leftTopy 		= leftBottomy - rec.height;
	int rightTopx 	= leftBottomx + rec.width;
	int rightTopy 		= leftTopy;
	int rightBottomx 	= rightTopx;
	int rightBottomy 	= leftBottomy;

	int cornorx = rec.width/4;
	int cornory = rec.height/4;
	
	Osd_cvPoint start;
	Osd_cvPoint end;

	//leftBottom
	start.x 	= leftBottomx;
	start.y 	= leftBottomy;
	end.x	= leftBottomx + cornorx;
	end.y 	= leftBottomy;
	DrawcvLine(frame,&start,&end,color,1);
	start.x 	= leftBottomx;
	start.y 	= leftBottomy;
	end.x	= leftBottomx;
	end.y 	= leftBottomy - cornory;
	DrawcvLine(frame,&start,&end,color,1);	
	//leftTop
	start.x 	= leftTopx;
	start.y 	= leftTopy;
	end.x	= leftTopx + cornorx;
	end.y 	= leftTopy;
	DrawcvLine(frame,&start,&end,color,1);
	start.x 	= leftTopx;
	start.y 	= leftTopy;
	end.x	= leftTopx;
	end.y 	= leftTopy + cornory;
	DrawcvLine(frame,&start,&end,color,1);	
	//rightTop
	start.x 	= rightTopx;
	start.y 	= rightTopy;
	end.x	= rightTopx - cornorx;
	end.y 	= rightTopy;
	DrawcvLine(frame,&start,&end,color,1);
	start.x 	= rightTopx;
	start.y 	= rightTopy;
	end.x	= rightTopx;
	end.y 	= rightTopy + cornory;
	DrawcvLine(frame,&start,&end,color,1);
	//rightBottom
	start.x 	= rightBottomx;
	start.y 	= rightBottomy;
	end.x	= rightBottomx - cornorx;
	end.y 	= rightBottomy;
	DrawcvLine(frame,&start,&end,color,1);
	start.x 	= rightBottomx;
	start.y 	= rightBottomy;
	end.x	= rightBottomx;
	end.y 	= rightBottomy - cornory;
	DrawcvLine(frame,&start,&end,color,1);	

	return ;
}

void CProcess::DrawRect(Mat frame,cv::Rect rec,int frcolor)
{
	int x = rec.x,y = rec.y;
	int width = rec.width;
	int height = rec.height;
	drawcvrect(frame,x,y,width,height,frcolor);
	return ;
}



int majormmtid=0;
int primajormmtid=0;

void CProcess::erassdrawmmt(TARGET tg[],bool bShow)
{
			int startx=0;
			int starty=0;
			int endx=0;
			int endy=0;
			Mat frame=m_display.m_imgOsd[extInCtrl->SensorStat];
			int i=0,j=0;
			cv::Rect result;
			short tempmmtx=0;
			short tempmmty=0;
			int tempdata=0;
			int testid=0;
			extInCtrl->Mmttargetnum=0;
			char numbuf[3];
			int frcolor= gSYS_Osd.osdDrawColor;
			unsigned char Alpha = (bShow) ? frcolor : 0;
			CvScalar colour=GetcvColour(Alpha);

			tempdata=primajormmtid;
			for(i=0;i<MAX_TARGET_NUMBER;i++)
				{

						//if(m_mtd[chId]->tg[i].valid)
						
						if((tg[primajormmtid].valid)&&(i==0))
						{
							//majormmtid=i;
							result.width = 32;
							result.height = 32;
							tempmmtx=result.x = ((int)tg[primajormmtid].cur_x) % vdisWH[extInCtrl->SensorStat][0];
							tempmmty=result.y = ((int)tg[primajormmtid].cur_y ) % vdisWH[extInCtrl->SensorStat][1];


							extInCtrl->MmtPixelX=result.x;
							extInCtrl->MmtPixelY=result.y;
							extInCtrl->MmtValid=1;
							result.x = result.x - result.width/2;
							result.y = result.y - result.height/2;

							
							 startx=PiexltoWindowsx(result.x,prisensorstatus);
							 starty=PiexltoWindowsy(result.y,prisensorstatus);
							 endx=PiexltoWindowsx(result.x+result.width,prisensorstatus);
						 	 endy=PiexltoWindowsy(result.y+result.height,prisensorstatus);

							rectangle( frame,
								Point( startx, starty ),
								Point( endx, endy),
								colour, 1, 8);
							
						}
						
						else if(tg[tempdata].valid)
							{
								testid++;
								result.width = 32;
								result.height = 32;
								tempmmtx=result.x = ((int)tg[tempdata].cur_x) % vdisWH[extInCtrl->SensorStat][0];
								tempmmty=result.y = ((int)tg[tempdata].cur_y ) % vdisWH[extInCtrl->SensorStat][1];

								 startx=PiexltoWindowsx(result.x,prisensorstatus);
								 starty=PiexltoWindowsy(result.y,prisensorstatus);
								line(frame, cvPoint(startx-16,starty), cvPoint(startx+16,starty), colour, 1, 8, 0 ); 
								line(frame, cvPoint(startx,starty-16), cvPoint(startx,starty+16), colour, 1, 8, 0 ); 
								//OSA_printf("******************the num  majormmtid=%d\n",majormmtid);
								sprintf(numbuf,"%d",(tempdata+MAX_TARGET_NUMBER-primajormmtid)%MAX_TARGET_NUMBER);
								putText(frame,numbuf,cvPoint(startx+14,starty+14),CV_FONT_HERSHEY_SIMPLEX,1,colour);
								
								
							}
				
				
						tempdata=(tempdata+1)%MAX_TARGET_NUMBER;

					}


}


void CProcess::drawmmt(TARGET tg[],bool bShow)
{
	int startx=0;
	int starty=0;
	int endx=0;
	int endy=0;
	Mat frame=m_display.m_imgOsd[extInCtrl->SensorStat];
	int i=0,j=0;
	cv::Rect result;
	short tempmmtx=0;
	short tempmmty=0;
	int tempdata=0;
	int testid=0;
	extInCtrl->Mmttargetnum=0;
	char numbuf[3];
	int frcolor= gSYS_Osd.osdDrawColor;
	unsigned char Alpha = (bShow) ? frcolor : 0;
	CvScalar colour=GetcvColour(Alpha);
	
	for(i=0;i<20;i++)
	{
		extInCtrl->MmtOffsetXY[i]=0;
	}
	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{

		if(tg[majormmtid].valid==0)
		{
			//majormmtid++;
			majormmtid=(majormmtid+1)%MAX_TARGET_NUMBER;
		}
		if(tg[i].valid==1)
		{
			extInCtrl->Mmttargetnum++;
		}
	}

	primajormmtid=tempdata=majormmtid;
	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{
		if((tg[majormmtid].valid)&&(i==0))
		{
			//majormmtid=i;
			result.width = 32;
			result.height = 32;
			tempmmtx=result.x = ((int)tg[majormmtid].cur_x) % vdisWH[extInCtrl->SensorStat][0];
			tempmmty=result.y = ((int)tg[majormmtid].cur_y ) % vdisWH[extInCtrl->SensorStat][1];


			extInCtrl->MmtPixelX=result.x;
			extInCtrl->MmtPixelY=result.y;
			extInCtrl->MmtValid=1;
			
			//OSA_printf("the num  majormmtid=%d\n",majormmtid);
			result.x = result.x - result.width/2;
			result.y = result.y - result.height/2;

			 startx=PiexltoWindowsx(result.x,extInCtrl->SensorStat);
			 starty=PiexltoWindowsy(result.y,extInCtrl->SensorStat);
			 endx=PiexltoWindowsx(result.x+result.width,extInCtrl->SensorStat);
		 	 endy=PiexltoWindowsy(result.y+result.height,extInCtrl->SensorStat);


			if(((extInCtrl->AvtTrkStat == eTrk_mode_mtd)||(extInCtrl->AvtTrkStat == eTrk_mode_acq)))
			{
				rectangle( frame,
					Point( startx, starty ),
					Point( endx, endy),
					colour, 1, 8);
			}
			//OSA_printf("******************the num  majormmtid=%d x=%d y=%d w=%d h=%d\n",majormmtid,
			//	result.x,result.y,result.width,result.height);
			extInCtrl->MmtOffsetXY[j]		=	tempmmtx&0xff;
			extInCtrl->MmtOffsetXY[j+1]	=	(tempmmtx>>8)&0xff;
			extInCtrl->MmtOffsetXY[j+2]	=	tempmmty&0xff;
			extInCtrl->MmtOffsetXY[j+3]	=	(tempmmty>>8)&0xff;
		}	
		else if(tg[tempdata].valid)
		{
			testid++;
			result.width = 32;
			result.height = 32;
			tempmmtx=result.x = ((int)tg[tempdata].cur_x) % vdisWH[extInCtrl->SensorStat][0];
			tempmmty=result.y = ((int)tg[tempdata].cur_y ) % vdisWH[extInCtrl->SensorStat][1];

			 startx=PiexltoWindowsx(result.x,extInCtrl->SensorStat);
			 starty=PiexltoWindowsy(result.y,extInCtrl->SensorStat);
			if(((extInCtrl->AvtTrkStat == eTrk_mode_mtd)||(extInCtrl->AvtTrkStat == eTrk_mode_acq)))
			{
				line(frame, cvPoint(startx-16,starty), cvPoint(startx+16,starty), colour, 1, 8, 0 ); 
				line(frame, cvPoint(startx,starty-16), cvPoint(startx,starty+16), colour, 1, 8, 0 ); 
				//OSA_printf("******************the num  majormmtid=%d\n",majormmtid);
				sprintf(numbuf,"%d",(tempdata+MAX_TARGET_NUMBER-majormmtid)%MAX_TARGET_NUMBER);
				putText(frame,numbuf,cvPoint(startx+14,starty+14),CV_FONT_HERSHEY_SIMPLEX,1,colour);
			}
			extInCtrl->MmtOffsetXY[j+testid*4]=tempmmtx&0xff;
			extInCtrl->MmtOffsetXY[j+1+testid*4]=(tempmmtx>>8)&0xff;
			extInCtrl->MmtOffsetXY[j+2+testid*4]=tempmmty&0xff;
			extInCtrl->MmtOffsetXY[j+3+testid*4]=(tempmmty>>8)&0xff;	
		}
		tempdata=(tempdata+1)%MAX_TARGET_NUMBER;
	}

	if(Mmtsendtime==0)
		;
	Mmtsendtime++;
	if(Mmtsendtime==1)
	{
		Mmtsendtime=0;
	}
}


void CProcess::erassdrawmmtnew(TARGETDRAW tg[],bool bShow)
{
	int startx=0;
	int starty=0;
	int endx=0;
	int endy=0;
	Mat frame=m_display.m_imgOsd[extInCtrl->SensorStat];
	int i=0,j=0;
	cv::Rect result;
	short tempmmtx=0;
	short tempmmty=0;
	int tempdata=0;
	int testid=0;
	extInCtrl->Mmttargetnum=0;
	char numbuf[3];
	int frcolor= gSYS_Osd.osdDrawColor;
	unsigned char Alpha = (bShow) ? frcolor : 0;
	CvScalar colour=GetcvColour(Alpha);

	primajormmtid;
	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{
		if((tg[primajormmtid].valid)&&(i==primajormmtid))
		{	
			 startx=tg[primajormmtid].startx;//PiexltoWindowsx(result.x,prisensorstatus);
			 starty=tg[primajormmtid].starty;//PiexltoWindowsy(result.y,prisensorstatus);
			 endx=tg[primajormmtid].endx;//PiexltoWindowsx(result.x+result.width,prisensorstatus);
		 	 endy=tg[primajormmtid].endy;//PiexltoWindowsy(result.y+result.height,prisensorstatus);

			rectangle( frame,
				Point( startx, starty ),
				Point( endx, endy),
				colour, 1, 8);
			rectangle( frame,
				Point( startx-1, starty-1 ),
				Point( endx+1, endy+1),
				colour, 1, 8);
			sprintf(numbuf,"%d",primajormmtid+1);
			putText(frame,numbuf,cvPoint(startx,starty-2),CV_FONT_HERSHEY_SIMPLEX,0.8,colour);	
		}

		if((tg[i].valid)&&(i!=primajormmtid))
		{
			 startx=tg[i].startx;//PiexltoWindowsx(result.x,prisensorstatus);
			 starty=tg[i].starty;//PiexltoWindowsy(result.y,prisensorstatus);
			 endx=tg[i].endx;
			 endy=tg[i].endy;

			rectangle( frame,
			Point( startx, starty ),
			Point( endx, endy),
			colour, 1, 8);

			//OSA_printf("******************the num  majormmtid=%d\n",majormmtid);
			sprintf(numbuf,"%d",i+1);
			putText(frame,numbuf,cvPoint(startx,starty-2),CV_FONT_HERSHEY_SIMPLEX,0.8,colour);
		}
	}
}


void CProcess::drawmmtnew(TARGET tg[],bool bShow)
{
	int startx=0;
	int starty=0;
	int endx=0;
	int endy=0;
	Mat frame=m_display.m_imgOsd[extInCtrl->SensorStat];
	int i=0,j=0;
	cv::Rect result;
	short tempmmtx=0;
	short tempmmty=0;
	int tempdata=0;
	int testid=0;
	extInCtrl->Mmttargetnum=0;
	char numbuf[3];
	int frcolor= gSYS_Osd.osdDrawColor;
	unsigned char Alpha = (bShow) ? frcolor : 0;
	CvScalar colour=GetcvColour(Alpha);
	
	//memset(extInCtrl->MmtOffsetXY,0,20);
	for(i=0;i<20;i++)
	{
		extInCtrl->MmtOffsetXY[i]=0;
	}
	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{

		if(tg[majormmtid].valid==0)
		{
			//majormmtid++;
			//find mmt major target;
			if(extInCtrl->MMTTempStat==3)
				majormmtid=(majormmtid+1)%MAX_TARGET_NUMBER;
			else if(extInCtrl->MMTTempStat==4)
				majormmtid=(majormmtid-1+MAX_TARGET_NUMBER)%MAX_TARGET_NUMBER;
			else
				majormmtid=(majormmtid+1)%MAX_TARGET_NUMBER;

		}
		if(tg[i].valid==1)
		{
			//valid mmt num;
			extInCtrl->Mmttargetnum++;
		}
		Mdrawbak[i].valid=0;//reset

	}
	
	primajormmtid=tempdata=majormmtid;
	for(i=0;i<MAX_TARGET_NUMBER;i++)
	{
		if((tg[majormmtid].valid)&&(i==majormmtid))
		{

			if(extInCtrl->SensorStat==0)
			{
				if(extInCtrl->FovCtrl!=5)
				{
					result.width 	= 32;
					result.height 	= 32;
				}
				else
				{
					result.width 	= 16;
					result.height 	= 16;
				}
			}
			else
			{
				result.width 	= 16;
				result.height 	= 16;
			}
			tempmmtx=result.x = ((int)tg[majormmtid].cur_x) % vdisWH[extInCtrl->SensorStat][0];
			tempmmty=result.y = ((int)tg[majormmtid].cur_y ) % vdisWH[extInCtrl->SensorStat][1];

			
			//mmt track target set
			extInCtrl->MmtPixelX=result.x;
			extInCtrl->MmtPixelY=result.y;
			extInCtrl->MmtValid=1;

			
		
			result.x = result.x - result.width/2;
			result.y = result.y - result.height/2;
			
			 startx=PiexltoWindowsxzoom(result.x,extInCtrl->SensorStat);
			 starty=PiexltoWindowsyzoom(result.y,extInCtrl->SensorStat);
			 endx=PiexltoWindowsxzoom(result.x+result.width,extInCtrl->SensorStat);
		 	 endy=PiexltoWindowsyzoom(result.y+result.height,extInCtrl->SensorStat);
			 //erase param
			 Mdrawbak[i].startx=startx;
			 Mdrawbak[i].starty=starty;
			 Mdrawbak[i].endx=endx;
			 Mdrawbak[i].endy=endy;
			 Mdrawbak[i].valid=1;

			if(((extInCtrl->AvtTrkStat == eTrk_mode_mtd)||(extInCtrl->AvtTrkStat == eTrk_mode_acq)))
			{
				rectangle( frame,
				Point( startx, starty ),
				Point( endx, endy),
				colour, 1, 8);
				Osdflag[osdindex]=1;

				rectangle( frame,
				Point( startx-1, starty-1 ),
				Point( endx+1, endy+1),
				colour, 1, 8);

				sprintf(numbuf,"%d",majormmtid+1);
				putText(frame,numbuf,cvPoint(startx,starty-2),CV_FONT_HERSHEY_SIMPLEX,0.8,colour);

			}
			//OSA_printf("******************the num  majormmtid=%d x=%d y=%d w=%d h=%d\n",majormmtid,
			//	result.x,result.y,result.width,result.height);
			tempmmtx  =PiexltoWindowsx(tempmmtx,extInCtrl->SensorStat);
			tempmmty  =PiexltoWindowsy(tempmmty,extInCtrl->SensorStat);
			extInCtrl->MmtOffsetXY[j]=tempmmtx&0xff;
			extInCtrl->MmtOffsetXY[j+1]=(tempmmtx>>8)&0xff;
			extInCtrl->MmtOffsetXY[j+2]=tempmmty&0xff;
			extInCtrl->MmtOffsetXY[j+3]=(tempmmty>>8)&0xff;
			
		}
		
		if((tg[i].valid)&&(i!=majormmtid))
		{
			testid++;
			if(extInCtrl->SensorStat==0)
			{
				if(extInCtrl->FovCtrl!=5)
				{
					result.width = 32;
					result.height = 32;
				}
				else
				{
					result.width = 16;
					result.height = 16;
				}
			}
			else
			{
				result.width = 16;
				result.height = 16;

			}
			
			tempmmtx=result.x = ((int)tg[i].cur_x) % vdisWH[extInCtrl->SensorStat][0];
			tempmmty=result.y = ((int)tg[i].cur_y ) % vdisWH[extInCtrl->SensorStat][1];		

			//OSA_printf("+++++++++++++++the num  majormmtid=%d x=%d y=%d w=%d h=%d\n",majormmtid,
			//result.x,result.y,result.width,result.height);
			result.x = result.x - result.width/2;
			result.y = result.y - result.height/2;
			//OSA_printf("the num  majormmtid=%d\n",tempdata);

			startx=PiexltoWindowsxzoom(result.x,extInCtrl->SensorStat);
			starty=PiexltoWindowsyzoom(result.y,extInCtrl->SensorStat);
			endx=PiexltoWindowsxzoom(result.x+result.width,extInCtrl->SensorStat);
			endy=PiexltoWindowsyzoom(result.y+result.height,extInCtrl->SensorStat);

			Mdrawbak[i].startx=startx;
			Mdrawbak[i].starty=starty;
			Mdrawbak[i].endx=endx;
			Mdrawbak[i].endy=endy;
			Mdrawbak[i].valid=1;
			if(((extInCtrl->AvtTrkStat == eTrk_mode_mtd)||(extInCtrl->AvtTrkStat == eTrk_mode_acq)))
			{
				//DrawCross(result.x,result.y,frcolor,bShow);
				//trkimgcross(frame,result.x,result.y,16);
				#if 1
				rectangle( frame,
				Point( startx, starty ),
				Point( endx, endy),
				colour, 1, 8);
				#endif
				//OSA_printf("******************the num  majormmtid=%d\n",majormmtid);
				sprintf(numbuf,"%d",i+1);
				putText(frame,numbuf,cvPoint(startx,starty-2),CV_FONT_HERSHEY_SIMPLEX,0.8,colour);
			}
			
			extInCtrl->MmtOffsetXY[j+testid*4]=tempmmtx&0xff;
			extInCtrl->MmtOffsetXY[j+1+testid*4]=(tempmmtx>>8)&0xff;
			extInCtrl->MmtOffsetXY[j+2+testid*4]=tempmmty&0xff;
			extInCtrl->MmtOffsetXY[j+3+testid*4]=(tempmmty>>8)&0xff;

			extInCtrl->MmtOffsetXY[j+testid*4]    =PiexltoWindowsx(extInCtrl->MmtOffsetXY[j+testid*4],extInCtrl->SensorStat);
			extInCtrl->MmtOffsetXY[j+1+testid*4]=PiexltoWindowsx(extInCtrl->MmtOffsetXY[j+1+testid*4],extInCtrl->SensorStat);
			extInCtrl->MmtOffsetXY[j+2+testid*4]=PiexltoWindowsy(extInCtrl->MmtOffsetXY[j+2+testid*4],extInCtrl->SensorStat);
			extInCtrl->MmtOffsetXY[j+3+testid*4]=PiexltoWindowsy(extInCtrl->MmtOffsetXY[j+3+testid*4],extInCtrl->SensorStat);
			//j++;
			
		}

		//mmt show
		tempmmtx=result.x = ((int)tg[i].cur_x) % vdisWH[extInCtrl->SensorStat][0];
		tempmmty=result.y = ((int)tg[i].cur_y ) % vdisWH[extInCtrl->SensorStat][1];
		Mmtpos[i].x=tempmmtx-result.width/2;
		Mmtpos[i].y=tempmmty-result.height/2;
		Mmtpos[i].w=result.width;
		Mmtpos[i].h=result.height;
		Mmtpos[i].valid=tg[i].valid;

	}	

	if(Mmtsendtime==0)
		;//MSGAPI_AckSnd( AckMtdInfo);
	Mmtsendtime++;
	if(Mmtsendtime==1)
	{
		Mmtsendtime=0;
	}
	
	msgdriv_event(MSGID_EXT_INPUT_MMTSHOWUPDATE, NULL);

}



void CProcess::DrawMeanuCross(int lenx,int leny,int fcolour , bool bShow ,int centerx,int centery)
{
	int templenx=lenx;
	int templeny=leny;
	int lenw=35;
	unsigned char colour = (bShow) ?fcolour : 0;
	Osd_cvPoint start;
	Osd_cvPoint end;

	////v
	start.x=centerx-templenx;
	start.y=centery-templeny;
	end.x=centerx-templenx+lenw;
	end.y=centery-templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

	start.x=centerx+templenx-lenw;
	start.y=centery-templeny;
	end.x=centerx+templenx;
	end.y=centery-templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);


	start.x=centerx-templenx;
	start.y=centery+templeny;
	end.x=centerx-templenx+lenw;
	end.y=centery+templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

	start.x=centerx+templenx-lenw;
	start.y=centery+templeny;
	end.x=centerx+templenx;
	end.y=centery+templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

	//h
	start.x=centerx-templenx;
	start.y=centery-templeny;
	end.x=centerx-templenx;
	end.y=centery-templeny+lenw;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

	start.x=centerx+templenx;
	start.y=centery-templeny;
	end.x=centerx+templenx;
	end.y=centery-templeny+lenw;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);


	start.x=centerx-templenx;
	start.y=centery+templeny-lenw;
	end.x=centerx-templenx;
	end.y=centery+templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

	start.x=centerx+templenx;
	start.y=centery+templeny-lenw;
	end.x=centerx+templenx;
	end.y=centery+templeny;
	DrawcvLine(m_display.m_imgOsd[extInCtrl->SensorStat],&start,&end,colour,1);

}

void CProcess::DrawdashCross(int x,int y,int fcolour ,bool bShow /*= true*/)
{

	int startx=0;
	int starty=0;
	int endx=0;
	int endy=0;
	unsigned char colour = (bShow) ?fcolour : 0;
	Line_Param_fb lineparm;

	startx=WindowstoPiexlx(extInCtrl->AvtPosX[extInCtrl->SensorStat],extInCtrl->SensorStat);
	starty=WindowstoPiexly(extInCtrl->AvtPosY[extInCtrl->SensorStat],extInCtrl->SensorStat);
	
	lineparm.x=startx;
	lineparm.y=starty;
	lineparm.width=50;
	lineparm.height=50;
	lineparm.frcolor=colour;

	int dashlen=2;

	Point start,end;

	if(!bShow)
	{
		lineparm.x=secBak[1].x;
		lineparm.y=secBak[1].y;
		DrawcvDashcross(m_display.m_imgOsd[extInCtrl->SensorStat],&lineparm,dashlen,dashlen);
		startx=secBak[0].x;
		starty=secBak[0].y;
		endx=secBak[1].x;
		endy=secBak[1].y;
		
		drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],startx,starty,endx,endy,dashlen,dashlen,colour);
	}

	else
	{
		DrawcvDashcross(m_display.m_imgOsd[extInCtrl->SensorStat],&lineparm,dashlen,dashlen);
		startx=PiexltoWindowsxzoom(extInCtrl->AvtPosX[extInCtrl->SensorStat ],extInCtrl->SensorStat);
		starty=PiexltoWindowsyzoom(extInCtrl->AvtPosY[extInCtrl->SensorStat ],extInCtrl->SensorStat);
		endx=lineparm.x;
		endy=lineparm.y;
		drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],startx,starty,endx,endy,dashlen,dashlen,colour);

		secBak[0].x=startx;
		secBak[0].y=starty;
		secBak[1].x=endx;
		secBak[1].y=endy;
		
		Osdflag[osdindex]=1;	
	}
}


void CProcess::DrawdashRect(int startx,int starty,int endx,int endy,int colour)
{
	int dashlen=3;
	drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],startx,starty,endx,starty,dashlen,dashlen,colour);
	drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],startx,endy,endx,endy,dashlen,dashlen,colour);
	drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],endx,starty,endx,endy,dashlen,dashlen,colour);
	drawdashlinepri(m_display.m_imgOsd[extInCtrl->SensorStat],startx,starty,startx,endy,dashlen,dashlen,colour);
}

#if __MOVE_DETECT__
char CProcess::getMvListValidNum()
{
	char tmp = 0;	
	for(int i =0 ;i < 10 ; i++)
	{
		if(validMtdRecord[i])
			tmp++;
	}
	return tmp;
}

char CProcess::getMvListFirstValidNum()
{
	for(int i =0 ;i < 10 ; i++)
	{
		if(validMtdRecord[i])
			return i;
	}
	return -1;
}


char CProcess::getMvListNextValidNum(char index)
{
	for(int i = index+1 ;i < 10 ; i++)
	{
		if(validMtdRecord[i])
			return i;
	}

	for(int i = 0 ; i<index ; i++)
	{
		if(validMtdRecord[i])
			return i;
	}
	return 10;
}

char CProcess::getMvListFirstUnusedNum()
{	
	for(int i =0 ;i < 10 ; i++)
	{
		if(!validMtdRecord[i])
			return i;
	}
	return 10;
}


void CProcess::memsetMvList()
{
	memset(validMtdRecord,0,10);
}

void CProcess::addMvListValidNum(char num)
{
	if(num < 10)
		validMtdRecord[num] = true;
	return ;
}

void CProcess::removeMvListValidNum(char num)
{
	if(num < 10)
		validMtdRecord[num] = false;
	return ;
}


void CProcess::switchMvTargetForwad()
{
	if(getMvListValidNum())
	{
		do{
			chooseDetect = (chooseDetect + 1)%10;
		}while(!validMtdRecord[chooseDetect]);
	}		
	return ;
}

void CProcess::switchMvTargetBack()
{
	if(getMvListValidNum())
	{
		do{
			chooseDetect = (chooseDetect+10-1)%10;
		}while(!validMtdRecord[chooseDetect]);
	}	
	return ;
}

bool comp(const TRK_RECT_INFO &a,const TRK_RECT_INFO &b)
{
	unsigned int tmpa ,tmpb;
	unsigned int mx,my;
	mx = abs(a.targetRect.x - vcapWH[plat->extInCtrl->SensorStat][0]/2);
	my = abs(a.targetRect.y - vcapWH[plat->extInCtrl->SensorStat][1]/2);
	tmpa = mx*mx + my*my;
	mx = abs(b.targetRect.x - vcapWH[plat->extInCtrl->SensorStat][0]/2);
	my = abs(b.targetRect.y - vcapWH[plat->extInCtrl->SensorStat][1]/2);
	tmpb = mx*mx + my*my;
	return tmpa<tmpb;
}

void CProcess::getTargetNearToCenter()
{
	using namespace std;
	
	int sizeNum = sThis->detect_bak.size();
	if(sizeNum)
		sort(sThis->detect_bak.begin(),sThis->detect_bak.end(),comp);
}

void CProcess::mvIndexHandle(std::vector<TRK_INFO_APP> &mvList,std::vector<TRK_RECT_INFO> &detect,int detectNum)
{	
	int tmpIndex , i ;
	bool flag;
	TRK_INFO_APP pTmpMv;
	
	if(!mvList.empty())
	{	
		i = 0;
		std::vector<TRK_INFO_APP>::iterator pMvList = mvList.begin();
		
		for( ; pMvList !=  mvList.end(); )
		{
			tmpIndex = (*pMvList).trkobj.index;

			flag = 0;
			std::vector<TRK_RECT_INFO>::iterator pDetect = detect.begin();
			for( ; pDetect != detect.end(); )
			{
				if( tmpIndex == (*pDetect).index )
				{
					memcpy((void*)&((*pMvList).trkobj.targetRect),(void*)&((*pDetect).targetRect),sizeof(TRK_RECT_INFO));				
					if((chooseDetect == (*pMvList).number) && (losenumber != (*pMvList).number))
					{
						cur_targetRect = (*pMvList).trkobj.targetRect;
					}
					detect.erase(pDetect);
					flag = 1;
					break;
				}
				else
					++pDetect;
			}	

			if(!flag)
			{
				if((chooseDetect == (*pMvList).number))
				{
					losenumber = (*pMvList).number;
					chooseDetect = 10;
					cur_targetRect.width = 0;
				}
				removeMvListValidNum((*pMvList).number);
				mvList.erase(pMvList);
			}
			else
				++pMvList;
		
		}

		i = 0;
		Rect2d tmpTarget;
		while(detect.size() > 0)
		{	
			if(mvList.size() >= detectNum)
				break ;
			if(i >= detect.size())
				break;
			i++;
			tmpTarget.x = detect[i].targetRect.x;
			tmpTarget.y = detect[i].targetRect.y;
			pTmpMv.number = getMvListFirstUnusedNum();
			if(pTmpMv.number < 10)
			{
				addMvListValidNum(pTmpMv.number);
				memcpy((void*)&(pTmpMv.trkobj),(void *)&(detect[i].targetRect),sizeof(TRK_RECT_INFO));
				mvList.push_back(pTmpMv);	
			}
		}	
	}
	else
	{
		Rect2d tmpTarget;
		
		while( mvList.size()<detectNum &&  detect.size()>0 )
		{
			tmpTarget.x = detect[0].targetRect.x;
			tmpTarget.y = detect[0].targetRect.y;			
			pTmpMv.number = getMvListFirstUnusedNum();
			if(pTmpMv.number < 10)
			{
				addMvListValidNum(pTmpMv.number);
				memcpy((void*)&(pTmpMv.trkobj),(void *)&(detect[0].targetRect),sizeof(TRK_RECT_INFO));
				mvList.push_back(pTmpMv);
			}
			detect.erase(detect.begin());
		}		
	}

	if( chooseDetect == 10 )
	{
		chooseDetect = getMvListNextValidNum(10);
		for(int i=0; i<mvList.size(); i++ )
		{			
			if(mvList[i].number == chooseDetect)
				cur_targetRect = mvList[i].trkobj.targetRect;
		}
	}

	
}

#endif


void CProcess::DrawMtd_Rigion_Target()
{
	static bool mainObjFlag = false;
	unsigned int mat_Id = 1;
	Osd_cvPoint startwarnpoly,endwarnpoly;
	int polwarn_flag = 0;
	static bool drawflag = false;
	int color = 0;;
	int i ,j ,cnt;
	int detectNum = 10;
	cv::Rect tmp;
	mouserect recttmp;
	Rect2d tmpTarget;
	int setx, sety = 0;

	if(drawflag)
	{
		color = 0;
		for(int j = 0; j < edge_contours_bak.size(); j++)
		{
			polwarn_flag = (j+1)%edge_contours_bak.size();
			startwarnpoly.x = edge_contours_bak[j].x;
			startwarnpoly.y = edge_contours_bak[j].y;
			endwarnpoly.x = edge_contours_bak[polwarn_flag].x;
			endwarnpoly.y = edge_contours_bak[polwarn_flag].y;
			DrawcvLine(m_display.m_imgOsd[mat_Id],&startwarnpoly,&endwarnpoly,color,2);	
		}
	
		for(std::vector<TRK_INFO_APP>::iterator plist = mvList.begin(); plist != mvList.end(); ++plist)
		{	
			color = 0;
			recttmp.x = (*plist).trkobj.targetRect.x;
			recttmp.y = (*plist).trkobj.targetRect.y;
			recttmp.w = (*plist).trkobj.targetRect.width;
			recttmp.h = (*plist).trkobj.targetRect.height;
			tmpTarget.x = recttmp.x + recttmp.w/2;
			tmpTarget.y = recttmp.y + recttmp.h/2;
			
			recttmp = mapfullscreen2gunv20(recttmp);
			tmp.x = recttmp.x;
			tmp.y = recttmp.y;
			tmp.width = recttmp.w;
			tmp.height = recttmp.h;
			DrawRect(m_display.m_imgOsd[mat_Id], tmp ,color);
		}

		if(mainObjFlag){
			DrawRect(m_display.m_imgOsd[mat_Id], m_mainObjBK ,0);
			mainObjFlag = false;
		}
	}

	if(m_stateManger->m_curState == LINKAUTO)
		if(m_bMoveDetect)
		{
			edge_contours_bak = m_stateManger->getEdgecounter();
			color = 5;
			for(int j = 0; j < edge_contours_bak.size(); j++)
			{
				polwarn_flag = (j+1)%edge_contours_bak.size();
				startwarnpoly.x = edge_contours_bak[j].x;
				startwarnpoly.y = edge_contours_bak[j].y;
				endwarnpoly.x = edge_contours_bak[polwarn_flag].x;
				endwarnpoly.y = edge_contours_bak[polwarn_flag].y;
				DrawcvLine(m_display.m_imgOsd[mat_Id],&startwarnpoly,&endwarnpoly,color,2);	
			}
	
			detect_bak = detect_vect;
			mvIndexHandle(mvList,pThis->detect_bak,detectNum);

			if(forwardflag)
			{
				initagainMv();
				switchMvTargetForwad();
				forwardflag = 0;
			}
			else if(backflag)
			{
				//switchMvTargetForwad();
				backflag = 0;
			}

			for(std::vector<TRK_INFO_APP>::iterator plist = mvList.begin(); plist != mvList.end(); ++plist)
			{	
				color = 3;

				recttmp.x = (*plist).trkobj.targetRect.x;
				recttmp.y = (*plist).trkobj.targetRect.y;
				recttmp.w = (*plist).trkobj.targetRect.width;
				recttmp.h = (*plist).trkobj.targetRect.height;
				tmpTarget.x = recttmp.x + recttmp.w/2;
				tmpTarget.y = recttmp.y + recttmp.h/2;

				recttmp = mapfullscreen2gunv20(recttmp);
				tmp.x = recttmp.x;
				tmp.y = recttmp.y;
				tmp.width = recttmp.w;
				tmp.height = recttmp.h;

				DrawRect(m_display.m_imgOsd[mat_Id], tmp ,color);
			}
		
			if((mvList.size()>0) && cur_targetRect.width )		
			{		
				cur_targetRect_bak = cur_targetRect;
				if( m_bAutoLink && (0 == m_chSceneNum)){
					OSA_semSignal(&m_mvObjSync);
				}
				if(false == m_bAutoLink)
					m_bAutoLink = true;
			}

			if(m_mainObjDrawFlag){
				recttmp.x = m_sceInitRectBK.x;
				recttmp.y = m_sceInitRectBK.y;
				recttmp.w = m_sceInitRectBK.width;
				recttmp.h = m_sceInitRectBK.height;
				recttmp = mapfullscreen2gunv20(recttmp);
				m_mainObjBK.x = recttmp.x;
				m_mainObjBK.y = recttmp.y;
				m_mainObjBK.width = recttmp.w;
				m_mainObjBK.height = recttmp.h;
				DrawRect(m_display.m_imgOsd[mat_Id], m_mainObjBK ,6);
				mainObjFlag = true;				
			}
			
			drawflag = true;
		}
	return;	
}

void DrawArrow(Mat frame, cv::Point jos_mouse_bak, int linecolor, int color)
{
	int arrow_angle = 60;
	int angle1 = 25;
	int angle2 = 65;
	int body_length = 20;
	int tail_width = 3;
	int head_length = 17;
	cv::Point point_arr[1][7];
	cv::Point point_start, point_end;
	const double PI = 3.1415926;

	point_start.x = jos_mouse_bak.x;
	point_start.y = jos_mouse_bak.y;
	point_end.x = point_start.x + body_length * cos(PI * arrow_angle / 180);
	point_end.y = point_start.y + body_length * sin(PI * arrow_angle / 180);
	
	point_arr[0][0].x = point_start.x;
	point_arr[0][0].y = point_start.y;

	point_arr[0][3].x = point_end.x + tail_width / 2 * sin(PI * arrow_angle / 180);
	point_arr[0][3].y = point_end.y - tail_width / 2 * cos(PI * arrow_angle / 180);
	point_arr[0][4].x = point_end.x - tail_width / 2 * sin(PI * arrow_angle / 180);
	point_arr[0][4].y = point_end.y + tail_width / 2 * cos(PI * arrow_angle / 180);

	point_arr[0][1].x = point_arr[0][0].x + head_length  * cos(PI * (arrow_angle - angle1) / 180);
	point_arr[0][1].y = point_arr[0][0].y + head_length  * sin(PI * (arrow_angle - angle1) / 180);
	point_arr[0][6].x = point_arr[0][0].x + head_length  * sin(PI * (90 - arrow_angle - angle1) / 180);
	point_arr[0][6].y = point_arr[0][0].y + head_length  * cos(PI * (90 - arrow_angle - angle1) / 180);

	int length = (head_length * sin(PI * angle1 / 180) - tail_width / 2) / sin(PI * angle2 / 180);
	point_arr[0][2].x = point_arr[0][1].x - length  * cos(PI * (angle2 - arrow_angle) / 180);
	point_arr[0][2].y = point_arr[0][1].y + length  * sin(PI * (angle2 - arrow_angle) / 180);
	point_arr[0][5].x = point_arr[0][6].x + length  * sin(PI * (angle2 + arrow_angle - 90) / 180);
	point_arr[0][5].y = point_arr[0][6].y - length  * cos(PI * (angle2 + arrow_angle - 90) / 180);

	const Point * ppt[1] = {point_arr[0]};
	int npt[] = {7};
	cv::fillPoly(frame, ppt, npt, 1, GetcvColour(color), 0);
	cv::polylines(frame, ppt, npt, 1, 1, GetcvColour(linecolor), 1, 8, 0);
}

void CProcess::DrawMouse()
{
	Mat frame = m_display.m_imgOsd[1];
	int linecolor, color;
	static cv::Point jos_mouse_bak;
	static int flag = 0; 
	
	if(flag)
	{
		linecolor = 0;
		color = 0;
		DrawArrow(frame, jos_mouse_bak, linecolor, color);
		flag = 0;
	}
	if(m_stateManger->m_mousectrlState)
	{
		jos_mouse_bak = m_josMouse;
	//printf("mouse x,y = (%d, %d)\n" , jos_mouse_bak.x,jos_mouse_bak.y);
		linecolor = 1;
		color = 2;
		DrawArrow(frame, jos_mouse_bak, linecolor, color);
		flag = 1;
	}
}

bool CProcess::OnProcess()
{				
	int frcolor= gSYS_Osd.osdDrawColor;
	int startx=0;
	int starty=0;
	int endx=0;
	int endy=0;
	int i;
	cv::Rect recIn;
	static int coastCnt = 1;
	static int bDraw = 0;
	int color = 0;
	static unsigned char bdrawMvRect = 0;		
	static int changesensorCnt = 0;
	static cv::Rect sceneBak;
	
	if(extInCtrl->changeSensorFlag == 1)
		++changesensorCnt;
	if(changesensorCnt == 3){
		extInCtrl->changeSensorFlag =  0; 
		changesensorCnt = 0;
	}
	
	
	if(((++coastCnt)%10) == 0)
	{
		bDraw = !bDraw;	
		coastCnt = 0;
	}
	
	CvScalar colour=GetcvColour(frcolor);
	static unsigned int countnofresh = 0;
	if((countnofresh ) >= 5)
	{
		countnofresh=0;
	}
	
	countnofresh++;

	osdindex=0;	

	Point center;
	Point start,end;
	Osd_cvPoint start1,end1;

	osdindex++;
	{
		if(Osdflag[osdindex])
		{
			Osdflag[osdindex]=0;
		}
	}


	DrawMtdPolygonRoi();
	DrawMtdPolygonUnRoi();
	DrawPipCross();
	Drawfeaturepoints();
	DrawMouse();
	
	prisensorstatus=extInCtrl->SensorStat;
	
/////////////////////////////////////////////
	
	//static unsigned int count = 0;
	//if((count & 1) == 1)
		OSA_semSignal(&(sThis->m_display.tskdisSemmain));
	//count++;
	return true;
}


void CProcess::drawPatternRect()
{
	//algboxBK = trackbox;
	algboxBK = m_algbox;
	for(int i=0; i<algboxBK.size(); i++)
	{
		//if( 1 == algboxBK[i].trackstatus )
		{
			cv::Rect r = algboxBK[i];
			rectangle(m_display.m_imgOsd[extInCtrl->SensorStat], r.tl(), r.br(), Scalar(255,0,0,255), 3);
		}
	}
	return ;
}


void CProcess::Drawfeaturepoints()
{
	m_stateManger->drawPoints(m_display.m_imgOsd[1]);
	return;
}



static inline void my_rotate(GLfloat result[16], float theta)
{
	float rads = float(theta/180.0f) * CV_PI;
	const float c = cosf(rads);
	const float s = sinf(rads);

	memset(result, 0, sizeof(GLfloat)*16);

	result[0] = c;
	result[1] = -s;
	result[4] = s;
	result[5] = c;
	result[10] = 1.0f;
	result[15] = 1.0f;
}

void CProcess::OnMouseLeftDwn(int x, int y)
{

};
void CProcess::OnMouseLeftUp(int x, int y){};
void CProcess::OnMouseRightDwn(int x, int y){};
void CProcess::OnMouseRightUp(int x, int y){};
void CProcess::OnSpecialKeyDwn(int key,int x, int y)
{
	static int f;
	static float p = -1.0,t = -1.0,z = 0;
	switch(key) 
	{
		case 1:
		case 2:
		case 3:
			m_stateManger->specialEvent(key);
			//m_display.linkage.OnJosEvent(JOSF2_ENTER_MENU,f);
			break;

		case SPECIAL_KEY_UP:
			m_stateManger->upMenu();
			//m_display.linkage.app_ctrl_upMenu();
			break;
					
		case SPECIAL_KEY_DOWN:
			m_stateManger->downMenu();
			//m_display.linkage.app_ctrl_downMenu();
			break; 

		case 5:
			sendIpc4PTZpos();
			break;
			
		default:
			break;
	}
	
}

void CProcess::OnKeyDwn(unsigned char key)
{
	char flag = 0;
	CMD_EXT *pIStuts = extInCtrl;
	CMD_EXT tmpCmd = {0};
		
	if (key == 'e' || key == 'E')
	{
		forwardflag = true;
	}

	if((key >= '0') && (key <= '9'))
	{
		m_stateManger->normalKeyEvent(key);
		//m_display.linkage.app_ctrl_setnumber(key);
	}

	if(key == 13)
	{
		m_stateManger->enterKeyEvent();
		//m_display.linkage.app_ctrl_enter();
	}

	
}


void CProcess::msgdriv_event(MSG_PROC_ID msgId, void *prm)
{
	int tempvalue=0;
	static unsigned char recordNum = 0;
	CMD_EXT *pIStuts = extInCtrl;
	CMD_EXT *pInCmd = NULL;
	CMD_EXT tmpCmd = {0};
	if(msgId == MSGID_EXT_INPUT_SENSOR || msgId == MSGID_EXT_INPUT_ENPICP)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->SensorStat = pInCmd->SensorStat;
			pIStuts->PicpSensorStat = pInCmd->PicpSensorStat;
		}
		int itmp;
		//chage acq;
		m_rcAcq.width		=	pIStuts->AimW[pIStuts->SensorStat];
		m_rcAcq.height	=	pIStuts->AimH[pIStuts->SensorStat];

		m_rcAcq.x=pIStuts->opticAxisPosX[pIStuts->SensorStat]-m_rcAcq.width/2;
		m_rcAcq.y=pIStuts->opticAxisPosY[pIStuts->SensorStat]-m_rcAcq.height/2;

		OSA_printf("recv   the rctrack x=%f y=%f w=%f h=%f  sensor=%d picpsensor=%d\n",m_rcAcq.x,m_rcAcq.y,
			m_rcAcq.width,m_rcAcq.height,pIStuts->SensorStat,pIStuts->PicpSensorStat);
		
		itmp = pIStuts->SensorStat;
		dynamic_config(VP_CFG_MainChId, itmp, NULL);

#if 1//change the sensor picp change too
		if((pIStuts->PicpSensorStat>=eSen_CH0)&&(pIStuts->PicpSensorStat<eSen_Max))
		{
			for(int chi=eSen_CH0;chi<eSen_Max;chi++)
			{
				if(pIStuts->ImgPicp[chi]==1)
					pIStuts->PicpSensorStatpri=pIStuts->PicpSensorStat;
			}
			
		}
#endif

		itmp = pIStuts->PicpSensorStat;//freeze change
		dynamic_config(VP_CFG_SubChId, itmp, NULL);

//sensor 1 rect

		DS_Rect lay_rect;
			lay_rect.w = vcapWH[0][0]/3;
			lay_rect.h = vcapWH[0][1]/3;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;

		if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 2){
			lay_rect.w = vcapWH[0][0]/12;
			lay_rect.h = vcapWH[0][1]/12;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;
		}
		else if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 4){
			lay_rect.w = vcapWH[0][0]/18;
			lay_rect.h = vcapWH[0][1]/18;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;
		}
		else if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 6){
			lay_rect.w = vcapWH[0][0]/24;
			lay_rect.h = vcapWH[0][1]/24;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;
		}
		else if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 8){
			lay_rect.w = vcapWH[0][0]/30;
			lay_rect.h = vcapWH[0][1]/30;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] - lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] - lay_rect.h/2;
		}

		m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 1, &lay_rect);

//picp position
		lay_rect=rendpos[pIStuts->PicpPosStat];
		
		lay_rect.w = VIDEO_DIS_WIDTH/3;
		lay_rect.h =VIDEO_DIS_HEIGHT/3;
		lay_rect.x = VIDEO_DIS_WIDTH*2/3;
		lay_rect.y = VIDEO_DIS_HEIGHT*2/3;
		m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 1, &lay_rect);

///sensor zoom

		if(0)//(pIStuts->ImgZoomStat[pIStuts->SensorStat])
		{
			/*
			memset(&lay_rect, 0, sizeof(DS_Rect));
			if(pIStuts->SensorStat==0)//just tv zooom
			{
				lay_rect.w = vcapWH[pIStuts->SensorStat][0]/2;
				lay_rect.h = vcapWH[pIStuts->SensorStat][1]/2;
				lay_rect.x = vcapWH[pIStuts->SensorStat][0]/4;
				lay_rect.y = vcapWH[pIStuts->SensorStat][1]/4;
			}
			*/
			
			//m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 0, &lay_rect);
			if(pIStuts->PicpSensorStat==1)
			{
				lay_rect.w = vcapWH[1][0]/6;
				lay_rect.h = vcapWH[1][1]/6;
				lay_rect.x = vcapWH[1][0]/2-lay_rect.w/2;
				lay_rect.y = vcapWH[1][1]/2-lay_rect.h/2;
				m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 1, &lay_rect);
			}
			if(pIStuts->PicpSensorStat==0)
			{	
				lay_rect.w = vcapWH[0][0]/6;
				lay_rect.h = vcapWH[0][1]/6;
				lay_rect.x = vcapWH[0][0]/2-lay_rect.w/2;
				lay_rect.y = vcapWH[0][1]/2-lay_rect.h/2;
				m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 1, &lay_rect);
			}
		}


//mmt show change
	if(pIStuts->ImgMmtshow[pIStuts->SensorStat^1]==0x01)
		{
			
			int mmtchid=0;
			int chid=pIStuts->SensorStat;
			pIStuts->ImgMmtshow[pIStuts->SensorStat^1]=0;
			pIStuts->ImgMmtshow[pIStuts->SensorStat]=1;
			itmp = chid;
			mmtchid=2;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=3;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=4;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=5;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=6;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			lay_rect.w = vdisWH[0][0]/3*2;
			lay_rect.h = vdisWH[0][1]/3*2;
			lay_rect.x = 0;
			lay_rect.y = vdisWH[0][1]/3;
			m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 0, &lay_rect);
			//m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 7, NULL);
			



		}

		
		
	
	}

	if(msgId == MSGID_EXT_INPUT_TRACK)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->AvtTrkStat = pInCmd->AvtTrkStat;
		}
		char procStr[][10] = {"ACQ", "TARGET", "MTD", "SECTRK", "SEARCH", "ROAM", "SCENE", "IMGTRK"};
		UTC_RECT_float rc;
		if (pIStuts->AvtTrkStat == eTrk_mode_acq)
		{
			OSA_printf(" %d:%s set track to [%s]\n", OSA_getCurTimeInMsec(), __func__,
					   procStr[pIStuts->AvtTrkStat]);

			dynamic_config(VP_CFG_TrkEnable, 0);
			pIStuts->AvtPosX[extInCtrl->SensorStat] = pIStuts->AxisPosX[extInCtrl->SensorStat];
			pIStuts->AvtPosY[extInCtrl->SensorStat] = pIStuts->AxisPosY[extInCtrl->SensorStat];	
			//pIStuts->AimW[pIStuts->SensorStat] = 60;
			//pIStuts->AimH[pIStuts->SensorStat] = 60;
			
			pIStuts->unitAimX = pIStuts->AvtPosX[extInCtrl->SensorStat] ;
			if(pIStuts->unitAimX < 0)
			{
				pIStuts->unitAimX = 0;
			}

			pIStuts->unitAimY = pIStuts->AvtPosY[extInCtrl->SensorStat];

			if(pIStuts->unitAimY<0)
			{
				pIStuts->unitAimY=0;
			}
			rc.width	= pIStuts->AimW[pIStuts->SensorStat];
			rc.height	= pIStuts->AimH[pIStuts->SensorStat];
			rc.x=pIStuts->unitAimX-rc.width/2;
			rc.y=pIStuts->unitAimY-rc.height/2;
			dynamic_config(VP_CFG_TrkEnable, 0,&rc);
			return ;
		}

		if (pIStuts->AvtTrkStat == eTrk_mode_sectrk)
		{
			OSA_printf(" %d:%s line:%d set track to [%s]\n", OSA_getCurTimeInMsec(), __func__,
					   __LINE__,procStr[pIStuts->AvtTrkStat]);
			pIStuts->unitAimX = pIStuts->AvtPosX[extInCtrl->SensorStat];
			pIStuts->unitAimY = pIStuts->AvtPosY[extInCtrl->SensorStat] ;
		}
		else if (pIStuts->AvtTrkStat == eTrk_mode_search)
		{
			OSA_printf(" %d:%s line:%d set track to [%s]\n", OSA_getCurTimeInMsec(), __func__,
					   __LINE__,procStr[pIStuts->AvtTrkStat]);

		  	//pIStuts->AvtTrkStat = eTrk_mode_search;
		 	pIStuts->unitAimX = pIStuts->AvtPosX[extInCtrl->SensorStat];
		   	pIStuts->unitAimY = pIStuts->AvtPosY[extInCtrl->SensorStat] ;
		}
		else if (pIStuts->AvtTrkStat == eTrk_mode_mtd)
		{
			pIStuts->unitAimX = pIStuts->AvtPosX[extInCtrl->SensorStat];
		   	pIStuts->unitAimY = pIStuts->AvtPosY[extInCtrl->SensorStat] ;
			dynamic_config(VP_CFG_TrkEnable, 0,NULL);
			return ;
			
			pIStuts->AvtTrkStat = eTrk_mode_target;

			//zoom for mtdTrk change xy 	
			pIStuts->unitAimX=pIStuts->MmtPixelX;
			pIStuts->unitAimY=pIStuts->MmtPixelY;
		 	
			if(pIStuts->MmtValid)
			{
				tempvalue=pIStuts->MmtPixelX;
				
				if(tempvalue<0)
					{
						pIStuts->unitAimX=0;
					}
				else
					{
						pIStuts->unitAimX=tempvalue;

					}
				tempvalue=pIStuts->MmtPixelY ;
				//- pIStuts->unitAimH/2;
				if(tempvalue<0)
					{
						pIStuts->unitAimY=0;
					}
				else
					{
						pIStuts->unitAimY=tempvalue;

					}
				
				OSA_printf(" %d:%s set track to x =%f y=%f  mtdx=%d mtdy=%d  w=%d  h=%d\n", OSA_getCurTimeInMsec(), __func__,
						pIStuts->unitAimX,pIStuts->unitAimY, pIStuts->MmtPixelX,pIStuts->MmtPixelY,pIStuts->unitAimW/2,pIStuts->unitAimH/2);
			}
			else
			{
				pIStuts->unitAimX = pIStuts->opticAxisPosX[extInCtrl->SensorStat ] - pIStuts->unitAimW/2;
				pIStuts->unitAimY = pIStuts->opticAxisPosY[extInCtrl->SensorStat ] - pIStuts->unitAimH/2;
			}
		}

		//printf("%s,line:%d   aimx,aimy=(%d,%d)\n",__func__,__LINE__,pIStuts->AvtPosX[0],pIStuts->AvtPosY[0]);
		if((m_curChId== video_gaoqing0)||(m_curChId== video_gaoqing)||(m_curChId== video_gaoqing2)||(m_curChId== video_gaoqing3))
		{
			rc.width= pIStuts->AimW[pIStuts->SensorStat];
			rc.height=pIStuts->AimH[pIStuts->SensorStat];
			pIStuts->unitAimX = pIStuts->AvtPosX[pIStuts->SensorStat];
			pIStuts->unitAimY = pIStuts->AvtPosY[pIStuts->SensorStat];
/*
			printf("AvtPosX[%d] , AvtPosY[%d] (%d,%d) \n",pIStuts->SensorStat,
				pIStuts->SensorStat,pIStuts->AvtPosX[pIStuts->SensorStat],
				pIStuts->AvtPosY[pIStuts->SensorStat]);
*/
		}
		else if(m_curChId == video_pal)
		{
			rc.width= pIStuts->AcqRectW[pIStuts->SensorStat];
			rc.height=pIStuts->AcqRectH[pIStuts->SensorStat];
			pIStuts->unitAimX = pIStuts->AvtPosX[pIStuts->SensorStat];
			pIStuts->unitAimY = pIStuts->AvtPosY[pIStuts->SensorStat];
		}
		if(pIStuts->AvtTrkStat == eTrk_mode_sectrk || pIStuts->AvtTrkStat ==eTrk_mode_acqmove){
			pIStuts->unitAimX = pIStuts->AvtPosX[pIStuts->SensorStat];
			pIStuts->unitAimY = pIStuts->AvtPosY[pIStuts->SensorStat];
			printf("set set set   x  , y (%d , %d ) \n",pIStuts->unitAimX,pIStuts->unitAimY);
		}

		rc.x=pIStuts->unitAimX-rc.width/2;
		rc.y=pIStuts->unitAimY-rc.height/2;
		
			
		OSA_printf("%s,line:%d   rc. xy(%f,%f),wh(%f,%f)\n",__func__,__LINE__,rc.x,rc.y,rc.width,rc.height);
		dynamic_config(VP_CFG_TrkEnable, 1,&rc);
		if(pIStuts->AvtTrkStat == eTrk_mode_sectrk)
		{
			m_intervalFrame=2;
			m_rcAcq=rc;
			pIStuts->AvtTrkStat = eTrk_mode_target;
			OSA_printf("%s  line:%d		set sec track\n ",__func__,__LINE__);	
		}		
 	}

	if(msgId == MSGID_EXT_INPUT_ENMTD)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->MmtStat[0] = pInCmd->MmtStat[0];
			pIStuts->MmtStat[1] = pInCmd->MmtStat[1];
		}
		int MMTStatus = (pIStuts->MmtStat[pIStuts->SensorStat]&0x01) ;
		if(MMTStatus)
			dynamic_config(VP_CFG_MmtEnable, 1);
		else
			dynamic_config(VP_CFG_MmtEnable, 0);
		//FOR DUMP FRAME
		if(MMTStatus)
			dynamic_config(CDisplayer::DS_CFG_MMTEnable, pIStuts->SensorStat, &MMTStatus);
		else
			dynamic_config(CDisplayer::DS_CFG_MMTEnable, pIStuts->SensorStat, &MMTStatus);
	}

	if(msgId == MSGID_EXT_INPUT_ENENHAN)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->ImgEnhStat[0] = pInCmd->ImgEnhStat[0];
			pIStuts->ImgEnhStat[1] = pInCmd->ImgEnhStat[1];
		}
		int ENHStatus = (pIStuts->ImgEnhStat[pIStuts->SensorStat]&0x01) ;
		OSA_printf(" %d:%s set mtd enMask %d\n", OSA_getCurTimeInMsec(),__func__,ENHStatus);
		if(ENHStatus)
			dynamic_config(CDisplayer::DS_CFG_EnhEnable, pIStuts->SensorStat, &ENHStatus);
		else
			dynamic_config(CDisplayer::DS_CFG_EnhEnable, pIStuts->SensorStat, &ENHStatus);
	}


	if(msgId == MSGID_EXT_INPUT_AIMPOS || msgId == MSGID_EXT_INPUT_AIMSIZE)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->AvtTrkAimSize = pInCmd->AvtTrkAimSize;
			pIStuts->aimRectMoveStepX = pInCmd->aimRectMoveStepX;
			pIStuts->aimRectMoveStepY= pInCmd->aimRectMoveStepY;
		}
		
		if(pIStuts->AvtTrkStat)
		{
			UTC_RECT_float rc;
			if(msgId == MSGID_EXT_INPUT_AIMSIZE)
			{
				pIStuts->unitAimW  =  pIStuts->AimW[pIStuts->SensorStat];
				pIStuts->unitAimH	  =  pIStuts->AimH[pIStuts->SensorStat];

				rc.x	=	pIStuts->unitAimX-pIStuts->unitAimW/2;
				rc.y	=	pIStuts->unitAimY-pIStuts->unitAimH/2;
				rc.width=pIStuts->unitAimW;
				rc.height=pIStuts->unitAimH;
				//OSA_printf("***xy = (%f,%f)  WH(%f,%f)\n",rc.x,rc.y,rc.width,rc.height);
			}
			else
			{
				moveStat = true;
				//printf("----- XY(%d,%d),WH(%d,%d)\n",pIStuts->unitAimX,pIStuts->unitAimY,pIStuts->unitAimW,pIStuts->unitAimH);
				
				//printf("111W,H : (%d,%d)\n",pIStuts->unitAimW,pIStuts->unitAimH);
				rc.width=pIStuts->unitAimW;
				rc.height=pIStuts->unitAimH;
				//printf("222rc.width,rc.height : (%f,%f)\n",rc.width,rc.height);
				
				rc.x = pIStuts->unitAimX-pIStuts->unitAimW/2 + pIStuts->aimRectMoveStepX;
				rc.y = pIStuts->unitAimY-pIStuts->unitAimH/2  + pIStuts->aimRectMoveStepY;
				//printf("333rc.x,rc.y : (%d,%d)\n",rc.x,rc.y);

				pIStuts->aimRectMoveStepX = 0;
				pIStuts->aimRectMoveStepY = 0;
				
			}
			m_intervalFrame=1;
			m_rcAcq=rc;
			OSA_printf(" %d:%s refine move (%d, %d), wh(%f, %f)  aim(%d,%d) rc(%f,%f)\n", OSA_getCurTimeInMsec(), __func__,
						pIStuts->aimRectMoveStepX, pIStuts->aimRectMoveStepY, 
						rc.width, rc.height,pIStuts->unitAimX,pIStuts->unitAimY,rc.x,rc.y);
		}

		return ;
	}

	if(msgId == MSGID_EXT_INPUT_ENZOOM)
	{
		if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->ImgZoomStat[0] = pInCmd->ImgZoomStat[0];
			pIStuts->ImgZoomStat[1] = pInCmd->ImgZoomStat[1];
		}

		DS_Rect lay_rect;
		
		if(pIStuts->SensorStat==0)//tv
		{
			memset(&lay_rect, 0, sizeof(DS_Rect));
			if(pIStuts->ImgZoomStat[0] == 2)
			{
				lay_rect.w = vdisWH[0][0]/2;
				lay_rect.h = vdisWH[0][1]/2;
				lay_rect.x = vdisWH[0][0]/4;
				lay_rect.y = vdisWH[0][1]/4;
			}
			else if(pIStuts->ImgZoomStat[0] == 4)
			{
				lay_rect.w = vdisWH[0][0]/4;
				lay_rect.h = vdisWH[0][1]/4;
				lay_rect.x = vdisWH[0][0]/2 - lay_rect.w/2;
				lay_rect.y = vdisWH[0][1]/2 - lay_rect.h/2;
			}
			else if(pIStuts->ImgZoomStat[0] == 8)
			{
				lay_rect.w = vdisWH[0][0]/8;
				lay_rect.h = vdisWH[0][1]/8;
				lay_rect.x = vdisWH[0][0]/2 - lay_rect.w/2;
				lay_rect.y = vdisWH[0][1]/2 - lay_rect.h/2;
			}
			else
			{
				lay_rect.w = vdisWH[0][0];
				lay_rect.h = vdisWH[0][1];
				lay_rect.x = 0;
				lay_rect.y = 0;
			}		
			
			m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 0, &lay_rect);
			memset(&lay_rect, 0, sizeof(DS_Rect));
			
			lay_rect.w = vcapWH[0][0]/6;
			lay_rect.h = vcapWH[0][1]/6;
			lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] - lay_rect.w/2;
			lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] - lay_rect.h/2;
			
			if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 2){
				lay_rect.w =vdisWH[0][0]/12;
				lay_rect.h = vdisWH[0][1]/12;
				lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] - lay_rect.w/2;
				lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] - lay_rect.h/2;
			}
			if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 4){
				lay_rect.w = vcapWH[0][0]/24;
				lay_rect.h = vcapWH[0][1]/24;
				lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
				lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;
			}
			else if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 6){
				lay_rect.w = vcapWH[0][0]/48;
				lay_rect.h = vcapWH[0][1]/48;
				lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] -lay_rect.w/2;
				lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] -lay_rect.h/2;
			}
			else if(pIStuts->ImgZoomStat[pIStuts->SensorStat] == 8){
				lay_rect.w = vcapWH[0][0]/64;
				lay_rect.h = vcapWH[0][1]/64;
				lay_rect.x = pIStuts->AxisPosX[pIStuts->SensorStat] - lay_rect.w/2;
				lay_rect.y = pIStuts->AxisPosY[pIStuts->SensorStat] - lay_rect.h/2;
			}
			
			
			m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 1, &lay_rect);			
		}
		else
		{
			memset(&lay_rect, 0, sizeof(DS_Rect));
			memset(&lay_rect, 0, sizeof(DS_Rect));
			if(pIStuts->ImgZoomStat[0]&&(pIStuts->PicpSensorStat==0))
			{
				lay_rect.w = vcapWH[0][0]/6;
				lay_rect.h = vcapWH[0][1]/6;
				lay_rect.x = vcapWH[0][0]/2-lay_rect.w/2;
				lay_rect.y = vcapWH[0][1]/2-lay_rect.h/2;
			}
			else 
			{
				lay_rect.w = vcapWH[0][0]/3;
				lay_rect.h = vcapWH[0][1]/3;
				lay_rect.x = vcapWH[0][0]/2-lay_rect.w/2;
				lay_rect.y = vcapWH[0][1]/2-lay_rect.h/2;		
			}
			m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 1, &lay_rect);
		}

		return ;
	}
	
	if(msgId ==MSGID_EXT_INPUT_PICPCROP)
	{		
		m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 1, &rendpos[pIStuts->PicpPosStat]);
	}

	if(msgId ==MSGID_EXT_INPUT_VIDEOEN)
	{
		int status=pIStuts->unitFaultStat&0x01;
		status^=1;
		m_display.dynamic_config(CDisplayer::DS_CFG_VideodetEnable, 0, &status);
		OSA_printf("MSGID_EXT_INPUT_VIDEOEN status0=%d\n",status);
		 status=(pIStuts->unitFaultStat>1)&0x01;
		 status^=1;
		m_display.dynamic_config(CDisplayer::DS_CFG_VideodetEnable, 1, &status);
		OSA_printf("MSGID_EXT_INPUT_VIDEOEN status1=%d\n",status);
	}
	if(msgId ==MSGID_EXT_INPUT_MMTSHOW)
	{
		int itmp=0;
		int mmtchid=0;
		DS_Rect lay_rect;
		if(pIStuts->ImgMmtshow[pIStuts->SensorStat])
		{
			int chid=pIStuts->SensorStat;
			itmp = chid;
			mmtchid=2;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=3;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=4;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=5;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			//chid++;
			itmp=chid;
			mmtchid=6;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			lay_rect.w = vdisWH[0][0]/3*2;
			lay_rect.h = vdisWH[0][1]/3*2;
			lay_rect.x = 0;
			lay_rect.y = vdisWH[0][1]/3;
			m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 0, &lay_rect);
			//m_display.dynamic_config(CDisplayer::DS_CFG_Rendercount, 7, NULL);

			//m_display.m_renderCount
		}
		else
		{
			itmp = 8;
			mmtchid=2;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			itmp=8;
			mmtchid=3;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			itmp=8;
			mmtchid=4;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			itmp=8;
			mmtchid=5;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			itmp=8;
			mmtchid=6;
			dynamic_config(VP_CFG_SubPicpChId, itmp, &mmtchid);
			lay_rect.w = vdisWH[0][0];
			lay_rect.h = vdisWH[0][1];
			lay_rect.x = 0;
			lay_rect.y = 0;
			m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 0, &lay_rect);
			//m_display.dynamic_config(CDisplayer::DS_CFG_Rendercount, 2, NULL);
		}
		
	#if 1	
	lay_rect.w = 30;
	lay_rect.h = 30;
	lay_rect.x = 0;
	lay_rect.y = 0;
	m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 2, &lay_rect);
	m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 3, &lay_rect);
	m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 4, &lay_rect);
	m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 5, &lay_rect);
	m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, 6, &lay_rect);
	lay_rect.w = vdisWH[0][0]/3;
	lay_rect.h = vdisWH[0][1]/3;
	lay_rect.x = 0;
	lay_rect.y = 0;
	m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 2, &lay_rect);
	lay_rect.w = vdisWH[0][0]/3;
	lay_rect.h = vdisWH[0][1]/3;
	lay_rect.x = vdisWH[0][0]/3;
	lay_rect.y = 0;
	m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 3, &lay_rect);
	lay_rect.w = vdisWH[0][0]/3;
	lay_rect.h = vdisWH[0][1]/3;
	lay_rect.x = vdisWH[0][0]/3;
	lay_rect.x=lay_rect.x*2;
	lay_rect.y = 0;
	m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 4, &lay_rect);
	lay_rect.w = vdisWH[0][0]/3;
	lay_rect.h = vdisWH[0][1]/3;
	lay_rect.x = vdisWH[0][0]/3;
	lay_rect.x=lay_rect.x*2;
	lay_rect.y = vdisWH[0][1]/3;
	m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 5, &lay_rect);
	lay_rect.w = vdisWH[0][0]/3;
	lay_rect.h = vdisWH[0][1]/3;
	lay_rect.x = vdisWH[0][0]/3;
	lay_rect.x=lay_rect.x*2;
	lay_rect.y = vdisWH[0][1]/3;
	lay_rect.y=lay_rect.y*2;
	m_display.dynamic_config(CDisplayer::DS_CFG_RenderPosRect, 6, &lay_rect);
	#endif

	}

	if(msgId ==MSGID_EXT_INPUT_MMTSHOWUPDATE)
	{	
		for(int i=0;i<5;i++)
		{
			if(Mmtpos[i].valid)
			{
				//m_display.m_renders[i+2].videodect=1;
				m_display.dynamic_config(CDisplayer::DS_CFG_VideodetEnable, i+2, &Mmtpos[i].valid);
				m_display.dynamic_config(CDisplayer::DS_CFG_CropRect, i+2, &Mmtpos[i]);
			}
			else
			{
				//m_display.m_renders[i+2].videodect=0;
				//OSA_printf("the id=%d valid =%d\n",i+2,Mmtpos[i].valid);
				m_display.dynamic_config(CDisplayer::DS_CFG_VideodetEnable, i+2, &Mmtpos[i].valid);	
			}
		}
	}
#if __MOVE_DETECT__
    if(msgId == MSGID_EXT_MVDETECT)
    {
        int Mtdstatus = (pIStuts->MtdState[pIStuts->SensorStat]&0x01) ;
        if(Mtdstatus)
        {  
			m_pMovDetector->mvOpen(pIStuts->SensorStat);
			dynamic_config(VP_CFG_MvDetect, 1,NULL);
			chooseDetect = 10;
			pIStuts->MtdDetectStat = m_bMoveDetect;  
        }
        else
        {
			dynamic_config(VP_CFG_MvDetect, 0,NULL);
			m_pMovDetector->mvClose(pIStuts->SensorStat);
			pIStuts->MtdDetectStat = m_bMoveDetect;
        }
        OSA_printf("====== MTD cmdstat %d algstat %d \n", pIStuts->MtdState[pIStuts->SensorStat], pIStuts->MtdDetectStat);
    }
	if(msgId == MSGID_EXT_MVDETECTSELECT)
	{
		int MtdSelect = (pIStuts->MtdSelect[pIStuts->SensorStat]);
		if(eMTD_Next == MtdSelect)
		{
			forwardflag = true;
		}
		else if(eMTD_Prev == MtdSelect)
		{
			backflag = true;
		}
		else if(eMTD_Select == MtdSelect)
		{

		}
	}
#endif
	
	if(msgId == MSGID_EXT_INPUT_SCENETRK)
	{
	 	if(prm != NULL)
		{
			pInCmd = (CMD_EXT *)prm;
			pIStuts->SceneAvtTrkStat = pInCmd->SceneAvtTrkStat;
		}		
		if (pIStuts->SceneAvtTrkStat == eTrk_mode_acq)
			dynamic_config(VP_CFG_SceneTrkEnable, 0);
		else if(pIStuts->SceneAvtTrkStat == eTrk_mode_target)
			dynamic_config(VP_CFG_SceneTrkEnable, 1);

	}

	if( msgId == MSGID_EXT_PATTERNDETECT )
	{
		if (PatternDetect == eTrk_mode_acq)
			dynamic_config(VP_CFG_PatterDetectEnable, 0);
		else if(PatternDetect == eTrk_mode_target)
			dynamic_config(VP_CFG_PatterDetectEnable, 1);
	}

	if( msgId == MSGID_EXT_INPUT_GSTCTRL )
	{
#ifdef ENCTRANS_ON
		if(gSYS_Enc.srcType == 0)// ENCTRANS_SCREEN
		{
			EncTrans_screen_rtpout(gSYS_Enc.rtpEn, gSYS_Enc.rtpIpaddr, gSYS_Enc.rtpPort);
		}
		else if(gSYS_Enc.srcType == 1)	// ENCTRANS_APPCAP
		{
			for(int i=0; i<MAX_CHAN; i++)
			{
				if(gSYS_Enc.vinEncId[i] >= 0)
					EncTrans_appcap_rtpout(gSYS_Enc.vinEncId[i], gSYS_Enc.rtpEn, gSYS_Enc.rtpIpaddr, gSYS_Enc.rtpPort+gSYS_Enc.vinEncId[i]);
			}
		}
#endif
	}
}


/////////////////////////////////////////////////////
//int majormmtid=0;

 int  CProcess::MSGAPI_initial()
{
   MSGDRIV_Handle handle=&g_MsgDrvObj;
    assert(handle != NULL);
    memset(handle->msgTab, 0, sizeof(MSGTAB_Class) * MAX_MSG_NUM);
//MSGID_EXT_INPUT_MTD_SELECT
    MSGDRIV_attachMsgFun(handle,    MSGID_SYS_INIT,           			MSGAPI_init_device,       		0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_SENSOR,           	MSGAPI_inputsensor,       		0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_PICPCROP,      		MSGAPI_croppicp,       		    0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_TRACK,          	MSGAPI_inputtrack,     		    0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_ENMTD,              MSGAPI_inpumtd,       		    0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_MTD_SELECT,     	MSGAPI_inpumtdSelect,    		0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_AIMPOS,          	MSGAPI_setAimRefine,    		0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_AIMSIZE,          	MSGAPI_setAimSize,    		    0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_ENENHAN,           	MSGAPI_inpuenhance,       	    0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_ENBDT,           	MSGAPI_inputbdt,         		0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_ENZOOM,           	MSGAPI_inputzoom,               0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_ENFREZZ,           	MSGAPI_inputfrezz,              0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_MTD_SELECT,      	MSGAPI_inputmmtselect,          0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_AXISPOS,     	  	MSGAPI_inputpositon,            0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_COAST,             	MSGAPI_inputcoast,              0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_FOVSELECT,          MSGAPI_inputfovselect,          0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_FOVSTAT,            MSGAPI_inputfovchange,          0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_SEARCHMOD,          MSGAPI_inputsearchmod,          0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_VIDEOEN,            MSGAPI_inputvideotect,          0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_MMTSHOW,            MSGAPI_mmtshow,                 0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_FOVCMD,             MSGAPI_FOVcmd,                 	0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_CFGSAVE,            MSGAPI_SaveCfgcmd,              0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_MVDETECT,             	MSGAPI_setMtdState,             0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_MVDETECTSELECT,           MSGAPI_setMtdSelect,            0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_PATTERNDETECT,            MSGAPI_setMtdState,             0);
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_UPDATE_ALG,             	MSGAPI_update_alg,              0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_UPDATE_OSD,             	MSGAPI_update_osd,              0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_UPDATE_CAMERA,            MSGAPI_update_camera,           0);	

    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_MVDETECTAERA,         MSGAPI_handle_mvAera,        0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_MVDETECTUPDATE,         MSGAPI_handle_mvUpdate,        0);	
    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_SCENETRK,         MSGAPI_INPUT_SCENETRK,        0);	

    MSGDRIV_attachMsgFun(handle,    MSGID_EXT_INPUT_GSTCTRL,         MSGAPI_input_gstctrl,        0);	
    return 0;
}


void CProcess::MSGAPI_init_device(long lParam )
{
	sThis->msgdriv_event(MSGID_SYS_INIT,NULL);
}

  void CProcess::MSGAPI_inputsensor(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;	
	sThis->msgdriv_event(MSGID_EXT_INPUT_SENSOR,NULL);
}

void CProcess::MSGAPI_picp(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
		if(pIStuts->PicpSensorStat == 0xFF)
			pIStuts->PicpSensorStat = (pIStuts->SensorStat + 1)%eSen_Max;
		else
			pIStuts->PicpSensorStat = 0xFF;
	
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENPICP,NULL);
}


void CProcess::MSGAPI_croppicp(long lParam )
{
	//sThis->msgdriv_event(MSGID_EXT_INPUT_PICPCROP,NULL);
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENPICP,NULL);
}

void CProcess::MSGAPI_inputtrack(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	sThis->msgdriv_event(MSGID_EXT_INPUT_TRACK,NULL);
}


void CProcess::MSGAPI_inpumtd(long lParam )
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENMTD,NULL);
}

void CProcess::MSGAPI_inpumtdSelect(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	int i;
	if(pIStuts->MMTTempStat==3)
	{
		for(i=0;i<MAX_TARGET_NUMBER;i++)
		{
			if(sThis->m_mtd[pIStuts->SensorStat]->tg[majormmtid].valid==1)
			{
				//majormmtid++;
				majormmtid=(majormmtid+1)%MAX_TARGET_NUMBER;
			}
		}	
	}
	else if(pIStuts->MMTTempStat==4)
	{
		for(i=0;i<MAX_TARGET_NUMBER;i++)
		{
			if(sThis->m_mtd[pIStuts->SensorStat]->tg[majormmtid].valid==1)
			{
				//majormmtid++;
				if(majormmtid>0)
					majormmtid=(majormmtid-1);
				else
				{
					majormmtid=MAX_TARGET_NUMBER-1;
				}
			}
		}
	}
	OSA_printf("MSGAPI_inpumtdSelect\n");
}


void CProcess::MSGAPI_inpuenhance(long lParam )
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENENHAN,NULL);
}

void CProcess::MSGAPI_setMtdState(long lParam )
{
	sThis->msgdriv_event(MSGID_EXT_MVDETECT,NULL);
}

void CProcess::MSGAPI_setPatternDetect(long lParam )
{
	sThis->msgdriv_event(MSGID_EXT_PATTERNDETECT,NULL);
}

void CProcess::MSGAPI_setMtdSelect(long lParam )
{

	sThis->msgdriv_event(MSGID_EXT_MVDETECTSELECT,NULL);
}
	
void CProcess::MSGAPI_setAimRefine(long lParam)
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_AIMPOS,NULL);
}


void CProcess::MSGAPI_setAimSize(long lParam)
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_AIMSIZE,NULL);
}

void CProcess::MSGAPI_inputbdt(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	if(pIStuts->TvCollimation!=1)
		pIStuts->ImgBlobDetect[pIStuts->SensorStat] = eImgAlg_Disable;
	else
		pIStuts->ImgBlobDetect[pIStuts->SensorStat] = eImgAlg_Enable;
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENBDT,NULL);	
	OSA_printf("fun=%s line=%d \n",__func__,__LINE__);
}


void CProcess::MSGAPI_inputzoom(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	sThis->msgdriv_event(MSGID_EXT_INPUT_ENZOOM,NULL);
}


void CProcess::MSGAPI_inputfrezz(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;	
	if( pIStuts->FrCollimation==1)
	{
		pIStuts->PicpSensorStat=0;//tv picp sensor
		sThis->msgdriv_event(MSGID_EXT_INPUT_ENPICP, NULL);
		//dong jie chuang kou
		pIStuts->ImgFrezzStat[pIStuts->SensorStat] = eImgAlg_Enable;
		sThis->msgdriv_event(MSGID_EXT_INPUT_ENFREZZ,NULL);
	}
	else
	{	
		if((pIStuts->PicpSensorStatpri!=0))//tui picp the sensor is tv
		{
			pIStuts->PicpSensorStatpri=pIStuts->PicpSensorStat=2;//tui chu picp
			sThis->msgdriv_event(MSGID_EXT_INPUT_ENPICP, NULL);
			OSA_printf("MSGAPI_inputfrezz*****************************************disable \n");
		}
		else
		{
			pIStuts->PicpSensorStat=0;
		}
		//tui chu dong jie chuang kou
		pIStuts->ImgFrezzStat[pIStuts->SensorStat] = eImgAlg_Disable;
		sThis->msgdriv_event(MSGID_EXT_INPUT_ENFREZZ,NULL);
		
		OSA_printf("the*****************************************disable PicpSensorStatpri=%d\n",pIStuts->PicpSensorStatpri);
	}

			
	
	OSA_printf("%s\n",__func__);
}


void CProcess::MSGAPI_inputmmtselect(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	if(pIStuts->MmtSelect[pIStuts->SensorStat]  ==eMMT_Next)
		majormmtid=(majormmtid+1)%MAX_TARGET_NUMBER;
	else if(pIStuts->MmtSelect[pIStuts->SensorStat]  ==  eMMT_Prev)
	{
		majormmtid=(majormmtid-1+MAX_TARGET_NUMBER)%MAX_TARGET_NUMBER;
	}
	OSA_printf("%s\n",__func__);
}



void CProcess::MSGAPI_inputpositon(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;

	if((pIStuts->AxisPosX[pIStuts->SensorStat]>=50)&&(pIStuts->AxisPosX[pIStuts->SensorStat]<=vcapWH[pIStuts->SensorStat][0]-50))
	{
		if(pIStuts->axisMoveStepX != 0)
		{
			pIStuts->AxisPosX[pIStuts->SensorStat] += pIStuts->axisMoveStepX;
			pIStuts->axisMoveStepX = 0;
		}	
		pIStuts->unitAimX = pIStuts->AxisPosX[pIStuts->SensorStat];
	}
	if((pIStuts->AxisPosY[pIStuts->SensorStat]>=50)&&(pIStuts->AxisPosY[pIStuts->SensorStat]<=vcapWH[pIStuts->SensorStat][1]-50))
	{
		if(pIStuts->axisMoveStepY != 0)
		{
			pIStuts->AxisPosY[pIStuts->SensorStat] += pIStuts->axisMoveStepY;
			pIStuts->axisMoveStepY = 0;
		}
		pIStuts->unitAimY = pIStuts->AxisPosY[pIStuts->SensorStat];
	}
	
	OSA_printf("%s   THE=unitAimX=%d unitAxisY=%d\n",__func__,pIStuts->opticAxisPosX[pIStuts->SensorStat],pIStuts->opticAxisPosY[pIStuts->SensorStat]);
}

void CProcess::MSGAPI_inputcoast(long lParam )
{

	sThis->msgdriv_event(MSGID_EXT_INPUT_COAST,NULL);
	
	//printf("%s\n",__func__);
}

void CProcess::MSGAPI_inputfovselect(long lParam )
{

	CMD_EXT *pIStuts = sThis->extInCtrl;

	if(pIStuts->changeSensorFlag == 0)
	{
		//OSA_printf("FovStat = %d SensorStat=%d\n",pIStuts->FovStat,pIStuts->SensorStat);
		if(video_pal == pIStuts->SensorStat)
		{
		#if __TRACK__
			if(pIStuts->FovStat == 1)
				sThis->Track_fovreacq( 2400,pIStuts->SensorStat,0);
			else if(pIStuts->FovStat == 3)
				sThis->Track_fovreacq( 330,pIStuts->SensorStat,0);
			else if(pIStuts->FovStat == 4)	
				sThis->Track_fovreacq( 110,pIStuts->SensorStat,0);					
			else if(pIStuts->FovStat == 5)
				sThis->Track_fovreacq( 55,pIStuts->SensorStat,0);
			
		#endif
		}
		else if((video_gaoqing0 == pIStuts->SensorStat)||(video_gaoqing == pIStuts->SensorStat)||(video_gaoqing2 == pIStuts->SensorStat)||(video_gaoqing3 == pIStuts->SensorStat)){
		#if __TRACK__
			if(pIStuts->FovStat == 1)
				sThis->Track_fovreacq( 4000,pIStuts->SensorStat,0);
			else if(pIStuts->FovStat == 4)
				sThis->Track_fovreacq( 120,pIStuts->SensorStat,0);
			else if(pIStuts->FovStat == 5)
				sThis->Track_fovreacq( 60,pIStuts->SensorStat,0);
		#endif
		}

		//OSA_printf("fovselectXY(%f,%f),WH(%f,%f)\n",sThis->trackinfo_obj->reAcqRect.x,sThis->trackinfo_obj->reAcqRect.y,sThis->trackinfo_obj->reAcqRect.width,sThis->trackinfo_obj->reAcqRect.height);
		#if __TRACK__
		if(pIStuts->AvtTrkStat){	
			sThis->Track_reacq(sThis->trackinfo_obj->reAcqRect,2);
		}
		#endif
	}
}

void CProcess::MSGAPI_inputfovchange(long lParam )
{

	CMD_EXT *pIStuts = sThis->extInCtrl;

	//OSA_printf("%s:unitFovAngle = %f\n",__func__,pIStuts->unitFovAngle[pIStuts->SensorStat]);
	#if __TRACK__
	sThis->Track_fovreacq( pIStuts->unitFovAngle[pIStuts->SensorStat],pIStuts->SensorStat,0);
	#endif
}


void CProcess::MSGAPI_inputsearchmod(long lParam )
{
}


 void CProcess::MSGAPI_inputvideotect(long lParam )
{
	OSA_printf("MSGAPI_inputvideotect*******************\n");
	sThis->msgdriv_event(MSGID_EXT_INPUT_VIDEOEN,NULL);
}

  void CProcess::MSGAPI_mmtshow(long lParam )
{
	OSA_printf("MSGAPI_mmtshow\n");
}
void CProcess::MSGAPI_FOVcmd(long lParam )
{
	CMD_EXT *pIStuts = sThis->extInCtrl;
	if((pIStuts->FovCtrl==5)&&(pIStuts->SensorStat==0))
		sThis->tvzoomStat=1;
	else
		sThis->tvzoomStat=0;
}
void CProcess::MSGAPI_SaveCfgcmd(long lParam )
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_CFGSAVE,NULL);
}	

void CProcess::MSGAPI_update_osd(long lParam)
{
	plat->update_param_osd();
}

void CProcess::update_param_osd()
{
	CMD_EXT *pIStuts = extInCtrl;

	m_display.disptimeEnable = (gSYS_Osd.osdDrawShow)?1:0;
	m_display.m_bOsd = (gSYS_Osd.osdDrawShow)?true:false;
	m_display.m_userOsd = (gSYS_Osd.osdUserShow)?true:false;
	m_display.m_crossOsd = (gSYS_Osd.m_crossOsd)?true:false;
	m_display.m_boxOsd = (gSYS_Osd.m_boxOsd)?true:false;
	m_display.m_chidIDOsd = (gSYS_Osd.m_chidIDOsd)?true:false;
	m_display.m_chidNameOsd = (gSYS_Osd.m_chidNameOsd)?true:false;

	pIStuts->crossAxisWidth[video_pal] = 40;
	pIStuts->crossAxisHeight[video_pal] = 40;
	pIStuts->crossAxisWidth[video_gaoqing0] = 60;
	pIStuts->crossAxisHeight[video_gaoqing0] = 60;
	pIStuts->crossAxisWidth[video_gaoqing] = 60;
	pIStuts->crossAxisHeight[video_gaoqing] = 60;
	pIStuts->crossAxisWidth[video_gaoqing2] = 60;
	pIStuts->crossAxisHeight[video_gaoqing2] = 60;
	pIStuts->crossAxisWidth[video_gaoqing3] = 60;
	pIStuts->crossAxisHeight[video_gaoqing3] = 60;
	pIStuts->picpCrossAxisWidth = 40;
	pIStuts->picpCrossAxisHeight = 40;

	return;
}

void CProcess::MSGAPI_update_alg(long lParam)
{
	plat->update_param_alg();
}
#define UTCPARM 0
void CProcess::update_param_alg()
{
	UTC_DYN_PARAM dynamicParam;
	if(gCFG_Trk.occlusion_thred > 0.0001)
		dynamicParam.occlusion_thred = gCFG_Trk.occlusion_thred;
	else
		dynamicParam.occlusion_thred = 0.28;

	//dynamicParam.occlusion_thred = 0.30;
	
	if(gCFG_Trk.retry_acq_thred> 0.0001)
		dynamicParam.retry_acq_thred = gCFG_Trk.retry_acq_thred;
	else
		dynamicParam.retry_acq_thred = 0.38;

	//dynamicParam.retry_acq_thred = 0.40;
	
	float up_factor;
	if(gCFG_Trk.up_factor > 0.0001)
		up_factor = gCFG_Trk.up_factor;
	else
		up_factor = 0.0125;
	//up_factor = 0.025;

	TRK_SECH_RESTRAINT resTraint;
	if(gCFG_Trk.res_distance > 0)
		resTraint.res_distance = gCFG_Trk.res_distance;
	else
		resTraint.res_distance = 80;
	if(gCFG_Trk.res_area> 0)
		resTraint.res_area = gCFG_Trk.res_area;
	else
		resTraint.res_area = 5000;
	//printf("UtcSetRestraint: distance=%d area=%d \n", resTraint.res_distance, resTraint.res_area);

	int gapframe;
	if(gCFG_Trk.gapframe> 0)
		gapframe = gCFG_Trk.gapframe;
	else
		gapframe = 10;

   	bool enhEnable;
	enhEnable = gCFG_Trk.enhEnable;	

	float cliplimit;
	if(gCFG_Trk.cliplimit> 0)
		cliplimit = gCFG_Trk.cliplimit;
	else
		cliplimit = 4.0;

	bool dictEnable;
	dictEnable = gCFG_Trk.dictEnable;

	int moveX,moveY;
	if(gCFG_Trk.moveX > 0)
		moveX = gCFG_Trk.moveX;
	else
		moveX = 20;

	if(gCFG_Trk.moveY>0)
		moveY = gCFG_Trk.moveY;
	else
		moveY = 10;

	int moveX2,moveY2;
	if(gCFG_Trk.moveX2 > 0)
		moveX2 = gCFG_Trk.moveX2;
	else
		moveX2 = 30;

	if(gCFG_Trk.moveY2 > 0)
		moveY2 = gCFG_Trk.moveY2;
	else
		moveY2 = 20;

	int segPixelX,segPixelY;

	if(gCFG_Trk.segPixelX > 0)
		segPixelX = gCFG_Trk.segPixelX;
	else
		segPixelX = 600;
	if(gCFG_Trk.segPixelY > 0)
		segPixelY = gCFG_Trk.segPixelY;
	else
		segPixelY = 450;

	if(gCFG_Trk.ScalerLarge > 0)
		ScalerLarge = gCFG_Trk.ScalerLarge;
	else
		ScalerLarge = 256;
	if(gCFG_Trk.ScalerMid > 0)
		ScalerMid = gCFG_Trk.ScalerMid;
	else
		ScalerMid = 128;
	if(gCFG_Trk.ScalerSmall >0)
		ScalerSmall = gCFG_Trk.ScalerSmall;
	else
		ScalerSmall = 64;

	int Scatter;
	if(gCFG_Trk.Scatter > 0)
		Scatter = gCFG_Trk.Scatter;
	else
		Scatter = 10;

	float ratio;
	if(gCFG_Trk.ratio >0.1)
		ratio = gCFG_Trk.ratio;
	else
		ratio = 1.0;

	bool FilterEnable;
	FilterEnable = gCFG_Trk.FilterEnable;

	bool BigSecEnable;
	BigSecEnable = gCFG_Trk.BigSecEnable;

	int SalientThred;
	if(gCFG_Trk.SalientThred > 0)
		SalientThred = gCFG_Trk.SalientThred;
	else
		SalientThred = 40;
	bool ScalerEnable;
	ScalerEnable = gCFG_Trk.ScalerEnable;

	bool DynamicRatioEnable;
	DynamicRatioEnable = ScalerEnable = gCFG_Trk.DynamicRatioEnable;

	UTC_SIZE acqSize;
	if(gCFG_Trk.acqSize_width > 0)	
		acqSize.width = gCFG_Trk.acqSize_width;
	else
		acqSize.width = 8;
	if(gCFG_Trk.acqSize_height > 0)
		acqSize.height = gCFG_Trk.acqSize_height;
	else
		acqSize.height = 8;
	
	if((int)gCFG_Trk.TrkAim43_Enable == 1)
		TrkAim43 = true;
	else
		TrkAim43 = false;

	bool SceneMVEnable;
	SceneMVEnable = gCFG_Trk.SceneMVEnable;

	bool BackTrackEnable;
	BackTrackEnable = gCFG_Trk.BackTrackEnable;

	bool  bAveTrkPos;
	bAveTrkPos = gCFG_Trk.bAveTrkPos;

	float fTau;
	if(gCFG_Trk.fTau > 0.01)
		fTau = gCFG_Trk.fTau;
	else
		fTau = 0.5;

	int  buildFrms;
	if(gCFG_Trk.buildFrms > 0)
		buildFrms = gCFG_Trk.buildFrms;
	else
		buildFrms = 500;
	
	int  LostFrmThred;
	if(gCFG_Trk.LostFrmThred > 0)
		LostFrmThred = gCFG_Trk.LostFrmThred;
	else
		LostFrmThred = 30;

	float  histMvThred;
	if(gCFG_Trk.histMvThred > 0.01)
		histMvThred = gCFG_Trk.histMvThred;
	else
		histMvThred = 1.0;

	int  detectFrms;
	if(gCFG_Trk.detectFrms > 0)
		detectFrms = gCFG_Trk.detectFrms;
	else
		detectFrms = 30;

	int  stillFrms;
	if(gCFG_Trk.stillFrms > 0)
		stillFrms = gCFG_Trk.stillFrms;
	else
		stillFrms = 50;

	float  stillThred;
	if(gCFG_Trk.stillThred> 0.01)
		stillThred = gCFG_Trk.stillThred;
	else
		stillThred = 0.1;


	bool  bKalmanFilter;
	bKalmanFilter = gCFG_Trk.bKalmanFilter;

	float xMVThred, yMVThred;
	if(gCFG_Trk.xMVThred> 0.01)
		xMVThred = gCFG_Trk.xMVThred;
	else
		xMVThred = 3.0;
	if(gCFG_Trk.yMVThred> 0.01)
		yMVThred = gCFG_Trk.yMVThred;
	else
		yMVThred = 2.0;

	float xStillThred, yStillThred;
	if(gCFG_Trk.xStillThred> 0.01)
		xStillThred = gCFG_Trk.xStillThred;
	else
		xStillThred = 0.5;
	if(gCFG_Trk.yStillThred> 0.01)
		yStillThred= gCFG_Trk.yStillThred;
	else
		yStillThred = 0.3;

	float slopeThred;
	if(gCFG_Trk.slopeThred> 0.01)
		slopeThred = gCFG_Trk.slopeThred;
	else
		slopeThred = 0.08;

	float kalmanHistThred;
	if(gCFG_Trk.kalmanHistThred> 0.01)
		kalmanHistThred = gCFG_Trk.kalmanHistThred;
	else
		kalmanHistThred = 2.5;

	float kalmanCoefQ, kalmanCoefR;
	if(gCFG_Trk.kalmanCoefQ> 0.000001)
		kalmanCoefQ = gCFG_Trk.kalmanCoefQ;
	else
		kalmanCoefQ = 0.00001;
	if(gCFG_Trk.kalmanCoefR> 0.000001)
		kalmanCoefR = gCFG_Trk.kalmanCoefR;
	else
		kalmanCoefR = 0.0025;

	bool  bSceneMVRecord;
	bSceneMVRecord = 0;//gConfig_Alg_param.SceneMVEnable;
	
	if(bSceneMVRecord == true)
		wFileFlag = true;
	
#if __TRACK__
	UtcSetPLT_BS(m_track, tPLT_WRK, BoreSight_Mid);
#endif


	//enh
	if(gCFG_Enh.Enhmod_0 > 4)
		m_display.enhancemod = gCFG_Enh.Enhmod_0;
	else
		m_display.enhancemod = 1;
	
	if((gCFG_Enh.Enhparm_1>0.0)&&(gCFG_Enh.Enhparm_1<5.0))
		m_display.enhanceparam=gCFG_Enh.Enhparm_1;
	else
		m_display.enhanceparam=3.5;

	//mmt
	if((gCFG_Mmt.Mmtdparm_2<0) || (gCFG_Mmt.Mmtdparm_2>15))
		DetectGapparm = 10;
	else
		DetectGapparm = 3;
	
#if __MMT__
	m_MMTDObj.SetSRDetectGap(DetectGapparm);
#endif

	if(gCFG_Mmt.Mmtdparm_3 > 0)
		MinArea = gCFG_Mmt.Mmtdparm_3;
	else
		MinArea = 80;
	if(gCFG_Mmt.Mmtdparm_4 > 0)
		MaxArea = gCFG_Mmt.Mmtdparm_4;
	else
		MaxArea = 3600;

#if __MMT__
	m_MMTDObj.SetConRegMinMaxArea(MinArea, MaxArea);
#endif

	if(gCFG_Mmt.Mmtdparm_5 > 0)
		stillPixel = gCFG_Mmt.Mmtdparm_5;
	else
		stillPixel = 6;
	if(gCFG_Mmt.Mmtdparm_6 > 0)
		movePixel = gCFG_Mmt.Mmtdparm_6;
	else
		movePixel = 16;

#if __MMT__
	m_MMTDObj.SetMoveThred(stillPixel, movePixel);
#endif

	if(gCFG_Mmt.Mmtdparm_7 > 0.001)
		lapScaler = gCFG_Mmt.Mmtdparm_7;
	else
		lapScaler = 1.25;

#if __MMT__
	m_MMTDObj.SetLapScaler(lapScaler);
#endif

	if(gCFG_Mmt.Mmtdparm_8 > 0)
		lumThred = gCFG_Mmt.Mmtdparm_8;
	else
		lumThred = 50;

#if __MMT__
	m_MMTDObj.SetSRLumThred(lumThred);
#endif

#if __TRACK__
	UtcSetDynParam(m_track, dynamicParam);
	UtcSetUpFactor(m_track, up_factor);
	UtcSetBlurFilter(m_track,FilterEnable);
	UtcSetBigSearch(m_track, BigSecEnable);
#endif

#if UTCPARM

	UtcSetDynParam(m_track, dynamicParam);
	UtcSetUpFactor(m_track, up_factor);
	UtcSetRestraint(m_track, resTraint);
	UtcSetIntervalFrame(m_track, gapframe);
	UtcSetEnhance(m_track, enhEnable);
	UtcSetEnhfClip(m_track, cliplimit);	
	UtcSetPredict(m_track, dictEnable);
	UtcSetMvPixel(m_track,moveX,moveY);
	UtcSetMvPixel2(m_track,moveX2,moveY2);
	UtcSetSegPixelThred(m_track,segPixelX,segPixelY);
	UtcSetSalientScaler(m_track, ScalerLarge, ScalerMid, ScalerSmall);
	UtcSetSalientScatter(m_track, Scatter);
	UtcSetSRAcqRatio(m_track, ratio);
	UtcSetBlurFilter(m_track,FilterEnable);
	UtcSetBigSearch(m_track, BigSecEnable);
	UtcSetSalientThred(m_track,SalientThred);
	UtcSetMultScaler(m_track, ScalerEnable);
	UtcSetDynamicRatio(m_track, DynamicRatioEnable);
	UtcSetSRMinAcqSize(m_track,acqSize);
	UtcSetSceneMV(m_track, SceneMVEnable);
	UtcSetBackTrack(m_track, BackTrackEnable);
	UtcSetAveTrkPos(m_track, bAveTrkPos);
	UtcSetDetectftau(m_track, fTau);
	UtcSetDetectBuildFrms(m_track, buildFrms);
	UtcSetLostFrmThred(m_track, LostFrmThred);
	UtcSetHistMVThred(m_track, histMvThred);
	UtcSetDetectFrmsThred(m_track, detectFrms);
	UtcSetStillFrmsThred(m_track, stillFrms);
	UtcSetStillPixThred(m_track, stillThred);
	UtcSetKalmanFilter(m_track, bKalmanFilter);
	UtcSetKFMVThred(m_track, xMVThred, yMVThred);
	UtcSetKFStillThred(m_track, xStillThred, yStillThred);
	UtcSetKFSlopeThred(m_track, slopeThred);
	UtcSetKFHistThred(m_track, kalmanHistThred);
	UtcSetKFCoefQR(m_track, kalmanCoefQ, kalmanCoefR);
	UtcSetSceneMVRecord(m_track, bSceneMVRecord);
	UtcSetRoiMaxWidth(m_track, 400);

#endif
	return ;
}

void CProcess::MSGAPI_update_camera(long lParam)
{
}


float  CProcess::PiexltoWindowsyf(float y,int channel)
{
	 float ret=0;
	 ret= (y*1.0/vcapWH[channel][1]*vdisWH[channel][1]);

	  if(ret<0)
 	{
		ret=0;
 	}
	 else if(ret>=vdisWH[channel][1])
 	{
		ret=vdisWH[channel][1];
 	}
	
	return  ret;
}

void CProcess::MSGAPI_handle_mvAera(long lParam)
{
	if(sThis->m_pMovDetector == NULL)
		return ;
	
	std::vector<cv::Point> polyWarnRoi ;
	polyWarnRoi.resize(4);
	cv::Point tmp;
	int cx,cy,w,h,ich;

	ich = sThis->extInCtrl->SensorStat;
	cx = gCFG_Mtd.detectArea_X;
	cy = gCFG_Mtd.detectArea_Y;
	w = gCFG_Mtd.detectArea_wide;
	h = gCFG_Mtd.detectArea_high;

	tmp.x = cx - w/2;
	tmp.y = cy - h/2;
	if(tmp.x < 0)
		tmp.x = 0;
	if(tmp.y < 0)
		tmp.y = 0;
	polyWarnRoi[0]= cv::Point(tmp.x,tmp.y);
	sThis->polWarnRect[ich][0].x = tmp.x ;
	sThis->polWarnRect[ich][0].y = tmp.y ;

	tmp.x = cx + w/2;
	tmp.y = cy - h/2;
	if(tmp.x > vcapWH[ich][0])
		tmp.x = vcapWH[ich][0];
	if(tmp.y < 0)
		tmp.y = 0;
	polyWarnRoi[1]= cv::Point(tmp.x,tmp.y);
	sThis->polWarnRect[ich][1].x = tmp.x ;
	sThis->polWarnRect[ich][1].y = tmp.y ;


	tmp.x = cx + w/2;
	tmp.y = cy + h/2;
	if(tmp.x > vcapWH[ich][0])
		tmp.x = vcapWH[ich][0];
	if(tmp.y > vcapWH[ich][1])
		tmp.y = vcapWH[ich][1];
	polyWarnRoi[2]= cv::Point(tmp.x,tmp.y);
	sThis->polWarnRect[ich][2].x = tmp.x ;
	sThis->polWarnRect[ich][2].y = tmp.y ;

	tmp.x = cx - w/2;
	tmp.y = cy + h/2;
	if(tmp.x < 0 )
		tmp.x = 0;
	if(tmp.y > vcapWH[ich][1])
		tmp.y = vcapWH[ich][1];	
	polyWarnRoi[3]= cv::Point(tmp.x,tmp.y);
	sThis->polWarnRect[ich][3].x = tmp.x ;
	sThis->polWarnRect[ich][3].y = tmp.y ;

	sThis->polwarn_count[ich] = 4 ; 
	/*OSA_printf(" [%d] MSGAPI_handle_mvAera update ich%d (%d, %d)-(%d, %d)-(%d, %d)-(%d, %d) \n",
		OSA_getCurTimeInMsec(), ich, 
		sThis->polWarnRect[ich][0].x, sThis->polWarnRect[ich][0].y,
		sThis->polWarnRect[ich][1].x, sThis->polWarnRect[ich][1].y,
		sThis->polWarnRect[ich][2].x, sThis->polWarnRect[ich][2].y,
		sThis->polWarnRect[ich][3].x, sThis->polWarnRect[ich][3].y);*/

	pThis->m_pMovDetector->setWarningRoi( polyWarnRoi, ich );
}

void CProcess::MSGAPI_handle_mvUpdate(long lParam)
{
	pThis->m_pMovDetector->setUpdateFactor( gCFG_Mtd.tmpUpdateSpeed ,sThis->extInCtrl->SensorStat );
}

void CProcess::MSGAPI_input_gstctrl(long lParam)
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_GSTCTRL, NULL);
}

void CProcess::MvdetectObjHandle_FarToCenter()
{
	unsigned int distance = 0;
	unsigned int tmp=0 ;
	unsigned int x,y;
	for(int i=0;i<mvList.size();i++)
	{
		x = abs(mvList[i].trkobj.targetRect.x - vcapWH[extInCtrl->SensorStat][0]);	
		y = abs(mvList[i].trkobj.targetRect.y - vcapWH[extInCtrl->SensorStat][1]);
		tmp = (x*x + y*y);
		if( tmp > distance )
		{
			distance = tmp;
			chooseDetect = i;
		}
	}
}

void CProcess::MvdetectObjHandle_NearToCenter()
{
	unsigned int distance = 4000*4000;
	unsigned int tmp=0 ;
	unsigned int x,y;
	for(int i=0;i<mvList.size();i++)
	{
		x = abs(mvList[i].trkobj.targetRect.x - vcapWH[extInCtrl->SensorStat][0]);	
		y = abs(mvList[i].trkobj.targetRect.y - vcapWH[extInCtrl->SensorStat][1]);
		tmp = (x*x + y*y);
		if( tmp < distance )
		{
			distance = tmp;
			chooseDetect = i;
		}
	}
}

void CProcess::MvdetectObjHandle_BrightnessMax()
{
	float briMax = 0.0;
	for(int i=0;i<mvList.size();i++)
	{
		if( mvList[i].trkobj.var > briMax )
		{
			briMax = mvList[i].trkobj.var;
			chooseDetect = i;
		}
	}
}

void CProcess::MvdetectObjHandle_BrightnessMin()
{
	float briMin = 255*255;
	for(int i=0;i<mvList.size();i++)
	{
		if( mvList[i].trkobj.var < briMin )
		{
			chooseDetect = i;
		}
	}
}

void CProcess::MvdetectObjHandle_AreaMax()
{
	unsigned int aeraMax = 0;
	unsigned int tmp =0 ;
	for(int i=0;i<mvList.size();i++)
	{
		tmp = mvList[i].trkobj.targetRect.x * mvList[i].trkobj.targetRect.y ; 
		if( tmp > aeraMax )
		{	
			aeraMax = tmp;
			chooseDetect = i;
		}
	}
}

void CProcess::MvdetectObjHandle_AreaMin()
{
	unsigned int aeraMin = 1920*1920;
	unsigned int tmp =0 ;
	for(int i=0;i<mvList.size();i++)
	{
		tmp = mvList[i].trkobj.targetRect.x * mvList[i].trkobj.targetRect.y ; 
		if( tmp < aeraMin )
		{	
			aeraMin = tmp;
			chooseDetect = i;
		}
	}
}

void CProcess::MSGAPI_INPUT_SCENETRK(long lParam)
{
	sThis->msgdriv_event(MSGID_EXT_INPUT_SCENETRK,NULL);
}

unsigned int CProcess::stringip2int(string str)
{
	unsigned int intip = 0;
	int value;
	vector<string> AllStr = csplit(str, ".");
	for(int i = 0; i < AllStr.size(); i++)
	{
		if(i > 3)
			break;

		value = atoi(AllStr[i].c_str());
		int offset = (3 - i) * 8;
		intip |= ((value & 0xff) << offset);
	}
	return intip;
}

vector<string> CProcess::csplit(const string& str, const string& delim)
{
	vector<string> res;
	if("" == str) return res;

	char strs[str.length() + 1] ;
	strcpy(strs, str.c_str());

	char d[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);
	}
	return res;
}


void CProcess::drawPolyRoi(bool bdraw)
{
	Osd_cvPoint start,end;
	int color = 0;
	int cnt = m_polyBak.size();
	int index;
	if(cnt < 2)
		return;

	if(bdraw)
	{
		m_bDrawPolyRoi = true;
		color = 3;
	}
	else
		m_bDrawPolyRoi = false;

	for(int i=0;i<cnt;i++)
	{
		index = (i+1)%cnt;
		start.x = m_polyBak[i].x;
		start.y = m_polyBak[i].y;
		end.x = m_polyBak[index].x;
		end.y = m_polyBak[index].y;
		DrawcvLine(m_display.m_imgOsd[1],&start,&end,color,1);
	}
	return;
}

void CProcess::DrawMtdPolygonRoi()
{
	if(m_bDrawPolyRoi)
		drawPolyRoi(false);
	
	if(m_stateManger->getMenuState() == MENU_MTD_REGION)
	{
		m_polyBak = m_stateManger->getPoly();
		drawPolyRoi(true);
	}

	return;
}

void CProcess::DrawMtdPolygonUnRoi()
{
	if(m_bDrawPolyUnRoi)
		drawPolyUnRoi(false);
	if(m_bDrawEdgeUnRoi)
		drawEdgecounterUnRoi(false);
	if(m_bDrawFullEdgeUnRoi)
		drawEdgeFullUnRoi(false);
	
	if(m_stateManger->getMenuState() == MENU_MTD_UNREGION)
	{
		m_polyTmpBak = m_stateManger->getPolyTmp();
		edge_contoursFullUnRoi_bak = m_stateManger->getEdgeFullUnRoi();
		drawPolyUnRoi(true);	
		drawEdgeFullUnRoi(true);
	}
	if(m_stateManger->m_curState == LINKAUTO)
	{
		edge_contoursUnRoi_bak = m_stateManger->getEdgeUnRoi();
		drawEdgecounterUnRoi(true);
	}
	return;
}

void CProcess::DrawPipCross()
{
	int Chid = 1;
	Mat frame = m_display.m_imgOsd[Chid];
	cv::Rect recIn;
	static int osd_flag = 0;
	static int mode = 0;
	int color = 0;

	if(osd_flag)
	{
	
		color = 0;
		if(0 == mode)
		{
			recIn.x = 0 + outputWHF[0] / 8;
		 	recIn.y = 0 + outputWHF[1] / 8;
		}			
		else if(1 == mode)
		{
			recIn.x = outputWHF[0] / 4 * 3 + outputWHF[0] / 8;
		 	recIn.y = 0 + outputWHF[1] / 8;
		}			
		else if(2 == mode)
		{
			recIn.x = outputWHF[0] / 4 * 3 + outputWHF[0] / 8;
		 	recIn.y = outputWHF[1] /4 * 3 + outputWHF[1] / 8;
		}
		else if(3 == mode)
		{
			recIn.x = 0 + outputWHF[0] / 8;
		 	recIn.y = outputWHF[1] /4 * 3 + outputWHF[1] / 8;
		}
		recIn.width = 15;
		recIn.height = 15;
		DrawCross(recIn,color,Chid,false);
		
		osd_flag = 0;
	}


	if(MENU_CALIB == m_stateManger->getMenuState())
	{	

		color = 6;
		mode = m_stateManger->gettrig_pip_mode();
		if(0 == mode)
		{
			recIn.x = 0 + outputWHF[0] / 8;
		 	recIn.y = 0 + outputWHF[1] / 8;
		}			
		else if(1 == mode)
		{
			recIn.x = outputWHF[0] / 4 * 3 + outputWHF[0] / 8;
		 	recIn.y = 0 + outputWHF[1] / 8;
		}			
		else if(2 == mode)
		{
			recIn.x = outputWHF[0] / 4 * 3 + outputWHF[0] / 8;
		 	recIn.y = outputWHF[1] /4 * 3 + outputWHF[1] / 8;
		}
		else if(3 == mode)
		{
			recIn.x = 0 + outputWHF[0] / 8;
		 	recIn.y = outputWHF[1] /4 * 3 + outputWHF[1] / 8;
		}
		recIn.width = 15;
		recIn.height = 15;
		DrawCross(recIn,color,Chid,true);
		osd_flag = 1;
	}

}

void CProcess::drawPolyUnRoi(bool bdraw)
{
	Osd_cvPoint start,end;
	int color = 0;
	int index , cnt;

	if(bdraw)
	{
		m_bDrawPolyUnRoi = true;
		color = 3;
	}
	else
		m_bDrawPolyUnRoi = false;

	cnt = m_polyTmpBak.size();

	for(int i=0;i<cnt;i++)
	{
		index = (i+1)%cnt;
		start.x = m_polyTmpBak[i].x;
		start.y = m_polyTmpBak[i].y;
		end.x = m_polyTmpBak[index].x;
		end.y = m_polyTmpBak[index].y;
		DrawcvLine(m_display.m_imgOsd[1],&start,&end,color,1);
	}

	return;
}

void CProcess::drawEdgeFullUnRoi(bool bdraw)
{
	Osd_cvPoint start,end;
	int color = 0;
	int index , cnt , count;

	if(bdraw)
	{
		m_bDrawFullEdgeUnRoi= true;
		color = 3;
	}
	else
		m_bDrawFullEdgeUnRoi = false;


	cnt = edge_contoursFullUnRoi_bak.size();

	for(int j=0;j<cnt;j++)
	{
		count = edge_contoursFullUnRoi_bak[j].size();
		for(int i=0;i<count;i++)
		{
			index = (i+1)%count;
			start.x = edge_contoursFullUnRoi_bak[j][i].x;
			start.y = edge_contoursFullUnRoi_bak[j][i].y;
			end.x = edge_contoursFullUnRoi_bak[j][index].x;
			end.y = edge_contoursFullUnRoi_bak[j][index].y;
			DrawcvLine(m_display.m_imgOsd[1],&start,&end,color,1);	
		}
	}
	return;
}


void CProcess::drawEdgecounterUnRoi(bool bdraw)
{
	Osd_cvPoint start,end;
	int color = 0;
	int index , cnt , count;

	if(bdraw)
	{
		m_bDrawEdgeUnRoi = true;
		color = 3;
	}
	else
		m_bDrawEdgeUnRoi = false;


	cnt = edge_contoursUnRoi_bak.size();

	for(int j=0;j<cnt;j++)
	{
		count = edge_contoursUnRoi_bak[j].size();
		for(int i=0;i<count;i++)
		{
			index = (i+1)%count;
			start.x = edge_contoursUnRoi_bak[j][i].x;
			start.y = edge_contoursUnRoi_bak[j][i].y;
			end.x = edge_contoursUnRoi_bak[j][index].x;
			end.y = edge_contoursUnRoi_bak[j][index].y;
			DrawcvLine(m_display.m_imgOsd[1],&start,&end,color,1);	
		}
	}
	return;
}


void CProcess::OnJosCtrl(int key, int param)
{
	switch(key)
	{
		case JOSF1_OPEN_AUTOLINKMODE:
		{
			GB_WorkMode nextMode = (GB_WorkMode)(param - 1);
			//setWorkMode(nextMode);
		}
			break;
		case JOSF2_ENTER_MENU:				
			//app_ctrl_setMenu_jos(param);
			
			break;
		default:
			break;
	}
	return;
}

