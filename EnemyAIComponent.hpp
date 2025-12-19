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
namespace EnemyState {
	struct PatrolMap {
		// そのポイントへのIdx
		int Idx;
		// そのポイントに滞在する時間
		int CoolDown;
	};
	// ステートの列挙型
	enum class ENEMYSTATE
	{
		NONE = 0,
		CHASE,
		PREDICT,
		SEARCH,
		BACK,
		MAX
	};
	// プレイヤーの位置のエラー値を設定
	static const D3DXVECTOR3 InvalidPlayerPosition = { 0.0f,-100000.0f,0.0f };
}

// 敵のステートコンポーネント
struct EnemyAIComp {
	// コンストラクタ
	EnemyAIComp(EnemyState::ENEMYSTATE Default, std::vector<EnemyState::PatrolMap>& _PointList) :
		State(Default), LastLookPlayerPosition(VEC3_NULL), HalfPatrolRoute(_PointList), NowIdx(-1), NextIdx(NowIdx + 1), IsFinish(false), CoolDownCnt(0), BackIdx(0) {};
	~EnemyAIComp() {
		if (Emitter != nullptr) Emitter->Uninit();
	}
	// 管理者
	EnemyState::ENEMYSTATE State;
	// 最後に見たプレイヤーの位置
	D3DXVECTOR3 LastLookPlayerPosition;
	// 半周分の巡回ポイント
	std::vector<EnemyState::PatrolMap> HalfPatrolRoute;
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
};