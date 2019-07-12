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
	m_curState = LINKMANUAL;
	pThis = this;
}

StateManger::~StateManger()
{
	
}


void StateManger::init(OSDFUNC func,CHANGESTAT chDisMode,CMvDectInterface *pMov)
{
	m_pMv = pMov;
	m_state->StateInit(func,chDisMode,callbackChangeStat,callbackChangeDefaultWorkMode);
	return;
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

void StateManger::callbackChangeDefaultWorkMode(char workmode)
{
	pThis->m_defworkmode = workmode;
	//wait to add save param in param;
	printf("m_defworkmode = %d \n" , 	pThis->m_defworkmode);
	return;
}


void StateManger::mouseMove(int xMouse, int yMouse)
{
	return;
}

char StateManger::getMenuState()
{
	return m_state->m_pMenu->getMenuState();
}


void StateManger::mouseEvent(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(getMenuState() == MENU_MTD_REGION)
			getPoly().push_back(cv::Point(x,y));
	}
		
	

	return;
}


void StateManger::normalKeyEvent(char key)
{
	
	m_state->inputNumber(key);
	return;
}

std::vector<cv::Point>& StateManger::getPoly()
{
	return m_state->m_pMenu->m_poly;
}


void StateManger::enterKeyEvent()
{
	switch(getMenuState())
	{
		case MENU_MTD_REGION:
			getPoly().push_back(getPoly()[0]);
			//wait to save poly to yml
			break;

		default:
			m_state->enter();
			break;

	}	
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

