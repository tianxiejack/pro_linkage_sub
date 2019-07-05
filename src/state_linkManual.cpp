
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
	#if 0
	if(m_menuShow)
	{
		if(inputtingStat)
		{
			if(!strcmp(init_passwd, m_passwd)){
				inputtingStat = true;
				clearPw();
				ChangeState(LEVELTWO);
				
				printf("watching :::!!!!!!! enter next state \n");
			}
			else{
				//printf("watching :::!!!!!!! error pw \n");
				inputtingStat = false;
				inputErrorPWOsd();
				clearPw();
			}
		}else{
			inputPWOsd();
			inputtingStat = true;
		}
	}
	#endif
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

