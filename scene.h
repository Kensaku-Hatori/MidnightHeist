//****************************************************************
//
// シーンの処理[scene.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _SCENE_H_
#define _SCENE_H_

// インクルード
#include "object.h"

// シーンクラスを定義
class CScene
{
public:
	// シーンの列挙型
	typedef enum
	{
		MODE_TITLE = 0,		// タイトルシーン
		MODE_TUTORIAL,		// チュートリアルシーン
		MODE_GAME,			// ゲームシーン
		MODE_RESULT,		// リザルトシーン
		MODE_MAX,			// シーンの最大数
	}MODE;

	// コンストラクタ・デストラクタ
	CScene(MODE Mode);
	virtual ~CScene();

	// メンバ関数(純粋仮想)
	virtual HRESULT Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Draw(void) = 0;

	// ゲッター
	MODE GetMode(void) { return m_Mode; };
private:
	// メンバ変数
	MODE m_Mode;	// 今のモード
};
#endif // !_SCENE_H_