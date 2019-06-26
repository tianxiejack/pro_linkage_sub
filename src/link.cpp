/*
 * menu.cpp
 *
 *  Created on: 2019年6月25日
 *      Author: alex
 */


#include "link.hpp"
#include <string.h>
#include "locale.h"
#include <stdio.h>
#include <wchar.h>

CLink::CLink()
{
	displayMode = MENU_MAIN_VIEW;
	g_AppWorkMode = HANDLE_LINK_MODE;
	mouse_show = 0;
	oresoltype = r1920x1080_f60;
	curBaudRate = 9600;
	setrigion_flagv20 = 0;

	
}

	
void CLink::init()
{
	m_autofr = new CAutoManualFindRelation(outputWHF[0],outputWHF[1], 6, 6);
	menuOsdInit();


	
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
	
	m_menuindex = index;

	if(-1 != index)
		set_mouse_show(0);
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


void CLink::app_ctrl_setnumber(char key)
{
	menu_param_t *pMenuStatus = &msgextMenuCtrl;

	
	if(MENU_GUN == displayMode)
	{
		if(key == '0')
		{
			m_autofr->deleteallPoints();
			set_jos_mouse_mode(mouse_mode);
		}
		else if(key == '1')
		{
			m_autofr->writeParams();
		}
	}
	/*

	else if(mainmenu0 == pMenuStatus->MenuStat)
	{
		int offset = strlen(pMenuStatus->Passwd) * sizeof(char);		
		if(offset < sizeof(pMenuStatus->Passwd) - 1)
		{
			sprintf(pMenuStatus->Passwd + offset,"%c", key);
			sprintf(pMenuStatus->disPasswd + offset,"%c", '*');
			swprintf(plat->m_display.disMenu[mainmenu0][1], 33, L"%s", pMenuStatus->disPasswd);
		}
		else
			printf("password reached max length:128");
		
		printf("%s,%d,passwd=%s\n",__FILE__,__LINE__,pMenuStatus->Passwd);
	}
	else if(submenu_setimg == pMenuStatus->MenuStat)
	{
		if(key == '0')
		{
			if(plat->save_flag)
			{
				plat->dtimer.stopTimer(plat->resol_apply_id);
				pMenuStatus->resol_type_tmp = pMenuStatus->resol_type;
				plat->setresol(pMenuStatus->resol_type);
				plat->save_flag = 0;
				memset(plat->m_display.disMenu[submenu_setimg][4], 0, sizeof(plat->m_display.disMenu[submenu_setimg][4]));
				memset(plat->m_display.disMenu[submenu_setimg][5], 0, sizeof(plat->m_display.disMenu[submenu_setimg][5]));
				MSGDRIV_send(MSGID_EXT_SETRESOL, 0);
			}
		}
		else if(key == '1')
		{
			if(plat->save_flag)
			{
				plat->dtimer.stopTimer(plat->resol_apply_id);
				pMenuStatus->resol_type = pMenuStatus->resol_type_tmp;
				plat->save_flag = 0;
				memset(plat->m_display.disMenu[submenu_setimg][4], 0, sizeof(plat->m_display.disMenu[submenu_setimg][4]));
				memset(plat->m_display.disMenu[submenu_setimg][5], 0, sizeof(plat->m_display.disMenu[submenu_setimg][5]));
				MSGDRIV_send(MSGID_EXT_SAVERESOL, 0);
			}

		}
	}
	else if((1 == m_mtdSetRigion) && (key == '2'))
	{
		CMD_EXT tmpCmd = {0};
		tmpCmd.MtdSetRigion = 0;
		app_ctrl_setMtdRigionStat(&tmpCmd);
		app_ctrl_setMenuStat(submenu_mtd);
		g_displayMode = MENU_MAIN_VIEW;
		memset(plat->m_display.disMtd[0][4], 0, sizeof(plat->m_display.disMenu[0][4]));
	}
#if __POLYGON_MTD_ROI__
	else if((1 == m_mtdSetRigion) && (key == '1'))
	{
		memset(plat->pol_rectn, 0, sizeof(plat->pol_rectn));
		memset(plat->polRect, 0, sizeof(plat->polRect));
	}
#endif

	else if((submenu_mtd == pMenuStatus->MenuStat) && (pMenuStatus->mtdnum_deng == 1))
	{
		int offset = strlen(pMenuStatus->mtdnum_arr) * sizeof(char);
		if(offset < sizeof(pMenuStatus->mtdnum_arr) - 1)
			sprintf(pMenuStatus->mtdnum_arr + offset,"%c", key);
		else
			printf("mtdnum reached max length:128");

		int num = atoi(pMenuStatus->mtdnum_arr);
		pMenuStatus->osd_mudnum = atoi(pMenuStatus->mtdnum_arr);
		printf("%s,%d,osd_mudnum=%d\n",__FILE__,__LINE__,pMenuStatus->osd_mudnum);
		MSGDRIV_send(MSGID_EXT_SETMTDNUM, 0);
	}
	else if((submenu_mtd == pMenuStatus->MenuStat) && (pMenuStatus->trktime_deng == 1))
	{
		int offset = strlen(pMenuStatus->trktime_arr) * sizeof(char);
		if(offset < sizeof(pMenuStatus->trktime_arr) - 1)
			sprintf(pMenuStatus->trktime_arr + offset,"%c", key);
		else
			printf("trktime reached max length:128");

		int num = atoi(pMenuStatus->trktime_arr);
		pMenuStatus->osd_trktime = atoi(pMenuStatus->trktime_arr);
		printf("%s,%d,osd_trktime=%d\n",__FILE__,__LINE__,pMenuStatus->osd_trktime);
		MSGDRIV_send(MSGID_EXT_SETMTDTRKTIME, 0);
	}
	else if((submenu_mtd == pMenuStatus->MenuStat) && (pMenuStatus->maxsize_deng == 1))
	{
		int offset = strlen(pMenuStatus->maxsize_arr) * sizeof(char);
		if(offset < sizeof(pMenuStatus->maxsize_arr) - 1)
			sprintf(pMenuStatus->maxsize_arr + offset,"%c", key);
		else
			printf("maxsize reached max length:128");

		int num = atoi(pMenuStatus->maxsize_arr);
		pMenuStatus->osd_maxsize = atoi(pMenuStatus->maxsize_arr);
		printf("%s,%d,osd_maxsize=%d\n",__FILE__,__LINE__,pMenuStatus->osd_maxsize);
		MSGDRIV_send(MSGID_EXT_SETMTDMAXSIZE, 0);
	}
	else if((submenu_mtd == pMenuStatus->MenuStat) && (pMenuStatus->minsize_deng == 1))
	{
		int offset = strlen(pMenuStatus->minsize_arr) * sizeof(char);
		if(offset < sizeof(pMenuStatus->minsize_arr) - 1)
			sprintf(pMenuStatus->minsize_arr + offset,"%c", key);
		else
			printf("minsize reached max length:128");

		int num = atoi(pMenuStatus->minsize_arr);
		pMenuStatus->osd_minsize = atoi(pMenuStatus->minsize_arr);
		printf("%s,%d,osd_minsize=%d\n",__FILE__,__LINE__,pMenuStatus->osd_minsize);
		MSGDRIV_send(MSGID_EXT_SETMTDMINSIZE, 0);
	}
	else if((submenu_mtd == pMenuStatus->MenuStat) && (pMenuStatus->sensi_deng == 1))
	{
		int offset = strlen(pMenuStatus->sensi_arr) * sizeof(char);
		if(offset < sizeof(pMenuStatus->sensi_arr) - 1)
			sprintf(pMenuStatus->sensi_arr + offset,"%c", key);
		else
			printf("sensi reached max length:128");

		int num = atoi(pMenuStatus->sensi_arr);
		pMenuStatus->osd_sensi = atoi(pMenuStatus->sensi_arr);
		printf("%s,%d,osd_sensi=%d\n",__FILE__,__LINE__,pMenuStatus->osd_sensi);
		MSGDRIV_send(MSGID_EXT_SETMTDSENSI, 0);
	}
	*/
}



void CLink::menuOsdInit()
{
	m_menuindex = -1;
	memcpy(&dismenuarray, extMenuCtrl.menuarray, sizeof(dismenuarray));
	unsigned char menubuf[menumaxid][MAX_SUBMENU][128] = {
			{"请输入密码呼出菜单", "", "按回车确认", "按F2退出"},
			{"请输入密码呼出菜单", "密码输入错误，","按回车后再次输入", "按回车确认", "按F2退出"},
			/*{"内参标定","枪球画面标定","移动检测设置","画面设置","球机设定","固件升级","密码更改"},*/
			/*{"枪球自动标定","枪球手动标定","返回"},*/
			{"设置默认工作模式","枪球标定","移动检测设置","画面设置","球机设定","固件升级","密码更改"},
			/*{"网格标定","移动检测设置","画面设置","球机设定","固件升级","密码更改"},*/
			/*{"枪机内参标定","球机内参标定","返回"},*/
			{"手动联动模式","自动联动模式","单控球机模式","返回"},
		   /* {"   ","枪机画面网格标定","	"},*/
			{"检测区域选择","目标个数 	1","跟踪持续时间 1秒","最大目标面积 10000像素","最小目标面积 9像素","灵敏度		30","返回"},
			{"扫描方式均为逐行扫描","格式 1920x1080@60Hz","应用","返回"},
			{"使用串口设置","使用网络设置","返回"},
			{"波特率 	9600","球机地址   001","球机协议   PALCO-D","工作模式	485半双工","返回"},
			{"网络协议","IP地址","登录用户名","登录密码","返回"},
			{"手动选择特征点","开始标定","返回"}
};

	unsigned char mtdbuf[1][7][128] = {{"检测区域设置","用鼠标指针左键点选:增加点","按1删除所有点","按回车保存设置，按2退出"}};

	unsigned char resolbuf[maxresolid][128] = {
			"格式 1920x1080@60Hz","格式 1024x768@60Hz","格式 1280x1024@60Hz"};
	
	for(int i = 0; i < menumaxid; i++)
	{
		for(int j = 0; j < MAX_SUBMENU; j++)
		{
			if(j >= dismenuarray[i].submenu_cnt)
				break;
			disMenuBuf[i][j].alpha = 2;
			disMenuBuf[i][j].ctrl = 0;
			disMenuBuf[i][j].posx = (int)((float)outputWHF[0] *0.78125f);
			disMenuBuf[i][j].color = 2;
			disMenuBuf[i][j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
			setlocale(LC_ALL, "zh_CN.UTF-8");
			swprintf(disMenu[i][j], 33, L"%s", menubuf[i][j]);
			m_currentMenuPos[i][j].posX = disMenuBuf[i][j].posx;
			m_currentMenuPos[i][j].posY = disMenuBuf[i][j].posy;
		}
	}

//=====================================================================

		swprintf(disMenu[submenu_mtd][1], 33, L"目标个数	 %d", msgextMenuCtrl.osd_mudnum);
		swprintf(disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒",msgextMenuCtrl.osd_trktime);
		swprintf(disMenu[submenu_mtd][3], 33, L"最大目标面积 %d像素", msgextMenuCtrl.osd_maxsize);
		swprintf(disMenu[submenu_mtd][4], 33, L"最小目标面积 %d像素", msgextMenuCtrl.osd_minsize);
		swprintf(disMenu[submenu_mtd][5], 33, L"灵敏度		%d", msgextMenuCtrl.osd_sensi);

//=====================================================================

	for(int j = 0; j < 5; j++)
	{
		disMtdBuf[0][j].alpha = 2;
		disMtdBuf[0][j].ctrl = 0;
		disMtdBuf[0][j].posx = (int)((float)outputWHF[0] *0.78125f);
		disMtdBuf[0][j].color = 2;
		disMtdBuf[0][j].posy = (j + 1) * (int)((float)outputWHF[1] *0.056f);
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMtd[0][j], 33, L"%s", mtdbuf[0][j]);
	}

	swprintf(disMenu[submenu_setimg][1], 33, L"%s", resolbuf[oresoltype]);

	disMenuBuf[mainmenu0][2].posy = 4 * (int)((float)outputWHF[1] *0.056f);
	disMenuBuf[mainmenu0][3].posy = 5 * (int)((float)outputWHF[1] *0.056f);
	
	disMenuBuf[submenu_setball][2].posy = 4 * (int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setcom][4].posy = 6 * (int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setnet][4].posy = 6 * (int)((float)outputWHF[1] *0.056f);
	m_currentMenuPos[submenu_setball][2].posY = 4 * (int)((float)outputWHF[1] *0.056f); 
	m_currentMenuPos[submenu_setcom][4].posY = 6 * (int)((float)outputWHF[1] *0.056f);

	disMtdBuf[0][0].posx = outputWHF[0]/2 - strlen((char *)mtdbuf[0][0])*3;
	disMtdBuf[0][1].posx = outputWHF[0]/2 - strlen((char *)mtdbuf[0][1])*3;
	disMtdBuf[0][2].posx = outputWHF[0]/2 - strlen((char *)mtdbuf[0][2])*3;
	disMtdBuf[0][3].posx = outputWHF[0]/2 - strlen((char *)mtdbuf[0][3])*3;
	disMtdBuf[0][4].posx = (int)((float)outputWHF[0] *0.833f);
	disMtdBuf[0][4].posy =	(int)((float)outputWHF[1] *0.056f);
	disMtdBuf[0][0].color = 1;
	disMtdBuf[0][1].color = 1;
	disMtdBuf[0][2].color = 1;
	disMtdBuf[0][3].color = 1;
	disMtdBuf[0][4].color = 3;

	disMenuBuf[mainmenu1][1].color= 3;
	disMenuBuf[mainmenu1][2].color= 3;
	disMenuBuf[mainmenu2][0].color= 3;
	disMenuBuf[submenu_DefaultWorkMode][0].color= 3;
	//disMenuBuf[submenu_gridMapCalibrate][0].color= 3;
	disMenuBuf[submenu_mtd][0].color= 3;
	disMenuBuf[submenu_setball][0].color= 3;
	disMenuBuf[submenu_setcom][0].color= 3;
	disMenuBuf[submenu_setnet][0].color= 3;

	disMenuBuf[submenu_setimg][4].posx = (int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setimg][4].posy = 6*(int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setimg][5].posx = (int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setimg][5].posy = 7*(int)((float)outputWHF[1] *0.056f);
	disMenuBuf[submenu_setimg][4].color = 3;
	disMenuBuf[submenu_setimg][5].color = 3;

#if 1
		switch(curBaudRate){
			case 2400:
				//disbaud_type = 0;
				setlocale(LC_ALL, "zh_CN.UTF-8");
				swprintf(disMenu[submenu_setcom][0] , 33, L"%s","波特率	   2400" );
				break;
			case 4800:
				//disbaud_type = 1;
				setlocale(LC_ALL, "zh_CN.UTF-8");
				swprintf(disMenu[submenu_setcom][0] , 33, L"%s","波特率	   4800" );
				break;
			case 9600:
				//disbaud_type = 2;
				setlocale(LC_ALL, "zh_CN.UTF-8");
				swprintf(disMenu[submenu_setcom][0] , 33, L"%s","波特率	   9600" );
				break;
			case 115200:
				//disbaud_type = 3;
				setlocale(LC_ALL, "zh_CN.UTF-8");
				swprintf(disMenu[submenu_setcom][0] , 33, L"%s","波特率	   115200" );
				break;
			default:
				break;
		}
#endif	
}


void CLink::menuLoadIpcParam(int* config)
{
	extMenuCtrl.resol_type_tmp = extMenuCtrl.resol_type = oresoltype;
	extMenuCtrl.MenuStat = -1;
	extMenuCtrl.Trig_Inter_Mode = 0;
	memset(extMenuCtrl.Passwd, 0, sizeof(extMenuCtrl.Passwd));
	memset(extMenuCtrl.disPasswd, 0, sizeof(extMenuCtrl.disPasswd));

	int cnt[menumaxid] = {4,5,3,4,7,6,3,5,5,3}; 
	memset(extMenuCtrl.menuarray, 0, sizeof(extMenuCtrl.menuarray));
	for(int i = 0; i < menumaxid; i++)
	{
		extMenuCtrl.menuarray[i].id = i;
		extMenuCtrl.menuarray[i].pointer = 0;
		extMenuCtrl.menuarray[i].submenu_cnt = cnt[i];
	}
		
	if(config == NULL)
	{
		extMenuCtrl.osd_mudnum = 1;
		extMenuCtrl.osd_trktime = 1;
		extMenuCtrl.osd_maxsize = 10000;
		extMenuCtrl.osd_minsize = 9;
		extMenuCtrl.osd_sensi = 30; 				
	}else{	
		extMenuCtrl.osd_mudnum = config[CFGID_MTD_detnum];
		extMenuCtrl.osd_trktime = config[CFGID_MTD_maxtrk];
		extMenuCtrl.osd_maxsize = config[CFGID_MTD_maxpixel];
		extMenuCtrl.osd_minsize = config[CFGID_MTD_minpixel];
		extMenuCtrl.osd_sensi = config[CFGID_INPUT_SENISIVITY(CFGID_INPUT1_BKID)];
	}
	return ;
}


void CLink::set_jos_mouse_mode(jos_mouse_Mode mode)
{
	printf("%s, %d,set_jos_mouse_mode=%d\n", __FILE__,__LINE__, mode);
	set_josctrl_mode(mode);
	#if 0
	SENDST trkmsg2={0};
	trkmsg2.cmd_ID = jos_mouse_mode;
	trkmsg2.param[0] = mode;
	ipc_sendmsg(&trkmsg2, IPC_FRIMG_MSG);
	waittochange
	#endif
}



