//================================================================
//
// シンプルなブルームの処理[SimpleBloom.h]
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
class CSimpleBloom : public CShader
{
public:
	// デストラクタ
	~CSimpleBloom();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
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
	/// 全テクスチャのクリア
	/// </summary>
	/// <param name=""></param>
	void ClearAll(void);
	/// <summary>
	/// パラメーター設定(ぼかすとき)
	/// </summary>
	/// <param name="Scene">テクスチャ</param>
	void SetBrightParameters(LPDIRECT3DTEXTURE9 Scene);
	/// <summary>
	/// パラメーター設定(輝度を抽出するとき)
	/// </summary>
	/// <param name=""></param>
	void SetBloomParameters(void);
	/// <summary>
	/// GPUにブルームマップを渡す
	/// </summary>
	/// <param name=""></param>
	void SetBloomMap(void);
	/// <summary>
	/// 輝度マップへの書き込みを開始
	/// </summary>
	/// <param name=""></param>
	void BeginBrightMap(void);
	/// <summary>
	/// 書き込み終了
	/// </summary>
	/// <param name=""></param>
	void EndBrightMap(void);
	/// <summary>
	/// ブルームマップへの書き込みを開始
	/// </summary>
	/// <param name=""></param>
	void BeginBloomMap(void);
	/// <summary>
	/// 書き込み終了
	/// </summary>
	/// <param name=""></param>
	void EndBloomMap(void);
	/// <summary>
	/// ブルームマップを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline LPDIRECT3DTEXTURE9& GetBloomMap(void) { return m_BloomMap; }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CSimpleBloom& Instance(void) {
		static CSimpleBloom Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CSimpleBloom() {};
	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); };
	// メンバ変数
	// 明るさ抽出マップ
	LPDIRECT3DTEXTURE9 m_BrightMap;
	// ブルーム適応ごマップ(ブルーム部分のみ)
	LPDIRECT3DTEXTURE9 m_BloomMap;
	// サーフェイス
	LPDIRECT3DSURFACE9 m_BloomMapSurface;
	LPDIRECT3DSURFACE9 m_BrightMapSurface;
	// 既存のレンダータゲット
	LPDIRECT3DSURFACE9 m_OldRT;
	// ハンドル
	D3DXHANDLE m_SceneTexHandle;	// シーンテクスチャのハンドル
	D3DXHANDLE m_BrightMapHandle;	// 輝度抽出テクスチャのハンドル
	D3DXHANDLE m_BloomMapHandle;	// 最終結果テクスチャのハンドル
};