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
#include "OutLineComp.hpp"
#include "Velocity.hpp"
#include "TagComp.hpp"
#include "CapsuleComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "LockOnUIAnim.hpp"
#include "ParentComponent.hpp"
#include "SizeComponent.hpp"
#include "ColorComponent.hpp"
#include "SystemManager.h"
#include "game.h"
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
		// コンポーネント取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(entity);
		auto& OutLineCmp = reg.get<OutLineComp>(entity);
		auto& CapsuleCmp = reg.get<CapsuleComp>(entity);
		auto& PlayerAnimCmp = reg.get<PlayerAnimComp>(entity);

		// カウンタインクリメント
		if (PlayerAnimCmp.FirstDelayFrame > PlayerAnimCmp.FirstDelayCounter)PlayerAnimCmp.FirstDelayCounter++;
		// 昔のフラグを保存
		PlayerAnimCmp.IsFinishedBeltOld = PlayerAnimCmp.IsFinishedBelt;

		OutLineCmp.Height -= 1.0f;

		// トランスフォームを取得
		btTransform trans;
		RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

		// 描画モデルの位置
		btVector3 newPos;

		// 位置を取得
		newPos = trans.getOrigin();

		// 位置を計算、設定
		TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - CapsuleCmp.ToCenterOffset, newPos.z()));

		// 最初のアニメーションに必要な情報
		btCapsuleShape myCapsule(btScalar(CapsuleCmp.Radius), btScalar(CapsuleCmp.AllHeight));
		D3DXVECTOR3 VecDest = { 700.0f,0.0f,375.0f };
		VecDest -= TransformCmp.Pos;
		VecDest.y = 0.0f;

		// 着地判定
		RBCmp.IsJump = !RBCmp.IsGround(myCapsule);

		if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
		{
			// 画面にっ入ったら
			if (TransformCmp.Pos.x < 900.0f)PlayerAnimCmp.IsScreen = true;
			// ベルトコンベアアニメーションが終わっていて地面に着いてアニメーションが終了していなかったら
			if (PlayerAnimCmp.IsFinishedBelt == true && RBCmp.IsGround(myCapsule) == true && PlayerAnimCmp.IsFinishedAnim == false)PlayerAnimCmp.IsFinishedAnim = true;
			// ベルトコンベアアニメーションが終わっていて地面に着いていたら通常の更新
			if (PlayerAnimCmp.IsFinishedBelt == true && RBCmp.IsJump == false)
			{
				UpdateRB(TransformCmp, RBCmp, ColliderCmp, CapsuleCmp);
				UpdateMovement(TransformCmp, RBCmp);
			}
			// ベルトコンベアの上に乗っていたら
			if (PlayerAnimCmp.IsFinishedBelt == false && PlayerAnimCmp.FirstDelayFrame <= PlayerAnimCmp.FirstDelayCounter)
			{
				// ベルトコンベアの移動量を設定
				RBCmp.RigitBody->setLinearVelocity(btVector3(-15.0f, RBCmp.RigitBody->getLinearVelocity().getY(), 0.0f));
			}
			// ベルトコンベアの端のほうに着いたら
			if (D3DXVec3Length(&VecDest) < 10.0f)
			{
				// ベルトコンベアの上に載っていない判定
				PlayerAnimCmp.IsFinishedBelt = true;
			}
			// ベルトコンベアの上に乗っているフラグが切り替わった瞬間
			if (PlayerAnimCmp.IsFinishedBeltOld == false && PlayerAnimCmp.IsFinishedBelt == true)
			{
				// ジャンプする
				RBCmp.RigitBody->applyCentralImpulse(btVector3(-20.0f, 25.0f, 0.0f));
			}
			if (PlayerAnimCmp.IsScreen == true)
			{
				UpdateLockOn(reg, entity);
			}
		}
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void PlayerUpdateSystem::UpdateRB(Transform3D& TransformCmp, RigitBodyComp& RBCmp, SingleCollisionShapeComp& ColliderCmp, CapsuleComp& CapsuleCmp)
{
	if (RBCmp.RigitBody != nullptr) return;

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(CapsuleCmp.Radius), btScalar(CapsuleCmp.AllHeight));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + CapsuleCmp.ToCenterOffset, TransformCmp.Pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.RigitBody->setUserPointer(this);
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_PLAYER, CollisionGroupAndMasks::MASK_PLAYER);
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

	// カメラから見た移動量を計算
	D3DXVECTOR3 V = CManager::GetCamera()->GetPosR() - CManager::GetCamera()->GetPosV();
	D3DXVECTOR3 H;
	D3DXVECTOR3 VecU = VEC_UP;
	V.y = 0.0f;

	// 外積でX軸を計算
	D3DXVec3Cross(&H, &V, &VecU);
	D3DXVec3Normalize(&V, &V);
	D3DXVec3Normalize(&H, &H);

	// 速さ
	float Speed = 12.5f;

	// 素早さ
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)Speed = 7.5f;
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)Speed = 17.5f;

	// キーボード検知
	if (CManager::GetInputKeyboard()->GetPress(DIK_W))  moveDir += btVector3(V.x, V.y, V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_S)) moveDir += btVector3(-V.x, -V.y, -V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_A)) moveDir += btVector3(H.x, H.y, H.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_D)) moveDir += btVector3(-H.x, -H.y, -H.z);

	// 移動していたら
	if (moveDir.length2() > 0.0f)
	{
		D3DXVECTOR3 VecUp = VEC_UP;
		D3DXQUATERNION SetQuat;
		// 移動値を方向ベクトルに変換
		moveDir.normalize();

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(moveDir.x(), moveDir.z());
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);
		TransformCmp.QuatDest = SetQuat;
		// スピードを掛ける
		moveDir *= Speed;
	}

	// Yの移動量を取得(重力)
	moveDir.setY(RBCmp.RigitBody->getLinearVelocity().y());

	// 設定
	RBCmp.RigitBody->setLinearVelocity(moveDir);
}

//*********************************************
// ロックオンの更新
//*********************************************
void PlayerUpdateSystem::UpdateLockOn(entt::registry& reg, entt::entity Player)
{
	// ゲーム以外なら早期リターン
	if (CManager::GetScene()->GetMode() != CScene::MODE_GAME) return;
	// ロックオンが無効だったら
	if (reg.valid(reg.get<SingleParentComp>(Player).Parent) == false) return;

	// ロックオンエンティティを取得
	entt::entity LockOnEntity = reg.get<SingleParentComp>(Player).Parent;

	// ロックオンエンティティのコンポーネントを取得
	auto& LockAnimCmp = reg.get<LockOnAnimComp>(LockOnEntity);
	auto& LockOnSize = reg.get<SizeComp>(LockOnEntity);
	auto& LockOnColor = reg.get<ColorComp>(LockOnEntity);
	auto& TransformLockOn = reg.get<Transform2D>(LockOnEntity);
	// ロックオンの位置を決めるためにコンポーネントを取得
	auto& RBCmp = reg.get<RigitBodyComp>(Player);

	// トランスフォームを取得
	btTransform trans;
	RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

	// 剛体の位置
	btVector3 newPos;
	newPos = trans.getOrigin();

	// 剛体の位置を２D座標に変換
	D3DXVECTOR3 PlayerCenter = CMath::SetVec(newPos);
	PlayerCenter = CMath::Get3Dto2DPosition(PlayerCenter);
	// ロックオンの参照位置を剛体の２D座標に設定
	LockAnimCmp.Reference = { PlayerCenter.x,PlayerCenter.y };
	// ロックオンの参照位置を描画ポリゴンの位置に設定
	TransformLockOn.Pos = LockAnimCmp.Reference;

	// ロックオン
	// 大きさの差分
	D3DXVECTOR2 DiffSize = LockAnimCmp.DestSize - LockAnimCmp.ApperSize;
	// 大きさアニメーションの進行度
	float RatioLock = (float)LockAnimCmp.ToLockonCounter / (float)LockAnimCmp.ToLockonFrame;
	// 設定する用の大きさ
	D3DXVECTOR2 Size = LockAnimCmp.ApperSize + (DiffSize * RatioLock);

	// ロックオンリリース
	// 色の差分
	D3DXCOLOR DiffColor = LockAnimCmp.DestColor - LockAnimCmp.ApperColor;
	// 色アニメーションの進行度
	float RatioRelease = (float)LockAnimCmp.ReleaseLockCounter / (float)LockAnimCmp.ReleaseLockFrame;
	// 設定する用の色
	D3DXCOLOR Color = LockAnimCmp.ApperColor + (DiffColor * RatioRelease);

	// ステートを見て処理を変える
	switch (LockAnimCmp.NowState)
	{
		// 照準を合わせる
	case LockOnAnimState::State::TOLOCK:
		// カウンタが最大数まで到達していなかったら
		if (LockAnimCmp.ToLockonFrame > LockAnimCmp.ToLockonCounter)LockAnimCmp.ToLockonCounter++;
		else
		{
			// ホールドステートに変更
			LockAnimCmp.NowState = LockOnAnimState::State::LOCKEDIN;
		}
		break;
		// ホールド
	case LockOnAnimState::State::LOCKEDIN:
		// カウンタが最大数まで到達していなかったら
		if (LockAnimCmp.LockedInFrame > LockAnimCmp.LockedInCounter)LockAnimCmp.LockedInCounter++;
		else
		{
			// 解除状態
			LockAnimCmp.NowState = LockOnAnimState::State::RELEASELOCK;
		}
		break;
		// 解除
	case LockOnAnimState::State::RELEASELOCK:
		// カウンタが最大数まで到達していなかったら
		if (LockAnimCmp.ReleaseLockFrame > LockAnimCmp.ReleaseLockCounter)LockAnimCmp.ReleaseLockCounter++;
		else
		{
			// 終わった状態
			LockAnimCmp.NowState = LockOnAnimState::State::MAX;
		}
		break;
		// 終わった状態
	case LockOnAnimState::State::MAX:
		// 削除リストに追加
		CSystemManager::AddDestroyList(LockOnEntity);
		break;
	default:
		break;
	}
	// 情報を設定
	LockOnSize.Size = Size;
	LockOnColor.Col = Color;
}