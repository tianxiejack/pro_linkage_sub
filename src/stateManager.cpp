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


StateManger* StateManger::pThis = NULL;


StateManger::StateManger()
{
	m_state = new CLinkManual();
	m_curState = LEVELONE;
}

StateManger::~StateManger()
{
	
}


void StateManger::init(OSDFUNC func,CMvDectInterface *pMov)
{
	m_pMv = pMov;
	m_state->StateInit(func,callbackChangeStat);
}


void StateManger::ChangeState(State* state)
{
	m_state = state;
	m_state->operationChangeState();
	return;
}

void StateManger::specialEvent(char key)
{
	switch(key)
	{
		case JOSF1_AUTOLINKMODE:
			{
				//GB_WorkMode nextMode = (GB_WorkMode)(param - 1);
				//setWorkMode(nextMode);
			}
			break;
		case JOSF2_MENU:			
				m_state->buttonMenu();
			break;
		default:
			break;
	}

	return;
}


void StateManger::callbackChangeStat(char nextmode)
{
	

	return;
}


void StateManger::normalKeyEvent(char key)
{
	m_state->inputNumber(key);
	return;
}

void StateManger::enterKeyEvent()
{
	m_state->enter();
	return;
}


void StateManger::upMenu()
{
	m_state->upMenu();
	return;
}

void StateManger::downMenu()
{
	m_state->downMenu();
	return;
}

void StateManger::showOsd()
{
	m_state->showOsd();
	return;
}

