// Win32 TileMapTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Win32 TileMapTool.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND menuDlg;
HWND g_MainHwnd;
WCHAR szTitle[MAX_LOADSTRING];   // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
TCHAR Title[256] = TEXT("타일맵 툴 - made by vinoo");
TCHAR windowClass[256] = TEXT("타일맵 툴 - made by vinoo");

RECT ClientRect = { 0,0,670,710 };
CMapInfo MapInfo(10, 10);
CSeletRect CurRect(10, 10);

SIZE g_szWndSize;

bool ActiveGridLine = false;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MenuDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32TILEMAPTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TILEMAPTOOL));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TILEMAPTOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 670, 710, nullptr, nullptr, hInstance, nullptr);
   
   g_MainHwnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
HBITMAP CurBitmap;

void HBITMAP2BMP(HBITMAP hbit, char* Path)

{

	BITMAPFILEHEADER fh;

	BITMAPINFOHEADER ih;

	BITMAP bit;

	BITMAPINFO* pih;

	int PalSize;

	HANDLE hFile;

	DWORD dwWritten, Size;

	HDC hdc;



	hdc = GetDC(NULL);



	GetObject(hbit, sizeof(BITMAP), &bit);

	ih.biSize = sizeof(BITMAPINFOHEADER);

	ih.biWidth = bit.bmWidth;

	ih.biHeight = bit.bmHeight;

	ih.biPlanes = 1;

	ih.biBitCount = bit.bmPlanes * bit.bmBitsPixel;

	if (ih.biBitCount > 8) ih.biBitCount = 24;

	ih.biCompression = BI_RGB;

	ih.biSizeImage = 0;

	ih.biXPelsPerMeter = 0;

	ih.biYPelsPerMeter = 0;

	ih.biClrUsed = 0;

	ih.biClrImportant = 0;



	PalSize = (ih.biBitCount == 24 ? 0 : 1 << ih.biBitCount) * sizeof(RGBQUAD);

	pih = (BITMAPINFO*)malloc(ih.biSize + PalSize);

	pih->bmiHeader = ih;



	GetDIBits(hdc, hbit, 0, bit.bmHeight, NULL, pih, DIB_RGB_COLORS);

	ih = pih->bmiHeader;



	if (ih.biSizeImage == 0) {

		ih.biSizeImage = ((((ih.biWidth * ih.biBitCount) + 31) & ~31) >> 3) * ih.biHeight;

	}



	Size = ih.biSize + PalSize + ih.biSizeImage;

	pih = (BITMAPINFO*)realloc(pih, Size);



	GetDIBits(hdc, hbit, 0, bit.bmHeight, (PBYTE)pih + ih.biSize + PalSize, pih, DIB_RGB_COLORS);



	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + PalSize;

	fh.bfReserved1 = 0;

	fh.bfReserved2 = 0;

	fh.bfSize = Size + sizeof(BITMAPFILEHEADER);

	fh.bfType = 0x4d42;



	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL,

		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, &fh, sizeof(fh), &dwWritten, NULL);

	WriteFile(hFile, pih, Size, &dwWritten, NULL);



	ReleaseDC(NULL, hdc);

	CloseHandle(hFile);

}

int FileOpenDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter, char* strDefExt)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "읽을 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = strDefExt;

	if (GetOpenFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}

int FileSaveDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "저장할 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = "map";

	if (GetSaveFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc,memDC,memDC2;
	PAINTSTRUCT ps;
	HBITMAP OldBitmap, CopyBitmap;
	HPEN Pen, oPen;
	HBRUSH Brush, oBrush;
	
	RECT rtWnd = { 0,0,670,710 }, rtDlg;
	static SIZE szDlgSize;

	static char strFilePath[MAX_PATH], strFileName[100];
	static char strFilter[] = "맵 파일(*.map)\0*.map\0";

    switch (message)
    {
	case WM_CREATE:
		menuDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_MENU), hWnd, MenuDlgProc);
		ShowWindow(menuDlg, SW_SHOW);
		AdjustWindowRect(&rtWnd, WS_OVERLAPPEDWINDOW, TRUE);
		GetWindowRect(menuDlg, &rtDlg);
		g_szWndSize.cx = rtWnd.right - rtWnd.left;
		g_szWndSize.cy = rtWnd.bottom - rtWnd.top;
		szDlgSize.cx = rtDlg.right - rtDlg.left;
		szDlgSize.cy = rtDlg.bottom - rtDlg.top;
		MoveWindow(hWnd, 100, 100, g_szWndSize.cx, g_szWndSize.cy, TRUE);
		MoveWindow(menuDlg, 100 + g_szWndSize.cx, 100, szDlgSize.cx, szDlgSize.cy, TRUE);
		MapInfo.InitMap(hInst);
		CurRect.InitSeletRect();
		
		
		return FALSE;
	case WM_MOVE:
		GetWindowRect(hWnd, &rtWnd);
		MoveWindow(menuDlg, LOWORD(lParam) + 670, HIWORD(lParam) - 50, szDlgSize.cx, szDlgSize.cy, TRUE);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_MENU_OPENFILE:
				if (FileOpenDilaog(hWnd, strFilePath, strFileName, strFilter, "map") != -1)
				{
					MapInfo.LoadMapFile(strFilePath);
					InvalidateRect(hWnd, NULL, TRUE);
				}
                break;
			case ID_MENU_SAVEFILE:
				if (FileSaveDilaog(hWnd, strFilePath, strFileName, strFilter) != -1)
				{
					MapInfo.SaveMapFile(strFileName);
				}
				break;
            case ID_END:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		hdc = BeginPaint(hWnd, &ps);
		MapInfo.PrintMap(hdc);
		//MapInfo.DrawShop(hdc);
		if (ActiveGridLine)
		{
			MapInfo.DrawGrid(hdc);
		}
		CurRect.PrintCurRect(hdc);
		EndPaint(hWnd, &ps);
		
        }
        break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			CurRect.CursorMove_Left();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP:
			CurRect.CursorMove_Up();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			CurRect.CursorMove_Right();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			CurRect.CursorMove_Down();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_GETMINMAXINFO: 
		//윈도우 크기를 650*650으로 고정
		/*((MINMAXINFO*)lParam)->ptMaxTrackSize.x = 670;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 710;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 670;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 710;*/
		return FALSE;
    case WM_DESTROY:
		
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


BOOL CALLBACK MenuDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int indexTemp;
	int sceneIndexTemp;
	switch (iMessage) {
	case WM_INITDIALOG:
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SBTN_WALL:
			MapInfo.SetTileType(CurRect.GetCurX(), CurRect.GetCurY(), 1);
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL); //활성 윈도우를 원래 윈도우로 돌림
			return TRUE;
		case IDC_SBTN_ROAD:
			MapInfo.SetTileType(CurRect.GetCurX(), CurRect.GetCurY(), 2);
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			return TRUE;
		case IDC_SBTN_COLL:
			MapInfo.SetCollider(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_DELCOLL:
			MapInfo.DeleteCollider(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_CHECK_GRID:
			ActiveGridLine = !ActiveGridLine;
			InvalidateRect(g_MainHwnd, NULL, TRUE);

			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_GATE:
			indexTemp=GetDlgItemInt(hDlg, IDC_EDIT_NEXTINDEX, NULL, TRUE);
			if (indexTemp < 0)
				break;
			MapInfo.SetGate(CurRect.GetCurX(), CurRect.GetCurY());
			MapInfo.SetNextIndex(CurRect.GetCurX(), CurRect.GetCurY(), indexTemp);
			MapInfo.DeleteCollider(CurRect.GetCurX(), CurRect.GetCurY());

			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);

			break;
		case IDC_SBTN_CANGATE:
			MapInfo.CancelGate(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_SHOPSET:
			MapInfo.SetShop(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_SHOPCANCEL:
			MapInfo.CancelShop(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_MONSTER_EYE:
			sceneIndexTemp = GetDlgItemInt(hDlg, IDC_EDIT_SCENEINDEX, NULL, TRUE);
			if (sceneIndexTemp < 0)
				break;
			MapInfo.SetMonster(CurRect.GetCurX(), CurRect.GetCurY(), 1, sceneIndexTemp);
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_SBTN_MONSTER_DRAGONE:
			sceneIndexTemp = GetDlgItemInt(hDlg, IDC_EDIT_SCENEINDEX, NULL, TRUE);
			if (sceneIndexTemp < 0)
				break;
			MapInfo.SetMonster(CurRect.GetCurX(), CurRect.GetCurY(), 2, sceneIndexTemp);
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_BUTTON_MONSTERCANCLE:
			MapInfo.CancelMonster(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_BUTTON_SPAWNSET:
			MapInfo.SetSpawnPoint(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		case IDC_BUTTON_SPAWNCANCLE:
			MapInfo.CancelSpawnPoint(CurRect.GetCurX(), CurRect.GetCurY());
			InvalidateRect(g_MainHwnd, NULL, TRUE);
			SendMessage(g_MainHwnd, WM_ACTIVATE, WA_ACTIVE, NULL);
			break;
		}
		break;
	case WM_DESTROY:
		DestroyWindow(hDlg);
		break;
	}
	return FALSE;
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
	case WM_DESTROY:
		EndDialog(hDlg, 0);
		return TRUE;
    }
    return (INT_PTR)FALSE;
}
