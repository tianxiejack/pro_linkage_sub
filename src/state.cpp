
#include "state.hpp"


DxTimer* State::m_timer = NULL;
State* State::m_level1 = NULL;
State* State::m_level2 = NULL;
char State::m_curState = LEVELONE;
CHANGESTAT State::m_changeStatFunc = NULL;
CMenu* State::m_pMenu = NULL;


State::State()
{	
}

State::~State()
{
}


void State::StateInit(OSDFUNC pDraw , CHANGESTAT pChangeStat)
{
	if(m_level1 == NULL)
		m_level1 = new CLinkManual();
	if(m_level2 == NULL)
		m_level2 = new LevelTwo();
	if(m_timer == NULL)
		m_timer = new DxTimer();
	
	if(m_pMenu == NULL)
		m_pMenu = new CMenu(pDraw,pChangeStat);

	m_changeStatFunc = pChangeStat;
		
	return ;
}


void State::create()
{
	
}


int State::ChangeState(char nextState)
{
	m_curState = nextState;
	switch(m_curState)
	{
		case LEVELONE:
			m_changeStatFunc(LEVELONE);
			break;
			
		case LEVELTWO:
			break;
			
		case SETWORKMODE:
			break;

		default:
			break;
	}
	return m_curState;
}


void State::showMenuOsd(osdInfo_t& disMenuBuf)
{
	#if 0
	unsigned char r, g, b, a, color;
	short x, y;

	char font = 1;
	char fontsize = 4;

	for(int i = 0; i < disMenuBuf.cnt; i++)
	{
		if(disMenuBuf.osdBuffer[i].bshow)
		{
			x = disMenuBuf.osdBuffer[i].posx;
			y = disMenuBuf.osdBuffer[i].posy;
			a = disMenuBuf.osdBuffer[i].alpha;
			color = disMenuBuf.osdBuffer[i].color;
			getRGBA(color,r,g,b,a);
			drawFunc(x, y,disMenuBuf.osdBuffer[i].disMenu, font ,fontsize, r, g, b, a, VIDEO_DIS_WIDTH, VIDEO_DIS_HEIGHT);
		}
	}
	#endif
	return;
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

