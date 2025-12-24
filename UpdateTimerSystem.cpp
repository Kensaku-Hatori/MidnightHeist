//****************************************************************
//
// タイマー更新システムの処理[UpdateTimerSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateTimerSystem.h"
#include "TagComp.hpp"
#include "TimerComp.hpp"
#include "ChildComp.hpp"
#include "Factories.h"
#include "TransformComponent.hpp"
#include "UVComponent.hpp"

// 名前空間
using namespace Tag;

//***************************************
// 更新処理
//***************************************
void UpdateTimerSystem::Update(entt::registry& reg)
{
	// ビューを作成
	auto view = reg.view<TimerComponent>();
	// アクセス
	for (auto Entity : view)
	{
		// コンポーネントを取得
		auto& Trans = reg.get<Transform2D>(Entity);
		auto& TimerCmp = reg.get<TimerComp>(Entity);
		auto& Children = reg.get<ChildrenComp>(Entity);

		// 一時変数に代入
		int Time = TimerCmp.nData;
		// 桁数用の変数
		int DigitNum = 0;

		// 桁数を求める
		do {
			// 桁数をインクリメント
			DigitNum++;
			// 割る
			Time /= 10;
		} while (Time > 0);
		// 桁数を割り当てる
		if (DigitNum > 0) TimerCmp.nDigitNum = DigitNum;

		// 足りない分だけ生成
		if (Children.Children.size() < TimerCmp.nDigitNum)
		{
			D3DXVECTOR2 SettPos = Trans.Pos - (TimerCmp.DigitOffset * static_cast<int>(Children.Children.size() + 1));
			Children.Children.push_back(Factories::makeObject2D(reg, 4, "data/TEXTURE/number001.png", SettPos, TimerCmp.DigitSize));
			continue;
		}

		// 保存
		Time = TimerCmp.nData;
		// 桁ごとのデータ用の配列
		std::vector<int> DigitInfo;

		// 桁数分繰り返す
		for (int nCount = 0; nCount < TimerCmp.nDigitNum; nCount++)
		{
			int Scale = (int)pow(10, (double)nCount);
			int SetValue = Time / Scale;
			DigitInfo.push_back(SetValue);
		}
		// その桁の数字に応じたUVと位置を設定
		for (int nCount = 0; nCount < TimerCmp.nDigitNum; nCount++)
		{
			// コンポーネントを取得
			auto& UVCmp = reg.get<UVComp>(Children.Children[nCount]);
			auto& TransDigit = reg.get<Transform2D>(Children.Children[nCount]);

			// 位置を設定
			D3DXVECTOR2 SettPos = Trans.Pos - (TimerCmp.DigitOffset * nCount);
			TransDigit.Pos = SettPos;
			// UV座標を設定
			UVCmp.UV[0] = D3DXVECTOR2(0.0f + ((1.0f / 10) * DigitInfo[nCount]), 0.0f);
			UVCmp.UV[1] = D3DXVECTOR2((1.0f / 10) + ((1.0f / 10) * DigitInfo[nCount]), 0.0f);
			UVCmp.UV[2] = D3DXVECTOR2(0.0f + ((1.0f / 10) * DigitInfo[nCount]), 1.0f);
			UVCmp.UV[3] = D3DXVECTOR2((1.0f / 10) + ((1.0f / 10) * DigitInfo[nCount]), 1.0f);
		}
	}
}