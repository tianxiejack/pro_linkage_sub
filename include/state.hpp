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

typedef enum{
	LEVELONE,
	LEVELTWO,
	LEVELTHREE,
	LEVELFOUR,
	LEVELFIVE,
	LEVELMAX,
}state_enum;




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
	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);
	
public:
	static state_enum m_curState;
	static DxTimer* m_timer;
	static State *m_level1 , *m_level2;
	static OSDFUNC drawFunc;
};


class LevelOne:public State
{
public:
	LevelOne();
	virtual ~LevelOne();
	
	void initOsd();
	void showOsd();

	void showMenuOsd();

private:
	static LevelOne* pThis;



	osdInfo_t disMenuBuf;


};


class LevelTwo:public State
{
public:
	LevelTwo();
	virtual ~LevelTwo();
	
	void showOsd();

private:
	static LevelTwo* pThis;


};

#endif /* STATE_HPP_ */
