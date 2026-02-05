//****************************************************************
//
// 円形UIの更新処理[UpdateUICircleSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateUICircleSystem.hpp"
#include "TagComp.hpp"
#include "VertexRenderingComponent.hpp"
#include "SizeComponent.hpp"
#include "ColorComponent.hpp"
#include "NormalComponent.hpp"
#include "UVComponent.hpp"
#include "UICircleComponent.hpp"
#include "math_T.h"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void UpdateUICircleSystem::Update(entt::registry& reg)
{
	// ビュー作成
	auto view = reg.view<UICircle>();

	// アクセス
	for (auto entity : view)
	{
		// 情報を取得
		auto& VtxCmp = reg.get<VertexComponent>(entity);
		auto& SizeCmp = reg.get<SizeComponent>(entity);
		auto& ColorCmp = reg.get<ColorComponent>(entity);
		auto& NorCmp = reg.get<NorComponent>(entity);
		auto& UVCmp = reg.get<UVComponent>(entity);
		auto& UICircleCmp = reg.get<UICircleComponent>(entity);

		// 塗りつぶし量をクランプ
		UICircleCmp.FillAmount = Clamp(UICircleCmp.FillAmount, 0.0f, 1.0f);

		// 頂点バッファを編集する用変数
		VERTEX_3D* pVtx = NULL;

		// 頂点バッファが生成されていたら
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

	}
}