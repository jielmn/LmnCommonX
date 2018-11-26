#ifdef _DEBUG
#include "vld.h"
#endif

#include "LmnCommon.h"
#include "LmnExcel.h"

//#pragma comment(lib,"User32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Oleaut32.lib")

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
	b.WriteGrid(0, 0, "14:27:54");
	b.WriteGrid(0, 1, "32.01");
	b.WriteGrid(1, 0, "14:28:08");
	b.WriteGrid(1, 1, "34.1");
	b.WriteGrid(2, 0, "14:29:08");
	b.WriteGrid(2, 1, "32.52");

	b.WriteGrid(0, 3, "14:29:54");
	b.WriteGrid(0, 4, "33.01");
	b.WriteGrid(1, 3, "14:30:08");
	b.WriteGrid(1, 4, "35.1");
	b.WriteGrid(2, 3, "14:31:08");
	b.WriteGrid(2, 4, "33.52");

	CExcelEx::Series a[2];
	a[0].dwStartRowIndex = 0;
	a[0].dwStartColIndex = 0;
	a[0].dwEndRowIndex   = 2;
	STRNCPY(a[0].szName, "胸口", sizeof(a[0].szName));

	a[1].dwStartRowIndex = 0;
	a[1].dwStartColIndex = 3;
	a[1].dwEndRowIndex = 2;
	STRNCPY(a[1].szName, "腋下", sizeof(a[1].szName));


	b.PrintChartWithMultiSeries( a, 2, "商目", 0, 0, TRUE);
	//b.Quit();

	printf("end testing excel.\n");
	return 0;
}

int main()
{
	CoInitialize(NULL);
	test_excel_2();
	CoUninitialize();

	getchar();
	return 0;
}