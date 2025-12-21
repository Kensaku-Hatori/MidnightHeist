//****************************************************************
//
// サインカーブの描画処理[RenderingVisibleSineCurve.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateVisibleSineCurve.h"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "UVComponent.hpp"
#include "ColorComponent.hpp"
#include "VisibleSineCurveComp.hpp"
#include "TagComp.hpp"
#include "math_T.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateVisibleSineCurveSystem::Update(entt::registry& reg)
{
	// ビューを作成
	auto view = reg.view<VisibleSound>();

	// アクセス
	for (auto entity : view)
	{
		// 情報取得
		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& ColorCmp = reg.get<ColorComp>(entity);
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(entity);
		auto& UVCmp = reg.get<UVComp>(entity);

		// カウンタを進める
		SineCurveCmp.nCntSineCurve = Clamp(SineCurveCmp.nCntSineCurve + 1, 0, INT_MAX);

		VERTEX_3D* pVtx = NULL;

		if (VtxCmp.pVertex != NULL)
		{
			VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);
		}
		else return;

		if (pVtx != NULL)
		{
			pVtx[0].pos = D3DXVECTOR3(-SineCurveCmp.Radius, 0.0f, SineCurveCmp.Radius);
			pVtx[1].pos = D3DXVECTOR3(SineCurveCmp.Radius, 0.0f, SineCurveCmp.Radius);
			pVtx[2].pos = D3DXVECTOR3(-SineCurveCmp.Radius, 0.0f, -SineCurveCmp.Radius);
			pVtx[3].pos = D3DXVECTOR3(SineCurveCmp.Radius, 0.0f, -SineCurveCmp.Radius);

			pVtx[0].col = ColorCmp.Col;
			pVtx[1].col = ColorCmp.Col;
			pVtx[2].col = ColorCmp.Col;
			pVtx[3].col = ColorCmp.Col;

			pVtx[0].nor = VEC3_NULL;
			pVtx[1].nor = VEC3_NULL;
			pVtx[2].nor = VEC3_NULL;
			pVtx[3].nor = VEC3_NULL;

			pVtx[0].tex = UVCmp.UV[0];
			pVtx[1].tex = UVCmp.UV[1];
			pVtx[2].tex = UVCmp.UV[2];
			pVtx[3].tex = UVCmp.UV[3];
		}
		// 頂点バッファをアンロック
		VtxCmp.pVertex->Unlock();
	}
}