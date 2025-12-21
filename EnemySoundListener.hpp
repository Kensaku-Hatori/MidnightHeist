//****************************************************************
//
// プレイヤーが鳴らしている音を聞ける大きさのコンポーネント[EnemySoundListenderComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "EnemyAIComponent.hpp"

namespace EnemyListenerConfig {
	static constexpr float Bace = 50.0f;
	static constexpr float Scale[static_cast<int>(EnemyState::ENEMYSTATE::MAX)] = {
		1.0f,1.0f,1.0f,1.0f,1.0f
	};
}
// 情報
struct EnemyListenerComp {
	// コンスタント
	EnemyListenerComp(const float _Default = NULL) : ListenerVolume(_Default) {}
	// 情報
	float ListenerVolume;
};