/*
 * state_linkAuto.cpp
 *
 *  Created on: 2019年8月27日
 *      Author: alex
 */

#include "state.hpp"

CLinkAuto::CLinkAuto()
{
}

CLinkAuto::~CLinkAuto()
{
}

void CLinkAuto::showOsd()
{
}

void CLinkAuto::inputNumber(char key)
{
}

void CLinkAuto::enter()
{
}

void CLinkAuto::operationChangeState()
{
	CMD_EXT stat;
	memset(&stat,0,sizeof(CMD_EXT));
	app_ctrl_getSysData(&stat);
	
	stat.MtdState[stat.SensorStat] = eImgAlg_Enable;
	app_ctrl_setMtdStat(&stat);

	return;
}

