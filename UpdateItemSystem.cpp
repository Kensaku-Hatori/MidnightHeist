//****************************************************************
//
// アイテム更新システムの処理[UpdateItemSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateItemSystem.h"
#include "OutLineComp.hpp"
#include "TagComp.hpp"
#include "RigitBodyComponent.hpp"
#include "CapsuleComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "ParentComponent.hpp"
#include "ColorComponent.hpp"
#include "SizeComponent.hpp"
#include "LockOnUIAnim.hpp"
#include "SystemManager.h"
#include "game.h"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void UpdateItemSystem::Update(entt::registry& reg)
{
	// ビューを取得
	auto view = reg.view<ItemComponent>();

	// アクセス
	for (auto entity : view)
	{
		auto& TransformCmp = reg.get<Transform3D>(entity);

		auto PlayerView = reg.view<PlayerComponent>();

		// プレイヤーが存在したら
		if (!PlayerView.empty())
		{
			// Entityを取得
			auto playerEntity = *PlayerView.begin();
			// コンポーネントを取得
			auto& PlayerTransCmp = reg.get<Transform3D>(playerEntity);
			D3DXVECTOR3 ToPlayer = TransformCmp.Pos - PlayerTransCmp.Pos;
			// アイテムを削除
			if (D3DXVec3Length(&ToPlayer) < 50.0f) reg.destroy(entity);
		}

		// 自身が無効なら
		if (reg.valid(entity) == false) continue;
		// 最初のノイズが終わったら
		if (CGame::IsFinishedFirstNoise() == false) break;

		// コンポーネントを取得
		auto& OutLineCmp = reg.get<OutLineComp>(entity);
		// アウトラインを徐々に描画
		OutLineCmp.Height -= 1.0f;
		// ロックオンアニメーションの更新
		UpdateLockOn(reg, entity);
	}
}

//***************************************
// ロックオン更新
//***************************************
void UpdateItemSystem::UpdateLockOn(entt::registry& Reg, entt::entity& Entity)
{
	// ロックオンが無効だったら
	if (Reg.valid(Reg.get<SingleParentComp>(Entity).Parent) == false) return;

	// ロックオンのエンティティを取得
	entt::entity LockOnEntity = Reg.get<SingleParentComp>(Entity).Parent;

	// ロックオンのコンポーネントを取得
	auto& LockAnimCmp = Reg.get<LockOnAnimComp>(LockOnEntity);
	auto& LockOnSize = Reg.get<SizeComp>(LockOnEntity);
	auto& LockOnColor = Reg.get<ColorComp>(LockOnEntity);
	auto& TransformLockOn = Reg.get<Transform2D>(LockOnEntity);
	// ロックオンの位置を決めるためにコンポーネントを取得
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);

	// トランスフォームを取得
	btTransform trans;
	RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

	// 剛体の位置
	btVector3 newPos;
	newPos = trans.getOrigin();

	// 剛体の位置を２D座標に変換
	D3DXVECTOR3 ItemCenter = CMath::SetVec(newPos);
	ItemCenter = CMath::Get3Dto2DPosition(ItemCenter);
	// ロックオンの参照位置を剛体の２D座標に設定
	LockAnimCmp.Reference = { ItemCenter.x,ItemCenter.y };
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