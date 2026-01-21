//****************************************************************
//
// シンプルなブルームの処理[SimpleBloom.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "shader.h"

// クラスを定義
class CSimpleBloom : public CShader
{
public:
	// デストラクタ
	~CSimpleBloom();

	// メンバ関数
	HRESULT Init(void);
	void ReSet(void);
	void ReStart(void);

	// セッター
	void ClearAll(void);

	// パラメータ設定
	void SetBrightParameters(LPDIRECT3DTEXTURE9 Scene);
	void SetBloomParameters(void);
	void SetBloomMap(void);

	// レンダータゲット切り替え
	void BeginBrightMap(void);
	void EndBrightMap(void);
	void BeginBloomMap(void);
	void EndBloomMap(void);

	// ゲッター
	LPDIRECT3DTEXTURE9& GetBloomMap(void) { return m_BloomMap; };
	// 静的メンバ関数
	static CSimpleBloom* Instance(void) {
		static std::unique_ptr<CSimpleBloom> Instance(new CSimpleBloom);
		return Instance.get();
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