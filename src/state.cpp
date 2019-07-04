/*
 * state.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"

DxTimer* State::m_timer = NULL;

State* State::m_level1 = NULL;
State* State::m_level2 = NULL;



State::State()
{	
}

State::~State()
{
}


void State::StateInit()
{
	if(m_level1 == NULL)
		m_level1 = new LevelOne();
	if(m_level2 == NULL)
		m_level2 = new LevelTwo();
	if(m_timer == NULL)
		m_timer = new DxTimer();
	return ;
}


void State::create()
{
}


void State::OperationInterface(StateManger* con)
{


	return ;
}

void State::OperationChangeState(StateManger* con)
{
}


int State::ChangeState(StateManger* con, char nextState)
{


	
}


