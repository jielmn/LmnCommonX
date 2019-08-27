#pragma once

#include "LmnCommon.h"

// 画出一条虚线
// 参数：
//      nWidth:  虚线的粗细
//      nSegmentLength: 虚线中，间隔的实线的最大长度
//      nGapLength: 虚线中，间隔的长度

typedef void (*DrawLineCb)(void * pArg, POINT x, POINT y);
typedef void (*DrawPolygonCb)(void * pArg, const POINT * pts, int cpt);

BOOL   DashLineTo( POINT start, POINT end, int nSegmentLength, int nGapLength, 
	               int nWidth, void * pArg, DrawLineCb pfnLine, DrawPolygonCb pfnPolygon = 0);