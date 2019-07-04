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


typedef void (*OSDFUNC)(int x,int y,wchar_t* text,char font,char fontsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a,int win_width,int win_height);

class State;
class StateManger
{
public:
	StateManger(State* state);
	~StateManger();

	void* returnIpc();
	void GetParams(int* p);
	int CurStateInterface();


public:
	void init(OSDFUNC func,CMvDectInterface *pMov);
	void menuOsdInit();
	void menuLoadIpcParam(int* config);
	void OnJosEvent(int key, int param);

	OSDFUNC drawtext;

	

private:
	//friend class State;
	bool ChangeState(State* state);
	void OperationInterface();
	void OperationChangState();

private:
	int m_curState;

public:
	State* m_state;
};



#endif /* STATEMANAGER_HPP_ */
