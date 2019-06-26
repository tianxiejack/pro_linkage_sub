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

bool CLink::storeWorkModeFlag = false;
bool CLink::storeMtdConfigFlag = false;

CLink::CLink()
{
	displayMode = MENU_MAIN_VIEW;
	g_AppWorkMode = HANDLE_LINK_MODE;
	mouse_show = 0;
	oresoltype = r1920x1080_f60;
	curBaudRate = 9600;
	setrigion_flagv20 = 0;
	setting_WorkMode = HANDLE_LINK_MODE;
	
	mouse_workmode = DrawRectangle_Mode;
	m_josMode = mouse_mode;

	init_passwd ="0000";

	draw_pip_stat = false;
	drawpoints_stat = false;

	
}

	
void CLink::init(OSDFUNC func)
{
	m_autofr = new CAutoManualFindRelation(outputWHF[0],outputWHF[1], 6, 6);
	menuOsdInit();
	drawtext = func;
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
	menu_param_t *pMenuStatus = &m_menuCtrl;
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
		if(-1 == m_menuCtrl.MenuStat)
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
	if(MENU_GUN == displayMode)
	{
		if(key == '0')
		{
			m_autofr->deleteallPoints();
			set_jos_mouse_mode(mouse_mode);
		}
		else if(key == '1')
			m_autofr->writeParams();
	}
	else if(mainmenu0 == m_menuCtrl.MenuStat)
	{
		int offset = strlen(m_menuCtrl.Passwd) * sizeof(char);		
		if(offset < sizeof(m_menuCtrl.Passwd) - 1)
		{
			sprintf(m_menuCtrl.Passwd + offset,"%c", key);
			sprintf(m_menuCtrl.disPasswd + offset,"%c", '*');
			swprintf(disMenu[mainmenu0][1], 33, L"%s", m_menuCtrl.disPasswd);
		}
		else
			printf("password reached max length:128");
		
		printf("%s,%d,passwd=%s\n",__FILE__,__LINE__,m_menuCtrl.Passwd);
	}
	else if(submenu_setimg == m_menuCtrl.MenuStat)
	{
	}
	else if((1 == m_mtdSetRigion) && (key == '2'))
	{
		app_ctrl_setMtdRigionStat(0);
		app_ctrl_setMenuStat(submenu_mtd);
		
		displayMode = MENU_MAIN_VIEW;
		memset(disMtd[0][4], 0, sizeof(disMenu[0][4]));
	}
	else if((1 == m_mtdSetRigion) && (key == '1'))
	{
		memset(pol_rectn, 0, sizeof(pol_rectn));
		memset(polRect, 0, sizeof(polRect));
	}
	else if((submenu_mtd == m_menuCtrl.MenuStat) && (m_menuCtrl.mtdnum_deng == 1))
		set_mtd_num(key);	
	else if((submenu_mtd == m_menuCtrl.MenuStat) && (m_menuCtrl.trktime_deng == 1))
		set_mtd_trktime(key);
	else if((submenu_mtd == m_menuCtrl.MenuStat) && (m_menuCtrl.maxsize_deng == 1))
		set_mtd_maxsize(key);
	else if((submenu_mtd == m_menuCtrl.MenuStat) && (m_menuCtrl.minsize_deng == 1))
		set_mtd_minsize(key);
	else if((submenu_mtd == m_menuCtrl.MenuStat) && (m_menuCtrl.sensi_deng == 1))
		set_mtd_sensi(key);

	return ;
}


void CLink::menuOsdInit()
{
	m_menuindex = -1;
	memcpy(&dismenuarray, m_menuCtrl.menuarray, sizeof(dismenuarray));
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

		swprintf(disMenu[submenu_mtd][1], 33, L"目标个数	 %d", m_menuCtrl.osd_mudnum);
		swprintf(disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒",m_menuCtrl.osd_trktime);
		swprintf(disMenu[submenu_mtd][3], 33, L"最大目标面积 %d像素", m_menuCtrl.osd_maxsize);
		swprintf(disMenu[submenu_mtd][4], 33, L"最小目标面积 %d像素", m_menuCtrl.osd_minsize);
		swprintf(disMenu[submenu_mtd][5], 33, L"灵敏度		%d", m_menuCtrl.osd_sensi);

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
	if(m_config == NULL)
		m_config = config;
	m_menuCtrl.resol_type_tmp = m_menuCtrl.resol_type = oresoltype;
	m_menuCtrl.MenuStat = -1;
	m_menuCtrl.Trig_Inter_Mode = 0;
	memset(m_menuCtrl.Passwd, 0, sizeof(m_menuCtrl.Passwd));
	memset(m_menuCtrl.disPasswd, 0, sizeof(m_menuCtrl.disPasswd));

	int cnt[menumaxid] = {4,5,3,4,7,6,3,5,5,3}; 
	memset(m_menuCtrl.menuarray, 0, sizeof(m_menuCtrl.menuarray));
	for(int i = 0; i < menumaxid; i++)
	{
		m_menuCtrl.menuarray[i].id = i;
		m_menuCtrl.menuarray[i].pointer = 0;
		m_menuCtrl.menuarray[i].submenu_cnt = cnt[i];
	}
		
	if(config == NULL)
	{
		m_menuCtrl.osd_mudnum = 1;
		m_menuCtrl.osd_trktime = 1;
		m_menuCtrl.osd_maxsize = 10000;
		m_menuCtrl.osd_minsize = 9;
		m_menuCtrl.osd_sensi = 30; 				
	}else{	
		m_menuCtrl.osd_mudnum  = m_config[CFGID_MTD_detnum];
		m_menuCtrl.osd_trktime = m_config[CFGID_MTD_maxtrk];
		m_menuCtrl.osd_maxsize = m_config[CFGID_MTD_maxpixel];
		m_menuCtrl.osd_minsize = m_config[CFGID_MTD_minpixel];
		m_menuCtrl.osd_sensi   = m_config[CFGID_INPUT_SENISIVITY(CFGID_INPUT1_BKID)];
	}
	minsize = m_menuCtrl.osd_minsize;
	maxsize = m_menuCtrl.osd_maxsize;
	sensi = m_menuCtrl.osd_sensi;
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


void CLink::set_mtd_num(char key)
{
	int offset = strlen(m_menuCtrl.mtdnum_arr) * sizeof(char);
	if(offset < sizeof(m_menuCtrl.mtdnum_arr) - 1)
		sprintf(m_menuCtrl.mtdnum_arr + offset,"%c", key);
	else
		printf("mtdnum reached max length:128");
	
	int num = atoi(m_menuCtrl.mtdnum_arr);
	m_menuCtrl.osd_mudnum = atoi(m_menuCtrl.mtdnum_arr);
	printf("%s,%d,osd_mudnum=%d\n",__FILE__,__LINE__,m_menuCtrl.osd_mudnum);
	set_mtd_num_osd();
	return ;
}

void CLink::set_mtd_num_osd()
{
	if((m_menuCtrl.osd_mudnum < MIN_MTDTARGET_NUM) || (m_menuCtrl.osd_mudnum > MAX_MTDTARGET_NUM))
		swprintf(disMenu[submenu_mtd][1], 33, L"目标个数	   %d(超出范围%d~%d)", m_menuCtrl.osd_mudnum,MIN_MTDTARGET_NUM,MAX_MTDTARGET_NUM);
	else
		swprintf(disMenu[submenu_mtd][1], 33, L"目标个数	   %d", m_menuCtrl.osd_mudnum);
	return ;
}


void CLink::set_mtd_trktime(char key)
{
	int offset = strlen(m_menuCtrl.trktime_arr) * sizeof(char);
	if(offset < sizeof(m_menuCtrl.trktime_arr) - 1)
		sprintf(m_menuCtrl.trktime_arr + offset,"%c", key);
	else	
		printf("trktime reached max length:128");

	int num = atoi(m_menuCtrl.trktime_arr);
	m_menuCtrl.osd_trktime = atoi(m_menuCtrl.trktime_arr);
	printf("%s,%d,osd_trktime=%d\n",__FILE__,__LINE__,m_menuCtrl.osd_trktime);
	set_mtd_trktime_osd();
	return ;
}


void CLink::set_mtd_trktime_osd()
{
	if((m_menuCtrl.osd_trktime < MIN_MTDTRKTIME) || (m_menuCtrl.osd_trktime > MAX_MTDTRKTIME))
		swprintf(disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒(超出范围%d~%d)", m_menuCtrl.osd_trktime,MIN_MTDTRKTIME,MAX_MTDTRKTIME);
	else
		swprintf(disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒", m_menuCtrl.osd_trktime);
	return ;
}


void CLink::set_mtd_maxsize(char key)
{
	int offset = strlen(m_menuCtrl.maxsize_arr) * sizeof(char);
	if(offset < sizeof(m_menuCtrl.maxsize_arr) - 1)
		sprintf(m_menuCtrl.maxsize_arr + offset,"%c", key);
	else
		printf("maxsize reached max length:128");

	int num = atoi(m_menuCtrl.maxsize_arr);
	m_menuCtrl.osd_maxsize = atoi(m_menuCtrl.maxsize_arr);
	printf("%s,%d,osd_maxsize=%d\n",__FILE__,__LINE__,m_menuCtrl.osd_maxsize);
	set_mtd_maxsize_osd();
	return ;
}

void CLink::set_mtd_maxsize_osd()
{
	if((m_menuCtrl.osd_maxsize < MIN_MTDMINSIZE) || (m_menuCtrl.osd_maxsize > MAX_MTDMAXSIZE))
		swprintf(disMenu[submenu_mtd][3], 33, L"最大目标面积 %d(超出范围)", m_menuCtrl.osd_maxsize);
	else
		swprintf(disMenu[submenu_mtd][3], 33, L"最大目标面积 %d", m_menuCtrl.osd_maxsize);
	return;
}


void CLink::set_mtd_minsize(char key)
{
	int offset = strlen(m_menuCtrl.minsize_arr) * sizeof(char);
	if(offset < sizeof(m_menuCtrl.minsize_arr) - 1)
		sprintf(m_menuCtrl.minsize_arr + offset,"%c", key);
	else
		printf("minsize reached max length:128");

	int num = atoi(m_menuCtrl.minsize_arr);
	m_menuCtrl.osd_minsize = atoi(m_menuCtrl.minsize_arr);
	printf("%s,%d,osd_minsize=%d\n",__FILE__,__LINE__,m_menuCtrl.osd_minsize);
	set_mtd_minsize_osd();
	return ;
}

void CLink::set_mtd_minsize_osd()
{
	if((m_menuCtrl.osd_minsize < MIN_MTDMINSIZE) || (m_menuCtrl.osd_minsize > MAX_MTDMAXSIZE))
		swprintf(disMenu[submenu_mtd][4], 33, L"最小目标面积 %d(超出范围)", m_menuCtrl.osd_minsize);
	else
		swprintf(disMenu[submenu_mtd][4], 33, L"最小目标面积 %d", m_menuCtrl.osd_minsize);
	return;
}


void CLink::set_mtd_sensi(char key)
{
	int offset = strlen(m_menuCtrl.sensi_arr) * sizeof(char);
	if(offset < sizeof(m_menuCtrl.sensi_arr) - 1)
		sprintf(m_menuCtrl.sensi_arr + offset,"%c", key);
	else
		printf("sensi reached max length:128");

	int num = atoi(m_menuCtrl.sensi_arr);
	m_menuCtrl.osd_sensi = atoi(m_menuCtrl.sensi_arr);
	printf("%s,%d,osd_sensi=%d\n",__FILE__,__LINE__,m_menuCtrl.osd_sensi);
	set_mtd_sensi_osd();
	return ;
}

void CLink::set_mtd_sensi_osd()
{
	if((m_menuCtrl.osd_sensi < MIN_MTDSENSI) || (m_menuCtrl.osd_sensi > MAX_MTDSENSI))
		swprintf(disMenu[submenu_mtd][5], 33, L"灵敏度       %d(超出范围%d~%d)", m_menuCtrl.osd_sensi,MIN_MTDSENSI,MAX_MTDSENSI);
	else
		swprintf(disMenu[submenu_mtd][5], 33, L"灵敏度       %d", m_menuCtrl.osd_sensi);
	return;
}

void CLink::queryCurBallCamPosition()
{
	int flag =0;	
	int ret =0;
	#if 0
	SENDST trkmsg={0};
	trkmsg.cmd_ID = querypos;
	ipc_sendmsg(&trkmsg, IPC_FRIMG_MSG);
	printf("\r\n[%s]:Send Query PTZ Command ... ... \r\n",__func__);
	waittochange
	#endif
	return;
}

void CLink::SetDefaultWorkMode( GB_WorkMode workmode )
{
	int workModeType = 0;
	switch(workmode)
	{
		case HANDLE_LINK_MODE:
			workModeType = 0;
			break;
		case AUTO_LINK_MODE:
			workModeType = 1;
			break;
		case ONLY_BALL_MODE:
			workModeType = 2;
			break;
		default:
			workModeType = 0;
			break;			
	}
	#if 0
	SENDST	tmp = {0};
	tmp.cmd_ID = storeDefaultWorkMode;
	tmp.param[0] = workModeType+1;
	ipc_sendmsg(&tmp, IPC_FRIMG_MSG);
	waittochange
	#endif
	printf("\r\n[%s]:Send Set Default Work Mode <%d> to Linkage ctrl process !!!\r\n",__func__, workModeType );
	return ;
}


void CLink::save_polygon_roi()
{
	/*
	unsigned int curId = m_curChId;
	float floatx,floaty;
	int setx, sety = 0;
	int areanum = 1;

	if(pol_rectn[curId] >= 3)
		swprintf(disMtd[0][4], 33, L"点数:%d,保存成功", pol_rectn[curId]);
	else{
		swprintf(disMtd[0][4], 33, L"点数小于3,保存失败");
		return -1;
	}

	polyWarnRoi.resize(areanum);
	edge_contours.resize(areanum);

	for(int i = 0; i < areanum; i++)
	{
		polyWarnRoi[i].resize(pol_rectn[curId]);
		edge_contours[i].resize(pol_rectn[curId]);
		for(int j = 0; j < pol_rectn[curId]; j++)
		{
			floatx = polRect[curId][j].x;
			floaty = polRect[curId][j].y;
			map1080p2normal_point(&floatx, &floaty);
			mapnormal2curchannel_point(&floatx, &floaty, vdisWH[curId][0], vdisWH[curId][1]);

			setx = floatx;
			sety = floaty;
			polyWarnRoi[i][j] = cv::Point(setx, sety);

			mapfullscreen2gun_pointv20(&setx, &sety);
			edge_contours[i][j].x = setx;
			edge_contours[i][j].y = sety;
		}
	}

	if(polyWarnRoi.size() != 0)
	{
		SaveMtdSelectArea("SaveMtdArea.yml", polyWarnRoi);
	}

	printf("polygon mtd area num:%d\n", polyWarnRoi.size());
	for(int i = 0; i< polyWarnRoi.size(); i++)
	{
		for(int j = 0; j < polyWarnRoi[i].size(); j++)
			printf("(%d, %d),", polyWarnRoi[i][j].x,polyWarnRoi[i][j].y);
		printf("\n");
	}

	for(int i = 0; i < areanum; i++)
	{
		m_pMovDetector->setWarningRoi(polyWarnRoi[i], i);
	}

	edge_contours_notMap = polyWarnRoi ; 
	*/
}


void CLink::app_ctrl_settrig_inter(menu_param_t *pInCmd)
{
	menu_param_t *pMenuStatus = &m_menuCtrl;
	
	if(pMenuStatus->Trig_Inter_Mode != pInCmd->Trig_Inter_Mode)
	{
		pMenuStatus->Trig_Inter_Mode = pInCmd->Trig_Inter_Mode;
		if(pMenuStatus->Trig_Inter_Mode)
		{
			displayMode = MENU_GUN;
			set_showpip_stat(true);
			set_drawpoints_stat(true);
		}
		else
		{
			displayMode = MENU_MAIN_VIEW;
			set_drawpoints_stat(false);
		}
	}
	return ;
}


void CLink::menu0_handle()
{
	if(strcmp(init_passwd, m_menuCtrl.Passwd))
		app_ctrl_setMenuStat(mainmenu1);
	else
		app_ctrl_setMenuStat(mainmenu2);
}

void CLink::menu1_handle()
{
	app_ctrl_setMenuStat(mainmenu0);
}

void CLink::menu2_handle()
{
	switch(m_menuCtrl.menuarray[mainmenu2].pointer)
	{
		case 0:
			app_ctrl_setMenuStat(submenu_DefaultWorkMode);
			break;
		case 1:	
			if(AUTO_LINK_MODE != g_AppWorkMode){
				menu_param_t tmpMenuCmd = {0};
				tmpMenuCmd.Trig_Inter_Mode = 1;
				app_ctrl_settrig_inter(&tmpMenuCmd);
				set_jos_mouse_mode(mouse_mode);
				app_ctrl_setMenuStat(-1);
			}
			break;
		case 2:
			app_ctrl_setMenuStat(submenu_mtd);
			break;
		case 3:
			app_ctrl_setMenuStat(submenu_setimg);
			break;
		case 4:
			app_ctrl_setMenuStat(submenu_setball);
			break;
		default:
			break;
	}
	return;
}

void CLink::submenu_DefaultWorkMode_handle()
{
	if(INDEX_FOURTH == m_menuCtrl.menuarray[submenu_DefaultWorkMode].pointer) {
		app_ctrl_setMenuStat(mainmenu2);
		displayMode = MENU_MAIN_VIEW;	
	}
	else if(INDEX_FIRST == m_menuCtrl.menuarray[submenu_DefaultWorkMode].pointer) {
		setting_WorkMode = HANDLE_LINK_MODE;		
		storeWorkModeFlag = true;
	}
	else if(INDEX_SECOND == m_menuCtrl.menuarray[submenu_DefaultWorkMode].pointer) {
		setting_WorkMode = AUTO_LINK_MODE;
		storeWorkModeFlag = true;
	}
	else if(INDEX_THIRD == m_menuCtrl.menuarray[submenu_DefaultWorkMode].pointer) {
		setting_WorkMode = ONLY_BALL_MODE;
		storeWorkModeFlag = true;
	}	
	if(storeWorkModeFlag == true){
		storeWorkModeFlag = false;
		SetDefaultWorkMode(setting_WorkMode);
	}
}


void CLink::submenu_mtd_handle()
{
	if(0 == m_menuCtrl.menuarray[submenu_mtd].pointer){
		if(0 == m_config[CFGID_RTS_mtden])
		{
			mouse_workmode = SetMteRigion_Mode;
			app_ctrl_setMtdRigionStat(1);
			displayMode = MENU_GUN;
		}
	}
	else if(1 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		m_menuCtrl.mtdnum_deng = !m_menuCtrl.mtdnum_deng;
		if(m_menuCtrl.mtdnum_deng)
			m_dtimer.startTimer(mtdnum_light_id,500);
		else
		{
			m_dtimer.stopTimer(mtdnum_light_id);
			set_mtd_num_osd();
			if((m_menuCtrl.osd_mudnum >= MIN_MTDTARGET_NUM) && (m_menuCtrl.osd_mudnum <= MAX_MTDTARGET_NUM))
			{
				m_config[CFGID_MTD_detnum] = m_menuCtrl.osd_mudnum;
				storeMtdConfigFlag = true;
			}
			memset(m_menuCtrl.mtdnum_arr, 0, sizeof(m_menuCtrl.mtdnum_arr));
		}
	}
	else if(2 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		m_menuCtrl.trktime_deng = !m_menuCtrl.trktime_deng;
		if(m_menuCtrl.trktime_deng)
			m_dtimer.startTimer(trktime_light_id,500);
		else
		{
			m_dtimer.stopTimer(trktime_light_id);
			set_mtd_trktime_osd();
			if((m_menuCtrl.osd_trktime >= MIN_MTDTRKTIME) && (m_menuCtrl.osd_trktime <= MAX_MTDTRKTIME))
			{
				processdurationMenu_osd(m_menuCtrl.osd_trktime);
				m_config[CFGID_MTD_maxtrk] = m_menuCtrl.osd_trktime;
				storeMtdConfigFlag = true;
			}
			memset(m_menuCtrl.trktime_arr, 0, sizeof(m_menuCtrl.trktime_arr));
		}
	}
	
	else if(3 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		m_menuCtrl.maxsize_deng = !m_menuCtrl.maxsize_deng;
		if(m_menuCtrl.maxsize_deng)
			m_dtimer.startTimer(maxsize_light_id,500);
		else
		{
			m_dtimer.stopTimer(maxsize_light_id);
			set_mtd_maxsize_osd();
			if((m_menuCtrl.osd_maxsize >= minsize) && (m_menuCtrl.osd_maxsize <= MAX_MTDMAXSIZE))
			{
				maxsize = m_menuCtrl.osd_maxsize;
				m_config[CFGID_MTD_maxpixel] = m_menuCtrl.osd_maxsize;
				storeMtdConfigFlag = true;
			}
			memset(m_menuCtrl.maxsize_arr, 0, sizeof(m_menuCtrl.maxsize_arr));
		}
	}
	
	else if(4 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		m_menuCtrl.minsize_deng = !m_menuCtrl.minsize_deng;
		if(m_menuCtrl.minsize_deng)
			m_dtimer.startTimer(minsize_light_id,500);
		else
		{
			m_dtimer.stopTimer(minsize_light_id);
			set_mtd_sensi_osd();
			if((m_menuCtrl.osd_minsize >= MIN_MTDMINSIZE) && (m_menuCtrl.osd_minsize <= maxsize))
			{
				minsize = m_menuCtrl.osd_minsize;
				m_config[CFGID_MTD_minpixel] = m_menuCtrl.osd_minsize;
				storeMtdConfigFlag = true;
			}
			memset(m_menuCtrl.minsize_arr, 0, sizeof(m_menuCtrl.minsize_arr));
		}
	}
	else if(5 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		m_menuCtrl.sensi_deng = !m_menuCtrl.sensi_deng;
		if(m_menuCtrl.sensi_deng)
			m_dtimer.startTimer(sensi_light_id,500);
		else
		{
			m_dtimer.stopTimer(sensi_light_id);
			set_mtd_sensi_osd();
			if((m_menuCtrl.osd_sensi >= MIN_MTDSENSI) && (m_menuCtrl.osd_sensi <= MAX_MTDSENSI))
			{
				sensi = m_menuCtrl.osd_sensi;
				m_config[CFGID_INPUT_SENISIVITY(CFGID_INPUT1_BKID)] = m_menuCtrl.osd_sensi;
				storeMtdConfigFlag = true;
			}
			memset(m_menuCtrl.sensi_arr, 0, sizeof(m_menuCtrl.sensi_arr));
		}
	}
	else if(6 == m_menuCtrl.menuarray[submenu_mtd].pointer)
	{
		app_ctrl_setMenuStat(mainmenu2);
		storeMtdConfigFlag = true;
	}
	
	if(storeMtdConfigFlag == true)
	{
		storeMtdConfigFlag = false;
		sendIpcSaveMtd();
	}
	return ;
}


void CLink::app_ctrl_enter()
{
	if(MENU_GUN == displayMode)
		queryCurBallCamPosition();
	else if(1 == m_mtdSetRigion)
		;//save_polygon_roi();
	else if(mainmenu0 == m_menuCtrl.MenuStat)
		menu0_handle();
	else if(mainmenu1 == m_menuCtrl.MenuStat)
		menu1_handle();
	else if(mainmenu2 == m_menuCtrl.MenuStat)
		menu2_handle();
	else if(submenu_DefaultWorkMode == m_menuCtrl.MenuStat)
		submenu_DefaultWorkMode_handle();
	else if(submenu_mtd == m_menuCtrl.MenuStat)
		submenu_mtd_handle();
	
	/*
	else if(submenu_setimg == pMenuStatus->MenuStat)
	{
		if(1 == pMenuStatus->menuarray[submenu_setimg].pointer)
		{
			pMenuStatus->resol_deng = !pMenuStatus->resol_deng;
			if(pMenuStatus->resol_deng)
				plat->dtimer.startTimer(plat->resol_light_id,500);
			else
			{
				plat->dtimer.stopTimer(plat->resol_light_id);
				MSGDRIV_send(MSGID_EXT_SETRESOL, 0);
			}
		}
		else if(2 == pMenuStatus->menuarray[submenu_setimg].pointer)
		{	
			plat->setresol(pMenuStatus->resol_type_tmp);
			plat->save_flag = 1;
			plat->cnt_down = 10;
			plat->dtimer.startTimer(plat->resol_apply_id, 1000);
		}
		else if(3 == pMenuStatus->menuarray[submenu_setimg].pointer)
			app_ctrl_setMenuStat(mainmenu2);
	}
	else if(submenu_setball == pMenuStatus->MenuStat)
	{
		if(0 == pMenuStatus->menuarray[submenu_setball].pointer){
			app_ctrl_setMenuStat(submenu_setcom);
			show_circle_pointer = true;
		}
		else if(1 == pMenuStatus->menuarray[submenu_setball].pointer)
			app_ctrl_setMenuStat(submenu_setnet);
		else if(2 == pMenuStatus->menuarray[submenu_setball].pointer)
			app_ctrl_setMenuStat(mainmenu2);
	}
	else if(submenu_setcom == pMenuStatus->MenuStat)
	{
		if(0 == pMenuStatus->menuarray[submenu_setcom].pointer){
				pMenuStatus->baud_light= !pMenuStatus->baud_light;

				if(pMenuStatus->baud_light){
					plat->dtimer.startTimer(plat->baud_light_id,500);
				}
				else
				{
					plat->dtimer.stopTimer(plat->baud_light_id);
					MSGDRIV_send(MSGID_EXT_SETBAUD, 0);
				}

				
				if(setComBaud_select == true) {
					setComBaud_select = false;
					changeComBaud = true;
					MSGDRIV_send(MSGID_EXT_SETBAUD, 0);
				}
		}
		else if(4 == pMenuStatus->menuarray[submenu_setcom].pointer){
			show_circle_pointer = false;
			app_ctrl_setMenuStat(submenu_setball);
			
		}
		
	}
	else if(submenu_setnet == pMenuStatus->MenuStat)
	{
		if(4 == pMenuStatus->menuarray[submenu_setnet].pointer)
			app_ctrl_setMenuStat(submenu_setball);
	}

	*/
	//printf("\r\n[%s]: pIStuts->MenuStat = %d ",__FUNCTION__, pMenuStatus->MenuStat);
}

void CLink::TimerCreate()
{
	resol_light_id = m_dtimer.createTimer();
	resol_apply_id = m_dtimer.createTimer();
	mtdnum_light_id = m_dtimer.createTimer();
	trktime_light_id = m_dtimer.createTimer();
	maxsize_light_id = m_dtimer.createTimer();
	minsize_light_id = m_dtimer.createTimer();
	sensi_light_id = m_dtimer.createTimer();
	baud_light_id = m_dtimer.createTimer();
	mouse_show_id = m_dtimer.createTimer();

	m_dtimer.registerTimer(resol_light_id, Tcallback, &resol_light_id);
	m_dtimer.registerTimer(resol_apply_id, Tcallback, &resol_apply_id);
	m_dtimer.registerTimer(mtdnum_light_id, Tcallback, &mtdnum_light_id);
	m_dtimer.registerTimer(trktime_light_id, Tcallback, &trktime_light_id);
	m_dtimer.registerTimer(maxsize_light_id, Tcallback, &maxsize_light_id);
	m_dtimer.registerTimer(minsize_light_id, Tcallback, &minsize_light_id);
	m_dtimer.registerTimer(sensi_light_id, Tcallback, &sensi_light_id);
	m_dtimer.registerTimer(baud_light_id, Tcallback, &baud_light_id);
	m_dtimer.registerTimer(mouse_show_id, Tcallback, &mouse_show_id);
}

void CLink::Tcallback(void *p)
{
	static int resol_dianmie = 0;
	static int mtdnum_dianmie = 0;
	static int trktime_dianmie = 0;
	static int maxsize_dianmie = 0;
	static int minsize_dianmie = 0;
	static int sensi_dianmie = 0;
	static int baud_dianmie = 0;
	
	unsigned char baudlbuf[MAX_BAUDID][128] = {
		"波特率     2400","波特率     4800","波特率     9600","波特率     115200"};
	unsigned char resolbuf[maxresolid][128] = {
			"格式 1920x1080@60Hz","格式 1024x768@60Hz","格式 1280x1024@60Hz"};
	unsigned char resolapplybuf1[128] = "是否保存当前分辨率?";
	unsigned char resolapplybuf2[128] = "0:取消  1:保存";
	int a = *(int *)p;

	/*
	if(a == resol_light_id)
	{
		if(resol_dianmie)
			swprintf(disMenu[submenu_setimg][1], 33, L"%s", resolbuf[sThis->m_display.disresol_type_tmp]);
		else
			swprintf(disMenu[submenu_setimg][1], 33, L"格式");
		resol_dianmie = !resol_dianmie;
	}
	else if(a == resol_apply_id)
	{
		swprintf(disMenu[submenu_setimg][4], 33, L"%s %d", resolapplybuf1, sThis->cnt_down--);
		swprintf(disMenu[submenu_setimg][5], 33, L"%s", resolapplybuf2);
		if(sThis->cnt_down < 0)
		{
			sThis->dtimer.stopTimer(sThis->resol_apply_id);
			if(sThis->save_flag)
			{
				sThis->save_flag = 0;
				sThis->setresol(sThis->m_display.disresol_type);
				swprintf(sThis->m_display.disMenu[submenu_setimg][1], 33, L"%s", resolbuf[sThis->m_display.disresol_type]);
				memset(sThis->m_display.disMenu[submenu_setimg][4], 0, sizeof(sThis->m_display.disMenu[submenu_setimg][4]));
				memset(sThis->m_display.disMenu[submenu_setimg][5], 0, sizeof(sThis->m_display.disMenu[submenu_setimg][5]));
			}
		}
	}
	else if(a == sThis->mtdnum_light_id)
	{
		if(mtdnum_dianmie)
		{
			if((sThis->extMenuCtrl.osd_mudnum < MIN_MTDTARGET_NUM) || (sThis->extMenuCtrl.osd_mudnum > MAX_MTDTARGET_NUM))
				swprintf(sThis->m_display.disMenu[submenu_mtd][1], 33, L"目标个数     %d(超出范围%d~%d)", sThis->extMenuCtrl.osd_mudnum,MIN_MTDTARGET_NUM,MAX_MTDTARGET_NUM);
			else
				swprintf(sThis->m_display.disMenu[submenu_mtd][1], 33, L"目标个数     %d", sThis->extMenuCtrl.osd_mudnum);
		}
		else
			swprintf(sThis->m_display.disMenu[submenu_mtd][1], 33, L"目标个数");
		mtdnum_dianmie = !mtdnum_dianmie;
	}
	else if(a == sThis->trktime_light_id)
	{
		if(trktime_dianmie)
		{
			if((sThis->extMenuCtrl.osd_trktime < MIN_MTDTRKTIME) || (sThis->extMenuCtrl.osd_trktime > MAX_MTDTRKTIME))
				swprintf(sThis->m_display.disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒(超出范围%d~%d秒)", sThis->extMenuCtrl.osd_trktime,MIN_MTDTRKTIME,MAX_MTDTRKTIME);
			else
				swprintf(sThis->m_display.disMenu[submenu_mtd][2], 33, L"跟踪持续时间 %d秒", sThis->extMenuCtrl.osd_trktime);
		}
		else
			swprintf(sThis->m_display.disMenu[submenu_mtd][2], 33, L"跟踪持续时间  秒");
		trktime_dianmie = !trktime_dianmie;
	}
	else if(a == sThis->maxsize_light_id)
	{
		if(maxsize_dianmie)
		{
			if((sThis->extMenuCtrl.osd_maxsize < sThis->minsize) || (sThis->extMenuCtrl.osd_maxsize > MAX_MTDMAXSIZE))
				swprintf(sThis->m_display.disMenu[submenu_mtd][3], 33, L"最大目标面积 %d(超出范围)", sThis->extMenuCtrl.osd_maxsize);
			else
				swprintf(sThis->m_display.disMenu[submenu_mtd][3], 33, L"最大目标面积 %d", sThis->extMenuCtrl.osd_maxsize);
		}
		else
			swprintf(sThis->m_display.disMenu[submenu_mtd][3], 33, L"最大目标面积      ");
		maxsize_dianmie = !maxsize_dianmie;
	}
	else if(a == sThis->minsize_light_id)
	{
		if(minsize_dianmie)
		{
			if((sThis->extMenuCtrl.osd_minsize < MIN_MTDMINSIZE) || (sThis->extMenuCtrl.osd_minsize > sThis->maxsize))
				swprintf(sThis->m_display.disMenu[submenu_mtd][4], 33, L"最小目标面积 %d(超出范围)", sThis->extMenuCtrl.osd_minsize);
			else
				swprintf(sThis->m_display.disMenu[submenu_mtd][4], 33, L"最小目标面积 %d", sThis->extMenuCtrl.osd_minsize);
		}
		else
			swprintf(sThis->m_display.disMenu[submenu_mtd][4], 33, L"最小目标面积  ");
		minsize_dianmie = !minsize_dianmie;
	}
	else if(a == sThis->sensi_light_id)
	{
		if(sensi_dianmie)
		{
			if((sThis->extMenuCtrl.osd_sensi < MIN_MTDSENSI) || (sThis->extMenuCtrl.osd_sensi > MAX_MTDSENSI))
				swprintf(sThis->m_display.disMenu[submenu_mtd][5], 33, L"灵敏度       %d(超出范围%d~%d)", sThis->extMenuCtrl.osd_sensi,MIN_MTDSENSI,MAX_MTDSENSI);
			else
				swprintf(sThis->m_display.disMenu[submenu_mtd][5], 33, L"灵敏度       %d", sThis->extMenuCtrl.osd_sensi);
		}
		else
			swprintf(sThis->m_display.disMenu[submenu_mtd][5], 33, L"灵敏度");
		sensi_dianmie = !sensi_dianmie;
	}
	else if( a == sThis->baud_light_id ){
		if(baud_dianmie)
			swprintf(sThis->m_display.disMenu[submenu_setcom][0], 33, L"%s", baudlbuf[sThis->m_display.disbaud_type]);
		else
			memset(sThis->m_display.disMenu[submenu_setcom][0], 0, sizeof(sThis->m_display.disMenu[submenu_setcom][0]));
		baud_dianmie = !baud_dianmie;

	}
	else if(a == sThis->mouse_show_id)
	{
		if(sThis->mouse_show)
			sThis->set_mouse_show(0);
	}	
	*/
}

void CLink::sendIpcSaveMtd()
{
	/*
	MTD_Config tempConfig = {0};	
	tempConfig = mtdconfig;
	SENDST	message = {0};
	message.cmd_ID = storeMtdConfig;
	memcpy(&message.param[0],&tempConfig, sizeof(MTD_Config));
	ipc_sendmsg(&message, IPC_FRIMG_MSG);
	*/
	return ;
}


void CLink::processdurationMenu_osd(int value)
{
	#if 0
	SENDST test = {0};
	CMD_MTDTRKTIME mtdtime;
	
	test.cmd_ID = mtdtrktime;
	mtdtime.seconds = value;
	memcpy(test.param, &mtdtime, sizeof(mtdtime));
	ipc_sendmsg(&test, IPC_FRIMG_MSG);	
	waittochange
	#endif
}


void CLink::app_ctrl_upMenu()
{
	int menustate = m_menuCtrl.MenuStat; 
	if((menustate >= mainmenu2) && (menustate <= submenu_handleMatchPoints))
	{
		if((submenu_mtd == menustate) && (m_menuCtrl.mtdnum_deng == 1))
		{
			m_menuCtrl.osd_mudnum = (m_menuCtrl.osd_mudnum + 1) % (MAX_MTDTARGET_NUM+1);
			if(m_menuCtrl.osd_mudnum < MIN_MTDTARGET_NUM)
				m_menuCtrl.osd_mudnum = MIN_MTDTARGET_NUM;
			set_mtd_num_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.trktime_deng == 1))
		{
			m_menuCtrl.osd_trktime = (m_menuCtrl.osd_trktime + 1) % (MAX_MTDTRKTIME+1);
			if(m_menuCtrl.osd_trktime < MIN_MTDTRKTIME)
				m_menuCtrl.osd_trktime = MIN_MTDTRKTIME;
			set_mtd_trktime_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.maxsize_deng == 1))
		{
			m_menuCtrl.osd_maxsize = (m_menuCtrl.osd_maxsize + 1) % (MAX_MTDMAXSIZE+1);
			if(m_menuCtrl.osd_maxsize < minsize)
				m_menuCtrl.osd_maxsize = minsize;
			set_mtd_maxsize_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.minsize_deng == 1))
		{
			m_menuCtrl.osd_minsize = (m_menuCtrl.osd_minsize + 1) % (MAX_MTDMAXSIZE+1);
			if(m_menuCtrl.osd_minsize > maxsize)
				m_menuCtrl.osd_minsize =MIN_MTDMINSIZE;
			set_mtd_minsize_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.sensi_deng == 1))
		{
			m_menuCtrl.osd_sensi = (m_menuCtrl.osd_sensi + 1) % (MAX_MTDSENSI+1);
			if(m_menuCtrl.osd_sensi < MIN_MTDSENSI)
				m_menuCtrl.osd_sensi = MIN_MTDSENSI;
			set_mtd_sensi_osd();
		}
		else if(m_menuCtrl.menuarray[menustate].pointer > 0)
		{
			m_menuCtrl.menuarray[menustate].pointer--;
			upMenu();
		}
	}
}

void CLink::upMenu()
{
	int menustate = m_menuCtrl.MenuStat;
	int pointer = dismenuarray[menustate].pointer;
	if((menustate >= mainmenu2) && (menustate <= submenu_handleMatchPoints))
	{
		if(pointer > 0)
		{
			disMenuBuf[menustate][pointer].color = 2;
			dismenuarray[menustate].pointer= m_menuCtrl.menuarray[menustate].pointer;

			m_secondMenuIndex = dismenuarray[menustate].pointer;
			m_firstMenuIndex = menustate;
			
			disMenuBuf[menustate][dismenuarray[menustate].pointer].color = 3;
		}
	}
	return ;
}

void CLink::downMenu()
{
	int menustate = m_menuCtrl.MenuStat;
	int pointer = dismenuarray[menustate].pointer;
	if((menustate >= mainmenu2) && (menustate <= submenu_handleMatchPoints))
	{
		if(pointer < m_menuCtrl.menuarray[menustate].submenu_cnt - 1)
		{
			disMenuBuf[menustate][pointer].color = 2;
			dismenuarray[menustate].pointer = m_menuCtrl.menuarray[menustate].pointer;
			disMenuBuf[menustate][dismenuarray[menustate].pointer].color = 3;

			m_secondMenuIndex = dismenuarray[menustate].pointer;
			m_firstMenuIndex = menustate;
		}
	}
	return;
}

void CLink::app_ctrl_downMenu()
{
	int menustate = m_menuCtrl.MenuStat; 
	if((menustate >= mainmenu2) && (menustate <= submenu_handleMatchPoints))
	{
		if((submenu_mtd == menustate) && (m_menuCtrl.mtdnum_deng == 1))
		{
			if(MIN_MTDTARGET_NUM == m_menuCtrl.osd_mudnum)
				m_menuCtrl.osd_mudnum = MAX_MTDTARGET_NUM;
			else
				m_menuCtrl.osd_mudnum = (m_menuCtrl.osd_mudnum + MAX_MTDTARGET_NUM - 1) % MAX_MTDTARGET_NUM;
			set_mtd_num_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.trktime_deng == 1))
		{
			if(MIN_MTDTRKTIME == m_menuCtrl.osd_trktime)
				m_menuCtrl.osd_trktime = MAX_MTDTRKTIME;
			else
				m_menuCtrl.osd_trktime = (m_menuCtrl.osd_trktime + MAX_MTDTRKTIME - 1) % MAX_MTDTRKTIME;
			set_mtd_trktime_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.maxsize_deng == 1))
		{
			if(m_menuCtrl.osd_maxsize == minsize)
				m_menuCtrl.osd_maxsize = MAX_MTDMAXSIZE;
			else
				m_menuCtrl.osd_maxsize = (m_menuCtrl.osd_maxsize + MAX_MTDMAXSIZE - 1) % MAX_MTDMAXSIZE;
			set_mtd_maxsize_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.minsize_deng == 1))
		{
			if(MIN_MTDMINSIZE == m_menuCtrl.osd_minsize)
				m_menuCtrl.osd_minsize = maxsize;
			else
				m_menuCtrl.osd_minsize = (m_menuCtrl.osd_minsize + MAX_MTDMAXSIZE - 1) % MAX_MTDMAXSIZE;
			set_mtd_minsize_osd();
		}
		else if((submenu_mtd == menustate) && (m_menuCtrl.sensi_deng == 1))
		{
			if(MIN_MTDSENSI == m_menuCtrl.osd_sensi)
				m_menuCtrl.osd_sensi = MAX_MTDSENSI;
			else
				m_menuCtrl.osd_sensi = (m_menuCtrl.osd_sensi + MAX_MTDSENSI - 1) % MAX_MTDSENSI;
			set_mtd_sensi_osd();
		}
		else if(m_menuCtrl.menuarray[menustate].pointer < m_menuCtrl.menuarray[menustate].submenu_cnt - 1)
		{
			m_menuCtrl.menuarray[menustate].pointer++;
			downMenu();
		}
	}
	return ;
}

void CLink::osdshow()
{		
	MenuFunc();
	MtdOSDFunc();

	return ;
}


void CLink::getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a)
{
	switch(color)
	{
		case 1:
			r = 0;
			g = 0;
			b = 0;
			break;
		case 2:
			r = 255;
			g = 255;
			b = 255;
			break;
		case 3:
			r = 255;
			g = 0;
			b = 0;
			break;
		case 4:
			r = 255;
			g = 255;
			b = 0;
			break;
		case 5:
			r = 0;
			g = 0;
			b = 255;
			break;
		case 6:
			r = 0;
			g = 255;
			b = 0;
			break;
		default:
			break;
	}
	
	if(a > 0x0a)
		a = 0x0a;
	if(a == 0x0a)
		a = 0;
	else
		a = 255 - a*16;
	return ;
}

int CLink::MenuFunc()
{
	unsigned char r, g, b, a, color, Enable;
	short x, y;
	char font,fontsize;
	
	int index = m_menuindex;
	
	if(setrigion_flagv20)
		return -1;

	if(-1 == index)
		return -1;

	for(int i = 0; i < MAX_SUBMENU; i++)
	{
		if(i == dismenuarray[index].submenu_cnt)
			break;
		Enable = disMenuBuf[index][i].ctrl;
		if(!Enable)
		{
			 x = disMenuBuf[index][i].posx;
			 y = disMenuBuf[index][i].posy;
		 	 a = disMenuBuf[index][i].alpha;
			 color = disMenuBuf[index][i].color;
			 font = 1;
			 fontsize = 4;

			 getRGBA(color,r,g,b,a);
			 	
			 drawtext(x, y, disMenu[index][i],font ,fontsize, r, g, b, a, VIDEO_DIS_WIDTH, VIDEO_DIS_HEIGHT);
		}
	}
	
	return 0;
}



void CLink::MtdOSDFunc()
{
	unsigned char r, g, b, a, color, Enable;
	short x, y;
	char font,fontsize;

	if(0 == setrigion_flagv20)
		return;

	for(int i = 0; i < 5; i++)
	{
		Enable = disMtdBuf[0][i].ctrl;
		if(!Enable)
		{
			 x = disMtdBuf[0][i].posx;
			 y = disMtdBuf[0][i].posy;
		 	 a = disMtdBuf[0][i].alpha;
			 color = disMtdBuf[0][i].color;
			 font = 1;
			 fontsize = 4;

			 getRGBA(color,r,g,b,a);
			 
			 drawtext(x, y, disMtd[0][i],font ,fontsize, r, g, b, a, VIDEO_DIS_WIDTH, VIDEO_DIS_HEIGHT);
		}
	}
	return;
}


