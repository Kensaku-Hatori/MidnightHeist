//================================================================
//
// 画面の歪みの処理[distortion.h]
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
// 歪みシェーダのクラスを定義
//****************************************************************
class CDistortion : public CShader
{
public:
	// デストラクタ
	~CDistortion();

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
	/// ノイズ開始
	/// </summary>
	/// <param name="MaxNoiseRange">ノイズの上限値</param>
	/// <param name="MinNoiseRange">ノイズの下限値</param>
	/// <param name="NoiseSpeed">ノイズの速さ</param>
	void StartNoise(float MaxNoiseRange = 1.0f,float MinNoiseRange = 1.0f,float NoiseSpeed = 150.0f);
	/// <summary>
	/// ノイズ終了
	/// </summary>
	/// <param name=""></param>
	void EndNoise(void);
	/// <summary>
	/// パラメーター設定
	/// </summary>
	/// <param name="Scene"></param>
	void SetParameters(LPDIRECT3DTEXTURE9 Scene);
	/// <summary>
	/// ノイズカウンタを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カウンタ</returns>
	inline int GetNoiseCnt(void) { return m_NoiseCount; }
	/// <summary>
	/// ノイズ中かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>ノイズ中かどうか</returns>
	inline bool IsNoised(void) { return m_NoiseFrag; }
	///// <summary>
	///// エフェクトを取得
	///// </summary>
	///// <returns></returns>
	//inline LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CDistortion& Instance(void) {
		static CDistortion Instance;
		return Instance;
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