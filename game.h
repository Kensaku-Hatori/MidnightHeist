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
	static void AddStep(void) { m_Step++; }
	// ゲッター
	static bool IsFinishedFirstNoise(void) { return m_IsFinishedFirstNoise; }
	static bool IsOlfFinishedFirstNoise(void) { return m_IsOlfFinishedFirstNoise; }
private:
	// 定数を設定
	struct Config {
		// プレイヤー
		struct Player {
			// 位置
			static const D3DXVECTOR3 Pos;
		};
		// ２Dテクスチャ
		struct XDay {
			// 位置
			static const D3DXVECTOR2 Pos;
			// 大きさ
			static const D3DXVECTOR2 Size;
			// パス
			static constexpr const char* Path = "data/TEXTURE/XDay.png";
		};
		struct Rec {
			// 位置
			static const D3DXVECTOR2 Pos;
			// 大きさ
			static const D3DXVECTOR2 Size;
			// パス
			static constexpr const char* Path = "data/TEXTURE/Rec.png";
		};
		struct CameraWork {
			// 位置
			static const D3DXVECTOR2 Pos;
			// 大きさ
			static const D3DXVECTOR2 Size;
			// パス
			static constexpr const char* Path = "data/TEXTURE/CameraWork.png";
		};
		// マップ
		struct MapObjects {
			// パス
			static constexpr const char* Path = "data\\TEXT\\StageInfo.json";
		};
		// パトロールポイント一覧
		struct PatrolPoints {
			// パス
			static constexpr const char* Path = "data\\TEXT\\Patrol.json";
		};
	};
	// 静的メンバ変数宣言
	// ノイズ用のフラグ
	static bool m_IsFinishedFirstNoise;
	static bool m_IsOlfFinishedFirstNoise;
	static int m_Time;
	static int m_EnCount;
	static int m_Step;
	int m_Frame;
};
#endif // !_GAME_H_