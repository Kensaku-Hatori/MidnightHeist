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
#include "manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingVisibleSineCurveSystem::Rendering(entt::registry& reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = reg.view<VisibleSound>();

	// シェーダー起動
	CVisibleSineCurve::Instance()->Begin();

	for (auto entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& VtxComp = reg.get<VertexComp>(entity);
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(entity);

		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		// バイアス設定
		float bias = 0.000005f + (0.0000001f * static_cast<int>(entity));
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias);
		// シェーダー起動
		CVisibleSineCurve::Instance()->SetParameters(TransformComp.mtxWorld, SineCurveCmp.nCntSineCurve, SineCurveCmp.Speed, SineCurveCmp.Ripple, SineCurveCmp.SineMacro, SineCurveCmp.Radius);
		CVisibleSineCurve::Instance()->BeginPass();
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		// シェーダー終了
		CVisibleSineCurve::Instance()->EndPass();
	}
	CVisibleSineCurve::Instance()->End();
}