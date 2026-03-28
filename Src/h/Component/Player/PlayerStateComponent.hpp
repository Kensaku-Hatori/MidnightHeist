//****************************************************************
//
// プレイヤーの状態のコンポーネント[PlayerStateComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace PlayerState {
	// プレイヤーの状態
	enum class State {
		STEALTH = 0,	// 停止
		SILENT,			// スニーク
		NORMAL,			// 通常
		DUSH,			// ダッシュ
		RANGE_PICKING,	// 解錠可能位置にいるかどうか
		PICKING,		// 解錠
		MAX
	};
}
// アイテムの情報
struct PlayerStateComponent {
	// コンストラクタ
	PlayerStateComponent() :
		NowState(PlayerState::State::NORMAL), OldState(PlayerState::State::NORMAL) {};
	// プレイヤーの状態
	PlayerState::State OldState;
	PlayerState::State NowState;
};