//================================================================
//
// 物陰の処理[shapeshadow.h]
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
#include "shader.h"

//****************************************************************
// クラスを定義
//****************************************************************
class CShapeShadow : public CShader
{
public:
	// デストラクタ
	~CShapeShadow();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// テクスチャへの描画
	/// </summary>
	/// <param name=""></param>
	void DrawTex(void);
	/// <summary>
	/// テクスチャクリア
	/// </summary>
	/// <param name=""></param>
	void Clear(void);
	/// <summary>
	/// 障害物として書き込みを開始
	/// </summary>
	/// <param name=""></param>
	void BeginObject(void);
	/// <summary>
	/// オブジェクトとして書き込みを開始
	/// </summary>
	/// <param name=""></param>
	void BeginScene(void);
	/// <summary>
	/// 書き込みを終了
	/// </summary>
	/// <param name=""></param>
	void EndTexs(void);
	/// <summary>
	/// リセット処理(デバイスロスト時)
	/// </summary>
	/// <param name=""></param>
	void ReSet(void);
	/// <summary>
	/// リスタート処理(デバイスロスト復帰時)
	/// </summary>
	/// <param name=""></param>
	void ReStart(void);
	/// <summary>
	/// パラメータ設定
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	void SetParameters(D3DXMATRIX World);
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CShapeShadow& Instance(void) {
		static CShapeShadow Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CShapeShadow() {};
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffMT;		// 頂点バッファ

	LPDIRECT3DTEXTURE9 m_ObjectTex;					// Z値テクスチャ
	LPDIRECT3DSURFACE9 m_ObjectSurface;				// Z値サーフェイス
	LPDIRECT3DSURFACE9 m_ObjectDepthSurface;		// 震度バッファサーフェイス

	LPDIRECT3DTEXTURE9 m_SceneTex;					// シーンのテクスチャ
	LPDIRECT3DSURFACE9 m_SceneSurface;				// シーンサーフェイス
	LPDIRECT3DSURFACE9 m_SceneDepthSurface;			// 震度バッファサーフェイス

	LPDIRECT3DSURFACE9 m_OldRT;						// 既存のレンダータゲット
	LPDIRECT3DSURFACE9 m_OldDepthSurface;			// 既存の震度バッファ

	// ハンドル
	D3DXHANDLE m_mtxWorldHandle;					// ワールドマトリックスのハンドル
	D3DXHANDLE m_ViewHandle;						// ライトビューマトリックスのハンドル
	D3DXHANDLE m_ProjHandle;						// ライトプロジェクションマトリックスのハンドル
	D3DXHANDLE m_ObjectTexHandle;					// オブジェクトテクスチャのハンドル
};