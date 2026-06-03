//================================================================
//
// ライトの処理[light.h]
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
#include "main.h"

//****************************************************************
// ライトクラスを定義
//****************************************************************
class CLight
{
public:
	// 定数を宣言
	struct Config {
		static constexpr int NumLIght = 1;		// ライトの数
		static const D3DXVECTOR3 DefoultDir;	// ライトのデフォルト時の向き
		static const D3DXCOLOR Evening;			// 補完先のライトの色
	};
	// コンストラクタ・デストラクタ
	CLight();
	~CLight();

	/// <summary>
	/// 初期処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// ライトをセット
	/// </summary>
	/// <param name="Setter">セットしたいライトの情報</param>
	/// <returns></returns>
	int SetLight(D3DLIGHT9 Setter);
	/// <summary>
	/// ライトを削除
	/// </summary>
	/// <param name="Indx">インデックス</param>
	void DeleteLight(const int Indx);
	/// <summary>
	/// ライトの色を初期化
	/// </summary>
	/// <param name=""></param>
	void ResetCol(void);
private:
	// メンバ変数
	D3DLIGHT9 m_Light[Config::NumLIght];	// ライトを設定する用
	int m_nCntLight;						// ライトのカウンタ
};