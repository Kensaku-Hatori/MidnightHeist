//****************************************************************
//
// プレイヤーの最初のアニメーションのコンポーネント[PlayerAnimationComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// プレイヤーの最初のアニメーションの情報
struct PlayerAnimComp {
	// コンストラクタ
	PlayerAnimComp() : IsFinishedBelt(false), IsFinishedBeltOld(false), IsScreen(false), IsFinishedAnim(false), FirstDelayCounter(0), FirstDelayFrame(120) {};
	// プレイヤーのアニメーション情報
	int FirstDelayCounter;
	int FirstDelayFrame;
	bool IsFinishedBelt;
	bool IsFinishedBeltOld;
	// スクリーン上にプレイヤーがいるかどうか
	bool IsScreen;
	// アニメーションが終了したかどうか
	bool IsFinishedAnim;
};