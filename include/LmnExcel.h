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


#import "../Office2007/MSO.DLL"     rename("DocumentProperties", "DocumentPropertiesXL")  rename( "RGB", "MSORGB" ) no_namespace
#import "../Office2007/VBE6EXT.OLB" no_namespace   
#import "../Office2007/EXCEL.EXE"   rename("DialogBox", "DialogBoxXL") rename("CopyFile", "CopyFileXL") rename("ReplaceText", "ReplaceTextXL") rename("RGB", "RBGXL") rename("DocumentProperties", "DocumentPropertiesXL") no_dual_interfaces

class   CExcelEx {
public:
	static BOOL  IfExcelInstalled();
	CExcelEx(const char * szFilePath = 0, BOOL bVisible = FALSE);	
	~CExcelEx();
	int  WriteGrid(DWORD dwRowIndex, DWORD dwColIndex, const char * szValue);
	int  PrintChart( DWORD dwStartRowIndex, DWORD dwStartColIndex,
		             DWORD dwEndRowIndex,   DWORD dwEndColIndex );

private:
	Excel::_ApplicationPtr   m_pApp;
	Excel::WorkbooksPtr      m_pWorkBooks;
	Excel::_WorkbookPtr      m_pWorkBook;
	Excel::SheetsPtr         m_pSheets;
	Excel::_WorksheetPtr     m_pWorkSheet;
	Excel::RangePtr          m_pRange;
};





#endif