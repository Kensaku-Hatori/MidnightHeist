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
#include "ItemComp.h"
#include "ItemManagerComp.hpp"
#include "math_T.h"
#include "TransformComponent.hpp"
#include "ChildComp.hpp"
#include "fade.h"
#include "Result.h"
#include "game.h"
#include "GateManagerComponent.hpp"
#include "Components.hpp"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void UpdateItemSystem::Update(entt::registry& reg)
{
	// ビューを取得
	auto view = reg.view<ItemComponent>();

	// ゲームシーン中にアイテムがなくなったら
	if (static_cast<int>(view.size()) <= 0 && CManager::GetScene()->GetMode() == CScene::MODE_GAME  || CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
	{
		// ゲートマネージャーのビューを生成
		auto GateManagerView = reg.view<GateManager>();
		// エンティティを決め打ちで取得
		auto& GateManagerEntity = *GateManagerView.begin();
		// コンポーネント取得
		auto& GateManagerFragCmp = reg.get<GateManagerComponent>(GateManagerEntity);
		// フラグを有効にする
		GateManagerFragCmp.m_IsOpenTrigger = true;
	}

	// アクセス
	for (auto entity : view)
	{
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
	if (Reg.valid(Reg.get<ChildrenComp>(Entity).Children[0]) == false) return;

	// アイテムマネージャーのビューを生成
	auto ItemMangerView = Reg.view<ItemManagerComp>();
	// アイテムマネージャーの先頭のエンティティを取得
	entt::entity ItemManagerEntity = *ItemMangerView.begin();
	// コンポーネントを取得
	auto& ItemManagerCmp = Reg.get<ItemManagerComp>(ItemManagerEntity);
	// ロックオンのエンティティを取得
	entt::entity LockOnEntity = Reg.get<ChildrenComp>(Entity).Children[0];
	// ロックオンのコンポーネントを取得
	auto& LockAnimCmp = Reg.get<LockOnAnimComp>(LockOnEntity);

	// 現在フォーカスされているアイテムが自分自身じゃなかったらかつ起動していなかったら
	if (ItemManagerCmp.ItemLiset[ItemManagerCmp.NowAnimIdx] != Entity && LockAnimCmp.IsBoot == false) return;

	// コンポーネントを取得
	auto& LockOnSize = Reg.get<SizeComp>(LockOnEntity);
	auto& LockOnColor = Reg.get<ColorComp>(LockOnEntity);
	auto& TransformLockOn = Reg.get<Transform2D>(LockOnEntity);

	// ロックオンの位置を決めるためにコンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);

	if (RBCmp.Body == nullptr) return;

	// 起動状態にする
	if (LockAnimCmp.IsBoot == false) LockAnimCmp.IsBoot = true;
	// トランスフォームを取得
	btTransform trans;
	RBCmp.Body->getMotionState()->getWorldTransform(trans);

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
			ItemManagerCmp.NowAnimIdx = Clamp(ItemManagerCmp.NowAnimIdx + 1, 0, static_cast<int>(ItemManagerCmp.ItemLiset.size() - 1));
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
		if (ItemManagerCmp.NowAnimIdx >= static_cast<int>(ItemManagerCmp.ItemLiset.size() - 1)) ItemManagerCmp.IsFinished = true;
		break;
	default:
		break;
	}
	// 情報を設定
	LockOnSize.Size = Size;
	LockOnColor.Col = Color;
}