//****************************************************************
//
// 画面の歪みの処理[distortion.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "shader.h"

// 歪みシェーダのクラスを定義
class CDistortion : public CShader
{
public:
	// デストラクタ
	~CDistortion();

	// メンバ関数
	HRESULT Init(void);
	void ReSet(void);
	void ReStart(void);

	// パラメータ設定
	void StartNoise(float MaxNoiseRange = 1.0f,float MinNoiseRange = 1.0f,float NoiseSpeed = 150.0f);
	void EndNoise(void);
	void SetParameters(LPDIRECT3DTEXTURE9 Scene);

	// ゲッター
	int GetNoiseCnt(void) { return m_NoiseCount; }
	bool IsNoised(void) { return m_NoiseFrag; }
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CDistortion* Instance(void) {
		static std::unique_ptr<CDistortion> Instance(new CDistortion);
		return Instance.get();
	};
private:
	// コンストラクタ
	CDistortion() {};
	// ノイズフラグ
	bool m_NoiseFrag;
	// ノイズ用のカウンタ
	int m_NoiseCount;
	// ノイズの収束する速さ
	float m_NoiseSpeed;
	// ノイズの範囲
	float m_NoiseMinRange, m_NoiseMaxRange;
	// ハンドル
	D3DXHANDLE m_SceneTexHandle;// シーンテクスチャのハンドル
	D3DXHANDLE m_NoizeTexHandle;// ノイズテクスチャのハンドル
	D3DXHANDLE m_CounterHandle;	// カウンタハンドル
	D3DXHANDLE m_SpeedHandle;	// 速さハンドル
	D3DXHANDLE m_MaxRangeHandle;// 最大レンジハンドル
	D3DXHANDLE m_MinRangeHandle;// 最小レンジハンドル
	// ノイズテクスチャ
	LPDIRECT3DTEXTURE9 m_NoizeTex;	// ノイズテクスチャ
};