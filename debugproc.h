//****************************************************************
//
// デバッグ表示の処理[debugproc.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二十インクルード防止
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

// インクルード
#include "main.h"

// クラスを定義
class CDebugProc
{
public:
	// デバッグフォントの規定値を設定
	struct Config {
		static constexpr int BufferSize = 1024;
		static constexpr int FontSize = 20;
	};
	// コンストラクタ・デストラクタ
	CDebugProc();
	~CDebugProc();

	// メンバ関数
	void Init(void);
	void Uninit(void);

	// 静的メンバ関数
	static void print(const char* fmt, ...);
	static void Draw(int X,int Y);
	static void End(void) { m_nowIndx = 0; };

	// ゲッター
	static bool GetEnable(void) { return m_Enable; };
	// セッター
	static void SetEnable(bool Enable) { m_Enable = Enable; };
private:
	// 静的メンバ変数
	static int m_nowIndx;								// 今のバッファのインデックス
	static LPD3DXFONT m_pFont;							// 文字を出力する用
	static char m_aStr[Config::BufferSize];	// バッファ
	static bool m_Enable;								// 有効かどうか
};
#endif // !_DEBUGPROC_H_