

#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "ipc_custom_head.h"
#include "autoManualFindRelation.hpp"

using namespace cr_automanualfindrelation;

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

typedef enum _SelectMode{
	Click_Mode =0,
	DrawRectangle_Mode,
	SetMteRigion_Mode,
	Mode_Count
}SelectMode;


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
	void app_ctrl_setnumber(char key);

	void set_jos_mouse_mode(jos_mouse_Mode mode);
	void set_mtd_num(char key);
	void set_mtd_trktime(char key);
	void set_mtd_maxsize(char key);
	void set_mtd_minsize(char key);
	void set_mtd_sensi(char key);
	void app_ctrl_enter();
	void queryCurBallCamPosition();
	void save_polygon_roi();
	void app_ctrl_settrig_inter(menu_param_t *pInCmd);
	void SetDefaultWorkMode( GB_WorkMode workmode );


	void menu0_handle();
	void menu1_handle();
	void menu2_handle();
	void submenu_DefaultWorkMode_handle();
	void submenu_mtd_handle();

	

public:

	void set_mouse_show(int param){mouse_show = param;};
	void set_josctrl_mode(jos_mouse_Mode value){m_josMode = value;};
	void app_ctrl_setMtdRigionStat(int parm){setrigion_flagv20 = parm;};


	
	
public:
	int* m_config;
	menu_param_t msgextMenuCtrl;
	GB_WorkMode g_AppWorkMode ;
	MenuDisplay g_CurDisplayMode;
	MenuDisplay displayMode ;
	GB_WorkMode setting_WorkMode;

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
	int m_mtdSetRigion;
	CAutoManualFindRelation* m_autofr;

	
	jos_mouse_Mode m_josMode = mouse_mode;

	int pol_draw;
	PointNode polRect[MAX_CHAN][100];
	int pol_tempX, pol_tempY, pol_rectn[MAX_CHAN];


	std::vector< std::vector< cv::Point > > polyWarnRoi;

	SelectMode mouse_workmode;


	char *init_passwd;
	static bool storeWorkModeFlag,storeMtdConfigFlag;

	
};



#endif /* MENU_HPP_ */
