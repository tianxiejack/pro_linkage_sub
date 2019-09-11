/*
 * menu.cpp
 *
 *  Created on: 2019年7月5日
 *      Author: alex
 */


#include "menu.hpp"
#include <string.h>
#include "locale.h"
#include <stdio.h>
#include <wchar.h>


static CMenu* pThis = NULL;

CMenu::CMenu(OSDFUNC pfun,CHANGESTAT pDisplaymode,CHANGESTAT pchStatfun,CHDEFWORKMD pchDefwm,CMvDectInterface *pMov):m_menuPointer(-1),m_menuStat(MENU_BLANK),
	shin_mtdnum(false),shin_trktime(false),shin_maxsize(false),
	shin_minsize(false),shin_sensi(false),m_ctlBallMode(JOYWM_virtualMouse),
	m_pMv(pMov)
{
	init_passwd = "0000";
	drawFunc = pfun;
	changeStatFunc = pchStatfun;
	changeDefwmFunc = pchDefwm;
	changeDisModeFunc = pDisplaymode;
	disMenuBuf.cnt = 0;
	memset(m_passwd,0,sizeof(m_passwd));
	memset(m_dispasswd,0,sizeof(m_dispasswd));
	TimerCreate();
	pThis = this;
	m_poly.clear();
	m_polyTmp.clear();
	m_unroiPoly.clear();

	m_mtdnum = 8;
	m_mtdtrktime = 5;
	m_mtdmaxsize = 60000;
	m_mtdminsize = 300;
	m_mtdsensi = 12;

	m_ScreenWidth = 1920;
	m_ScreenHeight = 1080;

	readParams4MtdRegion();
	readMtdSelectArea("SaveMtdUnRoi.yml");
}

CMenu::~CMenu()
{
}

void CMenu::menuButton()
{
	if(m_menuStat == MENU_BLANK)
		gotoInputPW();
	else if(m_menuStat == MENU_INPUTPW || m_menuStat == MENU_ERRORPW)
		gotoBlankMenu();
	else if(m_menuStat == MENU_MTD_REGION || m_menuStat == MENU_MTD_UNREGION)
		gotoMtdparam();
	else if(m_menuStat == MENU_MAIN)
		gotoBlankMenu();

	notifyMenuStat(m_menuStat);
	return;
}

void CMenu::gotoBlankMenu()
{
	m_menuStat = MENU_BLANK;
	disMenuBuf.cnt = 0;
	return;
}

void CMenu::gotoInputPW()
{
	clearPw();
	lv_1_inputPWosd();
	m_menuStat = MENU_INPUTPW;
	return;
}

void CMenu::gotoErrorPW()
{
	lv_1_errorPWosd();
	m_menuStat = MENU_ERRORPW;
	return;
}

void CMenu::gotoMainMenu()
{
	m_menuPointer = 0;
	lv_2_osd();
	m_menuStat = MENU_MAIN;
	return;
}

void CMenu::gotoWorkMode()
{
	m_menuPointer = 0;
	lv_3_workmodeOsd();
	m_menuStat = MENU_WORKMODE;
	return;
}

void CMenu::gotoCalibMode()
{
	m_menuPointer = 0;
	lv_4_calibOsd();
	m_menuStat = MENU_CALIB;
	changeStatFunc(LINKCALIB);
	changeDisModeFunc(GUN_PIP_BALL);
	notifyMenuStat(m_menuStat);
	return;
}


void CMenu::gotoMtdRegion()
{
	m_menuPointer = 0;
	//m_poly.clear();
	lv_4_mtdregionOsd(false);
	m_menuStat = MENU_MTD_REGION;
	changeDisModeFunc(GUN_FULL);
	return;
}


void CMenu::gotoMtdUnRegion()
{
	m_menuPointer = 0;
	m_polyTmp.clear();
	lv_4_mtdUnregionOsd(false);
	m_menuStat = MENU_MTD_UNREGION;
	changeDisModeFunc(GUN_FULL);
	return;
}


void CMenu::gotoMtdparam(bool initPointer)
{
	changeDisModeFunc(MAIN_VIEW);
	if(initPointer)
		m_menuPointer = 0;
	lv_3_mtdparamOsd();
	set_mtd_num_osd();
	set_mtd_trktime_osd();
	set_mtd_maxsize_osd();
	set_mtd_minsize_osd();
	set_mtd_sensi_osd();
	m_menuStat = MENU_MTD;
	return;
}


char CMenu::getMenuState()
{
	return m_menuStat;
}

void CMenu::upMenu()
{
	switch(m_menuStat)
	{
		case MENU_MTD_SETNUM:
			m_mtdnum = (m_mtdnum + 1) % (MAX_MTDTARGET_NUM+1);
			if(m_mtdnum < MIN_MTDTARGET_NUM)
				m_mtdnum = MIN_MTDTARGET_NUM;
			set_mtd_num_osd();
			break;

		case MENU_MTD_TRKTIME:
			m_mtdtrktime = (m_mtdtrktime + 1) % (MAX_MTDTRKTIME+1);
			if(m_mtdtrktime < MIN_MTDTRKTIME)
				m_mtdtrktime = MIN_MTDTRKTIME;
			set_mtd_trktime_osd();
			break;

		case MENU_MTD_MAXSIZE:
			m_mtdmaxsize = (m_mtdmaxsize + 1) % (MAX_MTDMAXSIZE+1);
			if(m_mtdmaxsize < m_mtdminsize)
				m_mtdmaxsize = m_mtdminsize;
			set_mtd_maxsize_osd();
			break;
			
		case MENU_MTD_MINSIZE:
			m_mtdminsize = (m_mtdminsize + 1) % (MAX_MTDMAXSIZE+1);
			if(m_mtdminsize > MAX_MTDMAXSIZE)
				m_mtdminsize = MAX_MTDMAXSIZE;
			set_mtd_minsize_osd();
			break;
			
		case MENU_MTD_SENSI:
			m_mtdsensi = (m_mtdsensi + 1) % (MAX_MTDSENSI+1);
			if(m_mtdsensi < MIN_MTDSENSI)
				m_mtdsensi = MIN_MTDSENSI;
			set_mtd_sensi_osd();
			break;

		case MENU_BLANK:
		case MENU_INPUTPW:
		case MENU_ERRORPW:
			break;
			
		default:
			disMenuBuf.osdBuffer[m_menuPointer].color = 2;
			m_menuPointer = (m_menuPointer+disMenuBuf.cnt-1)%disMenuBuf.cnt;
			disMenuBuf.osdBuffer[m_menuPointer].color = 3;
			break;
	}
	return;
}


void CMenu::downMenu()
{
	switch(m_menuStat)
	{
		case MENU_MTD_SETNUM:
			if(MIN_MTDTARGET_NUM == m_mtdnum )
				m_mtdnum = MAX_MTDTARGET_NUM;
			else
				m_mtdnum = (m_mtdnum  + MAX_MTDTARGET_NUM - 1) % MAX_MTDTARGET_NUM;
			set_mtd_num_osd();
			break;

		case MENU_MTD_TRKTIME:	
			if(MIN_MTDTRKTIME == m_mtdtrktime)
				m_mtdtrktime = MAX_MTDTRKTIME;
			else
				m_mtdtrktime = (m_mtdtrktime + MAX_MTDTRKTIME - 1) % MAX_MTDTRKTIME;
			set_mtd_trktime_osd();
			break;

		
		case MENU_MTD_MAXSIZE:
			if(m_mtdmaxsize == m_mtdminsize)
				m_mtdmaxsize = MAX_MTDMAXSIZE;
			else
				m_mtdmaxsize = (m_mtdmaxsize + MAX_MTDMAXSIZE - 1) % MAX_MTDMAXSIZE;
			set_mtd_maxsize_osd();
			break;

		case MENU_MTD_MINSIZE:
			if(MIN_MTDMINSIZE > m_mtdminsize)
				m_mtdminsize = MIN_MTDMINSIZE;
			else
				m_mtdminsize = (m_mtdminsize + MAX_MTDMAXSIZE - 1) % MAX_MTDMAXSIZE;
			set_mtd_minsize_osd();
			break;
			
		case MENU_MTD_SENSI:
			if(MIN_MTDSENSI == m_mtdsensi)
				m_mtdsensi = MAX_MTDSENSI;
			else
				m_mtdsensi = (m_mtdsensi + MAX_MTDSENSI - 1) % MAX_MTDSENSI;
			set_mtd_sensi_osd();
			break;

		case MENU_BLANK:
		case MENU_INPUTPW:
		case MENU_ERRORPW:
			break;

		default:
			disMenuBuf.osdBuffer[m_menuPointer].color = 2;
			m_menuPointer = (++m_menuPointer)%disMenuBuf.cnt;
			disMenuBuf.osdBuffer[m_menuPointer].color = 3;
			break;
	}		
	return;
}

void CMenu::menuhandle_main()
{
	switch(m_menuPointer)
	{
		case 0:
			gotoWorkMode();
			break;
			
		case 1:
			gotoCalibMode();
			break;
			
		case 2:
			gotoMtdparam();
			break;

		default:
			break;
	}


	return;
}


void CMenu::menuhandle_workmode()
{
	switch(m_menuPointer)
	{
		case 0:
			changeDefwmFunc(LINKMANUAL);
			break;
		case 1:
			changeDefwmFunc(LINKAUTO);
			break;
		case 2:
			changeDefwmFunc(LINKBALL);
			break;
		case 3:
			gotoMainMenu();
			break;
		default:
			break;
	}
	return;
}


void CMenu::menuMtdparam_setnum()
{
	shin_mtdnum = !shin_mtdnum;
	if(shin_mtdnum)
		m_timer.startTimer(mtdnum_timeId,500);
	else
	{
		m_timer.stopTimer(mtdnum_timeId);
		set_mtd_num_osd();
		if((m_mtdnum >= MIN_MTDTARGET_NUM) && (m_mtdnum <= MAX_MTDTARGET_NUM))
		{
			writeMtdconfigfile();
		}
		memset(m_mtdnum_arr, 0, sizeof(m_mtdnum_arr));
	}
	return;
}


void CMenu::menuMtdparam_settrktime()
{
	shin_trktime = !shin_trktime;

	if(shin_trktime)
		m_timer.startTimer(trktime_timeId,500);
	else
	{
		m_timer.stopTimer(trktime_timeId);
		set_mtd_trktime_osd();
		if((m_mtdtrktime >= MIN_MTDTRKTIME) && (m_mtdtrktime <= MAX_MTDTRKTIME))
		{
			writeMtdconfigfile();
		}
		memset(m_trktime_arr, 0, sizeof(m_trktime_arr));
	}
}


void CMenu::menuMtdparam_setmaxsize()
{
	shin_maxsize = !shin_maxsize;
	if(shin_maxsize)
		m_timer.startTimer(maxsize_timeId,500);
	else
	{
		m_timer.stopTimer(maxsize_timeId);
		set_mtd_maxsize_osd();
		if((m_mtdmaxsize >= m_mtdminsize ) && (m_mtdmaxsize <= MAX_MTDMAXSIZE))
		{
			writeMtdconfigfile();
		}
		memset(m_maxsize_arr, 0, sizeof(m_maxsize_arr));
	}
	return;	
}


void CMenu::menuMtdparam_setminsize()
{
	shin_minsize = !shin_minsize;
	if(shin_minsize)
		m_timer.startTimer(minsize_timeId,500);
	else
	{
		m_timer.stopTimer(minsize_timeId);
		set_mtd_minsize_osd();
		if((m_mtdminsize >= MIN_MTDMINSIZE) && (m_mtdminsize <= m_mtdmaxsize))
		{
			writeMtdconfigfile();
		}
		memset(m_minsize_arr, 0, sizeof(m_minsize_arr));
	}
	return;
}


void CMenu::menuMtdparam_setsensi()
{
	shin_sensi = !shin_sensi;
	if(shin_sensi)
		m_timer.startTimer(sensi_timeId,500);
	else
	{
		m_timer.stopTimer(sensi_timeId);
		set_mtd_sensi_osd();
		if((m_mtdsensi >= MIN_MTDSENSI) && (m_mtdsensi <= MAX_MTDSENSI))
		{
			writeMtdconfigfile();
		}
		memset(m_sensi_arr, 0, sizeof(m_sensi_arr));	
	}
	return;
}


void CMenu::set_mtd_num(char key)
{
	int offset = strlen(m_mtdnum_arr) * sizeof(char);
	if(offset < sizeof(m_mtdnum_arr) - 1)
		sprintf(m_mtdnum_arr + offset,"%c", key);
	else
		printf("mtdnum reached max length:128");
	
	int num = atoi(m_mtdnum_arr);
	m_mtdnum = atoi(m_mtdnum_arr);
	printf("%s,%d,osd_mudnum=%d\n",__FILE__,__LINE__,m_mtdnum);
	set_mtd_num_osd();
	return ;
}

void CMenu::set_mtd_num_osd()
{
	if((m_mtdnum < MIN_MTDTARGET_NUM) || (m_mtdnum > MAX_MTDTARGET_NUM))
		swprintf(disMenuBuf.osdBuffer[2].disMenu, 33, L"目标个数	    %d(超出范围%d~%d)", m_mtdnum,MIN_MTDTARGET_NUM,MAX_MTDTARGET_NUM);
	else
		swprintf(disMenuBuf.osdBuffer[2].disMenu, 33, L"目标个数	    %d个", m_mtdnum);
	return ;
}


void CMenu::set_mtd_trktime(char key)
{
	int offset = strlen(m_trktime_arr) * sizeof(char);
	if(offset < sizeof(m_trktime_arr) - 1)
		sprintf(m_trktime_arr + offset,"%c", key);
	else	
		printf("trktime reached max length:128");

	int num = atoi(m_trktime_arr);
	m_mtdtrktime = atoi(m_trktime_arr);
	printf("%s,%d,osd_trktime=%d\n",__FILE__,__LINE__,m_mtdtrktime);
	set_mtd_trktime_osd();
	return ;
}


void CMenu::set_mtd_trktime_osd()
{
	if((m_mtdtrktime < MIN_MTDTRKTIME) || (m_mtdtrktime > MAX_MTDTRKTIME))
		swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"跟踪持续时间 %d秒(超出范围%d~%d)", m_mtdtrktime,MIN_MTDTRKTIME,MAX_MTDTRKTIME);
	else
		swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"跟踪持续时间 %d秒", m_mtdtrktime);
	return ;
}


void CMenu::set_mtd_maxsize_osd()
{
	if((m_mtdmaxsize < MIN_MTDMINSIZE) || (m_mtdmaxsize > MAX_MTDMAXSIZE))
		swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"最大目标面积 %d(超出范围)", m_mtdmaxsize);
	else
		swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"最大目标面积 %d", m_mtdmaxsize);
	return;
}


void CMenu::set_mtd_minsize_osd()
{
	if((m_mtdminsize < MIN_MTDMINSIZE) || (m_mtdminsize > MAX_MTDMAXSIZE))
		swprintf(disMenuBuf.osdBuffer[5].disMenu, 33, L"最小目标面积 %d(超出范围)", m_mtdminsize);
	else
		swprintf(disMenuBuf.osdBuffer[5].disMenu, 33, L"最小目标面积 %d", m_mtdminsize);
	return;
}


void CMenu::set_mtd_sensi_osd()
{
	if((m_mtdsensi < MIN_MTDSENSI) || (m_mtdsensi > MAX_MTDSENSI))
		swprintf(disMenuBuf.osdBuffer[6].disMenu, 33, L"灵敏度		%d(超出范围%d~%d)", m_mtdsensi,MIN_MTDSENSI,MAX_MTDSENSI);
	else
		swprintf(disMenuBuf.osdBuffer[6].disMenu, 33, L"灵敏度		%d", m_mtdsensi);
	return;
}

void CMenu::save_polygon_roi()
{
	int curId = 0;
	float floatx,floaty;
	int setx, sety = 0;
	int areanum = 1;

	lv_4_mtdregionOsd(true);

	if(m_poly.size() < 3)
		return;

	polyWarnRoi.resize(areanum);
	edge_contours.resize(areanum);

	for(int i = 0; i < areanum; i++)
	{
		polyWarnRoi[i].resize(m_poly.size());
		edge_contours[i].resize(m_poly.size());
		for(int j = 0; j < m_poly.size(); j++)
		{
			floatx = m_poly[j].x;
			floaty = m_poly[j].y;
			map1080p2normal_point(&floatx, &floaty);
			mapnormal2curchannel_point(&floatx, &floaty, vdisWH[curId][0], vdisWH[curId][1]);

			setx = floatx;
			sety = floaty;
			polyWarnRoi[i][j] = cv::Point(setx, sety);

			mapfullscreen2gun_pointv20(&setx, &sety);
			edge_contours[i][j].x = setx;
			edge_contours[i][j].y = sety;
		}
	}

	if(polyWarnRoi.size() != 0)
		SaveMtdSelectArea("SaveMtdRoi.yml", polyWarnRoi);

	for(int i = 0; i < areanum; i++)
	{
		m_pMv->setWarningRoi(polyWarnRoi[i], i);
	}

	return ;
}

void CMenu::save_polygon_unroi()
{
	int curId = 0;
	float floatx,floaty;
	int setx, sety = 0;
	
	std::vector< cv::Point >  polyUnRoi;
	std::vector< cv::Point >  mainUnRoi;

	polyUnRoi.clear();
	mainUnRoi.clear();
	
	lv_4_mtdUnregionOsd(true);
	
	if(m_polyTmp.size() >= 3)
	{
		for(int j = 0; j < m_polyTmp.size(); j++)
		{
			floatx = m_polyTmp[j].x;
			floaty = m_polyTmp[j].y;
			map1080p2normal_point(&floatx, &floaty);
			mapnormal2curchannel_point(&floatx, &floaty, vdisWH[curId][0], vdisWH[curId][1]);

			setx = floatx;
			sety = floaty;
			
			polyUnRoi.push_back(cv::Point(setx, sety));
			mapfullscreen2gun_pointv20(&setx, &sety);
			mainUnRoi.push_back(cv::Point(setx, sety));
		}
		edge_contours_FullUnRoi.push_back(polyUnRoi);
		edge_contours_UnRoi.push_back(mainUnRoi);
	}
	else
	{
		edge_contours_FullUnRoi.clear();
		edge_contours_UnRoi.clear();
	}
	
	SaveMtdSelectArea("SaveMtdUnRoi.yml", edge_contours_FullUnRoi);

	m_polyTmp.clear();
	return ;
}

int CMenu::map1080p2normal_point(float *x, float *y)
{
	if(NULL != x)
		*x /= m_ScreenWidth;
	if(NULL != y)
		*y /= m_ScreenHeight;

	return 0;
}

int CMenu::mapnormal2curchannel_point(float *x, float *y, int w, int h)
{
	if(NULL != x)
		*x *= w;
	if(NULL != y)
		*y *= h;
	
	return 0;
}

int CMenu::mapfullscreen2gun_pointv20(int *x, int *y)
{
	mouserect rect1080p;
	mouserect rectgun;
	
	rect1080p.x = 0;
	rect1080p.y = 0;
	rect1080p.w = m_ScreenWidth;// 1920;
	rect1080p.h = m_ScreenHeight;

	rectgun.x = 0;
	rectgun.y = m_ScreenHeight/2;
	rectgun.w = m_ScreenWidth;// 1920;
	rectgun.h = m_ScreenHeight/2;
	
	return maprect_point(x, y, rect1080p, rectgun);
}

int CMenu::maprect_point(int *x, int *y, mouserect rectsrc,mouserect rectdest)
{
	if(NULL != x)
		*x = (*x-rectsrc.x)*rectdest.w/rectsrc.w+rectdest.x;
	if(NULL != y)
		*y = (*y-rectsrc.y)*rectdest.h/rectsrc.h+rectdest.y;
	return 0;
}

void CMenu::SaveMtdSelectArea(const char* filename, std::vector< std::vector< cv::Point > > edge_contours)
{
	char paramName[40];
	memset(paramName,0,sizeof(paramName));
	m_fsWriteMtd.open(filename,FileStorage::WRITE);

	if(m_fsWriteMtd.isOpened())
	{		
		memset(paramName,0,sizeof(paramName));
		sprintf(paramName,"AreaCount");	
		int total_size = edge_contours.size();
		m_fsWriteMtd<< paramName  << total_size;

		for(int m = 0; m<edge_contours.size(); m++ )
		{			
			memset(paramName,0,sizeof(paramName));
			sprintf(paramName,"AreaIndex_%d",m);
			int count  =  edge_contours[m].size();
			m_fsWriteMtd<< paramName << count;
		}
				
		for(int i = 0; i < edge_contours.size(); i++)
		{
			for(int j = 0; j < edge_contours[i].size(); j++)
			{	
				sprintf(paramName,"Point_%d_%d_x",i,j);				
				m_fsWriteMtd<<paramName <<edge_contours[i][j].x;
				
				memset(paramName,0,sizeof(paramName));
				sprintf(paramName,"Point_%d_%d_y",i,j);				
				m_fsWriteMtd<<paramName <<edge_contours[i][j].y;		
			}		
		}		
		m_fsWriteMtd.release();			
	}
	return;
}


void CMenu::readMtdSelectArea(const char* filename)
{
	char paramName[40];
	memset(paramName,0,sizeof(paramName));
	m_readfs.open(filename,FileStorage::READ);
	cv::Point tmpPoint;
	int totalNum , num;
	if(m_readfs.isOpened())
	{				
		memset(paramName,0,sizeof(paramName));
		sprintf(paramName,"AreaCount");	
		m_readfs[paramName] >> totalNum;

		edge_contours_FullUnRoi.resize(totalNum);
		edge_contours_UnRoi.resize(totalNum);
	
		for(int j=0;j<totalNum;j++)
		{
			memset(paramName,0,sizeof(paramName));
			sprintf(paramName,"AreaIndex_%d",j); 
			m_readfs[paramName] >> num;

			for(int i=0;i< num;i++)
			{
				memset(paramName,0,sizeof(paramName));
				sprintf(paramName,"Point_%d_%d_x",j,i); 
				m_readfs[paramName] >> tmpPoint.x;
				memset(paramName,0,sizeof(paramName));
				sprintf(paramName,"Point_%d_%d_y",j,i); 
				m_readfs[paramName] >> tmpPoint.y;
				
				edge_contours_FullUnRoi[j].push_back(tmpPoint);
				mapfullscreen2gun_pointv20(&tmpPoint.x, &tmpPoint.y);
				edge_contours_UnRoi[j].push_back(tmpPoint);
			}	
		}
		m_readfs.release(); 		
	}
	return;
}


void CMenu::TimerCreate()
{
	mtdnum_timeId = m_timer.createTimer();
	trktime_timeId = m_timer.createTimer();
	maxsize_timeId = m_timer.createTimer();
	minsize_timeId = m_timer.createTimer();
	sensi_timeId = m_timer.createTimer();

	m_timer.registerTimer(mtdnum_timeId, TcallbackFunc, &mtdnum_timeId);
	m_timer.registerTimer(trktime_timeId, TcallbackFunc, &trktime_timeId);
	m_timer.registerTimer(maxsize_timeId, TcallbackFunc, &maxsize_timeId);
	m_timer.registerTimer(minsize_timeId, TcallbackFunc, &minsize_timeId);
	m_timer.registerTimer(sensi_timeId, TcallbackFunc, &sensi_timeId);
	return;
}


void CMenu::TcallbackFunc(void *p)
{
	pThis->TcallbackHandle(p);
	return;
}


void CMenu::TcallbackHandle(void *p)
{
	static bool mtdnum_shinOut = false;
	static bool mtdtrktime_shinOut = false;
	static bool maxsize_shinOut = false;
	static bool minsize_shinOut = false;
	static bool sensi_shinOut = false;
	
	int a = *(int *)p;

	if(a == mtdnum_timeId)
	{
		if(mtdnum_shinOut)
			set_mtd_num_osd();
		else{
			if(m_mtdnum >= 10)
				swprintf(disMenuBuf.osdBuffer[2].disMenu, 33, L"目标个数      个");
			else
				swprintf(disMenuBuf.osdBuffer[2].disMenu, 33, L"目标个数      个");
		}
		mtdnum_shinOut = !mtdnum_shinOut;
	}
	else if(a == trktime_timeId)
	{
		if(mtdtrktime_shinOut)
			set_mtd_trktime_osd();
		else{
			if(m_mtdtrktime >= 10)
				swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"跟踪持续时间   秒");
			else
				swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"跟踪持续时间  秒");
		}
		mtdtrktime_shinOut = !mtdtrktime_shinOut;
	}
	else if(a == maxsize_timeId)
	{
		if(maxsize_shinOut)
			set_mtd_maxsize_osd();
		else
			swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"最大目标面积      ");
		maxsize_shinOut = !maxsize_shinOut;
	}
	else if(a == minsize_timeId)
	{
		if(minsize_shinOut)
			set_mtd_minsize_osd();
		else
			swprintf(disMenuBuf.osdBuffer[5].disMenu, 33, L"最小目标面积  ");
		minsize_shinOut = !minsize_shinOut;
	}
	else if(a == sensi_timeId)
	{
		if(sensi_shinOut)
			set_mtd_sensi_osd();
		else
			swprintf(disMenuBuf.osdBuffer[6].disMenu, 33, L"灵敏度");
		sensi_shinOut = !sensi_shinOut;
	}
	return;
}



void CMenu::menuhandle_mtdparam()
{
	switch(m_menuPointer)
	{
		case 0:
			gotoMtdRegion();
			break;
		case 1:
			gotoMtdUnRegion();
			break;
		case 2:
			m_menuStat = MENU_MTD_SETNUM;
			menuMtdparam_setnum();
			break;
		case 3:
			m_menuStat = MENU_MTD_TRKTIME;
			menuMtdparam_settrktime();
			break;
		case 4:
			m_menuStat = MENU_MTD_MAXSIZE;
			menuMtdparam_setmaxsize();
			break;
		case 5:
			m_menuStat = MENU_MTD_MINSIZE;
			menuMtdparam_setminsize();
			break;
		case 6:
			m_menuStat = MENU_MTD_SENSI;
			menuMtdparam_setsensi();
			break;
		case 7:
			gotoMainMenu();
			break;
		default:
			break;
	}
	return;
}



void CMenu::enter()
{
	switch(m_menuStat)
	{
		case MENU_INPUTPW:
			if(!strcmp(init_passwd, m_passwd))
				gotoMainMenu();
			else{
				printf("watching!!!!!!! error pw \n");
				gotoErrorPW();
			}
			break;

		case MENU_ERRORPW:
			gotoInputPW();
			break;

		case MENU_MAIN:
			menuhandle_main();
			break;
			
		case MENU_WORKMODE:
			menuhandle_workmode();
			break;
			
		case MENU_CALIB:
			sendIpc4PTZpos();
			break;

		case MENU_MTD:
			menuhandle_mtdparam();
			break;	

		case MENU_MTD_REGION:
			//m_poly.push_back(m_poly[0]);
			save_polygon_roi();
			break;

		case MENU_MTD_UNREGION:
			//m_polyTmp.push_back(m_polyTmp[0]);
			//m_unroiPoly.push_back(m_polyTmp);
			//m_polyTmp.clear();
			save_polygon_unroi();
			break;

		case MENU_MTD_SETNUM:
			menuMtdparam_setnum();
			gotoMtdparam(false);
			break;

		case MENU_MTD_TRKTIME:
			menuMtdparam_settrktime();
			gotoMtdparam(false);
			break;

		case MENU_MTD_MAXSIZE:
			menuMtdparam_setmaxsize();
			gotoMtdparam(false);
			break;

		case MENU_MTD_MINSIZE:
			menuMtdparam_setminsize();
			gotoMtdparam(false);
			break;

		case MENU_MTD_SENSI:
			menuMtdparam_setsensi();
			gotoMtdparam(false);
			break;

		default:
			break;
	}
	return;
}


void CMenu::normalKey(char key)
{	
	switch(m_menuStat)
	{
		case MENU_INPUTPW:
			{
				int offset = strlen(m_passwd) * sizeof(char);		
				if(offset < sizeof(m_passwd) - 1)
				{
					sprintf(m_passwd + offset,"%c", key);
					sprintf(m_dispasswd + offset,"%c", '*');
					swprintf(disMenuBuf.osdBuffer[1].disMenu, 33, L"%s", m_dispasswd);		
				}
				else
					printf("password reached max length:128");
				
				printf("%s --> LINE:%d ****** passwd=%s\n",__FILE__,__LINE__,m_passwd);
			}
			break;

		case MENU_MTD_TRKTIME:
			{
				int offset = strlen(m_trktime_arr) * sizeof(char);
				if(offset < sizeof(m_trktime_arr) - 1)
					sprintf(m_trktime_arr + offset,"%c", key);
				else
					printf("trktime reached max length:128");
				m_mtdtrktime = atoi(m_trktime_arr);
			}
			break;

		case MENU_MTD_MAXSIZE:
			{
				int offset = strlen(m_maxsize_arr) * sizeof(char);
				if(offset < sizeof(m_maxsize_arr) - 1)
					sprintf(m_maxsize_arr + offset,"%c", key);
				else
					printf("trktime reached max length:128");
				m_mtdmaxsize = atoi(m_maxsize_arr);
			}
			break;

		case MENU_MTD_MINSIZE:
			{
				int offset = strlen(m_minsize_arr) * sizeof(char);
				if(offset < sizeof(m_minsize_arr) - 1)
					sprintf(m_minsize_arr + offset,"%c", key);
				else
					printf("trktime reached max length:128");
				m_mtdminsize = atoi(m_minsize_arr);
			}
			break;

		case MENU_MTD_SENSI:
			{
				int offset = strlen(m_sensi_arr) * sizeof(char);
				if(offset < sizeof(m_sensi_arr) - 1)
					sprintf(m_sensi_arr + offset,"%c", key);
				else
					printf("trktime reached max length:128");
				m_mtdsensi = atoi(m_sensi_arr);
			}
			break;

		case MENU_MTD_SETNUM:
			{
				int offset = strlen(m_mtdnum_arr) * sizeof(char);
				if(offset < sizeof(m_mtdnum_arr) - 1)
					sprintf(m_mtdnum_arr + offset,"%c", key);
				else
					printf("trktime reached max length:128");
				m_mtdnum = atoi(m_mtdnum_arr);
			}
			break;
				
		case MENU_MTD_REGION:	
			if('0' == key)
				m_poly.clear();
			break;

		case MENU_MTD_UNREGION:
			if('0' == key)
			{
				m_polyTmp.clear();
				edge_contours_FullUnRoi.clear();
				edge_contours_UnRoi.clear();
			}
			break;	
		default:
			break;
	}
	return;
}


void CMenu::clearPw()
{
	memset(m_passwd,0,sizeof(m_passwd));
}


void CMenu::lv_1_inputPWosd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = {"请输入密码呼出菜单", "", "按回车确认", "按F2退出"};
	disMenuBuf.cnt = 4;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 2;
		disMenuBuf.osdBuffer[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	return;
}


void CMenu::lv_1_errorPWosd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"请输入密码呼出菜单", "密码输入错误","按回车后再次输入",  "按F2退出"};
	disMenuBuf.cnt = 4;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{

		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 2;
		disMenuBuf.osdBuffer[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	disMenuBuf.osdBuffer[1].color = 3;
	disMenuBuf.osdBuffer[2].color = 3;
	return;
}

void CMenu::lv_2_osd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"设置默认工作模式","枪球标定","移动检测设置"};
	disMenuBuf.cnt = 3;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 2;
		disMenuBuf.osdBuffer[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}

void CMenu::lv_3_workmodeOsd()
{
	unsigned char menubuf[MAX_SUBMENU][128] =  	{"手动联动模式","自动联动模式","单控球机模式","返回"};
	disMenuBuf.cnt = 4;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 2;
		disMenuBuf.osdBuffer[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}

void CMenu::lv_3_mtdparamOsd()
{
	unsigned char menubuf[MAX_SUBMENU][128] =  	{"检测区域选择","不检测区域","目标个数      1","跟踪持续时间 1秒","最大目标面积 10000","最小目标面积 9","灵敏度		30","返回"};
	disMenuBuf.cnt = 8;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 2;
		disMenuBuf.osdBuffer[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;

	return;
}


void CMenu::lv_4_mtdregionOsd(bool show_result)
{
	//L"鼠标左键:选择点 鼠标右键:删除点 回车:确认 F1:控球模式 0:删除所有点  1:保存"
	int j;

	if(show_result)
		disMenuBuf.cnt = 3;
	else
		disMenuBuf.cnt = 2;
	
	j=0;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 800;
	disMenuBuf.osdBuffer[j].posy = 50;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"检测区域设置");

	j=1;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 600;
	disMenuBuf.osdBuffer[j].posy = 100;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"鼠标左键:增加点  0:清空点  回车:保存设置  F2:退出");

	if(show_result)
	{
		j = 2;
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 1;
		disMenuBuf.osdBuffer[j].posx = 1400;
		disMenuBuf.osdBuffer[j].posy = 50;
		setlocale(LC_ALL, "zh_CN.UTF-8");
		if(m_poly.size() >= 3)
			swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"点数:%d,保存成功", m_poly.size());
		else
			swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"点数小于3,保存失败");
	}

	return;
}

void CMenu::lv_4_mtdUnregionOsd(bool show_result)
{
	//L"鼠标左键:选择点 鼠标右键:删除点 回车:确认 F1:控球模式 0:删除所有点  1:保存"
	int j;
	if(show_result)
		disMenuBuf.cnt = 3;
	else
		disMenuBuf.cnt = 2;
	
	j=0;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 800;
	disMenuBuf.osdBuffer[j].posy = 50;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"不检测区域设置");

	j=1;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 600;
	disMenuBuf.osdBuffer[j].posy = 100;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"鼠标左键:增加点  0:清空点  回车:保存设置  F2:退出");

	if(show_result)
	{
		j = 2;
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 1;
		disMenuBuf.osdBuffer[j].posx = 1400;
		disMenuBuf.osdBuffer[j].posy = 50;
		setlocale(LC_ALL, "zh_CN.UTF-8");
		if(m_polyTmp.size() >= 3)
			swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"点数:%d,保存成功", m_polyTmp.size());
		else
			swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"确认");
	}
	return;
}


void CMenu::lv_4_calibOsd()
{
	int j;
	disMenuBuf.cnt = 2;

	j=0;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 700;
	disMenuBuf.osdBuffer[j].posy = 50;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"鼠标左键:选择点 鼠标右键:删除点");

	j=1;
	disMenuBuf.osdBuffer[j].bshow = true;
	disMenuBuf.osdBuffer[j].alpha = 2;
	disMenuBuf.osdBuffer[j].color = 1;
	disMenuBuf.osdBuffer[j].posx = 700;
	disMenuBuf.osdBuffer[j].posy = 100;
	setlocale(LC_ALL, "zh_CN.UTF-8");
	swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"回车:确认 0:清空点  1:保存");

	return;
}


void CMenu::showMenuOsd()
{
	unsigned char r, g, b, a, color;
	short x, y;

	char font = 1;
	char fontsize = 4;

	for(int i = 0; i < disMenuBuf.cnt; i++)
	{
		if(disMenuBuf.osdBuffer[i].bshow)
		{
			x = disMenuBuf.osdBuffer[i].posx;
			y = disMenuBuf.osdBuffer[i].posy;
			a = disMenuBuf.osdBuffer[i].alpha;
			color = disMenuBuf.osdBuffer[i].color;
			getRGBA(color,r,g,b,a);
			drawFunc(x, y,disMenuBuf.osdBuffer[i].disMenu, font ,fontsize, r, g, b, a, VIDEO_DIS_WIDTH, VIDEO_DIS_HEIGHT);
		}
	}
	return;
}


void CMenu::getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a)
{
	switch(color)
	{
		case 1:
			r = 0;
			g = 0;
			b = 0;
			break;
		case 2:
			r = 255;
			g = 255;
			b = 255;
			break;
		case 3:
			r = 255;
			g = 0;
			b = 0;
			break;
		case 4:
			r = 255;
			g = 255;
			b = 0;
			break;
		case 5:
			r = 0;
			g = 0;
			b = 255;
			break;
		case 6:
			r = 0;
			g = 255;
			b = 0;
			break;
		default:
			break;
	}
	
	if(a > 0x0a)
		a = 0x0a;
	if(a == 0x0a)
		a = 0;
	else
		a = 255 - a*16;
	return ;
}

bool CMenu::readParams4MtdRegion()
{
	char paramName[40];
	memset(paramName, 0, sizeof(paramName));
	string cfgFile;
	cfgFile = "SaveMtdRoi.yml";
	m_readfs.open(cfgFile, FileStorage::READ);
	int size,areanum;
	int setx, sety = 0;
	cv::Point tpoint;
	
	if (m_readfs.isOpened()) {
		sprintf(paramName, "AreaCount");
		m_readfs[paramName] >> areanum;
		sprintf(paramName, "AreaIndex_0");
		m_readfs[paramName] >> size;
		for(int k=0;k<areanum;k++)
		for (int i = 0; i < size; i++) {
			sprintf(paramName, "Point_%d_%d_x",k,i);
			m_readfs[paramName] >> tpoint.x;
			sprintf(paramName, "Point_%d_%d_y",k,i);
			m_readfs[paramName] >> tpoint.y;
			m_poly.push_back(tpoint);
		}
		m_readfs.release();

		if(m_poly.size() < 3)
			return false;
		
		polyWarnRoi.resize(areanum);
		edge_contours.resize(areanum);
		
		for(int i = 0; i < areanum; i++)
		{
			polyWarnRoi[i].resize(m_poly.size());
			edge_contours[i].resize(m_poly.size());
			for(int j = 0; j < m_poly.size(); j++)
			{		
				setx = m_poly[j].x;
				sety = m_poly[j].y;
				polyWarnRoi[i][j] = cv::Point(setx, sety);
		
				mapfullscreen2gun_pointv20(&setx, &sety);
				edge_contours[i][j].x = setx;
				edge_contours[i][j].y = sety;
			}
		}
				
		for(int i = 0; i < areanum; i++)
			m_pMv->setWarningRoi(polyWarnRoi[i], i);

		return true;
	}
	return false;
}

int CMenu::writeMtdconfigfile()
{
	char paramName[40];
	memset(paramName, 0, sizeof(paramName));
	string cfgFile;
	cfgFile = "mtdparam.yml";

	m_readfs.open(cfgFile, FileStorage::WRITE);
	if(m_readfs.isOpened())
	{		
		sprintf(paramName, "defaultWorkMode");
		m_readfs << paramName << (int)m_defworkmode;
		memset(paramName, 0, sizeof(paramName));

		sprintf(paramName, "mtdnum");
		m_readfs << paramName << (int)m_mtdnum;
		memset(paramName, 0, sizeof(paramName));
		
		sprintf(paramName, "sensi");
		m_readfs << paramName << (int)m_mtdsensi;
		memset(paramName, 0, sizeof(paramName));
		
		sprintf(paramName, "minsize");
		m_readfs << paramName << (int)m_mtdminsize;
		memset(paramName, 0, sizeof(paramName));
		
		sprintf(paramName, "maxsize");
		m_readfs << paramName << (int)m_mtdmaxsize;
		memset(paramName, 0, sizeof(paramName));
		
		sprintf(paramName, "trktime");
		m_readfs << paramName << (int)m_mtdtrktime;
		memset(paramName, 0, sizeof(paramName));
		
		m_readfs.release();
	}
	return 0;
	
}

int CMenu::readMtdConfigFile()
{
	char paramName[40];
	memset(paramName, 0, sizeof(paramName));
	string cfgFile;
	cfgFile = "mtdparam.yml";

	m_readfs.open(cfgFile, FileStorage::READ);

	if(m_readfs.isOpened())
	{
		sprintf(paramName, "defaultWorkMode");
		m_readfs[paramName] >> m_defworkmode;
		sprintf(paramName, "mtdnum");
		m_readfs[paramName] >> m_mtdnum;
		sprintf(paramName, "sensi");
		m_readfs[paramName] >> m_mtdsensi;
		sprintf(paramName, "minsize");
		m_readfs[paramName] >> m_mtdminsize;
		sprintf(paramName, "maxsize");
		m_readfs[paramName] >> m_mtdmaxsize;
		sprintf(paramName, "trktime");
		m_readfs[paramName] >> m_mtdtrktime;		
		m_readfs.release();	
	}
	return 0;
}


