//****************************************************************
//
// マップオブジェクト更新システムの処理[UpdateMapobjectSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateMapobjectSystem.h"
#include "SizeComponent.hpp"
#include "TagComp.hpp"
#include "math.h"

using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateMapobjectSystem::Update(entt::registry& reg)
{
	auto view = reg.view<MapObjectComponent>();

	for (auto entity : view)
	{
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& ColliderCmp = reg.get <SingleCollisionShapeComp>(entity);
		auto& SizeCmp = reg.get <Size3DComp>(entity);

		// リジットボディーの更新
		UpdateRB(TransformCmp, RBCmp, ColliderCmp, SizeCmp);

		// トランスフォーム
		btTransform trans;

		// ゲット
		RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

		// 回転行列を取得オフセット分掛ける
		// 物理世界での位置からオフセット分ずらした現実世界での位置を計算する用の変数
		btVector3 worldoffet = trans.getBasis() * btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z);

		// 物理世界の位置から回転行列をかけ合わせたオフセットを引く
		btVector3 pos = trans.getOrigin() - worldoffet;

		// 位置に代入
		TransformCmp.Pos = { pos.x(), pos.y(), pos.z() };
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void UpdateMapobjectSystem::UpdateRB(Transform3D& TransformCmp, RigitBodyComp& RBCmp, SingleCollisionShapeComp& ColliderCmp, Size3DComp& SizeCmp)
{
	// 剛体の削除
	if (RBCmp.RigitBody)
	{
		// 物理世界から削除
		CManager::GetDynamicsWorld()->removeRigidBody(RBCmp.RigitBody.get());

		// モーションステートを取得nullチェック
		if (RBCmp.RigitBody->getMotionState())
		{
			// モーションステートを削除
			delete RBCmp.RigitBody->getMotionState();
		}
		// リジットボディをクリア
		RBCmp.RigitBody.reset();
	}

	// 当たり判定を再生成
	ColliderCmp.CollisionShape.reset(new btBoxShape(btVector3(SizeCmp.Size.x * TransformCmp.Scale.x, SizeCmp.Size.y * TransformCmp.Scale.y, SizeCmp.Size.z * TransformCmp.Scale.z)));
	ColliderCmp.Offset.y = SizeCmp.Size.y * TransformCmp.Scale.y;

	// 質量を宣言
	btScalar Mass = 0;

	// 抗力を代入
	btVector3 Inertia = { 0.0f,0.0f,0.0f };

	// 抗力を設定
	ColliderCmp.CollisionShape->calculateLocalInertia(Mass, Inertia);

	// 物理世界の位置などを取得
	btTransform transform, origin, offset;

	// 初期化
	transform.setIdentity();
	origin.setIdentity();
	offset.setIdentity();

	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	rotation = CMath::SetQuad(TransformCmp.Quat);
	origin.setRotation(rotation);
	origin.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y, TransformCmp.Pos.z));
	offset.setOrigin(btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z));
	transform.mult(origin, offset);

	// インターフェイスを設定
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(Mass, motionState, ColliderCmp.CollisionShape.get());

	// リジットボディーを再生成
	RBCmp.RigitBody.reset(new btRigidBody(info));

	// 移動方向を制限
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	RBCmp.RigitBody->setUserPointer(RBCmp.RigitBody.get());

	// スリープ状態を設定
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界にリジットボディーを追加
	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get());
}