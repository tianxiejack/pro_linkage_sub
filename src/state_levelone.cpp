
#include "state.hpp"


LevelOne::LevelOne()
{
}

LevelOne::~LevelOne()
{
}

void LevelOne::buttonMenu()
{
	m_pMenu->menuButton();


	#if 0
	m_menuShow = !m_menuShow;
	if(m_menuShow == true)
	{
		clearPw();
		inputtingStat = true;
		inputPWOsd();
	}
	else
		inputtingStat = false;
	#endif
	
	return;
}


void LevelOne::enter()
{
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


void LevelOne::inputNumber(char key)
{
	#if 0
	if(inputtingStat)
	{
		int offset = strlen(m_passwd) * sizeof(char);		
		if(offset < sizeof(m_passwd) - 1)
		{
			sprintf(m_passwd + offset,"%c", key);
			sprintf(m_dispasswd + offset,"%c", '*');
			swprintf(disMenuBuf.osdBuffer[1].disMenu, 33, L"%s", m_dispasswd);		
		}
		else
			printf("password reached max length:128");
		
		printf("%s,%d,passwd=%s\n",__FILE__,__LINE__,m_passwd);
	}
	#endif
	return;
}


void LevelOne::showOsd()
{
	m_pMenu->showMenuOsd();
	#if 0
	if(m_menuShow)
		showMenuOsd(disMenuBuf);
	#endif
	return;
}

