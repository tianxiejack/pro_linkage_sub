/*
 * state_levelone.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelOne::LevelOne()
{
	initOsd();
}

LevelOne::~LevelOne()
{
}

void LevelOne::initOsd()
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


