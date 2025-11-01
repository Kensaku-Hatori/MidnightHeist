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
#include "scene.h"

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

	// 静的メンバ関数
	// セッター
	static void SetFade(CScene* pNextScene);

	// ゲッター
	static FADE GetFade(void) { return m_pSingleto->m_Fade; };
	static CFade* GetFadeSingle(void) { return m_pSingleto; };

	// 生成
	static CFade* CreateSingle(void);
private:
	// コンストラクタ
	CFade() {};

	// 静的メンバ変数
	static CFade* m_pSingleto;			// シングルトン
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// 頂点バッファ
	FADE m_Fade;						// フェードの状態
	CScene* m_pNexScene;				// 次のシーンのインスタンス
	D3DXCOLOR m_colorFade;				// フェードの色
};
#endif // !_FADE_H_