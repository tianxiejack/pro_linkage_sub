/*
 * state_linkCalib.cpp
 *
 *  Created on: 2019年7月16日
 *      Author: alex
 */

#include "state.hpp"


CLinkCalib::CLinkCalib()
{
}

CLinkCalib::~CLinkCalib()
{
}

void CLinkCalib::showOsd()
{
	m_pMenu->showMenuOsd();
}

void CLinkCalib::inputNumber(char key)
{
	m_pMenu->normalKey(key);
}

void CLinkCalib::enter()
{
	//m_pMenu->enter();
	//add point and add checked pos information
	SENDST trkmsg={0};
	trkmsg.cmd_ID = querypos;
	ipc_sendmsg(IPC_FRIMG_MSG,&trkmsg);
	printf("send query pos \n");
	return;
}

void CLinkCalib::buttonMenu()
{
	m_pMenu->menuButton();	

	m_pMenu->gotoMainMenu();
	m_pMenu->changeStatFunc(LINKMANUAL);
	m_pMenu->changeDisModeFunc(MAIN_VIEW);	
}
