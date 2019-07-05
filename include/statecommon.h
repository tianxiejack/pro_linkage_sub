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


typedef enum{
	LEVELONE,
	LEVELTWO,
	SETWORKMODE,
	SETCALIB,
	SETMTD,
	SETMTD_REGION,
	SETMTD_UNREGION,
	LEVELMAX,
}state_enum;


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
