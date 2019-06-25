/*
 * menu.cpp
 *
 *  Created on: 2019年6月25日
 *      Author: alex
 */


#include "link.hpp"
#include <string.h>

CLink::CLink()
{
	displayMode = MENU_MAIN_VIEW;
	g_AppWorkMode = HANDLE_LINK_MODE;
}

	
void CLink::init()
{
	
}

void CLink::setDisplayMode(MenuDisplay mode)
{
	displayMode = mode;
	return ;
}


void CLink::OnJosEvent(int key, int param)
{
	switch(key)
	{
		case JOSF1_OPEN_AUTOLINKMODE:
			{
				GB_WorkMode nextMode = (GB_WorkMode)(param - 1);
				setWorkMode(nextMode);
			}
			break;
		case JOSF2_ENTER_MENU:			
				app_ctrl_setMenu_jos(param);
			break;
		default:
			break;
	}
}

void CLink::setWorkMode(GB_WorkMode workmode)
{
	g_AppWorkMode = workmode;
	int value = 0;
	if( g_AppWorkMode == AUTO_LINK_MODE )
	{
		value = 1;
		//printf("\r\n[%s]:&&&&&&&&&&&&&&&&& (AUTO_LINK_MODE )\r\n",__func__);
	}
	else
	{
		value = 0;
	}

	if(g_AppWorkMode == AUTO_LINK_MODE)
	{
		//set_mouse_show(0);
	}
	else if(g_AppWorkMode == ONLY_BALL_MODE)
	{
		//set_mouse_show(0);
	}
	else if(g_AppWorkMode == HANDLE_LINK_MODE)
	{
		
	}	
	#if 0
	SENDST	tmp;
	tmp.cmd_ID = mtdmode;
	tmp.param[0] = value ;
	ipc_sendmsg(&tmp, IPC_FRIMG_MSG);	
	// waittochange
	#endif
}

void CLink::app_ctrl_setMenuStat(int index)
{
	menu_param_t *pMenuStatus = &msgextMenuCtrl;

	pMenuStatus->MenuStat = index;
	
	memset(pMenuStatus->Passwd, 0, sizeof(pMenuStatus->Passwd));
	memset(pMenuStatus->disPasswd, 0, sizeof(pMenuStatus->disPasswd));
	memset(disMenu[mainmenu0][1], 0, sizeof(disMenu[mainmenu0][1]));

	//MSGDRIV_send(MSGID_EXT_MENUSWITCH, 0);

	if(-1 != index)
		;//plat->set_mouse_show(0);
}

void CLink::app_ctrl_setMenu_jos(int menu_state)
{	
	if(1 == menu_state)
	{
		if(-1 == msgextMenuCtrl.MenuStat)
			app_ctrl_setMenuStat(mainmenu0);
	}
	else if(0 == menu_state)
	{
		app_ctrl_setMenuStat(-1);
	}
	return ;
}


