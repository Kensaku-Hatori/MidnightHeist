//****************************************************************
//
// 敵AIの更新処理[EnemyAIComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace EnemyState {
	// ステートの列挙型
	enum class ENEMYSTATE
	{
		NONE = 0,
		CHASE,
		PREDICTFRONT,
		PREDICTFOLLOW,
		PREDICTTURN,
		SEARCH,
		MAX
	};
	// プレイヤーの位置のエラー値を設定
	static const D3DXVECTOR3 InvalidPlayerPosition = { 0.0f,-100000.0f,0.0f };
}

// 敵のステートコンポーネント
struct EnemtAIComp {
	// コンストラクタ
	EnemtAIComp(EnemyState::ENEMYSTATE Default) : State(Default), LastLookPlayerPosition(VEC3_NULL) { NowPatrolPoint = 10; DestPatrolPoint = 10; IsFinish = false; DontRepeatStrength = 100; };
	// 管理者
	EnemyState::ENEMYSTATE State;
	D3DXVECTOR3 LastLookPlayerPosition;
	// 今いる巡回ポイントへのインデックス
	int NowPatrolPoint;
	// 目標
	int DestPatrolPoint;
	// 前回の巡回ポイントと被った場合何回までやり直すか
	int DontRepeatStrength;
	// 着いたかどうか
	bool IsFinish;
};