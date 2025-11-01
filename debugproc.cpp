//****************************************************************
//
// デバッグ表示の処理[debugproc.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "debugproc.h"
#include "manager.h"

// 静的メンバ変数宣言
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[CDebugProc::Config::BufferSize] = {};
int CDebugProc::m_nowIndx = 0;
bool CDebugProc::m_Enable = true;

//*********************************************
// コンストラクタ
//*********************************************
CDebugProc::CDebugProc()
{

}

//*********************************************
// デストラクタ
//*********************************************
CDebugProc::~CDebugProc()
{

}

//*********************************************
// 初期化処理
//*********************************************
void CDebugProc::Init()
{
	m_nowIndx = 0;
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// デバック表示用フォントの生成
	D3DXCreateFont(pDevice, CDebugProc::Config::FontSize, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	memset(m_aStr, NULL, sizeof(m_aStr));
}

//*********************************************
// 終了処理
//*********************************************
void CDebugProc::Uninit(void)
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//*********************************************
// バッファに情報を登録
//*********************************************
void CDebugProc::print(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	// 今の文字の位置が最大数より大きかったら
	if (m_nowIndx > CDebugProc::Config::BufferSize)
	{
		// 今の文字の位置をゼロに戻す
		m_nowIndx = 0;
		return;
	}
	// バッファに積み上げる
	m_nowIndx += vsprintf(&m_aStr[m_nowIndx], fmt, args);
	va_end(args);
}

//*********************************************
// 座標を指定して描画
//*********************************************
void CDebugProc::Draw(int X, int Y)
{
	if (m_Enable == true)
	{
		// 指定された位置から描画
		RECT rect = { X,Y,SCREEN_WIDTH,SCREEN_HEIGHT };

		// テキスト表示
		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, WHITE);
		// バッファをクリア
		memset(&m_aStr[0], NULL, sizeof(m_aStr));
	}
}