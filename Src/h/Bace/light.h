//****************************************************************
//
// ライトの処理[light.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _LIGHT_H_
#define _LIGHT_H_

// インクルード
#include "main.h"

// ライトクラスを定義
class CLight
{
public:
	struct Config {
		static constexpr int NumLIght = 1;
		static const D3DXVECTOR3 DefoultDir;
		static const D3DXCOLOR Evening;
	};
	// コンストラクタ・デストラクタ
	CLight();
	~CLight();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// セッター
	int SetLight(D3DLIGHT9 Setter);
	void DeleteLight(const int Indx);
	void ResetCol(void);
private:
	// メンバ変数
	D3DLIGHT9 m_Light[Config::NumLIght];	// ライトを設定する用
	int m_nCntLight;						// ライトのカウンタ
};
#endif // !_LIGHT_H_