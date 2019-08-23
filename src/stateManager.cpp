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
	m_linkmanual = new CLinkManual();
	m_linkcalib = new CLinkCalib();
	m_state = m_linkmanual;
	m_curState = LINKMANUAL;
	pThis = this;
}

StateManger::~StateManger()
{
	
}


void StateManger::init(OSDFUNC func,CHANGESTAT chDisMode,CMvDectInterface *pMov)
{
	m_pMv = pMov;
	m_state->StateInit(func,chDisMode,callbackChangeStat,callbackChangeDefaultWorkMode,pMov);
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
	switch(nextmode)
	{
		case LINKMANUAL:
			pThis->ChangeState(pThis->m_linkmanual);
			break;

		case LINKCALIB:
			pThis->ChangeState(pThis->m_linkcalib);
			break;

		default:
			break;
	}
	
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
		{
			getPoly().push_back(cv::Point(x,y));
			printf("%s : getPoly num = %d \n" , __func__, getPoly().size());
		}
		else if(getMenuState() == MENU_MTD_UNREGION)
		{
			getPolyTmp().push_back(cv::Point(x,y));	
			printf("%s : getPolyTmp num = %d \n" , __func__, getPolyTmp().size());
		}
		else if(getMenuState() == MENU_CALIB)
			m_state->process_trigmode_left_point(x, y);
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(getMenuState() == MENU_CALIB)
			;//m_state->process_trigmode_right_point(x, y);
		
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

std::vector<cv::Point>& StateManger::getPolyTmp()
{
	return m_state->m_pMenu->m_polyTmp;
}

std::vector< std::vector<cv::Point> >& StateManger::getunRoiPoly()
{
	return m_state->m_pMenu->m_unroiPoly;
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


void StateManger::drawPoints(cv::Mat frame)
{
	m_state->drawPoints(frame);
	return;
}


