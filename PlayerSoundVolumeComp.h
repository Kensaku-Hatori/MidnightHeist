//****************************************************************
//
// プレイヤーが鳴らしている音の大きさのコンポーネント[PlayerSoundVolumeComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 情報
struct PlayerSoundVolumeComp {
	// コンスタント
	PlayerSoundVolumeComp(const D3DXVECTOR3 _Origin = VEC3_NULL, const D3DXVECTOR3 _Dir = VEC_UP) : Origin(_Origin), Dir(_Dir) {}
	// 光線情報
	D3DXVECTOR3 Origin, Dir;
};