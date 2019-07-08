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

CMenu::CMenu(OSDFUNC pfun,CHANGESTAT pchStatfun,CHDEFWORKMD pchDefwm):m_menuPointer(-1),m_menuStat(MENU_BLANK),
	shin_mtdnum(false),shin_trktime(false)
{
	init_passwd = "0000";
	drawFunc = pfun;
	changeStatFunc = pchStatfun;
	changeDefwmFunc = pchDefwm;
	disMenuBuf.cnt = 0;
	memset(m_passwd,0,sizeof(m_passwd));
	memset(m_dispasswd,0,sizeof(m_dispasswd));
	TimerCreate();
	pThis = this;



	m_mtdnum = 8;
	m_mtdtrktime = 5;
	m_mtdmaxsize = 60000;
	m_mtdminsize = 300;
	m_mtdsensi = 12;
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


void CMenu::gotoMtdparam(bool initPointer)
{
	if(initPointer)
		m_menuPointer = 0;
	lv_3_mtdparamOsd();

	set_mtd_num_osd();
	set_mtd_trktime_osd();


	
	m_menuStat = MENU_MTD;
	return;
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
			//m_config[CFGID_MTD_detnum] = m_menuCtrl.osd_mudnum;
			//storeMtdConfigFlag = true;
			//wait to save param
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
			//processdurationMenu_osd(m_mtdtrktime);
			//m_config[CFGID_MTD_maxtrk] = m_mtdtrktime;
			//storeMtdConfigFlag = true;
			//wait to save param
		}
		memset(m_trktime_arr, 0, sizeof(m_trktime_arr));
	}
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
		swprintf(disMenuBuf.osdBuffer[5].disMenu, 33, L"灵敏度		%d(超出范围%d~%d)", m_mtdsensi,MIN_MTDSENSI,MAX_MTDSENSI);
	else
		swprintf(disMenuBuf.osdBuffer[5].disMenu, 33, L"灵敏度		%d", m_mtdsensi);
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
	static bool maxsize_dianmie = false;
	static bool minsize_dianmie = false;
	static bool sensi_dianmie = false;
	
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
	/*
	else if(a == sThis->maxsize_light_id)
	{
		if(maxsize_dianmie)
			sThis->set_mtd_maxsize_osd();
		else
			swprintf(sThis->disMenu[submenu_mtd][3], 33, L"最大目标面积      ");
		maxsize_dianmie = !maxsize_dianmie;
	}
	else if(a == sThis->minsize_light_id)
	{
		if(minsize_dianmie)
			sThis->set_mtd_minsize_osd();
		else
			swprintf(sThis->disMenu[submenu_mtd][4], 33, L"最小目标面积  ");
		minsize_dianmie = !minsize_dianmie;
	}
	else if(a == sThis->sensi_light_id)
	{
		if(sensi_dianmie)
			sThis->set_mtd_sensi_osd();
		else
			swprintf(sThis->disMenu[submenu_mtd][5], 33, L"灵敏度");
		sensi_dianmie = !sensi_dianmie;
	}
	*/
	return;
}



void CMenu::menuhandle_mtdparam()
{
	switch(m_menuPointer)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			m_menuStat = MENU_MTD_SETNUM;
			menuMtdparam_setnum();
			break;
		case 3:
			// trk time
			m_menuStat = MENU_MTD_TRKTIME;
			menuMtdparam_settrktime();
			break;
		case 4:
			// max area
			break;
		case 5:
			//min area
			break;
		case 6:
			//sensi
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

		case MENU_MTD:
			menuhandle_mtdparam();
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

			break;

		case MENU_MTD_MINSIZE:

			break;

		case MENU_MTD_SENSI:

			break;
	}

	return;
}


void CMenu::normalKey(char key)
{	
	if(m_menuStat == MENU_INPUTPW)
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




