
#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "statecommon.h"

#include "mvdectInterface.hpp"
#include "DxTimer.hpp"
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class CMenu
{
public:
	CMenu(OSDFUNC pfun,CHANGESTAT pDisplaymode,CHANGESTAT pchStatfun,CHDEFWORKMD pchDefwm,CMvDectInterface *pMov);
	~CMenu();

	void lv_1_inputPWosd();
	void lv_1_errorPWosd();
	void lv_2_osd();
	void lv_3_workmodeOsd();
	void lv_3_mtdparamOsd();
	void lv_4_mtdregionOsd(bool show_result);
	void lv_4_mtdUnregionOsd(bool show_result);
	void lv_4_calibOsd();
	

	void menuButton();
	void normalKey(char key);
	void enter();
	void upMenu();
	void downMenu();

	void showMenuOsd();

	char getMenuState();

	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);


	void clearPw();


	void menuhandle_main();
	void menuhandle_workmode();
	void menuhandle_mtdparam();


	void gotoInputPW();
	void gotoErrorPW();
	void gotoMainMenu();
	void gotoBlankMenu();
	void gotoWorkMode();
	void gotoMtdparam(bool initPointer = true);
	void gotoMtdRegion();
	void gotoMtdUnRegion();
	void gotoCalibMode();


	void set_mtd_num(char key);
	void set_mtd_trktime(char key);


	void set_mtd_num_osd();
	void set_mtd_trktime_osd();
	void set_mtd_maxsize_osd();
	void set_mtd_minsize_osd();
	void set_mtd_sensi_osd();
	
	void save_polygon_roi();
	void save_polygon_unroi();
	int map1080p2normal_point(float *x, float *y);
	int mapnormal2curchannel_point(float *x, float *y, int w, int h);
	int mapfullscreen2gun_pointv20(int *x, int *y);
	int maprect_point(int *x, int *y, mouserect rectsrc,mouserect rectdest);
	void SaveMtdSelectArea(const char* filename, std::vector< std::vector< cv::Point > > edge_contours);


	void menuMtdparam_setnum();
	void menuMtdparam_settrktime();
	void menuMtdparam_setmaxsize();
	void menuMtdparam_setminsize();	
	void menuMtdparam_setsensi();


	static void TcallbackFunc(void *p);
	void TimerCreate();
	void TcallbackHandle(void *p);

public:
	OSDFUNC drawFunc;
	CHDEFWORKMD changeDefwmFunc;
	CHANGESTAT changeStatFunc;
	CHANGESTAT changeDisModeFunc;

private:
	char *init_passwd;
	osdInfo_t disMenuBuf;
	char m_passwd[128];
	char m_dispasswd[128];
	DxTimer m_timer;

	bool shin_mtdnum,shin_trktime,shin_maxsize,shin_minsize,shin_sensi;
	int mtdnum_timeId, trktime_timeId, maxsize_timeId, minsize_timeId, sensi_timeId;

	int m_mtdnum , m_mtdtrktime , m_mtdmaxsize , m_mtdminsize , m_mtdsensi;
	
	char m_mtdnum_arr[128];
	char m_trktime_arr[128];
	char m_maxsize_arr[128];
	char m_minsize_arr[128];
	char m_sensi_arr[128];

	CHAR m_menuPointer;
	CHAR m_menuStat;
	CHAR m_ctlBallMode;

	std::vector< std::vector< cv::Point > > polyWarnRoi;
	std::vector< std::vector< cv::Point > > edge_contours;
	std::vector< std::vector< cv::Point > > polyWarnUnRoi;
	std::vector< std::vector< cv::Point > > edge_contours_UnRoi;
	int m_ScreenWidth,m_ScreenHeight;
	CMvDectInterface * m_pMv;
	cv::FileStorage m_fsWriteMtd;

public:
	std::vector<cv::Point> m_poly , m_polyTmp;
	std::vector< std::vector<cv::Point> > m_unroiPoly;
	
};



#endif /* MENU_HPP_ */
