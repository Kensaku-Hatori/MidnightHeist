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
	void WriteStutsInfo(void);

	// 静的メンバ関数
	// セッター
	static void ResetPlayer(void);
	static void AddEnCount(void) { m_EnCount++; }
	static void AddSteal(void) { m_Steal++; }
	// ゲッター
	static bool IsFinishedFirstNoise(void) { return m_IsFinishedFirstNoise; }
	static bool IsOlfFinishedFirstNoise(void) { return m_IsOlfFinishedFirstNoise; }
private:
	// 定数を設定
	struct Config {
		// セットアップ
		struct SetUp {
			static constexpr const char* Path = "data/TEXT/SetUp/GameSetUp.json";
		};
		// 手前の壁
		struct NearWall {
			static const btVector3 Size;
			static const btVector3 Origin;
		};
	};
	// 静的メンバ変数宣言
	// ノイズ用のフラグ
	static bool m_IsFinishedFirstNoise;
	static bool m_IsOlfFinishedFirstNoise;
	static int	m_Time;
	static int	m_EnCount;
	static int	m_Steal;
	// メンバ変数
	int m_Frame;
	// 手前の壁の当たり判定用変数
	std::unique_ptr<btBoxShape>		m_NearWallShape;
	std::unique_ptr<btRigidBody>	m_NearWallRB;
};
#endif // !_GAME_H_