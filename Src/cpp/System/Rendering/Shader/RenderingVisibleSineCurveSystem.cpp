//****************************************************************
//
// サインカーブの描画処理[RenderingVisibleSineCurve.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "System/Rendering/Shader/RenderingVisibleSineCurveSystem.h"
#include "Component/TransformComponent.hpp"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/TextureRenderingComponent.h"
#include "Shader/VisibleSineCurve.h"
#include "Component/Shader/Ripple3DComponent.hpp"
#include "TagComp.hpp"
#include "Bace/manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingVisibleSineCurveSystem::Rendering(entt::registry& Reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = Reg.view<VisibleSound>();

	// シェーダー起動
	CVisibleSineCurve::Instance().Begin();

	for (auto entity : view)
	{
		auto& TransformComp = Reg.get<Transform3D>(entity);
		auto& VtxComp = Reg.get<VertexComponent>(entity);
		auto& SineCurveCmp = Reg.get<Ripple3DComponent>(entity);

		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		// バイアス設定
		float bias = 0.000005f + (0.0000001f * static_cast<int>(entity));
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias);
		// シェーダー起動
		CVisibleSineCurve::Instance().SetParameters(TransformComp.mtxWorld, SineCurveCmp.nCntSineCurve, SineCurveCmp.Speed, SineCurveCmp.Ripple, SineCurveCmp.SineMacro, SineCurveCmp.Radius);
		CVisibleSineCurve::Instance().BeginPass();
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		// シェーダー終了
		CVisibleSineCurve::Instance().EndPass();
	}
	CVisibleSineCurve::Instance().End();
}