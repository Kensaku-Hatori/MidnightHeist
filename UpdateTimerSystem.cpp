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

		int Time = TimerCmp.nData;
		int DigitNum = 0;

		do {
			DigitNum++;
			Time /= 10;
		} while (Time > 0);
		if (DigitNum > 0) TimerCmp.nDigitNum = DigitNum;

		if (Children.Children.size() < TimerCmp.nDigitNum)
		{
			D3DXVECTOR2 SettPos = Trans.Pos - (TimerCmp.DigitOffset * static_cast<int>(Children.Children.size() + 1));
			Children.Children.push_back(Factories::makeObject2D(reg, 4, "data/TEXTURE/number001.png", SettPos, TimerCmp.DigitSize));
			continue;
		}
		Time = TimerCmp.nData;
		std::vector<int> DigitInfo;

		// 桁ごとの数字を算出
		int Second = Time;
		for (int nCount = 0; nCount < TimerCmp.nDigitNum; nCount++)
		{
			int Test = (int)pow(10, (double)nCount);
			int SetValue = Second / Test;
			DigitInfo.push_back(SetValue);
		}
		for (int nCount = 0; nCount < TimerCmp.nDigitNum; nCount++)
		{
			auto& UVCmp = reg.get<UVComp>(Children.Children[nCount]);
			auto& TransDigit = reg.get<Transform2D>(Children.Children[nCount]);

			D3DXVECTOR2 SettPos = Trans.Pos - (TimerCmp.DigitOffset * nCount);
			TransDigit.Pos = SettPos;
			UVCmp.UV[0] = D3DXVECTOR2(0.0f + ((1.0f / 10) * DigitInfo[nCount]), 0.0f);
			UVCmp.UV[1] = D3DXVECTOR2((1.0f / 10) + ((1.0f / 10) * DigitInfo[nCount]), 0.0f);
			UVCmp.UV[2] = D3DXVECTOR2(0.0f + ((1.0f / 10) * DigitInfo[nCount]), 1.0f);
			UVCmp.UV[3] = D3DXVECTOR2((1.0f / 10) + ((1.0f / 10) * DigitInfo[nCount]), 1.0f);
		}
	}
}