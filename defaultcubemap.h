//****************************************************************
//
// デフォルトキューブマップの処理[defaultcubemap.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "shader.h"

// クラスを定義
class CDefaultCubemap : public CShader
{
public:
	// デストラクタ
	~CDefaultCubemap() = default;

	// メンバ関数
	HRESULT Init(void);
	void ReSet(void);
	void ReStart(void);

	// セッター
	void SetParameters(const D3DXMATRIX& World, const LPDIRECT3DCUBETEXTURE9& Tex); // ライトやマテリアルなどの設定

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CDefaultCubemap* Instance(void) {
		static std::unique_ptr<CDefaultCubemap> Instance(new CDefaultCubemap);
		return Instance.get();
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