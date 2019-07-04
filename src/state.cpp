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
OSDFUNC State::drawFunc = NULL;
char State::m_curState = LEVELONE;

State::State()
{	
}

State::~State()
{
}


void State::StateInit(OSDFUNC func)
{
	if(m_level1 == NULL)
		m_level1 = new LevelOne();
	if(m_level2 == NULL)
		m_level2 = new LevelTwo();
	if(m_timer == NULL)
		m_timer = new DxTimer();
	drawFunc = func;
	return ;
}


void State::create()
{
	
}


int State::ChangeState(StateManger* con, char nextState)
{
	m_curState = nextState;
	switch(m_curState)
	{
		case LEVELTWO:
			con->ChangeState(m_level2);
			break;

		default:
			break;
	}

	return m_curState;
}



void State::getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a)
{
	switch(color)
	{
		case 1:
			r = 0;
			g = 0;
			b = 0;
			break;
		case 2:
			r = 255;
			g = 255;
			b = 255;
			break;
		case 3:
			r = 255;
			g = 0;
			b = 0;
			break;
		case 4:
			r = 255;
			g = 255;
			b = 0;
			break;
		case 5:
			r = 0;
			g = 0;
			b = 255;
			break;
		case 6:
			r = 0;
			g = 255;
			b = 0;
			break;
		default:
			break;
	}

	if(a > 0x0a)
		a = 0x0a;
	if(a == 0x0a)
		a = 0;
	else
		a = 255 - a*16;
	return ;
}

