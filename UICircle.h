//****************************************************************
//
// 円形UIの処理[UICircle.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "shader.h"

// クラスを定義
class CUICircle : public CShader
{
public:
	// デストラクタ
	~CUICircle() = default;

	// メンバ関数
	HRESULT Init(void);

	// セッター
	void SetParameters(const D3DXMATRIX& World, const D3DXMATRIX& OriginWorld, const float FillAmount,const float Radius,const float MaxFillAngle); // ライトやマテリアルなどの設定

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CUICircle* Instance(void) {
		static std::unique_ptr<CUICircle> Instance(new CUICircle);
		return Instance.get();
	};
private:
	// コンストラクタ
	CUICircle() = default;
};