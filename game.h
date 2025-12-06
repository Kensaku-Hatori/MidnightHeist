//****************************************************************
//
// ゲームの処理[game.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二十インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

// インクルード
#include "scene.h"

// 前方宣言
class CPlayer;
class CScore;
class CTimer;
class CMapManager;
class CPauseManager;
class CPlayerManager;

// ゲームシーンクラスを定義
class CGame : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CGame();
	~CGame();

	// メンバ関数
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;

	// 静的メンバ関数
	// セッター
	static void ResetPlayer(void);
private:
};
#endif // !_GAME_H_