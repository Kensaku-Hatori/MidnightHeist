//****************************************************************
//
// プレイヤーが鳴らしている音の大きさのコンポーネント[PlayerSoundVolumeComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "PlayerStateComp.hpp"

namespace PlayerSoundVolumeConfig {
	static constexpr float Bace = 50.0f;
	static constexpr float Scale[static_cast<int>(PlayerState::State::MAX)] = {
		0.5f,1.0f,1.5f,1.5f
	};
}
// 情報
struct PlayerSoundVolumeComp {
	// コンスタント
	PlayerSoundVolumeComp(const float _Default = NULL) : SoundVolume(_Default) {}
	// 光線情報
	float SoundVolume;
};