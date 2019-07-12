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
#include "statecommon.h"
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

	
	std::vector<cv::Point>& getPoly();

private:
	friend class State;
	void ChangeState(State* state);
	void OperationChangState();

private:
	char m_defworkmode;
	int m_curState;
	State* m_state;
	CMvDectInterface * m_pMv;
	static StateManger* pThis;

};



#endif /* STATEMANAGER_HPP_ */
