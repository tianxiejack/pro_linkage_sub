
#ifndef MENU_HPP_
#define MENU_HPP_

#include "configtable.h"
#include "statecommon.h"

class CMenu
{
	CMenu(OSDFUNC drawFunc);
	~CMenu();



public:

	void lv_1_inputPWosd();
	void lv_1_errorPWosd();
	void lv_2_osd();
	void lv_3_mtdosd();


	void menuButton();

	

	void showMenuOsd();


	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);


	void clearPw();



private:
	char *init_passwd;
	osdInfo_t disMenuBuf;
	char m_passwd[128];

	OSDFUNC drawFunc;
	char m_menuPointer;
	

};



#endif /* MENU_HPP_ */
