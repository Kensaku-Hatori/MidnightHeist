//****************************************************************
//
// ポーズメニューの更新処理[UpdatePauseMenu.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdatePauseMenu.h"
#include "manager.h"
#include "TagComp.hpp"
#include "ColorComponent.hpp"
#include "RenderingPauseMenu.h"
#include "VertexRenderingComponent.hpp"
#include "SizeComponent.hpp"
#include "UVComponent.hpp"
#include "math_T.h"
#include "TransformComponent.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdatePauseMenuSystem::Update(entt::registry& reg)
{
	// ビューを生成
	auto view = reg.view<PauseMenuComponent>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& TransformCmp = reg.get<Transform2D>(entity);
		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& SizeCmp = reg.get<SizeComp>(entity);
		auto& ColorCmp = reg.get<ColorComp>(entity);
		auto& UVCmp = reg.get<UVComp>(entity);

		VERTEX_2D* pVtx = NULL;

		if (VtxCmp.pVertex != NULL)
		{
			VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);
		}
		else return;
		if (pVtx != NULL)
		{
			//頂点座標の更新
			pVtx[0].pos.x = TransformCmp.Pos.x - (SizeCmp.Size.x * TransformCmp.Scale.x);
			pVtx[0].pos.y = TransformCmp.Pos.y - (SizeCmp.Size.y * TransformCmp.Scale.y);
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = TransformCmp.Pos.x + (SizeCmp.Size.x * TransformCmp.Scale.x);
			pVtx[1].pos.y = TransformCmp.Pos.y - (SizeCmp.Size.y * TransformCmp.Scale.y);
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = TransformCmp.Pos.x - (SizeCmp.Size.x * TransformCmp.Scale.x);
			pVtx[2].pos.y = TransformCmp.Pos.y + (SizeCmp.Size.y * TransformCmp.Scale.y);
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = TransformCmp.Pos.x + (SizeCmp.Size.x * TransformCmp.Scale.x);
			pVtx[3].pos.y = TransformCmp.Pos.y + (SizeCmp.Size.y * TransformCmp.Scale.y);
			pVtx[3].pos.z = 0.0f;

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = ColorCmp.Col;
			pVtx[1].col = ColorCmp.Col;
			pVtx[2].col = ColorCmp.Col;
			pVtx[3].col = ColorCmp.Col;

			pVtx[0].tex = UVCmp.UV[0];
			pVtx[1].tex = UVCmp.UV[1];
			pVtx[2].tex = UVCmp.UV[2];
			pVtx[3].tex = UVCmp.UV[3];
		}
		VtxCmp.pVertex->Unlock();
	}
}