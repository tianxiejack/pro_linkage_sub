
#include "state.hpp"


CLinkManual::CLinkManual()
{
}

CLinkManual::~CLinkManual()
{
}

void CLinkManual::buttonMenu()
{
	m_pMenu->menuButton();	
	return;
}


void CLinkManual::enter()
{
	m_pMenu->enter();
	return;
}


void CLinkManual::inputNumber(char key)
{
	m_pMenu->normalKey(key);
	return;
}


void CLinkManual::showOsd()
{
	m_pMenu->showMenuOsd();
	#if 0
	if(m_menuShow)
		showMenuOsd(disMenuBuf);
	#endif
	return;
}

