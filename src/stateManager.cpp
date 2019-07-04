/*
 * stateManager.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "stateManager.hpp"


StateManger::StateManger()
{
	m_state = new LevelOne();
	m_curState = LEVELONE;
}

StateManger::~StateManger()
{
}


void init(OSDFUNC func,CMvDectInterface *pMov)
{
	
}



