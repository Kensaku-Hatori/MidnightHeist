//****************************************************************
//
// フェードの処理[fade.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _FADE_H_
#define _FADE_H_

// インクルード
#include "Scene/scene.h"

// フェードクラスを定義
class CFade
{
public:
	// フェードの規定値を設定
	struct Config {
		static const D3DXCOLOR DefoultCol;		// 通常色
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
	~CFade();

	//プロトタイプ宣言
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetFade(CScene* pNextScene);

	// ゲッター
	FADE GetFade(void) { return m_Fade; };

	// 静的メンバ関数
	// ゲッター
	static CFade& Instance(void) {
		static CFade Instance;
		return Instance;
	}
private:
	// コンストラクタ
	CFade() {};

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	FADE m_Fade;						// フェードの状態
	CScene* m_pNexScene;				// 次のシーンのインスタンス
	D3DXCOLOR m_colorFade;				// フェードの色
};
#endif // !_FADE_H_