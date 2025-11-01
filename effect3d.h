//****************************************************************
//
// エフェクトのの処理[effect3D.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

// インクルード
#include "objectbillboard.h"

// 3Dエフェクトのクラスを定義
class CEffect3D : public CObjectBillBoard
{
public:
	// エフェクトの挙動
	struct EffectModule {
		// メンバ変数
		D3DXVECTOR3 Move;		// 移動量
		float fGravity;			// 重力
		float fAlphaDef;		// 1フレームで減るアルファ値
		float fSizeDef;			// 1フレームで減る大きさ
		int nLife;				// 寿命
		const char* FilePath;	// テクスチャへのパス
	};

	// テクスチャのパスを設定
	struct Config {
		static constexpr const char* Sphere = "data\\TEXTURE\\Effect\\effect000.jpg";
		static constexpr const char* Smoke = "data\\TEXTURE\\Effect\\smoke.jpg";
	};
	// コンストラクタ・デストラクタ
	CEffect3D(const int Priority = 3);
	~CEffect3D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// 生成
	static CEffect3D* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXCOLOR Col, D3DXVECTOR2 Size, float Gravity, int Life, const char* FilePath,const int Priority = 3);
private:
	// メンバ変数
	EffectModule m_Module;	// モジュール
};
#endif // !_EFFECT3D_H_