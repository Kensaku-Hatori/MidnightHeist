//*************************************
//
//　ウインドウ表示処理[main.cpp]
//　Author:Hatori Kensaku
//
//*************************************
// インクルード
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "SoundDevice.h"
#include "resource.h"
#include <crtdbg.h>

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int g_nCountFPS = 0;

//*************************************
// メイン関数
//*************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);//メモリリーク検知
	//_CrtSetBreakAlloc(131);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;

	if (FAILED(CSoundDevice::Instance()->Init()))
	{
		return -1;
	}

	CManager* pManager = NULL;
	pManager = new CManager;

	if (pManager != NULL)
	{
		//初期化処理
		if (FAILED(pManager->Init(hInstance,hWnd, TRUE)))
		{
			return -1;
		}
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	// 初期化
	DWORD dwFrameCount;					// フレームカウント
	DWORD dwFPSLastTime;				// 最後にFPSを計測した時刻
	dwFrameCount = 0;					// フレームカウントを初期化
	dwFPSLastTime = timeGetTime();		// 現在の時刻を取得

	srand((unsigned int)time(NULL));

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 乱数のシードを現在の時刻に設定
	srand(static_cast<int>(time(0)));

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻の取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// 保存
				dwFrameCount = 0;				// 0に戻す
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;	// 保存

				dwFrameCount++;

				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();
			}
		}
	}

	//終了処理
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	if (CSoundDevice::Instance() != nullptr)
	{
		CSoundDevice::Instance()->Uninit();
	}
	//
	timeEndPeriod(1);

	//ウインドウクラスの解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//*************************************
// ウインドウプロシージャ
//*************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	CRenderer* pRenderer = CManager::GetRenderer();

	switch (uMsg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) return 0;
		// 高さを HIWORD で渡す（間違い修正）
		pRenderer->SetSize(LOWORD(lParam), HIWORD(lParam));
	return 0;
	case WM_SYSCOMMAND:
		// Disable ALT application menu
		if ((wParam & 0xfff0) == SC_KEYMENU) 
			return 0;
		break;
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	//キー押下のメッセージ
	case WM_KEYDOWN:
		switch (wParam)
		{
		//ESCキーが押された場合
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
				break;
			}
			else
			{
				return 0;
			}
		default:
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//*************************************
// FPSを取得
//*************************************
int GetFPS(void)
{
	return g_nCountFPS;
}

//*************************************
// デルタタイムを取得
//*************************************
float GetDeltaTime(void)
{
	float dt = 1.0f / GetFPS();
	return dt;
}