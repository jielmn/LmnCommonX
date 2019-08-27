#pragma once

#include "LmnCommon.h"

// ����һ������
// ������
//      nWidth:  ���ߵĴ�ϸ
//      nSegmentLength: �����У������ʵ�ߵ���󳤶�
//      nGapLength: �����У�����ĳ���

typedef void (*DrawLineCb)(void * pArg, POINT x, POINT y);
typedef void (*DrawPolygonCb)(void * pArg, const POINT * pts, int cpt);

BOOL   DashLineTo( POINT start, POINT end, int nSegmentLength, int nGapLength, 
	               int nWidth, void * pArg, DrawLineCb pfnLine, DrawPolygonCb pfnPolygon = 0);

// ��һ������(ԭ���ĵ��x��������ͬ�ģ�����Щ���y����Ҫ��ƽ��ֵ)
// ������
//       pts��ԭ���ߵ�
//       cpt, �����         
//       nGap,�������ز��ڵ�x������Ϊx�����غ�
typedef DrawPolygonCb DrawPolylineCb;
void   LmnPolyline( const POINT * pts, int cpt, int nGap, void * pArg, 
	                DrawPolylineCb pfnPolyline );