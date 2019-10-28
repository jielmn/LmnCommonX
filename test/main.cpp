#ifdef _DEBUG
#include "vld.h"
#endif

#include "LmnCommon.h"
#include "LmnHttp.h"
#include "LmnSerialPort.h"

void clean_stdin()
{
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

enum  MenuIndex {
	MENU_MAIN,
	MENU_HTTP,
	MENU_HTTPS,
};

/* 用于非自动化测试 */

/************** 菜单 **************
 * 1. HTTP
 * 2. HTTPS
**********************************/

DWORD OnSelectChoice( ConsoleMenuHandle hMenu, const void * pArg, const char * szChoice ) {
	int e = (int)(long)pArg;

	if (0 == strcmp(szChoice, "1")) {
		return 0;
	}
	else if (0 == strcmp(szChoice, "2")) {
		return 1;
	}
	else if (0 == strcmp(szChoice, "3")) {
		return 2;
	}
	else if (0 == strcmp(szChoice, "4")) {
		return 3;
	}
	else if (0 == strcmp(szChoice, "5")) {
		return 4;
	}
	else if (0 == strcmp(szChoice, "6")) {
		return 5;
	}
	else if (0 == strcmp(szChoice, "q") && e == MENU_MAIN) {
		return QUIT_CONSOLE_MENU;
	}

	return 0xFF;
}

void OnHandleChoice( ConsoleMenuHandle hMenu, const void * pArg, DWORD dwIndex ) {
	int e = (int)(long)pArg;

	if ( e == MENU_HTTP ) {
		// visiting www.baidu.com
		if (dwIndex == 0) {
			CHttp * pHttp = CHttp::GetInstance();
			pHttp->Get("www.baidu.com");
		}
		else if (dwIndex == 1) {
			printf("input web site:");
			clean_stdin();
			char szWebSite[256];
			fgets(szWebSite, sizeof(szWebSite), stdin);
			StrTrim(szWebSite);
			CHttp * pHttp = CHttp::GetInstance();
			pHttp->Get(szWebSite);
		}
	}
	else if (e == MENU_HTTPS) {
		// visiting www.baidu.com
		if (dwIndex == 0) {
			CHttp * pHttp = CHttp::GetInstance();
			pHttp->Get("https://www.baidu.com");
		}
		else if (dwIndex == 1) {
			printf("input web site:");
			clean_stdin();
			char szWebSite[256];
			fgets(szWebSite, sizeof(szWebSite), stdin);
			StrTrim(szWebSite);
			CHttp * pHttp = CHttp::GetInstance();
			char  szWebSiteHttps[256];
			SNPRINTF(szWebSiteHttps, sizeof(szWebSiteHttps), "http://%s", szWebSite);
			pHttp->Get(szWebSiteHttps);
		}
	}
	else if (e == MENU_MAIN) {
		if (dwIndex == 2) {
			std::map<int, std::string> m;
			BOOL bRet = GetAllSerialPorts(m);
			if (bRet) {
				std::map<int, std::string>::iterator it;
				for ( it = m.begin(); it != m.end(); ++it ) {
					fprintf(stdout, "%d, %s\n", it->first, it->second.c_str());
				}				
			}
		}
	}
}

void OnHttpData ( int nError, DWORD dwCode, const char * szData, DWORD dwDataLen,
	        const char * szHeader, DWORD dwHeaderLen, void * context ) {

	printf("\n");
	printf("nError:%d\n", nError);

	if ( 0 == nError ) {
		printf("Code:%lu\n", dwCode);
		if (szHeader && dwHeaderLen > 0) {
			printf("Header:\n");
			printf("%s\n", szHeader);
		}
		
		if (szData && dwDataLen > 0) {
			printf("Content:\n");
			printf("%s\n", szData);
		}		
	}
}


int main () {
	int ret = 0;

	ret = InitConsole( OnSelectChoice, OnHandleChoice );

	/*菜单项及其子项*/
	ConsoleMenuHandle hMain   = CreateConsoleMenu( "LmnCommon Test", (void *)MENU_MAIN );

	ConsoleMenuHandle hHttp   = CreateConsoleMenu( "HTTP",  (void *)MENU_HTTP );
	ConsoleMenuHandle hHttps  = CreateConsoleMenu( "HTTPS", (void *)MENU_HTTPS );

	TConsoleMenuItem  items[10];
	memset(items, 0, sizeof(items));

	STRNCPY(items[0].szName, "1.HTTP",         sizeof(items[0].szName) );
	STRNCPY(items[1].szName, "2.HTTPS",        sizeof(items[1].szName));
	STRNCPY(items[2].szName, "3.Serial Ports", sizeof(items[2].szName));
	STRNCPY(items[3].szName, "q.QUIT",         sizeof(items[3].szName));
	items[0].hMenu = hHttp;
	items[1].hMenu = hHttps;

	ret = AddConsoleMenuItem(hMain, &items[0]);
	ret = AddConsoleMenuItem(hMain, &items[1]);
	ret = AddConsoleMenuItem(hMain, &items[2]);
	ret = AddConsoleMenuItem(hMain, &items[3]);

	// http 子菜单
	memset(items, 0, sizeof(items));
	STRNCPY(items[0].szName, "1.www.baidu.com", sizeof(items[0].szName));
	STRNCPY(items[1].szName, "2.any web site",  sizeof(items[1].szName));
	STRNCPY(items[2].szName, "3.RETURN",        sizeof(items[2].szName));
	items[2].hMenu = hMain;

	ret = AddConsoleMenuItem(hHttp, &items[0]);
	ret = AddConsoleMenuItem(hHttp, &items[1]);
	ret = AddConsoleMenuItem(hHttp, &items[2]);

	// https 子菜单
	memset(items, 0, sizeof(items));
	STRNCPY(items[0].szName, "1.www.baidu.com", sizeof(items[0].szName));
	STRNCPY(items[1].szName, "2.any web site", sizeof(items[1].szName));
	STRNCPY(items[2].szName, "3.RETURN", sizeof(items[2].szName));
	items[2].hMenu = hMain;

	ret = AddConsoleMenuItem(hHttps, &items[0]);
	ret = AddConsoleMenuItem(hHttps, &items[1]);
	ret = AddConsoleMenuItem(hHttps, &items[2]);

	InitHttpStack(OnHttpData);

	ret = DisplayConsoleMenu(hMain);
	ret = DeinitConsole();
	DeinitHttpStack();

	return 0;
}