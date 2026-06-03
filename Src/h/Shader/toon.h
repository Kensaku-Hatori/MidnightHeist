//================================================================
//
// トゥーンシェーダーの処理[toon.h]
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
// トゥーンシェーダークラスを定義
//****************************************************************
class CToon : public CShader
{
public:
	// デストラクタ
	~CToon();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// リスタート処理(デバイスロスト復帰時)
	/// </summary>
	/// <param name=""></param>
	void ReSet(void);
	/// <summary>
	/// 全テクスチャのクリア
	/// </summary>
	/// <param name=""></param>
	void ReStart(void);
	/// <summary>
	/// シャドウマップを使って描画
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	/// <param name="View">ビューマトリックス</param>
	/// <param name="Proj">プロジェクションマトリックス</param>
	/// <param name="Col">マテリアルの色</param>
	/// <param name="ShadowMap">シャドウマップ</param>
	/// <param name="ModelTex">モデルのテクスチャ</param>
	/// <param name="LightView">ライトのビューマトリックス</param>
	/// <param name="LightProj">ライトのプロジェクションマトリックス</param>
	void SetUseShadowMapParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Col = { 1.0f,1.0f,1.0f,1.0f },
		LPDIRECT3DTEXTURE9 ShadowMap = NULL, LPDIRECT3DTEXTURE9 ModelTex = NULL, D3DXMATRIX LightView = {}, D3DXMATRIX LightProj = {});
	/// <summary>
	/// アウトラインを使って描画
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	/// <param name="View">ビューマトリックス</param>
	/// <param name="Proj">プロジェクションマトリックス</param>
	/// <param name="Thickness">アウトラインの太さ</param>
	/// <param name="Color">アウトラインの色</param>
	/// <param name="Height">アウトラインを描画する高さの閾値</param>
	void SetUseOutLineParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, float Thickness = 6.0f, D3DXVECTOR4 Color = { 1.0f, 0.3f, 0.5f, 1.0f }, float Height = 0.0f);
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static CToon& Instance(void) {
		static CToon Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CToon() {};
	// トゥーンマップ
	LPDIRECT3DTEXTURE9 m_ToonMap;		// トゥーン調のテクスチャ
	D3DXHANDLE m_mtxWorldHandle;		// ワールドマトリックスハンドル
	D3DXHANDLE m_ViewHandle;			// ビューマトリックスハンドル
	D3DXHANDLE m_ProjHandle;			// プロジェクションマトリックスハンドル
	D3DXHANDLE m_DeffuseHandle;			// ディフューズハンドル
	D3DXHANDLE m_LightViewHandle;		// ライトビューマトリックスハンドル
	D3DXHANDLE m_LightProjHandle;		// ライトプロジェクションマトリックスハンドル
	D3DXHANDLE m_ShadowMapHandle;		// シャドウマップハンドル
	D3DXHANDLE m_VecLightHandle;		// ライトの方向ベクトルハンドル
	D3DXHANDLE m_ModelTexHandle;		// モデルのテクスチャハンドル
	D3DXHANDLE m_ToonMapHandle;			// トゥーン調テクスチャハンドル
	D3DXHANDLE m_OutLineHeightHandle;	// アウトライン描画する高さの閾値ハンドル
	D3DXHANDLE m_OutLineThicknessHandle;// アウトラインの太さハンドル
	D3DXHANDLE m_OutLineColorHandle;	// アウトラインの色ハンドル
	// 静的メンバ変数
	static std::unique_ptr<CToon> m_Instance;
};