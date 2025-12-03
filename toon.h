//****************************************************************
//
// トゥーンシェーダーの処理[toon.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _TOON_H_
#define _TOON_H_

// インクルード
#include "shader.h"

// トゥーンシェーダークラスを定義
class CToon : public CShader
{
public:
	// デストラクタ
	~CToon();

	// メンバ関数
	HRESULT Init(void);
	void ReSet(void);
	void ReStart(void);

	// セッター
	void SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Light, D3DXVECTOR4 Col = { 1.0f,1.0f,1.0f,1.0f },
		LPDIRECT3DTEXTURE9 ShadowMap = NULL, LPDIRECT3DTEXTURE9 ModelTex = NULL, D3DXMATRIX LightView = {}, D3DXMATRIX LightProj = {}); // ライトやマテリアルなどの設定

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CToon* Instance(void);
private:
	// コンストラクタ
	CToon() {};
	// 静的メンバ変数
	static std::unique_ptr<CToon> m_Instance;
};
#endif // !_TOON_H_