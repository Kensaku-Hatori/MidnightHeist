//================================================================
//
// フェードの処理[fade.h]
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
#include "Scene/scene.h"

//****************************************************************
// フェードクラスを定義
//****************************************************************
class CFade
{
public:
	// フェードの定数を宣言
	struct Config {
		static constexpr float Speed = 0.04f;	// 速さ
	};

	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,	// 通常
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX,		// 最大値
	}FADE;

	// デストラクタ
	~CFade() = default;

	//****************************************************************
	// メンバ関数
	//****************************************************************
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);
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
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	/// <summary>
	/// 次のシーンを設定
	/// </summary>
	/// <param name="pNextScene">次のシーンのポインタ</param>
	void SetFade(CScene* pNextScene);
	/// <summary>
	/// 遷移状態を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>遷移状態</returns>
	inline FADE GetFade(void) { return m_Fade; }
	/// <summary>
	/// シングルトンを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>シングルトン</returns>
	inline static CFade& Instance(void) {
		static CFade Instance;
		return Instance;
	}
private:
	// コンストラクタ
	CFade() = default;

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	FADE m_Fade;						// フェードの状態
	CScene* m_pNextScene;				// 次のシーンのインスタンス
	D3DXCOLOR m_ColorFade;				// フェードの色
};