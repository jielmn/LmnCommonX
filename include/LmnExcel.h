#ifndef _LEMON_EXCEL_
#define _LEMON_EXCEL_

#include <vector>
#include "LmnCommon.h"


class  CExcel {
public:
	CExcel(BOOL bVisible = FALSE);
	~CExcel();
	int  WriteGrid ( DWORD dwRowIndex, DWORD dwColIndex, const char * szValue );
	int  WriteRange( DWORD dwStartRowIndex, DWORD dwStartColIndex, 
		             DWORD dwEndRowIndex,   DWORD dwEndColIndex, const std::vector<const char *> & vValues );
	int  SaveAs(const char * szFilePath);
	int  Quit();

	static BOOL  IfExcelInstalled();

private:
	BOOL                 m_bInited;
	CLSID                m_clsid;	
	IDispatch *          m_pXlApp;
	IDispatch *          m_pXlBooks;
	IDispatch *          m_pXlBook;
	IDispatch *          m_pXlSheet;
};






#endif