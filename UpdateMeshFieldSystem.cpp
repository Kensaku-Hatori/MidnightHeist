//****************************************************************
//
// メッシュフィールド更新システムの処理[UpdateMeshFieldsystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateMeshFieldSystem.h"
#include "Mesh.hpp"
#include "VertexRenderingComponent.hpp"
#include "SizeComponent.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateMeshFieldSystem::Update(entt::registry& Reg)
{
	//auto view = Reg.view<MeshFieldComponent>();

	//for (auto entity : view)
	//{
	//	auto& NumVertexCmp = Reg.get<NumVertexBuffComp>(entity);
	//	auto& NumIndexCmp = Reg.get<NumIndexBufferComp>(entity);

	//	auto& VtxCmp = Reg.get<VertexComp>(entity);
	//	auto& IdxBuffCmp = Reg.get<IndexBufferComp>(entity);

	//	auto& SizeCmp = Reg.get<SizeComp>(entity);

	//	auto& DivH = Reg.get<DivitionHorizontalComp>(entity);
	//	auto& DivV = Reg.get<DivitionVerticalComp>(entity);
	//}
}