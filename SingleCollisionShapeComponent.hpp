//****************************************************************
//
// 単純な当たり判定のコンポーネント[SingleCollisionShapeComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 単純な当たり判定
struct SingleCollisionShapeComp {
	// デストラクタ
	~SingleCollisionShapeComp() {
		CollisionShape.reset();
	};
	// 当たり判定
	D3DXVECTOR3 Offset;
	std::unique_ptr<btCollisionShape> CollisionShape;
};