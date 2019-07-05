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
	
	static void callbackChangeStat(char nextmode);





	void* returnIpc();
	void GetParams(int* p);
	int CurStateInterface();

	void init(OSDFUNC func,CMvDectInterface *pMov);
	void menuOsdInit();
	void menuLoadIpcParam(int* config);

	

private:
	friend class State;
	void ChangeState(State* state);
	void OperationChangState();

private:
	int m_curState;
	State* m_state;
	CMvDectInterface * m_pMv;
	static StateManger* pThis;
};



#endif /* STATEMANAGER_HPP_ */
