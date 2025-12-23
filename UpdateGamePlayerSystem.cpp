//****************************************************************
//
// プレイヤー更新システムのクラスの処理[playerUpdateSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateGamePlayerSystem.h"
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
#include "ItemComp.h"
#include "PlayerStateComp.hpp"
#include "RenderFragComp.hpp"
#include "UICircleComp.hpp"
#include "ItemManagerComp.hpp"
#include "PlayerSoundVolumeComp.hpp"
#include "VisibleSineCurveComp.hpp"
#include "mapmanager.h"
#include "math_T.h"
#include "Sound3D.h"
#include "ChildComp.hpp"
#include "CharactorComp.hpp"
#include "math.h"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 更新
//*********************************************
void UpdateGamePlayerSystem::Update(entt::registry& reg)
{
	auto view = reg.view<PlayerComponent,InGameComp>();

	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& OutLineCmp = reg.get<OutLineComp>(entity);
		auto& CapsuleCmp = reg.get<CapsuleComp>(entity);
		auto& AnimCmp = reg.get<PlayerAnimComp>(entity);
		auto& StateCmp = reg.get<PlayerStateComp>(entity);
		auto& SoundCmp = reg.get<PlayerSoundVolumeComp>(entity);

		D3DXVECTOR3 Front = { -TransformCmp.mtxWorld._31,-TransformCmp.mtxWorld._32,-TransformCmp.mtxWorld._33 };
		CListener::Instance()->SetPos(TransformCmp.Pos);
		CListener::Instance()->SetFront(Front);

		// プレイヤーが発する音
		SoundCmp.SoundVolume = PlayerSoundVolumeConfig::Bace * PlayerSoundVolumeConfig::Scale[static_cast<int>(StateCmp.NowState)];

		// 円形UIの情報を取得
		auto& Parents = reg.get<ChildrenComp>(entity);
		auto& CircleRenderFrag = reg.get<RenderFragComp>(Parents.Children[1]);
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(Parents.Children[2]);

		SineCurveCmp.Radius = SoundCmp.SoundVolume;

		// 描画フラグを折る
		CircleRenderFrag.IsRendering = false;

		// 状態の初期化
		StateCmp.OldState = StateCmp.NowState;
		StateCmp.NowState = PlayerState::State::NORMAL;

		// カウンタインクリメント
		if (AnimCmp.FirstDelayFrame > AnimCmp.FirstDelayCounter)AnimCmp.FirstDelayCounter++;
		// 昔のフラグを保存
		AnimCmp.IsFinishedBeltOld = AnimCmp.IsFinishedBelt;

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

		// 画面にっ入ったら
		if (TransformCmp.Pos.x < 900.0f)AnimCmp.IsScreen = true;
		// ベルトコンベアアニメーションが終わっていて地面に着いてアニメーションが終了していなかったら
		if (AnimCmp.IsFinishedBelt == true && RBCmp.IsGround(myCapsule) == true && AnimCmp.IsFinishedAnim == false)AnimCmp.IsFinishedAnim = true;
		// ベルトコンベアアニメーションが終わっていて地面に着いていたら通常の更新
		if (AnimCmp.IsFinishedBelt == true && RBCmp.IsJump == false)
		{
			UpdateRB(reg,entity);
			UpdateMovement(reg, entity);
			UpdateUnLock(reg, entity);
			UpdateState(reg, entity);
		}
		auto ItemMangerView = reg.view<ItemManagerComp>();
		entt::entity ItemManagerEntity = *ItemMangerView.begin();
		auto& ItemManagerCmp = reg.get<ItemManagerComp>(ItemManagerEntity);

		// ベルトコンベアの上に乗っていたら
		if (AnimCmp.IsFinishedBelt == false && ItemManagerCmp.IsFinished == true)
		{
			// ベルトコンベアの移動量を設定
			RBCmp.RigitBody->setLinearVelocity(btVector3(-15.0f, RBCmp.RigitBody->getLinearVelocity().getY(), 0.0f));
		}
		// ベルトコンベアの端のほうに着いたら
		if (D3DXVec3Length(&VecDest) < 10.0f)
		{
			// ベルトコンベアの上に載っていない判定
			AnimCmp.IsFinishedBelt = true;
		}
		// ベルトコンベアの上に乗っているフラグが切り替わった瞬間
		if (AnimCmp.IsFinishedBeltOld == false && AnimCmp.IsFinishedBelt == true)
		{
			// ジャンプする
			RBCmp.RigitBody->applyCentralImpulse(btVector3(-20.0f, 25.0f, 0.0f));
		}
		if (AnimCmp.IsScreen == true)
		{
			UpdateLockOn(reg, entity);
		}
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void UpdateGamePlayerSystem::UpdateRB(entt::registry& reg, entt::entity Player)
{
	// コンポーネント取得
	auto& RBCmp = reg.get<RigitBodyComp>(Player);
	auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(Player);
	auto& CapsuleCmp = reg.get<CapsuleComp>(Player);
	auto& TransformCmp = reg.get<Transform3D>(Player);

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
void UpdateGamePlayerSystem::UpdateMovement(entt::registry& reg, entt::entity Player)
{
	// コンポーネント取得
	auto& RBCmp = reg.get<RigitBodyComp>(Player);
	auto& Trans = reg.get<Transform3D>(Player);
	auto& PlayerStateCmp = reg.get<PlayerStateComp>(Player);
	auto& CharactorCmp = reg.get<CharactorComp>(Player);

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
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{
		Speed = 7.5f;
		PlayerStateCmp.NowState = PlayerState::State::SILENT;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true) 
	{
		Speed = 17.5f;
		PlayerStateCmp.NowState = PlayerState::State::DUSH;
	}

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
		CharactorCmp.QuatDest = SetQuat;
		// スピードを掛ける
		moveDir *= Speed;
		CharactorCmp.QuatDest = SetQuat;
		D3DXQuaternionSlerp(&Trans.Quat, &Trans.Quat, &CharactorCmp.QuatDest, CharactorCmp.QuatSpeed);
	}
	else
	{
		auto& SoundCmp = reg.get<PlayerSoundVolumeComp>(Player);

		// プレイヤーが発する音
		SoundCmp.SoundVolume = 0.0f;
	}

	// Yの移動量を取得(重力)
	moveDir.setY(RBCmp.RigitBody->getLinearVelocity().y());

	// 設定
	RBCmp.RigitBody->setLinearVelocity(moveDir);
}

//*********************************************
// ロックオンの更新
//*********************************************
void UpdateGamePlayerSystem::UpdateLockOn(entt::registry& reg, entt::entity Player)
{
	// ロックオンが無効だったら
	if (reg.valid(reg.get<ChildrenComp>(Player).Children[0]) == false) return;

	// ロックオンエンティティを取得
	entt::entity LockOnEntity = reg.get<ChildrenComp>(Player).Children[0];

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

//*********************************************
// アイテムの解錠
//*********************************************
void UpdateGamePlayerSystem::UpdateUnLock(entt::registry& Reg, entt::entity Player)
{
	// アイテムのビュー
	auto ItemView = Reg.view<ItemComponent>();

	// 円形UIの情報を取得
	auto& CircleEntity = Reg.get<ChildrenComp>(Player);
	auto& CircleCmp = Reg.get<UICircleComp>(CircleEntity.Children[1]);

	// アクセス
	for (auto entity : ItemView)
	{
		// 自分自身のコンポーネントを取得
		auto& TransformCmp = Reg.get<Transform3D>(entity);
		auto& ItemCmp = Reg.get<ItemComp>(entity);
		auto& PlayerStateCmp = Reg.get<PlayerStateComp>(Player);

		// コンポーネントを取得
		auto& PlayerTransCmp = Reg.get<Transform3D>(Player);
		D3DXVECTOR3 ToPlayer = TransformCmp.Pos - PlayerTransCmp.Pos;

		// アイテムを削除
		if (D3DXVec3Length(&ToPlayer) > ItemCmp.InteractSize)
		{
			ItemCmp.nCntPicking--;
			continue;
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_F))
		{
			PlayerStateCmp.NowState = PlayerState::State::PICKING;
			ItemCmp.nCntPicking++;
			// 塗りつぶし量を進める
			CircleCmp.FillAmount = ItemConfig::Ratio * ItemCmp.nCntPicking;
			// 最大数以上だったら
			if (ItemCmp.nCntPicking >= ItemConfig::nFramePicking)
			{
				// 要素を削除
				Reg.destroy(entity);
				CMapManager::Instance()->Erase(entity);
			}
		}
		// クランプ
		ItemCmp.nCntPicking = Clamp(ItemCmp.nCntPicking, 0, ItemConfig::nFramePicking);
	}
}

//*********************************************
// 状態ごとの更新
//*********************************************
void UpdateGamePlayerSystem::UpdateState(entt::registry& Reg, entt::entity Player)
{
	// 状態コンポーネントを取得
	auto& PlayerStateCmp = Reg.get<PlayerStateComp>(Player);
	// 円の中心点をプレイヤーの座標基準に設定
	D3DXVECTOR3 CirlcePos = VEC3_NULL;
	// 少し上にあげる
	CirlcePos.y += 100.0f;
	// 円形UIの情報を取得
	auto& CircleEntity = Reg.get<ChildrenComp>(Player);
	auto& CircleTransform = Reg.get<Transform3D>(CircleEntity.Children[1]);
	auto& CircleRenderFrag = Reg.get<RenderFragComp>(CircleEntity.Children[1]);
	auto& CircleCmp = Reg.get<UICircleComp>(CircleEntity.Children[1]);

	// ステートによって処理を分ける
	switch (PlayerStateCmp.NowState)
	{
	case PlayerState::State::SILENT:
		// 塗りつぶし量を進める
		CircleCmp.FillAmount -= 0.01f;
		break;
	case PlayerState::State::NORMAL:
		// 塗りつぶし量を進める
		CircleCmp.FillAmount -= 0.01f;
		break;
	case PlayerState::State::DUSH:
		// 塗りつぶし量を進める
		CircleCmp.FillAmount -= 0.01f;
		break;
	case PlayerState::State::PICKING:
		// 位置を更新
		CircleTransform.Pos = CirlcePos;
		// ピッキング中だけ描画フラグを立てる
		CircleRenderFrag.IsRendering = true;
		break;
	default:
		break;
	}
}