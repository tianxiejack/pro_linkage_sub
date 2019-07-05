/*
 * state_leveltwo.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelTwo::LevelTwo()
{
	initOsd();
}

LevelTwo::~LevelTwo()
{

}

void LevelTwo::showOsd()
{
	showMenuOsd(disMenuBuf);
}

void LevelTwo::inputNumber(char key)
{
	
}

void LevelTwo::enter()
{
	
}


void LevelTwo::initOsd()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"设置默认工作模式","枪球标定","移动检测设置"};
	disMenuBuf.cnt = 3;
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


void LevelTwo::buttonMenu()
{
	ChangeState(LEVELONE);
	return;
}


