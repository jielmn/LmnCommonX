#pragma once

#include "LmnCommon.h"

// 画出一条虚线
// 参数：
//      nWidth:  虚线的粗细
//      nSegmentLength: 虚线中，间隔的实线的最大长度
//      nGapLength: 虚线中，间隔的长度
BOOL  DashLineTo( HDC  hDC, HPEN  hPen, POINT start, POINT end,
	              int nSegmentLength, int nGapLength, 
	              int nWidth = 1, HBRUSH  hBrush = 0 );