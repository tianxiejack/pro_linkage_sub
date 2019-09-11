
#include "state.hpp"


DxTimer* State::m_timer = NULL;
char State::m_curState = LINKMANUAL;
CHANGESTAT State::m_changeStatFunc = NULL;
CMenu* State::m_pMenu = NULL;
CAutoManualFindRelation* State::m_autofr = NULL;
int State::twinkle_point_id = 0;
bool State::twinkle_flag = false;
IPC_ONVIF_POS State::m_curpos = {0};
static State* pThis = NULL;
std::vector< FEATUREPOINT_T > State::app_recommendPoints;
std::vector< FEATUREPOINT_T > State::app_recommendPoints_bak;

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

	//if(get_drawpoints_stat())	
	if(MENU_CALIB == m_pMenu->getMenuState())
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



void State::manuallinkage_moveball(int x, int y)
{

	SENDST trkmsg={0};
	float zoom;
	Point2i inPoint;
	Point2f outPoint;
	
	int delta_X ;
	int offset_x = 0;
	zoom = 2.0;

	inPoint.x = x;
	inPoint.y = y;

	if( -1 != m_autofr->Point2getPos(inPoint, outPoint))
	{		
		sendIpc2setPos(outPoint.x, outPoint.y, zoom);
	}

	return;
}


void State::autolinkage_moveball(int x, int y)
{
	return ;
	SENDST trkmsg={0};
	float zoom;
	Point2i inPoint;
	Point2f outPoint;
	
	int delta_X ;
	int offset_x = 0;
	zoom = 2.0;
	
	inPoint.x = x;
	inPoint.y = y;
		
	if( -1 != m_autofr->Point2getPos(inPoint, outPoint))
	{		
		transPix2rate(outPoint.x,outPoint.y);
		sendIpc2setSpeed(outPoint.x, outPoint.y, zoom);
	}

	return;
}


float State::getPanSpeed(float deltax)
{
	float ret;
	if(deltax < 0.0002)
		ret = 0;
	if(deltax < 0.00027)
		ret = 0.08;
	else if( deltax <0.00333)
		ret = 0.09;
	else if( deltax <0.0039)
		ret = 0.109;
	else if( deltax <0.0044)
		ret = 0.125;
	else if( deltax <0.005)
		ret = 0.14;
	else if(deltax < 0.0056)
		ret = 0.156;
	else if( deltax <0.011)
		ret = 0.219;
	else if(deltax<0.017)
		ret = 0.28;
	else if(deltax< 0.022)
		ret = 0.33;
	else if( deltax <0.028)
		ret = 0.34;
	else if(deltax < 0.033)
		ret = 0.36;
	else if(deltax< 0.044)
		ret = 0.375;
	else if(deltax<0.056)
		ret = 0.39;
	else if(deltax<0.067)
		ret = 0.406;
	else if( deltax <0.083)
		ret = 0.438;
	else if(deltax < 0.1)
		ret = 0.469;
	else if(deltax < 0.11)
		ret = 0.5;
	else if(deltax<0.14)
		ret = 0.53;
	else if( deltax <0.167)
		ret = 0.56;
	else if(deltax < 0.194)
		ret = 0.594;
	else if(deltax < 0.22)
		ret = 0.7;
	else if(deltax < 0.25)
		ret = 0.78;
	else if( deltax <0.278)
		ret = 0.843;
	else if(deltax < 0.333)
		ret = 0.86;
	else if(deltax < 0.389)
		ret = 0.875;
	else if(deltax < 0.44)
		ret = 0.89;
	else if(deltax < 0.5)
		ret = 0.91;
	else if( deltax <0.56)
		ret = 0.922;
	else
		ret = 0.938;
	return ret;
}

float State::getTilSpeed(float deltax)
{
	float ret;
	if(deltax < 0.0002)
		ret = 0;
	if(deltax < 0.00027)
		ret = 0.08;
	else if( deltax <0.00333)
		ret = 0.09;
	else if( deltax <0.0039)
		ret = 0.109;
	else if( deltax <0.0044)
		ret = 0.125;
	else if( deltax <0.005)
		ret = 0.14;
	else if(deltax < 0.0056)
		ret = 0.156;
	else if( deltax <0.011)
		ret = 0.219;
	else if(deltax<0.017)
		ret = 0.28;
	else if(deltax< 0.022)
		ret = 0.33;
	else if( deltax <0.028)
		ret = 0.34;
	else if(deltax < 0.033)
		ret = 0.36;
	else if(deltax< 0.044)
		ret = 0.375;
	else if(deltax<0.056)
		ret = 0.39;
	else if(deltax<0.067)
		ret = 0.406;
	else if( deltax <0.083)
		ret = 0.438;
	else if(deltax < 0.1)
		ret = 0.469;
	else if(deltax < 0.11)
		ret = 0.5;
	else if(deltax<0.14)
		ret = 0.53;
	else if( deltax <0.167)
		ret = 0.56;
	else if(deltax < 0.194)
		ret = 0.594;
	else if(deltax < 0.22)
		ret = 0.7;
	else if(deltax < 0.25)
		ret = 0.78;
	else if( deltax <0.278)
		ret = 0.843;
	else if(deltax < 0.333)
		ret = 0.86;
	else if(deltax < 0.389)
		ret = 0.875;
	else if(deltax < 0.44)
		ret = 0.89;
	else if(deltax < 0.5)
		ret = 0.91;
	else if( deltax <0.56)
		ret = 0.922;
	else
		ret = 0.938;
	return ret;
}



float State::deltaPan2rate(float x)
{
	enum{
		LEFT,
		RIGHT
	};
	float delta = m_curpos.p - x;

	float ret = 0;
	bool moveway = LEFT; // 0 left , 1 right

	float deltax1,deltax2;

	if(delta < -1.0)
	{
		ret = delta + 2.0;
		moveway = LEFT;
	}
	else if(delta >= -1.0 && delta < 0)
	{
		ret = delta;
		moveway = RIGHT;

	}
	else if(delta >=0 && delta <1.0)
	{
		ret = delta;
		moveway = LEFT;
	}
	else if(delta >= 1.0)
	{
		ret = 2.0 - delta;
		moveway = RIGHT;
	}

	ret = getPanSpeed(fabs(delta));
	
	if( LEFT == moveway )
		ret = -1*ret;
	
	return ret;
}

float State::deltaTil2rate(float y)
{
	enum{
		UP,
		DOWN
	};
	float delta = m_curpos.t - y;

	float ret = 0;
	bool moveway = UP;

	float deltay;


	if(delta < 0)
		moveway = UP;
	else
		moveway = DOWN;

	ret = getTilSpeed(fabs(delta));

	if(!moveway)
		ret = -1*ret;
	
	return ret;
}



void State::transPix2rate(float& x, float& y)
{
	float tmpx = 0,tmpy = 0;
	
	tmpx = deltaPan2rate(x);
	tmpy = deltaTil2rate(y);

	x = tmpx;
	y = tmpy;
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
		{
			app_recommendPoints.erase(app_recommendPoints.begin() + i);
		}
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
	set_PTZ_flag(true);

	if(m_autofr->getcalibnum() < 4)
		;
	else
	{
		cv::Point tmp;
		tmp.x = twinkle_point.x;
		tmp.y = twinkle_point.y;
		mapout2inresol(&tmp);
		manuallinkage_moveball(tmp.x, tmp.y);
		app_set_triangle_point(tmp.x, tmp.y);
	}
	return;
}

void State::stoptwinkle()
{
	m_timer->stopTimer(twinkle_point_id);
	set_twinkle_flag(false);;
	get_featurepoint();
	set_PTZ_flag(false);
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

void State::app_selectPoint(int x, int y)
{
	cv::Point2i outPixel;
	if(!in_recommand_vector(x, y, outPixel))
	{
		app_manualInsertRecommendPoints(x, y);
	}

	printf("%s, %d, app select point(%d, %d)\n", __FILE__,__LINE__,outPixel.x,outPixel.y);
	m_autofr->selectPoint(outPixel);
}

void State::app_manualInsertRecommendPoints(int x, int y)
{
	printf("%s, %d,%s start\n",__FILE__,__LINE__,__FUNCTION__);
	cv::Point2i inPoint;
	inPoint.x = x;
	inPoint.y = y;
	m_autofr->manualInsertRecommendPoints(inPoint);
}

void State::app_insertPos(float x, float y)
{
	printf("%s, %d, app insertpos(%f, %f)\n", __FILE__,__LINE__,x, y);
	cv::Point2f inPos;
	inPos.x = x;
	inPos.y = y;
	m_autofr->insertPos(inPos);
}


void State::operationChangeState()
{
	CMD_EXT stat;
	memset(&stat,0,sizeof(CMD_EXT));
	app_ctrl_getSysData(&stat);
	
	stat.MtdState[stat.SensorStat] = eImgAlg_Disable;
	app_ctrl_setMtdStat(&stat);
	sendIpc2ballstop();
	return;
}

void State::link2pos(int x,int y)
{
	if( x > m_pMenu->m_ScreenWidth/4 && x < m_pMenu->m_ScreenWidth/4*3 
		&& y > 0 && y < m_pMenu->m_ScreenHeight/2)
	{
		//link ball itself	
	}
	else 
	{
		mapgun2fullscreen_auto(&x,&y);
		manuallinkage_moveball(x,y);
	}
	return;
}

int State::mapgun2fullscreen_auto(int *x, int *y)
{
	mouserect rect1080p;
	mouserect rectgun;
	
	rect1080p.x = 0;
	rect1080p.y = 0;
	rect1080p.w = m_pMenu->m_ScreenWidth;// 1920;
	rect1080p.h = m_pMenu->m_ScreenHeight;

	rectgun.x = 0;
	rectgun.y = m_pMenu->m_ScreenHeight/2;
	rectgun.w = m_pMenu->m_ScreenWidth;// 1920;
	rectgun.h = m_pMenu->m_ScreenHeight/2;
	
	return maprect_point(x, y, rectgun, rect1080p);
}

int State::maprect_point(int *x, int *y, mouserect rectsrc,mouserect rectdest)
{
	if(NULL != x)
		*x = (*x-rectsrc.x)*rectdest.w/rectsrc.w+rectdest.x;
	if(NULL != y)
		*y = (*y-rectsrc.y)*rectdest.h/rectsrc.h+rectdest.y;
	return 0;
}

