

#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "ipc_custom_head.h"


enum
{
	JOSF1_OPEN_AUTOLINKMODE = 1,
	JOSF2_ENTER_MENU=2,
	CONST_VARIABLE_COUNT		
};
enum DrawBehavior
{
	DRAW_NOTICE_TEXTS,
	ERASE_TEXTS,
	DRAW_COUNT
};

typedef struct{
		int button;
		int state;
		int x;
		int y;
}mouse_t;


typedef struct{  
    int x;  
    int y;   
}PointNode;


typedef struct{
	volatile unsigned int MenuStat;
	volatile int Trig_Inter_Mode;;
	AppMenu menuarray[menumaxid];
	char Passwd[128];
	char disPasswd[128];
	mouse_t Mtdmouseclick;
	PointNode Mtdmousemotion;
	
	int resol_deng, mtdnum_deng,trktime_deng,maxsize_deng,minsize_deng,sensi_deng;// dianmie fanying
	int resol_type_tmp, resol_type;
	int osd_mudnum, osd_trktime, osd_maxsize, osd_minsize, osd_sensi;
	int baud_light;
	int baud_type;// 2400,4800,9600, 115200
	char mtdnum_arr[128];
	char trktime_arr[128];
	char maxsize_arr[128];
	char minsize_arr[128];
	char sensi_arr[128];
}menu_param_t;



class CLink{
public:
	CLink();
	~CLink(){};

	void init();
	void menuOsdInit();
	void menuLoadIpcParam(int* config);
	void OnJosEvent(int key, int param);

public:
	
	void app_ctrl_setMenuStat(int index);
	void app_ctrl_setMenu_jos(int menu_state);
	void setWorkMode(GB_WorkMode workmode);
	
	void setDisplayMode(MenuDisplay mode);


	void set_mouse_show(int param)
	{
		mouse_show = param;
	}


public:
	menu_param_t msgextMenuCtrl;
	GB_WorkMode g_AppWorkMode ;
	DISPLAYMODE g_CurDisplayMode;
	MenuDisplay displayMode ;

	menu_param_t extMenuCtrl;
	//MenuDisplay g_displayMode ;


	int m_menuindex;
	AppMenu dismenuarray[menumaxid];
	osdbuffer_t disMenuBuf[32][MAX_SUBMENU];
	osdbuffer_t disMtdBuf[1][MAX_SUBMENU];
	wchar_t disMenu[menumaxid][MAX_SUBMENU][33];
	wchar_t disMtd[1][MAX_SUBMENU][33];

	int mouse_show ;

	MENU_POS m_currentMenuPos[32][7];
	int oresoltype;
	int curBaudRate;
	int setrigion_flagv20;


	
};



#endif /* MENU_HPP_ */
