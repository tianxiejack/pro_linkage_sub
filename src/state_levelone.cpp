/*
 * state_levelone.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelOne::LevelOne():inputtingStat(true)
{
	init_passwd ="0000";
	initOsd();
}

LevelOne::~LevelOne()
{
}

void LevelOne::initOsd()
{

	return;
}

void LevelOne::enter()
{
	if(inputtingStat)
	{
		if(strcmp(init_passwd, m_passwd)){
			inputtingStat = true;
			printf("watching :::!!!!!!! enter next state \n");
		}
		else{
			inputtingStat = false;
			inputErrorPWOsd();
		}
	}else{
		inputPWOsd();
		inputtingStat = true;
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
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"请输入密码呼出菜单", "密码输入错误，","按回车后再次输入", "按回车确认", "按F2退出"};
	disMenuBuf.cnt = 5;
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
	showMenuOsd();
	return;
}


void LevelOne::showMenuOsd()
{
	unsigned char r, g, b, a, color;
	short x, y;

	char font = 1;
	char fontsize = 4;

	for(int i = 0; i < disMenuBuf.cnt; i++)
	{
		if(disMenuBuf.osdBuffer_t[i].bshow)
		{
			x = disMenuBuf.osdBuffer_t[i].posx;
			y = disMenuBuf.osdBuffer_t[i].posy;
			a = disMenuBuf.osdBuffer_t[i].alpha;
			color = disMenuBuf.osdBuffer_t[i].color;
			getRGBA(color,r,g,b,a);
			drawFunc(x, y,disMenuBuf.osdBuffer_t[i].disMenu, font ,fontsize, r, g, b, a, VIDEO_DIS_WIDTH, VIDEO_DIS_HEIGHT);
		}
	}
	return;
}


