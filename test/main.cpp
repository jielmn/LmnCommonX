#ifdef _DEBUG
#include "vld.h"
#endif

#include "LmnCommon.h"
#include "LmnExcel.h"
#include <time.h>
//#pragma comment(lib,"User32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Oleaut32.lib")

char * Time2String(char * szDest, DWORD dwDestSize, const time_t * t) {
	struct tm  tmp;
	localtime_s(&tmp, t);

	_snprintf_s(szDest, dwDestSize, dwDestSize, "%02d:%02d:%02d", tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
	return szDest;
}

int test_excel() {
	printf("testing excel......\n");
	CoInitialize(NULL);

	if ( !CExcel::IfExcelInstalled() ) {
		printf("excel not installed!\n");
		return -1;
	}

	//CExcel a(TRUE);
	//for (int i = 0; i < 1000; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		a.WriteGrid(i, j, "hello");
	//	}
	//}

	CExcel b;
	std::vector<const char *> v;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 3; j++) {
			char * p = new char[10];
			SNPRINTF(p, 10, "%d", i * 3 + j + 1);
			v.push_back(p);
		}
	}
	b.WriteRange(0, 0, 3, 2, v);
	b.SaveAs("d:\\test\\a.xlsx");

	std::vector<const char *>::iterator it = v.begin();
	for (; it != v.end(); it++) {
		delete[] * it;
	}
	b.Quit();

	CoUninitialize();
	printf("end testing excel.\n");
	return 0;
}

int test_excel_1() {
	printf("testing excel......\n");

	if (!CExcelEx::IfExcelInstalled()) {
		printf("excel not installed!\n");
		return -1;
	}

	CExcelEx b(0, FALSE);
	b.WriteGrid(0, 0, "14:27:54");
	b.WriteGrid(0, 1, "32.01");
	b.WriteGrid(1, 0, "14:28:08");
	b.WriteGrid(1, 1, "34.1");
	b.WriteGrid(2, 0, "14:29:08");
	b.WriteGrid(2, 1, "32.52");

	//b.WriteGrid(0, 0, "5");
	//b.WriteGrid(0, 1, "10.0");
	//b.WriteGrid(1, 0, "15");
	//b.WriteGrid(1, 1, "20.0");
	
	b.PrintChartWithTwoColumns(0, 0, 1, "hello", 0, 0, TRUE );
	b.Quit();
	b.PrintChartWithTwoColumns(0, 0, 1, "hello", 0, 0, TRUE);

	printf("end testing excel.\n");
	return 0;
}

int test_excel_2() {
	printf("testing excel......\n");

	if (!CExcelEx::IfExcelInstalled()) {
		printf("excel not installed!\n");
		return -1;
	}

	CExcelEx b(0, TRUE);

	time_t t = time(0);
	char buf[8192];

	DWORD max_points_cnt = 100;
	for ( DWORD i = 0; i < max_points_cnt; i++ ) {
		Time2String(buf, sizeof(buf), &t);
		b.WriteGrid(i, 0, buf );

		DWORD d = GetRand(3200, 3900);
		SNPRINTF(buf, sizeof(buf), "%.2f", d / 100.0);
		b.WriteGrid(i, 1, buf);

		Time2String(buf, sizeof(buf), &t);
		b.WriteGrid(i, 2, buf);

		d = GetRand(3200, 3900);
		SNPRINTF(buf, sizeof(buf), "%.2f", d / 100.0);
		b.WriteGrid(i, 3, buf);

		t += 10;
	}

	CExcelEx::Series a[2];
	a[0].dwStartRowIndex = 0;
	a[0].dwStartColIndex = 0;
	a[0].dwEndRowIndex   = max_points_cnt-1;
	STRNCPY(a[0].szName, "胸口", sizeof(a[0].szName));
	a[0].bEmpty = FALSE;

	a[1].dwStartRowIndex = 0;
	a[1].dwStartColIndex = 2;
	a[1].dwEndRowIndex = max_points_cnt-1;
	STRNCPY(a[1].szName, "腋下", sizeof(a[1].szName));
	a[1].bEmpty = FALSE;

	double f = 30.0;
	b.PrintChartWithMultiSeries( a, 2, "商目", 0, 0, TRUE, &f);
	//b.Quit();

	printf("end testing excel.\n");
	return 0;
}

int test_excel_3()
{
	printf("testing excel......\n");

	if (!CExcelEx::IfExcelInstalled()) {
		printf("excel not installed!\n");
		return -1;
	}

	CExcelEx b(0, TRUE);
	CExcelEx c(0, TRUE);

	b.AddSheet();
	b.AddSheet();

	b.WriteGridEx(1, 0, 0, "123");
	b.WriteGridEx(1, 1, 1, "456");

	b.WriteGridEx(2, 0, 0, "abc");
	b.WriteGridEx(2, 1, 1, "xyz");

	b.WriteGridEx(3, 0, 0, "1c");
	b.WriteGridEx(3, 1, 1, "3z");

	b.SaveAs("d:\\123.xlsx");
	b.Quit();
	
	c.WriteGridEx(1, 0, 0, "aaad123");
	c.SaveAs("d:\\456.xlsx");
	c.Quit();

	printf("end testing excel.\n");
	return 0;
}

int main()
{
	CoInitialize(NULL);

	test_excel_3();

	CoUninitialize();

	getchar();
	return 0;
}