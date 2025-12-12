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
	void SetUseShadowMapParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Col = { 1.0f,1.0f,1.0f,1.0f },
		LPDIRECT3DTEXTURE9 ShadowMap = NULL, LPDIRECT3DTEXTURE9 ModelTex = NULL, D3DXMATRIX LightView = {}, D3DXMATRIX LightProj = {}); // ライトやマテリアルなどの設定

	void SetUseOutLineParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, float Thickness = 6.0f, D3DXVECTOR4 Color = { 1.0f, 0.3f, 0.5f, 1.0f }, float Height = 0.0f);

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CToon* Instance(void);
private:
	// コンストラクタ
	CToon() {};
	// トゥーンマップ
	LPDIRECT3DTEXTURE9 m_ToonMap;
	// 静的メンバ変数
	static std::unique_ptr<CToon> m_Instance;
};
#endif // !_TOON_H_