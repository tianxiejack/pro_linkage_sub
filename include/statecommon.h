/*
 * statecommon.h
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#ifndef STATECOMMON_H_
#define STATECOMMON_H_

#define MAX_SUBMENU 8

typedef void (*OSDFUNC)(int x,int y,wchar_t* text,char font,char fontsize,unsigned char r,unsigned char g,unsigned char b,unsigned char a,int win_width,int win_height);
typedef void (*CHANGESTAT)(char nextMode);

#define CHAR signed char

typedef enum{
	LEVELONE,
	LEVELTWO,
	SETWORKMODE,
	LEVELMAX,
}state_enum;


typedef enum{
	MENU_BLANK,
	MENU_INPUTPW,
	MENU_ERRORPW,
	MENU_MAIN,
	MENU_WORKMODE,
	MENU_CALIB,
	MENU_MTD,
	MENU_MTD_REGION,
	MENU_MTD_UNREGION,
	MENU_MAX,
}MenuState_t;


enum
{
	JOSF1_AUTOLINKMODE = 1,
	JOSF2_MENU,
};


typedef struct
{
	bool bshow;
	unsigned char color;
	unsigned char alpha;
	unsigned int posx;
	unsigned int posy;
	wchar_t disMenu[33];
}osdInfopos_t;

typedef struct{
	int cnt;
	osdInfopos_t osdBuffer[MAX_SUBMENU];
}osdInfo_t;


#endif /* STATECOMMON_H_ */
