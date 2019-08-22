
#include "state.hpp"


DxTimer* State::m_timer = NULL;
char State::m_curState = LINKMANUAL;
CHANGESTAT State::m_changeStatFunc = NULL;
CMenu* State::m_pMenu = NULL;
CAutoManualFindRelation* State::m_autofr = NULL;
int State::twinkle_point_id = 0;
bool State::twinkle_flag = false;

static State* pThis = NULL;

State::State():draw_print_stat(false),drawpoints_stat(false)
{	
	gridinter_mode = mouse_mode;
	twinkle_flag = false;
	pThis = this;
}

State::~State()
{
}


void State::StateInit(OSDFUNC pDraw ,CHANGESTAT pDisplaymode, CHANGESTAT pChangeStat , CHDEFWORKMD pChangeWkmode, CMvDectInterface *pMov)
{
	if(m_timer == NULL)
	{
		m_timer = new DxTimer();
		twinkle_point_id = m_timer->createTimer();
		m_timer->registerTimer(twinkle_point_id, TcallbackFunc, &twinkle_point_id);
	}
	if(m_pMenu == NULL)
		m_pMenu = new CMenu(pDraw,pDisplaymode,pChangeStat,pChangeWkmode,pMov);
	if(m_autofr == NULL)
	{
		m_autofr = new CAutoManualFindRelation(outputWHF[0],outputWHF[1], 6, 6);
		m_autofr->readParams(app_recommendPoints);
		m_autofr->create(pnotify_callback);
	}

	m_changeStatFunc = pChangeStat;
	return ;
}

void State::pnotify_callback(std::vector<FEATUREPOINT_T>& recommendPoints)
{
	printf("%s,%d, pnotify_callback start!\n",__FILE__,__LINE__);
	pThis->app_recommendPoints = recommendPoints;
	return;
}

void State::TcallbackFunc(void *p)
{
	static bool flag = false;
	int a = *(int *)p;
	if(a == pThis->twinkle_point_id)
	{
		flag = !flag;
		pThis->set_twinkle_flag(flag);
		//printf("%s : set flag = %d , get state = %d \n" ,__func__, flag , pThis->get_twinkle_flag());
	}
	return;
}


void State::create()
{
	
}


int State::ChangeState(char nextState)
{
	m_curState = nextState;
	switch(m_curState)
	{
		case LINKMANUAL:
			m_changeStatFunc(LINKMANUAL);
			break;

		default:
			break;
	}
	return m_curState;
}


void State::upMenu()
{
	m_pMenu->upMenu();
	return;
}

void State::downMenu()
{
	m_pMenu->downMenu();
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



/***************************************************************/

void State::drawPoints(cv::Mat frame)
{
	Drawfeaturepoints(frame);
	DrawTwinklePoint(frame);
	
	return;
}


void State::Drawfeaturepoints(cv::Mat frame)
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


void State::DrawTwinklePoint(cv::Mat frame)
{
	static bool bdraw =false;

	if(bdraw)
	{
		cv::circle(frame,twinkle_point_bak , 3 ,cvScalar(0,0,0,0), 2, 8, 0 );
		bdraw = false;
	}
		
	if(get_twinkle_flag())
	{
		twinkle_point_bak = twinkle_point;
		cv::circle(frame,twinkle_point_bak,3,cvScalar(0,0,255,255),2,8,0);
		bdraw = true;
	}
	return;
}


void State::mapout2inresol(cv::Point *tmppoint)
{
	int outputWHF_bak[3];
	memcpy(&outputWHF_bak, &outputWHF, sizeof(outputWHF_bak));
	tmppoint->x = tmppoint->x * vdisWH[1][0] / outputWHF_bak[0];
	tmppoint->y = tmppoint->y * vdisWH[1][1] / outputWHF_bak[1];	
}


void State::get_featurepoint()
{
	m_autofr->get_featurepoint(app_recommendPoints);
	return;
}



void State::grid_manuallinkage_moveball(int x, int y, int changezoom)
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



jos_mouse_Mode State::get_gridinter_mode()
{
	return gridinter_mode;
}

void State::set_gridinter_mode(jos_mouse_Mode value)
{
	gridinter_mode = value;
}

void State::set_jos_mouse_mode(jos_mouse_Mode mode)
{
	//printf("%s, %d,set_jos_mouse_mode=%d\n", __FILE__,__LINE__, mode);
	set_gridinter_mode(mode);
	//SENDST trkmsg2={0};
	//trkmsg2.cmd_ID = jos_mouse_mode;
	//trkmsg2.param[0] = mode;
	//ipc_sendmsg(&trkmsg2, IPC_FRIMG_MSG);
}


bool State::in_recommand_vector(int x, int y, cv::Point2i &outPixel)
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



void State::evade_pip(int x, int y)
{
	int mode = m_trig_pip_mode;
	mode = (mode + 1) % 4;
	m_trig_pip_mode = mode;
	return;
}


bool State::point_in_pip(int x, int y)
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



void State::process_trigmode_left_point(int x, int y)
{
	if(point_in_pip(x, y))
		evade_pip(x, y);
	else
		start_twinkle(x, y);
	return;
}


void State::app_self_deletePoint(cv::Point2i Pixel)
{
	for(int  i = 0; i < app_recommendPoints.size(); i++)
		if(app_recommendPoints[i].pixel == Pixel)
			app_recommendPoints.erase(app_recommendPoints.begin() + i);
	return;
}



void State::start_twinkle(int x, int y)
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
		twinkle_point.x = x;
		twinkle_point.y = y;
	}
	
	m_timer->startTimer(twinkle_point_id, 500);

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

void State::app_set_triangle_point(int x, int y)
{
	printf("%s, %d,%s start\n",__FILE__,__LINE__,__FUNCTION__);
	Point2i point_tmp;
	point_tmp.x = x;
	point_tmp.y = y;
	point_triangle = point_tmp;
	set_print_stat(true);
	return;
}


void State::process_trigmode_right_point(int x, int y)
{
	app_deletePoint(x, y);
}


void State::app_deletePoint(int x, int y)
{
	cv::Point2i outPixel;
	if(in_recommand_vector(x, y, outPixel))
		m_autofr->deletePoint(outPixel);
	return;
}

