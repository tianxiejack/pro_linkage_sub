/*
 * state_levelone.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelOne::LevelOne():inputtingStat(false),m_menuShow(false)
{
	init_passwd ="0000";
	initOsd();
}

LevelOne::~LevelOne()
{
}

void LevelOne::initOsd()
{
	inputPWOsd();
	return;
}

void LevelOne::buttonMenu()
{
	m_menuShow = !m_menuShow;
	if(m_menuShow == true)
	{
		clearPw();
		inputtingStat = true;
		inputPWOsd();
	}
	else
		inputtingStat = false;
	return;
}

void LevelOne::clearPw()
{
	memset(m_passwd,0,sizeof(m_passwd));
	return;
}

void LevelOne::enter()
{
	if(m_menuShow)
	{
		if(inputtingStat)
		{
			if(!strcmp(init_passwd, m_passwd)){
				inputtingStat = true;
				clearPw();
				ChangeState(LEVELTWO);
				
				printf("watching :::!!!!!!! enter next state \n");
			}
			else{
				//printf("watching :::!!!!!!! error pw \n");
				inputtingStat = false;
				inputErrorPWOsd();
				clearPw();
			}
		}else{
			inputPWOsd();
			inputtingStat = true;
		}
	}
	return;
}


void LevelOne::inputNumber(char key)
{
	if(inputtingStat)
	{
		int offset = strlen(m_passwd) * sizeof(char);		
		if(offset < sizeof(m_passwd) - 1)
		{
			sprintf(m_passwd + offset,"%c", key);
			sprintf(m_dispasswd + offset,"%c", '*');
			swprintf(disMenuBuf.osdBuffer_t[1].disMenu, 33, L"%s", m_dispasswd);		
		}
		else
			printf("password reached max length:128");
		
		printf("%s,%d,passwd=%s\n",__FILE__,__LINE__,m_passwd);
	}
	return;
}


void LevelOne::inputErrorPWOsd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"请输入密码呼出菜单", "密码输入错误","按回车后再次输入",  "按F2退出"};
	disMenuBuf.cnt = 4;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		
		disMenuBuf.osdBuffer_t[j].bshow = true;
		disMenuBuf.osdBuffer_t[j].alpha = 2;
		disMenuBuf.osdBuffer_t[j].color = 2;
		disMenuBuf.osdBuffer_t[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer_t[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer_t[j].disMenu, 33, L"%s", menubuf[j]);
	}
	disMenuBuf.osdBuffer_t[1].color = 3;
	disMenuBuf.osdBuffer_t[2].color = 3;
	return;
}

void LevelOne::inputPWOsd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = {"请输入密码呼出菜单", "", "按回车确认", "按F2退出"};
	disMenuBuf.cnt = 4;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer_t[j].bshow = true;
		disMenuBuf.osdBuffer_t[j].alpha = 2;
		disMenuBuf.osdBuffer_t[j].color = 2;
		disMenuBuf.osdBuffer_t[j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMenuBuf.osdBuffer_t[j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer_t[j].disMenu, 33, L"%s", menubuf[j]);
	}
	return;
}


void LevelOne::showOsd()
{
	if(m_menuShow)
		showMenuOsd(disMenuBuf);
	return;
}

