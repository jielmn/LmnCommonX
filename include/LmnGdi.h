#pragma once

#include "LmnCommon.h"

// ����һ������
// ������
//      nWidth:  ���ߵĴ�ϸ
//      nSegmentLength: �����У������ʵ�ߵ���󳤶�
//      nGapLength: �����У�����ĳ���
BOOL  DashLineTo( HDC  hDC, HPEN  hPen, POINT start, POINT end,
	              int nSegmentLength, int nGapLength, 
	              int nWidth = 1, HBRUSH  hBrush = 0 );