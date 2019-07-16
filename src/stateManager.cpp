/*
 * stateManager.cpp
 *
 *  Created on: 2019年7月4日
 *      Author: alex
 */

#include "stateManager.hpp"

#include <string.h>
#include "locale.h"
#include <stdio.h>
#include <wchar.h>



StateManger* StateManger::pThis = NULL;


StateManger::StateManger()
{
	m_state = new CLinkManual();
	m_curState = LINKMANUAL;
	pThis = this;
	gridinter_mode = mouse_mode;
	m_autofr = new CAutoManualFindRelation(outputWHF[0],outputWHF[1], 6, 6);
}

StateManger::~StateManger()
{
	
}


void StateManger::init(OSDFUNC func,CHANGESTAT chDisMode,CMvDectInterface *pMov)
{
	m_pMv = pMov;
	m_state->StateInit(func,chDisMode,callbackChangeStat,callbackChangeDefaultWorkMode);
	return;
}


void StateManger::ChangeState(State* state)
{
	m_state = state;
	m_state->operationChangeState();
	return;
}

void StateManger::specialEvent(char key)
{
	switch(key)
	{
		case JOSF1_AUTOLINKMODE:
			{
				//GB_WorkMode nextMode = (GB_WorkMode)(param - 1);
				//setWorkMode(nextMode);
			}
			break;
		case JOSF2_MENU:			
				m_state->buttonMenu();
			break;
		default:
			break;
	}

	return;
}


void StateManger::callbackChangeStat(char nextmode)
{
	

	return;
}

void StateManger::callbackChangeDefaultWorkMode(char workmode)
{
	pThis->m_defworkmode = workmode;
	//wait to add save param in param;
	printf("m_defworkmode = %d \n" , 	pThis->m_defworkmode);
	return;
}


void StateManger::mouseMove(int xMouse, int yMouse)
{
	return;
}

char StateManger::getMenuState()
{
	return m_state->m_pMenu->getMenuState();
}

void StateManger::process_trigmode_right_point(int x, int y)
{
	app_deletePoint(x, y);
}


void StateManger::app_deletePoint(int x, int y)
{
	cv::Point2i outPixel;
	if(in_recommand_vector(x, y, outPixel))
	{
		m_autofr->deletePoint(outPixel);
	}
	return;
}


void StateManger::mouseEvent(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(getMenuState() == MENU_MTD_REGION)
			getPoly().push_back(cv::Point(x,y));
		else if(getMenuState() == MENU_MTD_UNREGION)
			getPolyTmp().push_back(cv::Point(x,y));	
		else if(getMenuState() == MENU_CALIB)
			process_trigmode_left_point(x, y);
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if(getMenuState() == MENU_CALIB)
			process_trigmode_right_point(x, y);
		
	}
	




	return;
}


void StateManger::normalKeyEvent(char key)
{
	
	m_state->inputNumber(key);
	return;
}

std::vector<cv::Point>& StateManger::getPoly()
{
	return m_state->m_pMenu->m_poly;
}

std::vector<cv::Point>& StateManger::getPolyTmp()
{
	return m_state->m_pMenu->m_polyTmp;
}

std::vector< std::vector<cv::Point> >& StateManger::getunRoiPoly()
{
	return m_state->m_pMenu->m_unroiPoly;
}


void StateManger::enterKeyEvent()
{
	m_state->enter();
	return;
}


void StateManger::upMenu()
{
	m_state->upMenu();
	return;
}

void StateManger::downMenu()
{
	m_state->downMenu();
	return;
}

void StateManger::showOsd()
{
	m_state->showOsd();
	return;
}


void StateManger::process_trigmode_left_point(int x, int y)
{
	if(point_in_pip(x, y))
		evade_pip(x, y);
	else
		start_twinkle(x, y);
	return;
}


void StateManger::app_self_deletePoint(cv::Point2i Pixel)
{
	for(int  i = 0; i < app_recommendPoints.size(); i++)
		if(app_recommendPoints[i].pixel == Pixel)
			app_recommendPoints.erase(app_recommendPoints.begin() + i);
	return;
}



void StateManger::start_twinkle(int x, int y)
{
	get_featurepoint();
	cv::Point2i outPixel;

	if(in_recommand_vector(x, y, outPixel))
	{
		app_self_deletePoint(outPixel);
		twinkle_point = outPixel;
	}
	else
	{
		cv::Point2i outPixel_tmp;
		outPixel_tmp.x = x;
		outPixel_tmp.y = y;
		twinkle_point = outPixel_tmp;
	}
	
	//dtimer.startTimer(pThis->twinkle_point_id, 500);
	//set_twinkle_flag(true);

	if(m_autofr->getcalibnum() < 4)
		set_jos_mouse_mode(jos_mode);
	else
	{
		cv::Point tmp;
		tmp.x = twinkle_point.x;
		tmp.y = twinkle_point.y;
		mapout2inresol(&tmp);
		grid_manuallinkage_moveball(tmp.x, tmp.y, 0);
		app_set_triangle_point(tmp.x, tmp.y);
	}
	return;
}

void StateManger::app_set_triangle_point(int x, int y)
{
	printf("%s, %d,%s start\n",__FILE__,__LINE__,__FUNCTION__);
	Point2i point_tmp;
	point_tmp.x = x;
	point_tmp.y = y;
	point_triangle = point_tmp;
	set_print_stat(true);
	return;
}


void StateManger::grid_manuallinkage_moveball(int x, int y, int changezoom)
{
#if 0
	SENDST trkmsg={0};
	LinkagePos postmp;
	Point2i inPoint, outPoint;
	
	int delta_X ;
	int offset_x = 0;

	if(changezoom)
	{
		switch(m_display.g_CurDisplayMode) 
		{
			case TRIG_INTER_MODE:	
				offset_x = 0;  
				break;
			case MAIN_VIEW:
				offset_x =0;
				break;			
			default:
				break;
		}

		LeftPoint.x -= offset_x;
		RightPoint.x -=offset_x;
		
		delta_X = abs(LeftPoint.x - RightPoint.x) ;
		
		if(delta_X < MIN_VALID_RECT_WIDTH_IN_PIXEL)
		{
			postmp.zoom = 0;
		}
		else
		{
			postmp.zoom = checkZoomPosTable(delta_X);		
		}
	}
	else
		postmp.zoom = 0;

	inPoint.x = x;
	inPoint.y = y;
	//pThis->m_autofr.Point2getPos(inPoint, outPoint);
	if( -1 != pThis->m_autofr.Point2getPos(inPoint, outPoint))
	{		
		printf("%s, %d,grid inter mode: inPoint(%d,%d),outPos(%d,%d)\n", __FILE__,__LINE__,inPoint.x,inPoint.y,outPoint.x,outPoint.y);
		trkmsg.cmd_ID = acqPosAndZoom;
		postmp.panPos = outPoint.x;
		postmp.tilPos = outPoint.y;
		memcpy(&trkmsg.param,&postmp, sizeof(postmp));
		ipc_sendmsg(&trkmsg, IPC_FRIMG_MSG);
	}
#endif
	return;
}



jos_mouse_Mode StateManger::get_gridinter_mode()
{
	return gridinter_mode;
}

void StateManger::set_gridinter_mode(jos_mouse_Mode value)
{
	gridinter_mode = value;
}

void StateManger::set_jos_mouse_mode(jos_mouse_Mode mode)
{
	//printf("%s, %d,set_jos_mouse_mode=%d\n", __FILE__,__LINE__, mode);
	set_gridinter_mode(mode);
	//SENDST trkmsg2={0};
	//trkmsg2.cmd_ID = jos_mouse_mode;
	//trkmsg2.param[0] = mode;
	//ipc_sendmsg(&trkmsg2, IPC_FRIMG_MSG);
}


bool StateManger::in_recommand_vector(int x, int y, cv::Point2i &outPixel)
{
	std::vector<FEATUREPOINT_T> app_recommendPoints_tmp = app_recommendPoints;
	int delta_distance_bak;
	int insert_index = -1;
	int delta_distance;
	
	for(int i = 0; i < app_recommendPoints_tmp.size();  i++)
	{
		int deltax = abs(app_recommendPoints_tmp[i].pixel.x-x);
		int deltay = abs(app_recommendPoints_tmp[i].pixel.y-y);
		
		delta_distance = sqrt(deltax*deltax + deltay*deltay);
			
		if(i == 0)
		{
			delta_distance_bak = delta_distance;
			insert_index = i;
		}
		else
		{
			if(delta_distance < delta_distance_bak)
			{
				delta_distance_bak = delta_distance;
				insert_index = i;
			}

		}
	}
	printf("%s,%d,  delta_distance = %d\n",__FILE__,__LINE__,delta_distance_bak);
	if((insert_index >= 0) && (delta_distance_bak <= 15))
	{
		outPixel = app_recommendPoints_tmp[insert_index].pixel;
		return true;
	}

	outPixel.x = x;
	outPixel.y = y;
	return false;
}



void StateManger::evade_pip(int x, int y)
{
	int mode = m_trig_pip_mode;
	mode = (mode + 1) % 4;
	m_trig_pip_mode = mode;
	return;
}


bool StateManger::point_in_pip(int x, int y)
{
	int mode = m_trig_pip_mode;
	int startx,starty,endx,endy;
	
	if(0 == mode)
	{
		startx = 0;	
		starty = 0;
	}
	else if(1 == mode)
	{
		startx = outputWHF[0]/4*3;
		starty = 0;
	}
	else if(2 == mode)
	{
		startx = outputWHF[0]/4*3;
		starty = outputWHF[1]/4*3;
	}
	if(3 == mode)
	{
		startx = 0;
		starty = outputWHF[1]/4*3;
	}

	endx = startx + outputWHF[0]/4;
	endy = starty + outputWHF[1]/4;

	if(x>=startx && x<=endx && y>=starty && y<=endy)
		return true;
	else 
		return false;
}


void StateManger::get_featurepoint()
{
	m_autofr->get_featurepoint(app_recommendPoints);
	return;
}

void StateManger::mapout2inresol(cv::Point *tmppoint)
{
	int outputWHF_bak[3];
	memcpy(&outputWHF_bak, &outputWHF, sizeof(outputWHF_bak));
	tmppoint->x = tmppoint->x * vdisWH[1][0] / outputWHF_bak[0];
	tmppoint->y = tmppoint->y * vdisWH[1][1] / outputWHF_bak[1];	
}


void StateManger::Drawfeaturepoints(cv::Mat frame)
{
	static int drawpoint_falg = 0;
	if(drawpoint_falg)
	{
		m_autofr->drawPoints(frame, app_recommendPoints_bak, 0);
		drawpoint_falg = 0;
	}
	if(get_drawpoints_stat())
	{
		app_recommendPoints_bak = app_recommendPoints;
		m_autofr->drawPoints(frame, app_recommendPoints_bak, 1);
		drawpoint_falg = 1;
	}
	return;
}



