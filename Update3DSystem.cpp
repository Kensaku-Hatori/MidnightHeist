//****************************************************************
//
// 3D更新システムのクラスの処理[Update3DSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Update3DSystem.h"
#include "TransformComponent.hpp"
#include "TagComp.hpp"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "SizeComponent.hpp"
#include "ColorComponent.hpp"
#include "NormalComponent.hpp"
#include "UVComponent.hpp"
#include "TransformComponent.hpp"

using namespace Tag;

//*********************************************
// 更新
//*********************************************
void Update3DSystem::Update(entt::registry& reg)
{
	// ビューを生成
	auto view = reg.view<Object3D>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& Trans = reg.get<Transform3D>(entity);
		auto& VtxCmp = reg.get<VertexComponent>(entity);
		auto& SizeCmp = reg.get<SizeComponent>(entity);
		auto& ColorCmp = reg.get<ColorComponent>(entity);
		auto& NorCmp = reg.get<NorComponent>(entity);
		auto& UVCmp = reg.get<UVComponent>(entity);

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