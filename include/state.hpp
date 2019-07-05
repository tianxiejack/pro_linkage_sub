/*
 * state.hpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#ifndef STATE_HPP_
#define STATE_HPP_


#include "stateManager.hpp"
#include "DxTimer.hpp"
#include "configtable.h"
#include "statecommon.h"



class StateManger;
class State
{
public:
	State();
	void StateInit(OSDFUNC func,StateManger* con);
	void create();
	virtual ~State();

public:
	virtual void showOsd() = 0;
	virtual void inputNumber(char key) = 0;
	virtual void enter() = 0;

public:
	virtual void buttonWorkMode(){};
	virtual void buttonMenu(){};
	virtual void upMenu(){};
	virtual void downMenu(){};
	virtual void showMenuOsd(osdInfo_t& disMenuBuf);
	virtual int ChangeState(char nextState);
	virtual void operationChangeState(){};
	
	
	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);


	
public:
	static char m_curState;
	static DxTimer* m_timer;
	static State *m_level1 , *m_level2, *m_level_setworkmode;
	static OSDFUNC drawFunc;
	static StateManger* m_pStatManager;
};


class LevelOne:public State
{
public:
	LevelOne();
	virtual ~LevelOne();
	
	void showOsd();
	void inputNumber(char key);
	void enter();

public:
	void initOsd();
	void buttonMenu();

	void clearPw();
	void inputPWOsd();
	void inputErrorPWOsd();

private:
	osdInfo_t disMenuBuf;
	bool inputtingStat;

	char m_passwd[128];
	char m_dispasswd[128];
	char *init_passwd;
	bool m_menuShow;
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
	void upMenu();
	void downMenu();

	
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
