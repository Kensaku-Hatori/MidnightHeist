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
#include "manager.h"
#include "scene.h"

//*********************************************
// コンストラクタ
//*********************************************
UpdateMtxSystem::UpdateMtxSystem()
{
	entt::registry& Reg = CManager::GetScene()->GetReg();
	Reg.on_construct<Transform3D>().connect<&UpdateMtxSystem::OnTransformComponentConstruct>(this);
}

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
			D3DXMatrixMultiply(&TransformCmp.mtxWorld, &TransformCmp.mtxWorld, &ParentTrans.mtxWorld);
		}
	}
}

//*********************************************
// トランスフォームが追加されたら
//*********************************************
void UpdateMtxSystem::OnTransformComponentConstruct(entt::registry& Reg, entt::entity Entity)
{
	// ビューを生成
	auto TransformView = Reg.view<Transform3D>();
	auto ParentView = Reg.view<ParentComp, Transform3D>();

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

	if (Reg.any_of<ParentComp>(Entity))
	{
		// コンポーネントを取得
		auto& ParentCmp = Reg.get<ParentComp>(Entity);
		auto& TransformCmp = Reg.get<Transform3D>(Entity);
		// 有効だったら
		if (Reg.valid(ParentCmp.Parent) == true)
		{
			// 親のコンポーネントを取得
			auto& ParentTrans = Reg.get<Transform3D>(ParentCmp.Parent);

			// マトリックスを計算
			D3DXMatrixIdentity(&ParentTrans.mtxWorld);

			D3DXMatrixScaling(&ParentTrans.mtxScale, ParentTrans.Scale.x, ParentTrans.Scale.y, ParentTrans.Scale.z);
			D3DXMatrixMultiply(&ParentTrans.mtxWorld, &ParentTrans.mtxWorld, &ParentTrans.mtxScale);

			D3DXMatrixRotationQuaternion(&ParentTrans.mtxBasis, &ParentTrans.Quat);
			D3DXMatrixMultiply(&ParentTrans.mtxWorld, &ParentTrans.mtxWorld, &ParentTrans.mtxBasis);

			D3DXMatrixTranslation(&ParentTrans.mtxTrans, ParentTrans.Pos.x, ParentTrans.Pos.y, ParentTrans.Pos.z);
			D3DXMatrixMultiply(&ParentTrans.mtxWorld, &ParentTrans.mtxWorld, &ParentTrans.mtxTrans);

			// かけ合わせる
			D3DXMatrixMultiply(&TransformCmp.mtxWorld, &TransformCmp.mtxWorld, &ParentTrans.mtxWorld);
		}
	}
}