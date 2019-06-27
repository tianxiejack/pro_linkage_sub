

#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "ipc_custom_head.h"
#include "autoManualFindRelation.hpp"
#include "DxTimer.hpp"
#include "mvdectInterface.hpp"
#include <opencv2/core.hpp>

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

typedef void (*OSDFUNC)(int x,int y,wchar_t* text,char font,char fontsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a,int win_width,int win_height);

class CLink{
public:
	CLink();
	~CLink(){};

	void init(OSDFUNC func,CMvDectInterface *pMov);
	void menuOsdInit();
	void menuLoadIpcParam(int* config);
	void OnJosEvent(int key, int param);

	OSDFUNC drawtext;
	
	
public:
	
	void app_ctrl_setMenuStat(int index);
	void app_ctrl_setMenu_jos(int menu_state);
	void setWorkMode(GB_WorkMode workmode);
	void setDisplayMode(DisplayMode_t mode);
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
	void upMenu();
	void downMenu();
	void app_ctrl_downMenu();
	void app_ctrl_upMenu();



	void set_mtd_num_osd();
	void set_mtd_trktime_osd();
	void set_mtd_sensi_osd();
	void set_mtd_minsize_osd();
	void set_mtd_maxsize_osd();
	


	void TimerCreate();
	static void Tcallback(void *p);
	void processdurationMenu_osd(int value);
	void sendIpcSaveMtd();
	int MenuFunc();
	void MtdOSDFunc();
	void osdshow();
	
	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);
	void reminderOSDFunc();

	void reminderOSD_calib();

public:

	void set_mouse_show(int param){mouse_show = param;};
	void set_josctrl_mode(jos_mouse_Mode value){m_josMode = value;};
	jos_mouse_Mode get_josctrl_mode(){ return m_josMode;};
	void app_ctrl_setMtdRigionStat(int parm){setrigion_flagv20 = parm;};

	void set_showpip_stat(bool value){draw_pip_stat = value;};
	bool get_showpip_stat(){return draw_pip_stat;};
	void set_drawpoints_stat(bool value){drawpoints_stat = value;};
	bool get_drawpoints_stat(){return drawpoints_stat;};

	void SaveMtdSelectArea(const char* filename, std::vector< std::vector< cv::Point > > edge_contours);




	
public:
	int* m_config;

	GB_WorkMode g_AppWorkMode ;
	DisplayMode_t displayMode ;
	GB_WorkMode setting_WorkMode;
	
	menu_param_t m_menuCtrl; // msgextMenuCtrl


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

	
	jos_mouse_Mode m_josMode ;

	int pol_draw;
	PointNode polRect[MAX_CHAN][100];
	int pol_tempX, pol_tempY, pol_rectn[MAX_CHAN];


	std::vector< std::vector< cv::Point > > polyWarnRoi;
	std::vector< std::vector< cv::Point > > edge_contours;


	char *init_passwd;
	static bool storeWorkModeFlag,storeMtdConfigFlag;

	DxTimer m_dtimer;
	int resol_light_id, mouse_show_id, resol_apply_id, mtdnum_light_id, trktime_light_id, maxsize_light_id, minsize_light_id, sensi_light_id, baud_light_id;
	int maxsize, minsize , sensi;

	
	int m_secondMenuIndex;
	int m_firstMenuIndex;

	
	int m_firstLevel,m_secondLever;

	bool draw_pip_stat,drawpoints_stat;


	
	int map1080p2normal_point(float *x, float *y);
	int mapnormal2curchannel_point(float *x, float *y, int w, int h);
	int m_ScreenWidth,m_ScreenHeight;
	int maprect_point(int *x, int *y, mouserect rectsrc,mouserect rectdest);
	int mapfullscreen2gun_pointv20(int *x, int *y);

	CMvDectInterface *m_pMovDetector;
	cv::FileStorage m_fsReadMtd;
	cv::FileStorage m_fsWriteMtd;


	

};



#endif /* MENU_HPP_ */
