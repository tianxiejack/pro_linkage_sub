/*
 * mvdectInterface.h
 *
 *  Created on: 2017年9月4日
 *      Author: cr
 */

#ifndef MVDECTINTERFACE_H_
#define MVDECTINTERFACE_H_

#include "infoHead.h"

typedef void ( *LPNOTIFYFUNC)(void *context, int chId);

class	CMvDectInterface
{
public:
	virtual ~CMvDectInterface(){};

public:
	virtual int	 	init(LPNOTIFYFUNC	notifyFunc, void *context){return 1;};
	virtual int 	destroy(){return 1;};
	virtual void	setFrame(cv::Mat	src ,int chId,int accuracy=2,int inputMinArea=8,int inputMaxArea=200,int threshold = 30){};//输入视频帧
	virtual void	setWarningRoi(std::vector<cv::Point2i>	warnRoi,	int chId	= 0){};//设置缩放前的警戒区域
	virtual void	setWarnMode(WARN_MODE	warnMode,	int chId	= 0){};//设置警戒模式
	virtual void	getMoveTarget(std::vector<TRK_RECT_INFO>	&resTarget,	int chId	= 0){};//移动目标
	virtual void	getWarnTarget(std::vector<TRK_RECT_INFO>	&resTarget,	int chId	= 0){};
	virtual void	setDrawOSD(cv::Mat	dispOSD, int chId = 0){};//设置绘制OSD对象
	virtual void	enableSelfDraw(bool	bEnable, int chId = 0){};//使能绘制
	virtual void 	setMatchingThreshold(const uint32_t matchingThreshold, int chId = 0){};
	virtual void 	setUpdateFactor(const uint32_t updateFactor, int chId = 0){};
	virtual bool  	getFrameMV(cv::Mat preFrame, cv::Mat curFrame, cv::Point2f  &pt){return false;};
	virtual bool	isRun(int chId){};
	virtual bool	isWait(int chId){};
	virtual bool	isStopping(int chId){};
	virtual void	mvOpen(int chId){};
	virtual void	mvClose(int chId){};
	virtual void     stoppingReset(int chId){};
};
CMvDectInterface *MvDetector_Create();
#endif /* MVDECTINTERFACE_H_ */
