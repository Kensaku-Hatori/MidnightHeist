//****************************************************************
//
// 単純な当たり判定のコンポーネント[SingleCollisionShapeComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "RigitBodyComponent.hpp"

// 単純な当たり判定
struct SingleCollisionShapeComp {
	// コンストラクタ
	SingleCollisionShapeComp(const int _Group = CollisionGroupAndMasks::GROUP_MAPOBJECT, const int _Mask = CollisionGroupAndMasks::MASK_MAPOBJECT) :
		Group(_Group), Mask(_Mask), Offset(VEC3_NULL) {};
	// デストラクタ
	~SingleCollisionShapeComp() {
		CollisionShape.reset();
	};
	// 当たり判定用のフラグ
	int Group;
	int Mask;
	// 当たり判定
	D3DXVECTOR3 Offset;
	std::unique_ptr<btCollisionShape> CollisionShape;
};