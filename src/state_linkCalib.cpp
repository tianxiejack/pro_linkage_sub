/*
 * state_linkCalib.cpp
 *
 *  Created on: 2019年7月16日
 *      Author: alex
 */

#include "state.hpp"


CLinkCalib::CLinkCalib()
{
}

CLinkCalib::~CLinkCalib()
{
}

void CLinkCalib::showOsd()
{
	m_pMenu->showMenuOsd();
}

void CLinkCalib::inputNumber(char key)
{
	m_pMenu->normalKey(key);
}

void CLinkCalib::enter()
{
	m_pMenu->enter();
}

void CLinkCalib::buttonMenu()
{
	m_pMenu->menuButton();
}
