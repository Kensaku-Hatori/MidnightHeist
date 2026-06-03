//================================================================
//
// デフォルトキューブマップの処理[defaultcubemap.h]
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
class CDefaultCubemap : public CShader
{
public:
	// デストラクタ
	~CDefaultCubemap() = default;

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
	/// パラメーターを設定
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	/// <param name="Tex">テクスチャ</param>
	void SetParameters(const D3DXMATRIX& World, const LPDIRECT3DCUBETEXTURE9& Tex);
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
	inline static CDefaultCubemap& Instance(void) {
		static CDefaultCubemap Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CDefaultCubemap() = default;
	// メンバ変数
	D3DXHANDLE m_mtxWorldHandle;	// ワールドマトリックスのハンドル
	D3DXHANDLE m_ViewHandle;		// ビューマトリックスのハンドル
	D3DXHANDLE m_ProjHandle;		// プロジェクションマトリックスのハンドル
	D3DXHANDLE m_CubeMapHandle;		// キューブマップのハンドル
};