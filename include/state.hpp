/*
 * state.hpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#include "DxTimer.hpp"
#include "configtable.h"
#include "statecommon.h"
#include "menu.hpp"
#include "autoManualFindRelation.hpp"
#include "ipc_custom_head.h"

using namespace cr_automanualfindrelation;

class CMenu;
class State
{
public:
	State();
	void StateInit(OSDFUNC pDraw ,CHANGESTAT pDisplaymode, CHANGESTAT pChangeStat , CHDEFWORKMD pChangeWkmode, CMvDectInterface *pMov);
	void create();
	virtual ~State();

public:
	virtual void showOsd() = 0;
	virtual void inputNumber(char key) = 0;
	virtual void enter() = 0;
	
public:
	virtual void buttonWorkMode(){};
	virtual void buttonMenu(){};
	virtual void upMenu();
	virtual void downMenu();
	virtual void operationChangeState(){};
	
	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);

	static void TcallbackFunc(void *p);
	
public:
	static char m_curState;
	static DxTimer* m_timer;
	static CHANGESTAT m_changeStatFunc;
	static CMenu* m_pMenu;
	static void pnotify_callback(std::vector<FEATUREPOINT_T>& recommendPoints);

public:
	static CAutoManualFindRelation* m_autofr;

	static std::vector< FEATUREPOINT_T > app_recommendPoints;
	static std::vector< FEATUREPOINT_T > app_recommendPoints_bak;

	cv::Point2i twinkle_point, twinkle_point_bak;
	jos_mouse_Mode gridinter_mode;
	cv::Point2i point_triangle , point_triangle_bak;
	bool draw_print_stat, drawpoints_stat ;
	static int twinkle_point_id;
	int m_trig_pip_mode = 0;
	static bool twinkle_flag;
	int ptz_flag = 0;


public:
	bool get_print_stat(){return draw_print_stat;};
	void set_print_stat(bool value){draw_print_stat = value;};

	void grid_manuallinkage_moveball(int x, int y, int changezoom);
	void mapout2inresol(cv::Point *tmppoint);
	void app_set_triangle_point(int x, int y);
	void process_trigmode_right_point(int x, int y);
	void app_deletePoint(int x, int y);
	void app_selectPoint(int x, int y);
	void app_manualInsertRecommendPoints(int x, int y);
	void app_insertPos(float x, float y);
	bool get_drawpoints_stat(){return drawpoints_stat;};
	void set_drawpoints_stat(bool value){drawpoints_stat = value;};
	void set_twinkle_flag(bool flag){twinkle_flag = flag;};
	bool get_twinkle_flag(){return twinkle_flag;};
	void set_PTZ_flag(bool flag){ptz_flag = flag;};
	void DrawTwinklePoint(cv::Mat frame);
	void Drawfeaturepoints(cv::Mat frame);
	void set_jos_mouse_mode(jos_mouse_Mode mode);
	bool in_recommand_vector(int x, int y, cv::Point2i &outPixel);

	void drawPoints(cv::Mat frame);
	void get_featurepoint();
	bool point_in_pip(int x, int y);
	void process_trigmode_left_point(int x, int y);
	jos_mouse_Mode get_gridinter_mode();
	void set_gridinter_mode(jos_mouse_Mode value);

	void evade_pip(int x, int y);
	void app_self_deletePoint(cv::Point2i Pixel);
	void start_twinkle(int x, int y);
	void stoptwinkle();


};


class CLinkManual:public State
{
public:
	CLinkManual();
	virtual ~CLinkManual();
	
	void showOsd();
	void inputNumber(char key);
	void enter();

public:
	void buttonMenu();

private:

};


class CLinkCalib:public State
{
public:
	CLinkCalib();
	virtual ~CLinkCalib();
	
	void showOsd();
	void inputNumber(char key);
	void enter();

public:
	void buttonMenu();

private:

};




class LevelTwo:public State
{
public:
	LevelTwo();
	virtual ~LevelTwo();
	
	void showOsd();
	void inputNumber(char key);
	void enter();

public:
	void initOsd();
	void buttonMenu();
	void operationChangeState();

	
private:
	osdInfo_t disMenuBuf;
	char m_menuPointer;


};


class CSetWorkMode:public State
{
public:
	CSetWorkMode();
	virtual ~CSetWorkMode();
	
	void showOsd();


public:
	void initOsd();
	void buttonMenu();
	void operationChangeState();
	void upMenu();
	void downMenu();
	void enter();
	
private:
	osdInfo_t disMenuBuf;
	char m_menuPointer;

};


#endif /* STATE_HPP_ */
