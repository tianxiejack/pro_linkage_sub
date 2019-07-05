/*
 * state_leveltwo.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "state.hpp"


LevelTwo::LevelTwo():m_menuPointer(0)
{
	initOsd();
}

LevelTwo::~LevelTwo()
{

}

void LevelTwo::showOsd()
{

}

void LevelTwo::inputNumber(char key)
{
	
}

void LevelTwo::enter()
{
	switch(m_menuPointer)
	{
		case 0:

			break;

	}
}


void LevelTwo::initOsd()
{

}


void LevelTwo::operationChangeState()
{

}


void LevelTwo::buttonMenu()
{

	return;
}


void LevelTwo::upMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (m_menuPointer+disMenuBuf.cnt-1)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;
}

void LevelTwo::downMenu()
{
	disMenuBuf.osdBuffer[m_menuPointer].color = 2;
	m_menuPointer = (++m_menuPointer)%disMenuBuf.cnt;
	disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	return;	
}


