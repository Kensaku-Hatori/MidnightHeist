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
	auto TransformView = Reg.view<Transform3D>();
	auto ParentView = Reg.view<ParentComp, Transform3D>();
	for (auto Entity : TransformView)
	{
		auto& Transform = Reg.get<Transform3D>(Entity);

		D3DXMatrixIdentity(&Transform.mtxWorld);
		D3DXMatrixScaling(&Transform.mtxScale, Transform.Scale.x, Transform.Scale.y, Transform.Scale.z);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxScale);
		D3DXMatrixRotationQuaternion(&Transform.mtxBasis, &Transform.Quat);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxBasis);
		D3DXMatrixTranslation(&Transform.mtxTrans, Transform.Pos.x, Transform.Pos.y, Transform.Pos.z);
		D3DXMatrixMultiply(&Transform.mtxWorld, &Transform.mtxWorld, &Transform.mtxTrans);
	}
	std::vector<entt::entity> ParentEntityList(ParentView.begin(), ParentView.end());
	std::sort(ParentEntityList.begin(), ParentEntityList.end(), [&](entt::entity Other, entt::entity Self)
		{
			return Reg.get<ParentComp>(Other).nIdx < Reg.get<ParentComp>(Self).nIdx;
		}
	);
	for (auto Entity : ParentEntityList)
	{
		auto& ParentCmp = Reg.get<ParentComp>(Entity);
		auto& TransformCmp = Reg.get<Transform3D>(Entity);
		if (Reg.valid(ParentCmp.Parent) == true)
		{
			auto& ParentTrans = Reg.get<Transform3D>(ParentCmp.Parent);
			D3DXMatrixMultiply(&TransformCmp.mtxWorld, &ParentTrans.mtxWorld, &TransformCmp.mtxWorld);
		}
	}
}