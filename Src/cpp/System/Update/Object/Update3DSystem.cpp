//================================================================
//
// 3D更新システムのクラスの処理[Update3DSystem.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "System/Update/Obj/Update3DSystem.h"
#include "Component/TransformComponent.hpp"
#include "TagComp.hpp"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/TextureRenderingComponent.h"
#include "Component/SizeComponent.hpp"
#include "Component/Polygone/ColorComponent.hpp"
#include "Component/Polygone/NormalComponent.hpp"
#include "Component/Polygone/UVComponent.hpp"
#include "Component/TransformComponent.hpp"

//****************************************************************
// ネームスペース
//****************************************************************
using namespace Tag;

//****************************************************************
// 更新
//****************************************************************
void Update3DSystem::Update(entt::registry& Reg)
{
	// ビューを生成
	auto view = Reg.view<Object3D>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& Trans = Reg.get<Transform3D>(entity);
		auto& VtxCmp = Reg.get<VertexComponent>(entity);
		auto& SizeCmp = Reg.get<SizeComponent>(entity);
		auto& ColorCmp = Reg.get<ColorComponent>(entity);
		auto& NorCmp = Reg.get<NorComponent>(entity);
		auto& UVCmp = Reg.get<UVComponent>(entity);

		VERTEX_3D* pVtx = NULL;

		if (VtxCmp.pVertex != NULL)
		{
			VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);
		}
		else return;

		if (pVtx != NULL)
		{
			pVtx[0].pos = D3DXVECTOR3(-SizeCmp.Size.x, SizeCmp.Size.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SizeCmp.Size.x, SizeCmp.Size.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-SizeCmp.Size.x, -SizeCmp.Size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SizeCmp.Size.x, -SizeCmp.Size.y, 0.0f);

			pVtx[0].col = ColorCmp.Col;
			pVtx[1].col = ColorCmp.Col;
			pVtx[2].col = ColorCmp.Col;
			pVtx[3].col = ColorCmp.Col;

			pVtx[0].nor = NorCmp.Nor;
			pVtx[1].nor = NorCmp.Nor;
			pVtx[2].nor = NorCmp.Nor;
			pVtx[3].nor = NorCmp.Nor;

			pVtx[0].tex = UVCmp.UV[0];
			pVtx[1].tex = UVCmp.UV[1];
			pVtx[2].tex = UVCmp.UV[2];
			pVtx[3].tex = UVCmp.UV[3];
		}
		// 頂点バッファをアンロック
		VtxCmp.pVertex->Unlock();
		CMath::CalcMtxWorld(&Trans.mtxWorld, Trans.Pos, Trans.Scale, Trans.Quat);
	}
}