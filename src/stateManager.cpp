/*
 * stateManager.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "stateManager.hpp"

#include <string.h>
#include "locale.h"
#include <stdio.h>
#include <wchar.h>



StateManger::StateManger()
{
	m_state = new LevelOne();
	m_curState = LEVELONE;
}

StateManger::~StateManger()
{
	
}


void StateManger::init(OSDFUNC func,CMvDectInterface *pMov)
{
	m_pMv = pMov;
	m_state->StateInit(func);
	updateOsd();
}


void StateManger::updateOsd()
{
	
	m_state->showOsd();
	


	return ;
}



