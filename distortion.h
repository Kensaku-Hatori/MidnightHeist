//****************************************************************
//
// 画面の歪みの処理[distortion.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "shader.h"

// 歪みシェーダのクラスを定義
class CDistortion : public CShader
{
public:
	// デストラクタ
	~CDistortion();

	// メンバ関数
	HRESULT Init(void);
	void ReSet(void);
	void ReStart(void);

	void SetParameters(LPDIRECT3DTEXTURE9 Scene); // ライトやマテリアルなどの設定

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CDistortion* Instance(void) {
		static std::unique_ptr<CDistortion> Instance(new CDistortion);
		return Instance.get();
	};
private:
	// コンストラクタ
	CDistortion() {};
};