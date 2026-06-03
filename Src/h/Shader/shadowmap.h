//================================================================
//
// シャドウマップの処理[shadowmap.h]
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
class CShadowMap : public CShader
{
public:
	// デストラクタ
	~CShadowMap();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// テクスチャクリア
	/// </summary>
	/// <param name=""></param>
	void Clear(void);
	/// <summary>
	/// テクスチャに書き込み
	/// </summary>
	/// <param name=""></param>
	void WriteMaps(void);
	/// <summary>
	/// 書き込み終了
	/// </summary>
	/// <param name=""></param>
	void EndMaps(void);
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
	/// ビューマトリックスを取得
	/// </summary>
	/// <returns>マトリックス</returns>
	inline D3DXMATRIX& GetLightView() { return m_LightView; }
	/// <summary>
	/// プロジェクションマトリックスを取得
	/// </summary>
	/// <returns>マトリックス</returns>
	inline D3DXMATRIX& GetLightProj() { return m_LightProj; }
	/// <summary>
	/// シャドウマップを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マップ</returns>
	inline LPDIRECT3DTEXTURE9& GetTex(void) { return m_ShadowMap; }
	/// <summary>
	/// シャドウマップのサーフェイスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>サーフェイス</returns>
	inline LPDIRECT3DSURFACE9 GetSurface(void) { return m_ShadowMapSurface; }
	/// <summary>
	/// シャドウマップのライトの向き
	/// </summary>
	/// <param name=""></param>
	/// <returns>向き</returns>
	inline D3DXVECTOR3 GetLightVec(void) { return m_LightVec; }
	/// <summary>
	/// シャドウマップの描画範囲(奥行き)
	/// </summary>
	/// <param name=""></param>
	/// <returns>描画範囲</returns>
	inline float GetZRange(void) { return m_ZRange; }
	/// <summary>
	/// シャドウマップの描画範囲(縦横)
	/// </summary>
	/// <param name=""></param>
	/// <returns>大きさ</returns>
	inline float GetMapRange(void) { return m_MapRange; }
	/// <summary>
	/// 光源がどれくらい離れているか
	/// </summary>
	/// <param name=""></param>
	/// <returns>距離</returns>
	inline float GetLightDistance(void) { return m_LightDistance; }
	/// <summary>
	/// 各種パラメーターの設定
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	void SetParameters(D3DXMATRIX World);
	/// <summary>
	/// ライトの向きを設定
	/// </summary>
	/// <param name="LightVec">向き</param>
	inline void SetLightVec(const D3DXVECTOR3 LightVec) { m_LightVec = LightVec; }
	/// <summary>
	/// 奥行きを設定
	/// </summary>
	/// <param name="ZRange">奥行き</param>
	inline void SetZRange(const float ZRange) { m_ZRange = ZRange; }
	/// <summary>
	/// 大きさを設定
	/// </summary>
	/// <param name="MapRange">大きさ</param>
	inline void SetMapRange(const float MapRange) { m_MapRange = MapRange; }
	/// <summary>
	/// 距離を設定
	/// </summary>
	/// <param name="Distance">距離</param>
	inline void SetLightDistance(const float Distance) { m_LightDistance = Distance; }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CShadowMap& Instance(void) {
		static CShadowMap Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CShadowMap() {};

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_ShadowMap;					// Z値テクスチャ
	LPDIRECT3DSURFACE9 m_ShadowMapSurface;			// Z値サーフェイス
	LPDIRECT3DSURFACE9 m_ShadowDepthSurface;		// 震度バッファサーフェイス
	LPDIRECT3DSURFACE9 m_OldRT;						// 既存のレンダータゲット
	LPDIRECT3DSURFACE9 m_OldDepthSurface;			// 既存の震度バッファ
	D3DXMATRIX m_LightView;							// ライトのビュー行列
	D3DXMATRIX m_LightProj;							// ライトのプロジェクション行列
	D3DXVECTOR3 m_LightVec;							// ライトの方向ベクトル
	D3DXHANDLE m_mtxWorldHandle;					// ワールドマトリックスのハンドル
	D3DXHANDLE m_LightViewHandle;					// ライトビューマトリックスのハンドル
	D3DXHANDLE m_LightProjHandle;					// ライトプロジェクションマトリックスのハンドル
	float m_ZRange;									// シャドウマップの描画範囲
	float m_MapRange;								// シャドウマップの大きさ
	float m_LightDistance;							// ライトの距離
	UINT m_MapResolveW;								// シャドウマップの解像度W
	UINT m_MapResolveH;								// シャドウマップノ解像度H
};