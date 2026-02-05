//****************************************************************
//
// ロックオンアニメーションのコンポーネント[LockOnUIAnimationComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace LockOnAnimState {
	// ロックオンの状態
	enum class State {
		TOLOCK = 0,		// 焦点を合わせる
		LOCKEDIN,		// 焦点を合わせホールド
		RELEASELOCK,	// ロックオン解除
		MAX				// 最大数
	};
}

// ロックオンアニメーションの情報
struct LockOnAnimComponent {
	// コンストラクタ
	LockOnAnimComponent(const D3DXVECTOR2 Reference = VEC2_NULL, const int _ToLockFrame = NULL, const int _LockedFrame = NULL, const int _ReleaseFrame = NULL) :
		ToLockonCounter(0), ToLockonFrame(_ToLockFrame), LockedInCounter(0), LockedInFrame(_LockedFrame), ReleaseLockCounter(0), ReleaseLockFrame(_ReleaseFrame),
		NowState(LockOnAnimState::State::TOLOCK), Reference(Reference), ApperSize({ 1280.0f,720.0f }), DestSize({ 50.0f,50.0f }), ApperColor(WHITE),
		DestColor({ 0.0f,0.0f,0.0f,0.0f }), IsBoot(false) {};

	// ロックオンアニメーション情報
	int ToLockonCounter;
	int ToLockonFrame;
	int LockedInCounter;
	int LockedInFrame;
	int ReleaseLockCounter;
	int ReleaseLockFrame;
	bool IsBoot;
	LockOnAnimState::State NowState;
	D3DXVECTOR2 Reference;
	// イージング系
	D3DXVECTOR2 ApperSize;
	D3DXVECTOR2 DestSize;
	D3DXCOLOR ApperColor;
	D3DXCOLOR DestColor;
};