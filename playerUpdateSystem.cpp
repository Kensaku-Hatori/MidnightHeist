//****************************************************************
//
// プレイヤー更新システムのクラスの処理[playerUpdateSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "playerUpdateSystem.h"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"
#include "TagComp.hpp"
#include "math.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void PlayerUpdateSystem::Update(entt::registry& reg)
{
	auto view = reg.view<PlayerComponent>();

	for (auto entity : view)
	{
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(entity);

		UpdateRB(TransformCmp, RBCmp, ColliderCmp);
		UpdateMovement(TransformCmp, RBCmp);
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void PlayerUpdateSystem::UpdateRB(Transform3D& TransformCmp, RigitBodyComp& RBCmp, SingleCollisionShapeComp& ColliderCmp)
{
	if (RBCmp.RigitBody != nullptr) return;

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(7.0f), btScalar(20.0f));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + 17.0f, TransformCmp.Pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.RigitBody->setUserPointer(this);
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get());
}

//*********************************************
// 移動の更新
//*********************************************
void PlayerUpdateSystem::UpdateMovement(Transform3D& TransformCmp, RigitBodyComp& RBCmp)
{
	// 早期リターン
	if (RBCmp.RigitBody == nullptr) return;
	if (RBCmp.RigitBody->getMotionState() == nullptr) return;

	// 移動量を設定するための変数
	btVector3 moveDir(0, 0, 0);

	// 描画モデルの位置
	btVector3 newPos;

	// カメラから見た移動量を計算
	D3DXVECTOR3 V = CManager::GetCamera()->GetPosR() - CManager::GetCamera()->GetPosV();
	D3DXVECTOR3 H;
	D3DXVECTOR3 VecU = VEC_UP;
	V.y = 0.0f;

	// 外積でX軸を計算
	D3DXVec3Cross(&H, &V, &VecU);
	D3DXVec3Normalize(&V, &V);
	D3DXVec3Normalize(&H, &H);

	// 素早さ
	const float Speed = 10;

	// キーボード検知
	if (CManager::GetInputKeyboard()->GetPress(DIK_W))  moveDir += btVector3(V.x, V.y, V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_S)) moveDir += btVector3(-V.x, -V.y, -V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_A)) moveDir += btVector3(H.x, H.y, H.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_D)) moveDir += btVector3(-H.x, -H.y, -H.z);

	// 移動していたら
	if (moveDir.length2() > 0.0f)
	{
		D3DXVECTOR3 Axis = CMath::SetVec(moveDir);
		D3DXVECTOR3 VecFront = { 0.0f,0.0f,-1.0f };
		D3DXVECTOR3 VecUp = VEC_UP;
		// 移動値を方向ベクトルに変換
		moveDir.normalize();

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(moveDir.x(), moveDir.z());
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&TransformCmp.Quat, &VecUp, angle);
		// スピードを掛ける
		moveDir *= Speed;
	}

	// Yの移動量を取得(重力)
	moveDir.setY(RBCmp.RigitBody->getLinearVelocity().y());

	// 設定
	RBCmp.RigitBody->setLinearVelocity(moveDir);

	// トランスフォームを取得
	btTransform trans;
	RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

	// 位置を取得
	newPos = trans.getOrigin();

	// 位置を計算、設定
	TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 17.0f, newPos.z()));
}