/*
 * state_leveltwo.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelTwo::LevelTwo():m_menuPointer(0)
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
	switch(m_menuPointer)
	{
		case 0:
			ChangeState(char nextState);
			break;

	}
}


void LevelTwo::initOsd()
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


void LevelTwo::operationChangeState()
{
	m_menuPointer = 0;
	return;
}


void LevelTwo::buttonMenu()
{
	ChangeState(LEVELONE);
	return;
}


void LevelTwo::upMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (m_menuPointer+disMenuBuf.cnt-1)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}

void LevelTwo::downMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (++m_menuPointer)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;	
}


