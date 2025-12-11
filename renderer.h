//****************************************************************
//
// レンダー処理[renderer.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _CRENDERER_H_
#define _CRENDERER_H_

// インクルード
#include "main.h"

// 前方宣言
class CLine;

// レンダーの基本クラスと定義
class CRenderer
{
// パブリック
public:
	// コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawScene(void);

	// セッター
	void onWireFrame();
	void offWireFrame();
	void onFog();
	void offFog();
	void ToggleFullscreen(HWND hWnd, bool* isFullScrean);
	void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
	void SetSize(UINT W, UINT H) { m_Width = W, m_Height = H; }

	// ゲッター
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	// ウィンドウのサイズを取得
	RECT GetWindowsRect(void) { return m_RectWindow; }
	UINT GetWindoWidth(void) { return m_Width; }
	UINT GetWindoHeight(void) { return m_Height; }
// プライベート
private:
	// 静的メンバ変数
	static CDebugProc* m_pDebugProc;	// デバッグ表示用
	LPDIRECT3D9 m_pD3D;				// デバイス
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイス
	RECT m_RectWindow;				// ウィンドウの大きさ
	D3DPRESENT_PARAMETERS m_d3dpp;	// プレゼンテーションパラメータ
	UINT m_Width;					// Imgui用のWindowの大きさ(W)
	UINT m_Height;					// Imgui用のWindowの大きさ(H)
	// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;
	LPDIRECT3DTEXTURE9 m_SceneTex;
	LPDIRECT3DSURFACE9 m_SceneSurface;
	std::unique_ptr<CLine> m_pLine;	// 線の可変長配列
	D3DXCOLOR m_BackBufferCol;		// バックバッファの色
};
#endif // !_CRENDERER_H_