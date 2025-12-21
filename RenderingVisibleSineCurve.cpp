//****************************************************************
//
// サインカーブの描画処理[RenderingVisibleSineCurve.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingVisibleSineCurve.h"
#include "TransformComponent.hpp"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "VisibleSineCurve.h"
#include "VisibleSineCurveComp.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingVisibleSineCurveSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = reg.view<VisibleSound>();

	D3DXMATRIX mtxWorld;

	CVisibleSineCurve::Instance()->Begin();
	CVisibleSineCurve::Instance()->BeginPass();

	for (auto entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& VtxComp = reg.get<VertexComp>(entity);
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(entity);

		mtxWorld = TransformComp.GetWorldMatrix();

		CVisibleSineCurve::Instance()->SetParameters(mtxWorld, SineCurveCmp.nCntSineCurve, SineCurveCmp.Speed, SineCurveCmp.Ripple, SineCurveCmp.SineMacro);

		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	CVisibleSineCurve::Instance()->End();
	CVisibleSineCurve::Instance()->EndPass();
}