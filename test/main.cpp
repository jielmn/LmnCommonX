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
	b.WriteGrid(0, 1, "10");
	b.WriteGrid(1, 0, "14:28:08");
	b.WriteGrid(1, 1, "20");

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

int main()
{
	CoInitialize(NULL);
	test_excel_1();
	CoUninitialize();

	getchar();
	return 0;
}