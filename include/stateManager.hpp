/*
 * stateManager.hpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#ifndef STATEMANAGER_HPP_
#define STATEMANAGER_HPP_

#include "state.hpp"
#include "mvdectInterface.hpp"
//#include "statecommon.h"
#include <vector>


#define  GLUT_LEFT_BUTTON                   0x0000
#define  GLUT_MIDDLE_BUTTON                 0x0001
#define  GLUT_RIGHT_BUTTON                  0x0002
#define  GLUT_DOWN                          0x0000
#define  GLUT_UP                            0x0001


class State;
class StateManger
{
public:
	StateManger();
	~StateManger();

	void specialEvent(char key);
	void normalKeyEvent(char key);
	void enterKeyEvent();
	void upMenu();
	void downMenu();
	void showOsd();
	
	char getMenuState();

	static void callbackChangeStat(char nextmode);
	static void callbackChangeDefaultWorkMode(char workmode);

	void mouseMove(int xMouse, int yMouse);
	void mouseEvent(int button, int state, int x, int y);


	void* returnIpc();
	void GetParams(int* p);
	int CurStateInterface();

	void init(OSDFUNC func,CHANGESTAT chDisMode,CMvDectInterface *pMov);
	void menuOsdInit();
	void menuLoadIpcParam(int* config);


	void get_featurepoint();
	bool point_in_pip(int x, int y);
	void start_twinkle(int x, int y);
	bool in_recommand_vector(int x, int y, cv::Point2i &outPixel);
	void evade_pip(int x, int y);
	void app_self_deletePoint(cv::Point2i Pixel);
	void set_jos_mouse_mode(jos_mouse_Mode mode);
	jos_mouse_Mode get_gridinter_mode();
	void set_gridinter_mode(jos_mouse_Mode value);
	void drawPoints(cv::Mat frame);


	
	std::vector<cv::Point>& getPoly();
	std::vector<cv::Point>& getPolyTmp();
	std::vector< std::vector<cv::Point> >& getunRoiPoly();

private:
	friend class State;
	void ChangeState(State* state);
	void OperationChangState();

private:
	char m_defworkmode;
	int m_curState;
	State *m_state;
	State *m_linkmanual,*m_linkcalib;
	CMvDectInterface * m_pMv;
	static StateManger* pThis;

};



#endif /* STATEMANAGER_HPP_ */
