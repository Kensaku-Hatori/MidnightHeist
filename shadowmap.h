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
	HRESULT Init(std::string Path);
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

	// セッター
	void SetParameters(D3DXMATRIX World); // ライトやマテリアルなどの設定

	// 静的メンバ関数
	static CShadowMap* Instance(void);
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
	// 静的メンバ変数
	static std::unique_ptr<CShadowMap> m_Instance;	// インスタンス
};
#endif // !_SHAODWMAP_H_