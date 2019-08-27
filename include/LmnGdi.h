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

// 画一条折线(原来的点的x坐标有相同的，对这些点的y坐标要求平均值)
// 参数：
//       pts，原折线点
//       cpt, 点个数         
//       nGap,多少像素差内的x坐标视为x坐标重合
typedef DrawPolygonCb DrawPolylineCb;
void   LmnPolyline( const POINT * pts, int cpt, int nGap, void * pArg, 
	                DrawPolylineCb pfnPolyline );