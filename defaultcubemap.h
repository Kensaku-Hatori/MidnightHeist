#pragma once

#include "shader.h"

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
	static CDefaultCubemap* Instance(void);
private:
	// コンストラクタ
	CDefaultCubemap() = default;

		// 静的メンバ変数
	static std::unique_ptr<CDefaultCubemap> m_Instance;
};