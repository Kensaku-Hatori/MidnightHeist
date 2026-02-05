//****************************************************************
//
// スタッツアニメーション更新システムの処理[UpdateStutsAnimSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateStutsAnimSystem.h"
#include "ChildComponent.hpp"
#include "SizeComponent.hpp"
#include "StutsComponent.h"
#include "TimerComponent.hpp"
#include "TagComp.hpp"
#include "RenderFragComponent.hpp"
#include "StutsComponent.h"
#include "math_T.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateStutsAnimSystem::Update(entt::registry& reg)
{
	// スタッツマメージャーのビュー
	auto StutsManagerView = reg.view<StutsManager>();
	// 存在しなかったら
	if (StutsManagerView.size() <= 0) return;
	// 先頭のエンティティを取り出す
	entt::entity StutsManager = *StutsManagerView.begin();

	// コンポーネントを取得
	auto& StutsManagerCmp = reg.get<StutsManagerComponent>(StutsManager);
	auto& ChildrenCmp = reg.get<ChildrenComponent>(StutsManager);

	// アニメーションが終わっていたら
	if (StutsManagerCmp.IsFinishedAnim == true) return;

	// アニメーション中のエンティティのコンポーネントを取得
	auto& RenderFragCmp = reg.get<RenderFragComponent>(ChildrenCmp.Children[StutsConfig::WORD_TYPE_MAX + StutsManagerCmp.nIdx]);
	auto& TimerCmp = reg.get<TimerComponent>(ChildrenCmp.Children[StutsConfig::WORD_TYPE_MAX + StutsManagerCmp.nIdx]);

	// カウンタを進める
	StutsManagerCmp.nCountFrame++;
	// 描画フラグを有効にする
	RenderFragCmp.IsRendering = true;

	// イージング関数で割合を求める
	float Ratio = EaseInOutSine((float)StutsManagerCmp.nCountFrame / StutsConfig::nFrameAnim);
	// 目標の値までの差分
	D3DXVECTOR2 SizeDiff = StutsConfig::DestNumberTable[StutsManagerCmp.nIdx].Size - StutsConfig::ApperNumberTable[StutsManagerCmp.nIdx].Size;
	// 設定用一時変数
	D3DXVECTOR2 Setter = StutsConfig::ApperNumberTable[StutsManagerCmp.nIdx].Size + SizeDiff * Ratio;
	// 設定
	TimerCmp.DigitSize = Setter;

	// アニメーションが終わっていたら
	if (StutsManagerCmp.nCountFrame > StutsConfig::nFrameAnim)
	{
		// カウンタリセット
		StutsManagerCmp.nCountFrame = 0;
		// アニメーション中エンティティを進める
		StutsManagerCmp.nIdx++;
		// 最後まで行ったら
		if (StutsManagerCmp.nIdx > StutsConfig::NUM_TYPE_MAX - 1)StutsManagerCmp.IsFinishedAnim = true;
	}
}