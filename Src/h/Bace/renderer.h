//================================================================
//
// レンダー処理[renderer.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "main.h"
#include "line.h"

//****************************************************************
// 前方宣言
//****************************************************************
class CLine;

//****************************************************************
// レンダークラスを定義
//****************************************************************
class CRenderer
{
public:
	// コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="bWindow">ウィンドウかどうか</param>
	/// <returns>結果</returns>
	HRESULT Init(HWND hWnd,bool bWindow);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	/// <summary>
	/// シーンポリゴンを描画
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void);
	/// <summary>
	/// ブルームを適応して描画
	/// </summary>
	/// <param name=""></param>
	void DrawBloom(void);

	// セッター

	/// <summary>
	/// ワイヤーフレームを有効にする
	/// </summary>
	void onWireFrame();
	/// <summary>
	/// ワイヤーフレームを無効にする
	/// </summary>
	void offWireFrame();
	/// <summary>
	/// フォグを有効にする
	/// </summary>
	void onFog();
	/// <summary>
	/// フォグを無効にする
	/// </summary>
	void offFog();
	/// <summary>
	/// フルスクリーンを切り替える
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="isFullScrean">フルスクリーンかどうか</param>
	void ToggleFullscreen(HWND hWnd, bool* isFullScrean);
	/// <summary>
	/// フォグをセットアップ
	/// </summary>
	/// <param name="Color">色</param>
	/// <param name="Mode">フォグの種類</param>
	/// <param name="UseRange">範囲</param>
	/// <param name="Density">強さ</param>
	void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>デバイス</returns>
	inline LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	/// <summary>
	/// ウィンドウサイズを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>サイズ</returns>
	inline RECT GetWindowsRect(void) { return m_RectWindow; }
private:
	// メンバ変数
	// DirectX用
	LPDIRECT3D9 m_pD3D;				// デバイス
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイス
	RECT m_RectWindow;				// ウィンドウの大きさ
	D3DPRESENT_PARAMETERS m_d3dpp;	// プレゼンテーションパラメータ
	// ポストプロセス用
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// ポストプロセス用の頂点バッファ
	LPDIRECT3DTEXTURE9 m_SceneTex;		// ポストプロセス用のテクスチャ
	LPDIRECT3DSURFACE9 m_SceneSurface;	// ポストプロセス用のテクスチャサーフェイス
	// デバッグ用
	CLine m_pLine;					// 線を描画するクラスへのインスタンス
	D3DXCOLOR m_BackBufferCol;		// バックバッファの色
};