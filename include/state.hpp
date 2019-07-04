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
	virtual void OperationInterface(StateManger* con);
	virtual void OperationChangeState(StateManger* con) = 0;
	virtual int ChangeState(StateManger* con, char nextState);
	
public:
	static state_enum m_curState;
	static DxTimer* m_timer;
	static State* m_level1 , *m_level2;

};


class LevelOne:public State
{
public:
	LevelOne();
	virtual ~LevelOne();
	
private:
	static LevelOne* pThis;
	void OperationChangeState(StateManger* con);
	int curStateInterface();
	
};


class LevelTwo:public State
{
public:
	LevelTwo();
	virtual ~LevelTwo();
	
private:
	static LevelTwo* pThis;
	void OperationChangeState(StateManger* con);
	int curStateInterface();
	
};

/*
class LevelThree:public State
{
public:
	LevelThree();
	virtual ~LevelThree();
	
private:
	static LevelThree* pThis;
	void OperationChangeState(StateManger* con);
	int curStateInterface();
	
};
*/


#endif /* STATE_HPP_ */
