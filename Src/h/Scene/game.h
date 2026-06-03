//================================================================
//
// ゲームの処理[game.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二十インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "scene.h"

//****************************************************************
// 前方宣言
//****************************************************************
class CPlayer;
class CScore;
class CTimer;
class CMapManager;
class CPauseManager;
class CPlayerManager;

//****************************************************************
// ゲームシーンクラスを定義
//****************************************************************
class CGame : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CGame();
	~CGame();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void)	override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)	override;
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void)	override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void)		override;
	/// <summary>
	/// スタッツ情報を書き出し
	/// </summary>
	/// <param name=""></param>
	void WriteStutsInfo(void);

	/// <summary>
	/// 見つかった回数を増やす
	/// </summary>
	/// <param name=""></param>
	inline static void AddEnCount(void) { m_EnCount++; }
	/// <summary>
	/// 盗んだ展示品の数を増やす
	/// </summary>
	/// <param name=""></param>
	inline static void AddSteal(void) { m_Steal++; }
	/// <summary>
	/// 最初のアニメーションのノイズが終了したかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>終了したかどうか</returns>
	inline static bool IsFinishedFirstNoise(void) { return m_IsFinishedFirstNoise; }
	/// <summary>
	/// 最初のアニメーションのノイズが終了したかどうか(一フレーム前)
	/// </summary>
	/// <param name=""></param>
	/// <returns>終了したかどうか</returns>
	inline static bool IsOlfFinishedFirstNoise(void) { return m_IsOlfFinishedFirstNoise; }
private:
	// 定数を設定
	struct Config {
		// セットアップ
		struct SetUp {
			static constexpr const char* Path = "data/TEXT/SetUp/GameSetUp.json";
		};
		// 手前の壁
		struct NearWall {
			static const btVector3 Size;	// 大きさ
			static const btVector3 Origin;	// 位置
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