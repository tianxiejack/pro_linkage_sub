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

	void* returnIpc();
	void GetParams(int* p);
	int CurStateInterface();

	void init(OSDFUNC func,CMvDectInterface *pMov);
	void menuOsdInit();
	void menuLoadIpcParam(int* config);
	void OnJosEvent(int key, int param);
	void updateOsd();

	CMvDectInterface * m_pMv;
	

private:
	//friend class State;
	bool ChangeState(State* state);
	void OperationChangState();

private:
	int m_curState;

public:
	State* m_state;
};



#endif /* STATEMANAGER_HPP_ */
