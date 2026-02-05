//****************************************************************
//
// 敵AIの情報[AIComponentComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "Sound3D.h"

// 念のため
namespace BasicAI {
	struct PatrolMap {
		// そのポイントへのIdx
		int Idx;
		// そのポイントに滞在する時間
		int CoolDown;
	};
	// プレイヤーの位置のエラー値を設定
	static const D3DXVECTOR3 InvalidPlayerPosition = { 0.0f,-100000.0f,0.0f };
}

// 敵のステートコンポーネント
struct AIComponent {
	// コンストラクタ
	AIComponent(std::vector<BasicAI::PatrolMap>& _PointList) :
		LastLookPlayerPosition(VEC3_NULL), HalfPatrolRoute(_PointList), NowIdx(-1), NextIdx(NowIdx + 1), IsFinish(false), CoolDownCnt(0), BackIdx(0) {};
	~AIComponent() {
		if (Emitter != nullptr) Emitter->Uninit();
	}
	// 最後に見たプレイヤーの位置
	D3DXVECTOR3 LastLookPlayerPosition;
	// 半周分の巡回ポイント
	std::vector<BasicAI::PatrolMap> HalfPatrolRoute;
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