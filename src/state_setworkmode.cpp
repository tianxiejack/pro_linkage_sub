/*
 * state_setworkmode.cpp
 *
 *  Created on: 2019年7月5日
 *      Author: alex
 */



#include "state.hpp"

CSetWorkMode::CSetWorkMode():m_menuPointer(0)
{
	initOsd();
}

CSetWorkMode::~CSetWorkMode()
{
}

void CSetWorkMode::showOsd()
{
	showMenuOsd(&disMenuBuf);
	return;
}


void CSetWorkMode::enter()
{
	
}

void CSetWorkMode::buttonMenu()
{
}

void CSetWorkMode::operationChangeState()
{
}

void CSetWorkMode::upMenu()
{
}

void CSetWorkMode::downMenu()
{
}


void CSetWorkMode::initOsd()
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


