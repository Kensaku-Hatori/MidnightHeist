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
	// 規定値を設定
	struct Config {
		// プレイヤー
		struct Player {
			static const D3DXVECTOR3 Pos;				// 位置
			static const D3DXVECTOR3 Rot;				// 向き
		};
		// カメラ
		struct Camera {
			static const D3DXVECTOR3 Rot;				// 向き
			static const D3DXVECTOR3 PosR;				// 注視点
			static const D3DXVECTOR3 PosV;				// 始点
		};
		// 空
		struct Sky {
			static const D3DXVECTOR3 Pos;				// 位置
			static constexpr float Radius = 7000.0f;	// 半径
			static constexpr int Horizon = 8;			// 分割数
			static constexpr int Vertical = 5;			// 分割数
		};
	};
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

	// ゲッター
	static bool GetPause(void) { return m_isPause; };
	static CPlayer* GetPlayer(void) { return m_pPlayer; }

private:
	// 静的メンバ変数
	static CPauseManager* m_pPauseManager;				// ポーズマネージャーへのポインタ
	static CMapManager* m_pMapManager;					// プレイヤーマネージャー
	static CPlayer* m_pPlayer;
	static bool m_isPause;								// ポーズ中かどうか
	std::unique_ptr<btCollisionShape> m_GroundShape;	// 当たり判定
	std::unique_ptr<btRigidBody> m_RigitBody;			// リジットボディー
};
#endif // !_GAME_H_