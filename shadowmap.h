//****************************************************************
//
// シャドウマップの処理[shadowmap.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _SHAODWMAP_H_
#define _SHAODWMAP_H_

// インクルード
#include "shader.h"

// クラスを定義
class CShadowMap : public CShader
{
public:
	// デストラクタ
	~CShadowMap();

	// メンバ関数
	HRESULT Init(void);
	void Clear(void);
	void WriteMaps(void);
	void EndMaps(void);
	void ReSet(void);
	void ReStart(void);

	// ゲッター
	D3DXMATRIX& GetLightView() { return m_LightView; }
	D3DXMATRIX& GetLightProj() { return m_LightProj; }
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }
	LPDIRECT3DTEXTURE9& GetTex(void) { return m_ShadowMap; }
	LPDIRECT3DSURFACE9 GetSurface(void) { return m_ShadowMapSurface; }
	D3DXVECTOR3 GetLightVec(void) { return m_LightVec; }
	float GetZRange(void) { return m_ZRange; }
	float GetMapRange(void) { return m_MapRange; }
	float GetLightDistance(void) { return m_LightDistance; }

	// セッター
	void SetParameters(D3DXMATRIX World); // ライトやマテリアルなどの設定
	void SetLightVec(const D3DXVECTOR3 LightVec) { m_LightVec = LightVec; }
	void SetZRange(const float ZRange) { m_ZRange = ZRange; }
	void SetMapRange(const float MapRange) { m_MapRange = MapRange; }
	void SetLightDistance(const float Distance) { m_LightDistance = Distance; }

	// 静的メンバ関数
	static CShadowMap* Instance(void) {
		static std::unique_ptr<CShadowMap> Instance(new CShadowMap);
		return Instance.get();
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
	float m_ZRange;									// シャドウマップの描画範囲
	float m_MapRange;								// シャドウマップの大きさ
	float m_LightDistance;							// ライトの距離
	UINT m_MapResolveW;								// シャドウマップの解像度W
	UINT m_MapResolveH;								// シャドウマップノ解像度H
};
#endif // !_SHAODWMAP_H_