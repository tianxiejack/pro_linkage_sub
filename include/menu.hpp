
#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "statecommon.h"

class CMenu
{
public:
	CMenu(OSDFUNC pfun,CHANGESTAT pchStatfun);
	~CMenu();

	void lv_1_inputPWosd();
	void lv_1_errorPWosd();
	void lv_2_osd();
	void lv_3_mtdosd();


	void menuButton();
	void normalKey(char key);
	void enter();

	void showMenuOsd();


	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);


	void clearPw();




	void gotoInputPW();
	void gotoErrorPW();
	void gotoMainMenu();
	void gotoBlankMenu();

	

public:
	OSDFUNC drawFunc;

private:
	char *init_passwd;
	osdInfo_t disMenuBuf;
	char m_passwd[128];
	char m_dispasswd[128];
	static CHANGESTAT m_changeStatFunc;


	CHAR m_menuPointer;
	CHAR m_menuStat;
};



#endif /* MENU_HPP_ */
