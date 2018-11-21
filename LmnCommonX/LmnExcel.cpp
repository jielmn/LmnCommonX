#include <assert.h>
#include "LmnExcel.h"

#define  MAX_EXCEL_LINE_LENGTH           1024
#define  MAX_EXCEL_COLUMN_LENGTH         8
#define  MAX_EXCEL_GRID_LENGTH           16
#define  MAX_RANGE_LENGTH                32

// AutoWrap() - Automation helper function...
static HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...) {
	// Begin variable-argument list...
	va_list marker;
	va_start(marker, cArgs);

	if (!pDisp) {
		fprintf(stderr, "NULL IDispatch passed to AutoWrap()");
		return -1;
	}

	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char buf[200];
	char szName[200];

	// Convert down to ANSI
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	// Get DISPID for name passed...
	hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if (FAILED(hr)) {
		sprintf(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
		fprintf(stderr, buf);
		return hr;
	}

	// Allocate memory for arguments...
	VARIANT *pArgs = new VARIANT[cArgs + 1];
	// Extract arguments...
	for (int i = 0; i < cArgs; i++) {
		pArgs[i] = va_arg(marker, VARIANT);
	}

	// Build DISPPARAMS
	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	// Handle special-case for property-puts!
	if (autoType & DISPATCH_PROPERTYPUT) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	// Make the call!
	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
	if (FAILED(hr)) {
		sprintf(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
		fprintf(stderr, buf);
		delete[] pArgs;
		return hr;
	}
	// End variable-argument section...
	va_end(marker);
	delete[] pArgs;
	return hr;
}

// 整形形式的column转为excel形式的column，例如 A  XFD等
static char *  ColIndex2Excel(char * szColumn, DWORD dwColumnSize, DWORD dwColIndex ) {
	assert(dwColumnSize >= MAX_EXCEL_COLUMN_LENGTH);

	DWORD dwRet = 0;
	DWORD dwReminder = 0;
	DWORD pos = 0;
	char  szTemp[MAX_EXCEL_COLUMN_LENGTH] = { 0 };

	do 
	{
		if (dwRet > 0) {
			dwColIndex = dwRet - 1;
		}

		dwRet = dwColIndex / 26;
		dwReminder = dwColIndex % 26;
		szTemp[pos] = (char)('A' + dwReminder);
		pos++;
	} while ( dwRet > 0 );
	
	for (DWORD i = 0; i < pos; i++) {
		szColumn[i] = szTemp[pos - i - 1];
	}

	return szColumn;
}

static char *  RowColIndex2Excel(char * szGrid, DWORD dwGridSize, DWORD dwRowIndex, DWORD dwColIndex) {
	char szColumn[MAX_EXCEL_COLUMN_LENGTH] = { 0 };
	ColIndex2Excel(szColumn, sizeof(szColumn), dwColIndex);
	SNPRINTF(szGrid, dwGridSize, "%s%lu", szColumn, dwRowIndex + 1);
	return szGrid;
}



BOOL  CExcel::IfExcelInstalled() {
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);
	if (FAILED(hr)) {
		return FALSE;
	}	
	return TRUE;
}


CExcel::CExcel( BOOL bVisible /*= FALSE*/ ) {
	m_bInited = FALSE;
	HRESULT hr = CLSIDFromProgID( L"Excel.Application", &m_clsid );
	if (FAILED(hr)) {
		return;
	}

	hr = CoCreateInstance( m_clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&m_pXlApp );
	if ( FAILED(hr) ) {
		return;
	}

	VARIANT x;
	if (bVisible) {
		x.vt = VT_I4;
		x.lVal = 1;
		AutoWrap( DISPATCH_PROPERTYPUT, NULL, m_pXlApp, L"Visible", 1, x );
	}

	VARIANT result;
	VariantInit(&result);
	AutoWrap(DISPATCH_PROPERTYGET, &result, m_pXlApp, L"Workbooks", 0);
	m_pXlBooks = result.pdispVal;

	VariantInit(&result);
	AutoWrap(DISPATCH_PROPERTYGET, &result, m_pXlBooks, L"Add", 0);
	m_pXlBook = result.pdispVal;
	
	VariantInit(&result);
	AutoWrap(DISPATCH_PROPERTYGET, &result, m_pXlApp, L"ActiveSheet", 0);
	m_pXlSheet = result.pdispVal;

	m_bInited = TRUE;
}

CExcel::~CExcel() {
	if ( !m_bInited ) {
		return;
	}

	m_pXlSheet->Release();
	m_pXlBook->Release();
	m_pXlBooks->Release();
	m_pXlApp->Release();
}

int  CExcel::WriteGrid( DWORD dwRowIndex, DWORD dwColIndex, const char * szValue ) {
	if ( 0 == szValue ) {
		return 0;
	}

	if ( !m_bInited ) {
		return -1;
	}

	VARIANT arr;
	arr.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sab[2];
	sab[0].lLbound = 1; sab[0].cElements = 1;
	sab[1].lLbound = 1; sab[1].cElements = 1;
	arr.parray = SafeArrayCreate(VT_VARIANT, 2, sab);

	WCHAR szTmp[MAX_EXCEL_LINE_LENGTH];
	MultiByteToWideChar(CP_ACP, 0, szValue, -1, szTmp, MAX_EXCEL_LINE_LENGTH);

	VARIANT tmp;
	tmp.vt = VT_BSTR;
	tmp.bstrVal = SysAllocString(szTmp);
	long indices[] = { 1,1 };
	SafeArrayPutElement(arr.parray, indices, (void *)&tmp);
	VariantClear(&tmp);

	char szGrid[MAX_EXCEL_GRID_LENGTH] = { 0 };
	RowColIndex2Excel(szGrid, sizeof(szGrid), dwRowIndex, dwColIndex);
	MultiByteToWideChar(CP_ACP, 0, szGrid, -1, szTmp, MAX_EXCEL_LINE_LENGTH);

	IDispatch * pXlRange = 0;
	VARIANT parm;
	parm.vt = VT_BSTR;
	parm.bstrVal = ::SysAllocString(szTmp);

	VARIANT result;
	VariantInit(&result);
	AutoWrap(DISPATCH_PROPERTYGET, &result, m_pXlSheet, L"Range", 1, parm);
	VariantClear(&parm);

	pXlRange = result.pdispVal;

	AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlRange, L"Value", 1, arr);
	VariantClear(&arr);

	pXlRange->Release();
	return 0;
}

int  CExcel::WriteRange( DWORD dwStartRowIndex, DWORD dwStartColIndex,
	                     DWORD dwEndRowIndex, DWORD dwEndColIndex, 
	                     const std::vector<const char *> & vValues ) 
{
	if (!m_bInited) {
		return -1;
	}

	if ( dwStartRowIndex > dwEndRowIndex ) {
		return -1;
	}

	if (dwStartColIndex > dwEndColIndex) {
		return -1;
	}

	DWORD  dwRows = dwEndRowIndex - dwStartRowIndex + 1;
	DWORD  dwCols = dwEndColIndex - dwStartColIndex + 1;

	VARIANT arr;
	arr.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sab[2];
	sab[0].lLbound = 1; sab[0].cElements = dwRows;
	sab[1].lLbound = 1; sab[1].cElements = dwCols;
	arr.parray = SafeArrayCreate(VT_VARIANT, 2, sab);

	WCHAR wszTmp[MAX_EXCEL_LINE_LENGTH];
	std::vector<const char *>::const_iterator it;
	it = vValues.begin();

	for (DWORD i = 0; i < dwRows && it != vValues.end(); i++) {
		for (DWORD j = 0; j < dwCols && it != vValues.end(); j++, it++) {
			if (*it == 0) {
				wszTmp[0] = L'\0';
			}
			else {
				MultiByteToWideChar(CP_ACP, 0, *it, -1, wszTmp, MAX_EXCEL_LINE_LENGTH);
			}
			

			VARIANT tmp;
			tmp.vt = VT_BSTR;
			tmp.bstrVal = SysAllocString(wszTmp);
			long indices[] = { (long)(i+1), (long)(j+1) };
			SafeArrayPutElement(arr.parray, indices, (void *)&tmp);
			VariantClear(&tmp);
		}
	}

	char szStartGrid[MAX_EXCEL_GRID_LENGTH] = { 0 };
	RowColIndex2Excel(szStartGrid, sizeof(szStartGrid), dwStartRowIndex, dwStartColIndex);

	char szEndGrid[MAX_EXCEL_GRID_LENGTH] = { 0 };
	RowColIndex2Excel(szEndGrid, sizeof(szEndGrid), dwEndRowIndex, dwEndColIndex);

	char szRange[MAX_RANGE_LENGTH];
	SNPRINTF(szRange, sizeof(szRange), "%s:%s", szStartGrid, szEndGrid);
	MultiByteToWideChar(CP_ACP, 0, szRange, -1, wszTmp, MAX_EXCEL_LINE_LENGTH);

	IDispatch * pXlRange = 0;
	VARIANT parm;
	parm.vt = VT_BSTR;
	parm.bstrVal = ::SysAllocString(wszTmp);

	VARIANT result;
	VariantInit(&result);
	AutoWrap(DISPATCH_PROPERTYGET, &result, m_pXlSheet, L"Range", 1, parm);
	VariantClear(&parm);

	pXlRange = result.pdispVal;

	AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlRange, L"Value", 1, arr);
	VariantClear(&arr);

	pXlRange->Release();
	return 0;
}

int  CExcel::SaveAs(const char * szFilePath) {
	if (!m_bInited) {
		return -1;
	}

	if (0 == szFilePath) {
		return -1;
	}

	WCHAR wszTmp[MAX_EXCEL_LINE_LENGTH];
	MultiByteToWideChar(CP_ACP, 0, szFilePath, -1, wszTmp, MAX_EXCEL_LINE_LENGTH);

	VARIANT filename;
	filename.vt = VT_BSTR;
	filename.bstrVal = SysAllocString(wszTmp);

	VARIANT a;
	a.vt = VT_INT;
	a.llVal = 0;
	AutoWrap(DISPATCH_PROPERTYPUT, NULL, m_pXlApp, L"DisplayAlerts", 1, a);
	

	HRESULT hr = AutoWrap(DISPATCH_METHOD, NULL, m_pXlSheet, L"SaveAs", 1, filename); //保存
	VariantClear(&filename);
	if (FAILED(hr)) {
		return -1;
	}
	return 0;
}

int  CExcel::Quit() {
	if (!m_bInited) {
		return -1;
	}
	// Set .Saved property of workbook to TRUE so we aren't prompted
	// to save when we tell Excel to quit...
	VARIANT x;
	x.vt = VT_I4;
	x.lVal = 1;
	AutoWrap(DISPATCH_PROPERTYPUT, NULL, m_pXlBook, L"Saved", 1, x);

	// Tell Excel to quit (i.e. App.Quit)
	AutoWrap(DISPATCH_METHOD, NULL, m_pXlApp, L"Quit", 0);
	return 0;
}


BOOL CExcelEx::IfExcelInstalled() {
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);
	if (FAILED(hr)) {
		return FALSE;
	}
	return TRUE;
}

CExcelEx::CExcelEx(const char * szFilePath /*= 0*/, BOOL bVisible /*= FALSE*/ ) {
	HRESULT hr = m_pApp.CreateInstance( L"Excel.Application", 0, CLSCTX_LOCAL_SERVER );
	if ( FAILED(hr) ) {
		return;
	}

	if ( bVisible )
		m_pApp->Visible = VARIANT_TRUE;
	else 
		m_pApp->Visible = VARIANT_FALSE;

	m_pApp->UserControl = FALSE;
	m_pWorkBooks = m_pApp->Workbooks;

	if ( 0 == szFilePath ) {
		m_pWorkBook = m_pWorkBooks->Add(vtMissing);
	}
	else {
		try {
			m_pWorkBook = m_pWorkBooks->Open(szFilePath);
		}
		// 打开文件失败
		catch (_com_error & ) {
			m_pWorkBooks->Release();
			m_pApp->Release();
			return;
		}
	}

	m_pSheets = m_pWorkBook->GetWorksheets();
	m_pWorkSheet = m_pSheets->GetItem(_variant_t(1));
	m_pWorkSheet->Activate();

	m_pRange = m_pWorkSheet->GetCells();
}

CExcelEx::~CExcelEx() {
	if (m_pRange) {
		m_pRange->Release();
	}

	if ( m_pWorkSheet ) {
		m_pWorkSheet->Release();
	}

	if ( m_pSheets ) {
		m_pSheets->Release();
	}

	if (m_pWorkBook) {
		m_pWorkBook->Release();
	}

	if (m_pWorkBooks) {
		m_pWorkBooks->Release();
	}

	if (m_pApp) {
		m_pApp->Release();
	}
}

int  CExcelEx::WriteGrid(DWORD dwRowIndex, DWORD dwColIndex, const char * szValue) {
	if ( 0 == m_pRange ) {
		return -1;
	}
	m_pRange->Item[dwRowIndex + 1][dwColIndex + 1] = szValue;
	return 0;
}

int  CExcelEx::PrintChart( DWORD dwStartRowIndex, DWORD dwStartColIndex,
	                       DWORD dwEndRowIndex, DWORD dwEndColIndex ) {
	if ( 0 == m_pRange ) {
		return -1;
	}

	char szStartGrid[MAX_EXCEL_GRID_LENGTH] = { 0 };
	RowColIndex2Excel(szStartGrid, sizeof(szStartGrid), dwStartRowIndex, dwStartColIndex);

	char szEndGrid[MAX_EXCEL_GRID_LENGTH] = { 0 };
	RowColIndex2Excel(szEndGrid, sizeof(szEndGrid), dwEndRowIndex, dwEndColIndex);

	Excel::_ChartPtr pChart = m_pWorkBook->Charts->Add();
	pChart->PutChartType(Excel::xlXYScatterLinesNoMarkers);
	pChart->GetChartArea()->Width = 750;
	pChart->GetChartArea()->Height = 440;
	Excel::RangePtr pRange = m_pWorkSheet->Range[szStartGrid][szEndGrid];
	Excel::PageSetupPtr pageSetup = pChart->GetPageSetup();
	pageSetup->Orientation = Excel::xlLandscape;
	pageSetup->PutPaperSize(Excel::xlPaperA4);
	pChart->SetSourceData(pRange);
	pChart->ChartTitle->PutText("123567");
	Excel::LegendPtr lengend = pChart->GetLegend();
	lengend->Delete();

	pChart->PrintPreview();

	lengend->Release();
	pRange->Release();
	pageSetup->Release();
	pChart->Release();
	return 0;
}