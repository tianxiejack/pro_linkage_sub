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
	void StateInit(OSDFUNC func);
	void create();
	virtual ~State();



public:
	virtual void showOsd() = 0;
	virtual void inputNumber(char key) = 0;
	virtual void enter() = 0;




public:
	virtual void buttonWorkMode(){};
	virtual void buttonMenu(){};
	
	virtual int ChangeState(StateManger* con, char nextState);
	
	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);


	
public:
	static char m_curState;
	static DxTimer* m_timer;
	static State *m_level1 , *m_level2;
	static OSDFUNC drawFunc;
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
	void showMenuOsd();
	void buttonMenu();



	void clearPw();
	void inputPWOsd();
	void inputErrorPWOsd();


private:
	static LevelOne* pThis;
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

private:
	static LevelTwo* pThis;


};

#endif /* STATE_HPP_ */
