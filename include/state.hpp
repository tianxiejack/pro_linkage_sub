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
	void StateInit();
	void create();
	virtual ~State();

public:
	virtual void updateOsd() = 0;

	
public:
	static state_enum m_curState;
	static DxTimer* m_timer;
	static State *m_level1 , *m_level2;

};


class LevelOne:public State
{
public:
	LevelOne();
	virtual ~LevelOne();
	
private:
	static LevelOne* pThis;

	
	void updateOsd();
	
};


class LevelTwo:public State
{
public:
	LevelTwo();
	virtual ~LevelTwo();
	
private:
	static LevelTwo* pThis;

	void updateOsd();
};

/*
class LevelThree:public State
{
public:
	LevelThree();
	virtual ~LevelThree();
	
private:
	static LevelThree* pThis;
	int curStateInterface();
	
};
*/


#endif /* STATE_HPP_ */
