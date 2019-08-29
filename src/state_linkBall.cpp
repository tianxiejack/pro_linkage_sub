/*
 * state_linkBall.cpp
 *
 *  Created on: 2019年8月29日
 *      Author: alex
 */

#include "state.hpp"

CLinkBall::CLinkBall()
{
}

CLinkBall::~CLinkBall()
{
}

void CLinkBall::showOsd()
{
	m_pMenu->showMenuOsd();
}

void CLinkBall::inputNumber(char key)
{
	m_pMenu->normalKey(key);
}

void CLinkBall::enter()
{
	m_pMenu->enter();
}

void CLinkBall::buttonMenu()
{
	m_pMenu->menuButton();
	return;
}



