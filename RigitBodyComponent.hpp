//****************************************************************
//
// 剛体のコンポーネント[RigitBodyComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

#include "manager.h"

// 当たり判定のグループ
namespace CollisionGroupAndMasks {
	constexpr char GROUP_ENEMY =	1 << 0;	// 敵グループ
	constexpr char GROUP_PLAYER =	1 << 1;	// プレイヤーグループ
	constexpr char GROUP_MAPOBJECT =		1 << 2;	// マップグループ
	constexpr char MASK_ENEMY = GROUP_PLAYER | GROUP_MAPOBJECT;					// 敵のマスク
	constexpr char MASK_PLAYER = GROUP_ENEMY | GROUP_PLAYER | GROUP_MAPOBJECT;	// プレイヤーのマスク
	constexpr char MASK_MAPOBJECT = GROUP_ENEMY | GROUP_PLAYER | GROUP_MAPOBJECT;	// プレイヤーのマスク
}

// 剛体コンポーネント
struct RigitBodyComp {
	// デストラクタ
	~RigitBodyComp() {	
		// 剛体の削除
		if (RigitBody)
		{
			CManager::GetDynamicsWorld()->removeRigidBody(RigitBody.get());
			if (RigitBody->getMotionState())
			{
				delete RigitBody->getMotionState();
			}
			RigitBody.reset();
		}
	};
	// 着地したかどうか
	bool IsGround(btCapsuleShape& CastShape) {
		// プレイヤーの現在の中心座標
		btVector3 RayOrigin = RigitBody->getWorldTransform().getOrigin();

		// スイープの開始位置（下の半球の中心）
		btVector3 sweepStart = RayOrigin + btVector3(0, -CastShape.getHalfHeight(), 0);

		// スイープの終了位置（少し下へ伸ばす）
		btVector3 sweepEnd = sweepStart + btVector3(0, btScalar(-0.1), 0);

		// 判定用の形状
		btSphereShape sphereShape(CastShape.getRadius());

		// スイープ実行
		btCollisionWorld::ClosestConvexResultCallback callback(sweepStart, sweepEnd);
		CManager::GetDynamicsWorld()->convexSweepTest(&sphereShape, btTransform(btQuaternion(0, 0, 0, 1), sweepStart), btTransform(btQuaternion(0, 0, 0, 1), sweepEnd), callback);
		return callback.hasHit();
	}
	// 地面のオブジェクト取得
	const btCollisionObject* GetGroundCollisionObject(btCapsuleShape& CastShape) {
		// プレイヤーの現在の中心座標
		btVector3 RayOrigin = RigitBody->getWorldTransform().getOrigin();

		// スイープの開始位置（下の半球の中心）
		btVector3 sweepStart = RayOrigin + btVector3(0, -CastShape.getHalfHeight(), 0);

		// 誤差
		const float Epsilon = 1e-6f;

		// スイープの終了位置（少し下へ伸ばす）
		btVector3 sweepEnd = sweepStart + btVector3(0, -Epsilon, 0);

		// 判定用の形状
		btSphereShape sphereShape(CastShape.getRadius());

		// スイープ実行
		btCollisionWorld::ClosestConvexResultCallback callback(sweepStart, sweepEnd);
		CManager::GetDynamicsWorld()->convexSweepTest(&sphereShape, btTransform(btQuaternion(0, 0, 0, 1), sweepStart), btTransform(btQuaternion(0, 0, 0, 1), sweepEnd), callback);
		if (callback.hasHit() == false) return nullptr;
		return callback.m_hitCollisionObject;
	}
	// リジットボディー
	std::unique_ptr<btRigidBody> RigitBody;
	// ジャンプフラグ
	bool IsJump;
};