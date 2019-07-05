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


CHANGESTAT CMenu::m_changeStatFunc = NULL;


CMenu::CMenu(OSDFUNC pfun,CHANGESTAT pchStatfun):m_menuPointer(-1),m_menuStat(MENU_BLANK)
{
	init_passwd = "0000";
	drawFunc = pfun;
	m_changeStatFunc = pchStatfun;
	disMenuBuf.cnt = 0;
	memset(m_passwd,0,sizeof(m_passwd));
	memset(m_dispasswd,0,sizeof(m_dispasswd));
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
	lv_3_mtdosd();
	m_menuStat = MENU_WORKMODE;
	return;
}



void CMenu::upMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (m_menuPointer+disMenuBuf.cnt-1)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}


void CMenu::downMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (++m_menuPointer)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}


void CMenu::menuhandle_main()
{
	switch(m_menuPointer)
	{
		case 0:
			
			break;
		case 1:
		
			break;

		default:

			break;
	}


	return;
}


void CMenu::enter()
{
	if(m_menuStat == MENU_INPUTPW)
	{
		if(!strcmp(init_passwd, m_passwd))
			gotoMainMenu();
		else{
			printf("watching!!!!!!! error pw \n");
			gotoErrorPW();
		}
	}	
	else if(m_menuStat == MENU_ERRORPW)
		gotoInputPW();
	else if(m_menuStat == MENU_MAIN)
		menuhandle_main();

		
	
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

void CMenu::lv_3_mtdosd()
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




