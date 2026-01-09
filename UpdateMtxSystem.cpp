//****************************************************************
//
// マトリックスの計算処理[UpdateMtxsystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateMtxSystem.h"
#include "TransformComponent.hpp"
#include "ParentComponent.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateMtxSystem::Update(entt::registry& Reg)
{
	// ビューを生成
	auto TransformView = Reg.view<Transform3D>();
	auto ParentView = Reg.view<ParentComp, Transform3D>();

	// アクセス
	for (auto Entity : TransformView)
	{
		// コンポーネントを取得
		auto& Transform = Reg.get<Transform3D>(Entity);

		// マトリックスを計算
		D3DXMatrixIdentity(&Transform.mtxWorld);
		D3DXMatrixScaling(&Transform.mtxScale, Transform.Scale.x, Transform.Scale.y, Transform.Scale.z);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxScale);
		D3DXMatrixRotationQuaternion(&Transform.mtxBasis, &Transform.Quat);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxBasis);
		D3DXMatrixTranslation(&Transform.mtxTrans, Transform.Pos.x, Transform.Pos.y, Transform.Pos.z);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxTrans);
	}

	// 親子関係が生成された順にソート
	std::vector<entt::entity> ParentEntityList(ParentView.begin(), ParentView.end());
	std::sort(ParentEntityList.begin(), ParentEntityList.end(), [&](entt::entity Other, entt::entity Self)
		{
			return Reg.get<ParentComp>(Other).nIdx < Reg.get<ParentComp>(Self).nIdx;
		}
	);
	// アクセス
	for (auto Entity : ParentEntityList)
	{
		// コンポーネントを取得
		auto& ParentCmp = Reg.get<ParentComp>(Entity);
		auto& TransformCmp = Reg.get<Transform3D>(Entity);
		// 有効だったら
		if (Reg.valid(ParentCmp.Parent) == true)
		{
			// 親のコンポーネントを取得
			auto& ParentTrans = Reg.get<Transform3D>(ParentCmp.Parent);
			// かけ合わせる
			D3DXMatrixMultiply(&TransformCmp.mtxWorld, &ParentTrans.mtxWorld, &TransformCmp.mtxWorld);
		}
	}
}