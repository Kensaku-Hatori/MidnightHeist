//****************************************************************
//
// 敵AIの更新処理[EnemyAIComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "Sound3D.h"

// 念のため
namespace PatrolPoint {
	struct PatrolMap {
		// そのポイントへのIdx
		int Idx;
		// そのポイントに滞在する時間
		int CoolDown;
	};
	// プレイヤーの位置のエラー値を設定
	static const D3DXVECTOR3 InvalidPlayerPosition = { 0.0f,-100000.0f,0.0f };

	// 一つの巡回ポイントの構造体
	struct PatrolPointInfo
	{
		// 位置
		D3DXVECTOR3 Point;
		// 移動できる隣接ノードへのIdx
		std::vector<int> CanMove;
		// 自分自身のIdx
		int Idx;
	};
	// エースター用の構造体
	struct PatrolPointInfoForAStar
	{
		// 自分自身のIdx
		int Idx;
		// 自分自身のノードの親
		int Parent;
		// スタート地点からゴールまでのコスト
		// 今の位置からゴールまでのコスト
		// ヒューリスティック関数のコスト
		float StartGoalCost, MyGoalCost, Heuristic;
	};
}

// 敵のステートコンポーネント
struct PatrolAIComponent {
	// コンストラクタ
	PatrolAIComponent(std::vector<PatrolPoint::PatrolMap>& _PointList) :
		LastLookPlayerPosition(VEC3_NULL), HalfPatrolRoute(_PointList), NowIdx(-1), NextIdx(NowIdx + 1), IsFinish(false), CoolDownCnt(0), BackIdx(0) {};
	~PatrolAIComponent() {
		if (Emitter != nullptr) Emitter->Uninit();
	}
	// 最後に見たプレイヤーの位置
	D3DXVECTOR3 LastLookPlayerPosition;
	// 半周分の巡回ポイント
	std::vector<PatrolPoint::PatrolMap> HalfPatrolRoute;
	// 復帰用の巡回ポイントIdx記憶用
	std::vector<int> AStarRoute;
	// AStarが終わったかどうか
	bool IsFinishedAStar;
	// エミッター
	CEmitter* Emitter;
	// 目標の位置
	D3DXVECTOR3 DestPos;
	// 復帰用の巡回ポイント用Ｉｄｘカウンタ
	int BackIdx;
	// 現在のポイントへのIdx
	int NowIdx;
	// 次のポイントへのIdx
	int NextIdx;
	// クールダウンカウンタ
	int CoolDownCnt;
	// 半周下かどうか
	bool IsFinish;
	// プレイヤーまでの光線が遮られているかどうか
	bool IsOldBlockedToPlayer;
	bool IsBlockedToPlayer;
};