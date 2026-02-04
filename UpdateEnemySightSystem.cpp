//****************************************************************
//
// 敵の視界更新システムの処理[UpdateEnemySightSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySightSystem.h"
#include "TagComp.hpp"
#include "VertexRenderingComponent.hpp"
#include "ColorComponent.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemySightSystem::Update(entt::registry& reg)
{
	// ビュー生成
	auto view = reg.view<SightFanComponent>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& ColorCmp = reg.get<ColorComp>(entity);

		VERTEX_3D* pVtx = NULL;

		if (VtxCmp.pVertex != NULL)
		{
			VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);
		}
		else continue;

		if (pVtx != NULL)
		{
			pVtx[0].col = ColorCmp.Col;
			pVtx[1].col = D3DXCOLOR(ColorCmp.Col.r, ColorCmp.Col.g, ColorCmp.Col.b, 0.0f);
			pVtx[2].col = D3DXCOLOR(ColorCmp.Col.r, ColorCmp.Col.g, ColorCmp.Col.b, 0.0f);
		}
		// 頂点バッファをアンロック
		VtxCmp.pVertex->Unlock();
	}
}