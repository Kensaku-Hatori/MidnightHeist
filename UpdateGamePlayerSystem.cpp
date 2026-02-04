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
#include "fade.h"
#include "Sound2D.h"
#include "math.h"
#include "EnemySoundListener.hpp"
#include "Components.hpp"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 更新
//*********************************************
void UpdateGamePlayerSystem::Update(entt::registry& reg)
{
	auto view = reg.view<PlayerComponent, InGameComp>();

	for (auto [entity] : view.each())
	{
		// コンポーネント取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigidBodyComponent>(entity);
		auto& OutLineCmp = reg.get<OutLineComp>(entity);
		auto& AnimCmp = reg.get<PlayerAnimComp>(entity);
		auto& StateCmp = reg.get<PlayerStateComp>(entity);
		auto& SoundCmp = reg.get<PlayerSoundVolumeComp>(entity);

		if (RBCmp.Body == nullptr) continue;

		// プレイヤーの前方向ベクトル
		D3DXVECTOR3 Front = { -TransformCmp.mtxWorld._31,-TransformCmp.mtxWorld._32,-TransformCmp.mtxWorld._33 };
		// リスナーの位置と向きを設定
		CListener::Instance()->SetPos(TransformCmp.Pos);
		CListener::Instance()->SetFront(Front);

		// プレイヤーが発する音
		SoundCmp.SoundVolume = PlayerSoundVolumeConfig::Bace * PlayerSoundVolumeConfig::Scale[static_cast<int>(StateCmp.NowState)];

		// 円形UIの情報を取得
		auto& Parents = reg.get<ChildrenComp>(entity);
		auto& CircleRenderFrag = reg.get<RenderFragComp>(Parents.Children[1]);
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(Parents.Children[2]);

		// 発している範囲を設定
		SineCurveCmp.Radius = SoundCmp.SoundVolume;

		// 描画フラグを折る
		CircleRenderFrag.IsRendering = false;

		// 状態の初期化
		StateCmp.OldState = StateCmp.NowState;

		// カウンタインクリメント
		if (AnimCmp.FirstDelayFrame > AnimCmp.FirstDelayCounter)AnimCmp.FirstDelayCounter++;
		// 昔のフラグを保存
		AnimCmp.IsFinishedBeltOld = AnimCmp.IsFinishedBelt;
		// アウトラインを徐々に光らせる
		OutLineCmp.Height -= 1.0f;

		// 最初のアニメーションに必要な情報
		D3DXVECTOR3 VecDest = { 700.0f,0.0f,375.0f };
		VecDest -= TransformCmp.Pos;
		VecDest.y = 0.0f;

		// 画面にっ入ったら
		if (TransformCmp.Pos.x < 900.0f)AnimCmp.IsScreen = true;
		// ベルトコンベアアニメーションが終わっていて地面に着いてアニメーションが終了していなかったら
		if (AnimCmp.IsFinishedBelt == true && RBCmp.IsGround == true && AnimCmp.IsFinishedAnim == false)AnimCmp.IsFinishedAnim = true;
		// ベルトコンベアアニメーションが終わっていて地面に着いていたら通常の更新
		if (AnimCmp.IsFinishedBelt == true && RBCmp.IsGround == true)
		{
			UpdateMovement(reg, entity);
			UpdateToEnemyVibration(reg, entity);
			UpdateUnLock(reg, entity);
			UpdateState(reg, entity);
		}
		// アイテムマネージャーのビューを生成
		auto ItemMangerView = reg.view<ItemManagerComp>();
		// エンティティを取得
		entt::entity ItemManagerEntity = *ItemMangerView.begin();
		// コンポーネントを取得
		auto& ItemManagerCmp = reg.get<ItemManagerComp>(ItemManagerEntity);

		// ベルトコンベアの上に乗っていたら
		if (AnimCmp.IsFinishedBelt == false && ItemManagerCmp.IsFinished == true)
		{
			// ベルトコンベアの移動量を設定
			RBCmp.Body->setLinearVelocity(btVector3(-15.0f, RBCmp.Body->getLinearVelocity().getY(), 0.0f));
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
			RBCmp.Body->applyCentralImpulse(btVector3(-20.0f, 25.0f, 0.0f));
		}
		// プレイヤーが画面内に存在したら
		if (AnimCmp.IsScreen == true)
		{
			// ロックオンアニメーションを更新
			UpdateLockOn(reg, entity);
		}
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateGamePlayerSystem::UpdateMovement(entt::registry& reg, entt::entity Player)
{
	// コンポーネント取得
	auto& RBCmp = reg.get<RigidBodyComponent>(Player);
	auto& Trans = reg.get<Transform3D>(Player);
	auto& PlayerStateCmp = reg.get<PlayerStateComp>(Player);
	auto& CharactorCmp = reg.get<CharactorComp>(Player);

	// 早期リターン
	if (RBCmp.Body == nullptr) return;
	if (RBCmp.IsGround == false) return;

	// 移動量を設定するための変数
	btVector3 moveDir(0, 0, 0);

	// カメラから見た移動量を計算
	D3DXVECTOR3 V = CManager::GetCamera()->GetPosR() - CManager::GetCamera()->GetPosV();
	D3DXVECTOR3 H;
	D3DXVECTOR3 VecU = VEC_UP;
	// ベクトルの高さ成分をのぞく
	V.y = 0.0f;

	// 外積でX軸を計算
	D3DXVec3Cross(&H, &V, &VecU);
	D3DXVec3Normalize(&V, &V);
	D3DXVec3Normalize(&H, &H);

	// 速さ
	float Speed = 12.5f;

	// 素早さ
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true ||
		CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_L1) == true ||
		CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_L2) == true)
	{
		Speed = 7.5f;
		PlayerStateCmp.NowState = PlayerState::State::SILENT;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true ||
		CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_R1) == true ||
		CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY_R2) == true)
	{
		Speed = 17.5f;
		PlayerStateCmp.NowState = PlayerState::State::DUSH;
	}

	// スティックのステートを取得
	XINPUT_STATE* State = CManager::GetInputJoypad()->GetJoyStickAngle();
	// スティックが動いているかどうか
	const bool IsMoveStick = CManager::GetInputJoypad()->GetJoyStickL();

	// スティックが動いてなかったら
	if (IsMoveStick == false)
	{
		// キーボード検知
		if (CManager::GetInputKeyboard()->GetPress(DIK_W))  moveDir += btVector3(V.x, V.y, V.z);
		if (CManager::GetInputKeyboard()->GetPress(DIK_S)) moveDir += btVector3(-V.x, -V.y, -V.z);
		if (CManager::GetInputKeyboard()->GetPress(DIK_A)) moveDir += btVector3(H.x, H.y, H.z);
		if (CManager::GetInputKeyboard()->GetPress(DIK_D)) moveDir += btVector3(-H.x, -H.y, -H.z);
	}
	// スティックが動いていたら
	else if(IsMoveStick == true)
	{
		// スティックの角度を正規化
		float fx = static_cast<float>(State->Gamepad.sThumbLX) / 32767.0f;
		float fy = static_cast<float>(State->Gamepad.sThumbLY) / 32767.0f;

		// 移動方向を計算
		moveDir = btVector3(H.x * -fx + V.x * -fy,
			H.y * fx + V.y * fy,
			H.z * fx + V.z * fy);
	}

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
		PlayerStateCmp.NowState = PlayerState::State::NORMAL;
		// コンポーネントを取得
		auto& SoundCmp = reg.get<PlayerSoundVolumeComp>(Player);
		// プレイヤーが発する音
		SoundCmp.SoundVolume = 0.0f;
	}

	// Yの移動量を取得(重力)
	moveDir.setY(RBCmp.Body->getLinearVelocity().y());

	// 設定
	RBCmp.Body->setLinearVelocity(moveDir);
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
	auto& RBCmp = reg.get<RigidBodyComponent>(Player);

	// トランスフォームを取得
	btTransform trans;
	RBCmp.Body->getMotionState()->getWorldTransform(trans);

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

	// 描画フラグを設定する用のフラグ
	bool IsRenderingLocalFrag = false;

	// アクセス
	for (auto entity : ItemView)
	{
		// 自分自身のコンポーネントを取得
		auto& RBCmp = Reg.get<RigidBodyComponent>(entity);
		auto& ItemCmp = Reg.get<ItemComp>(entity);
		auto& PlayerStateCmp = Reg.get<PlayerStateComp>(Player);

		// コンポーネントを取得
		auto& PlayerRBCmp = Reg.get<RigidBodyComponent>(Player);

		// アイテムとプレイヤーのトランスフォームを取得
		btTransform ItemTrans, PlayerTrans;
		RBCmp.Body->getMotionState()->getWorldTransform(ItemTrans);
		PlayerRBCmp.Body->getMotionState()->getWorldTransform(PlayerTrans);
		D3DXVECTOR3 ToPlayer = CMath::SetVec(ItemTrans.getOrigin()) - CMath::SetVec(PlayerTrans.getOrigin());
		ToPlayer.y = 0.0f;

		// 範囲外だったら
		if (D3DXVec3Length(&ToPlayer) > ItemCmp.InteractSize || entity == entt::null)
		{
			ItemCmp.nCntPicking = Clamp(ItemCmp.nCntPicking - 1, 0, INT_MAX);
			continue;
		}

		IsRenderingLocalFrag = true;

		// 移動量を取得
		btVector3 Move = PlayerRBCmp.Body->getLinearVelocity();
		Move.setY(0.0f);
		D3DXVECTOR3 ConvertMove = CMath::SetVec(Move);

		float Dot = D3DXVec3Dot(&ToPlayer, &ConvertMove);

		// ピッキング範囲内ステートに変える
		PlayerStateCmp.NowState = PlayerState::State::RANGE_PICKING;

		if (D3DXVec3Length(&ConvertMove) > 0.0f && Dot > 100.0f)
		{
			CManager::GetInputJoypad()->BeginVibration(0.1f, 0.1f);

			PlayerStateCmp.NowState = PlayerState::State::PICKING;
			ItemCmp.nCntPicking++;
			// 塗りつぶし量を進める
			CircleCmp.FillAmount = ItemConfig::Ratio * ItemCmp.nCntPicking;
			// 最大数以上だったら
			if (ItemCmp.nCntPicking >= ItemConfig::nFramePicking)
			{
				CSound2D::Instance()->Play(SoundDevice::LABEL_PICKING);
				CGame::AddSteal();
				// 要素を削除
				Reg.destroy(entity);
				CMapManager::Instance()->Erase(entity);
				PlayerStateCmp.NowState = PlayerState::State::NORMAL;
			}
		}
		else
		{
			ItemCmp.nCntPicking--;
			// 塗りつぶし量を進める
			CircleCmp.FillAmount = ItemConfig::Ratio * ItemCmp.nCntPicking;
		}
		// クランプ
		ItemCmp.nCntPicking = Clamp(ItemCmp.nCntPicking, 0, ItemConfig::nFramePicking);
	}
	// 円形UIの情報を取得
	auto& CircleRenderFrag = Reg.get<RenderFragComp>(CircleEntity.Children[1]);
	CircleRenderFrag.IsRendering = IsRenderingLocalFrag;
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
		break;
	case PlayerState::State::NORMAL:
		break;
	case PlayerState::State::DUSH:
		break;
	case PlayerState::State::RANGE_PICKING:
		break;
	case PlayerState::State::PICKING:
		// 位置を更新
		CircleTransform.Pos = CirlcePos;
		break;
	default:
		break;
	}
}

//*********************************************
// 近くの敵へのバイブレーション
//*********************************************
void UpdateGamePlayerSystem::UpdateToEnemyVibration(entt::registry& Reg, entt::entity Player)
{
	// 敵のビュー
	auto EnemyView = Reg.view<EnemyComponent>();
	auto& PlayerTransformCmp = Reg.get<Transform3D>(Player);
	auto& PlayerSoundVolumeCmp = Reg.get<PlayerSoundVolumeComp>(Player);

	float MinDistance = FLT_MAX;
	float Range = 0.0f;

	for (auto Entity : EnemyView)
	{
		auto& EnemyTransform = Reg.get<Transform3D>(Entity);
		auto& EnemyListenerCmp = Reg.get<EnemyListenerComp>(Entity);

		D3DXVECTOR3 ToEnemyVec = EnemyTransform.Pos - PlayerTransformCmp.Pos;
		float NowDistance = D3DXVec3Length(&ToEnemyVec);
		if (NowDistance < MinDistance)
		{
			MinDistance = NowDistance;
			Range = PlayerSoundVolumeCmp.SoundVolume + EnemyListenerCmp.ListenerVolume;
		}
	}
	if (Range < 0.0f) return;
	float Ratio = 0.1f - ((MinDistance / Range * 0.5f) * 0.1f);
	Ratio = Clamp(Ratio, 0.0f, 1.0f);
	CManager::GetInputJoypad()->BeginVibration(Ratio, Ratio);
}