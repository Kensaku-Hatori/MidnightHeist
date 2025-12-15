//****************************************************************
//
// 円形UIの描画システムの処理[RenderingUICircleSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingUICircle.h"
#include "TagComp.hpp"
#include "manager.h"
#include "VertexRenderingComponent.hpp"
#include "texmanager.h"
#include "TextureRenderingComponent.h"
#include "UICircle.h"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void RenderingUICircleSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = reg.view<UICircleComponent>();

	D3DXMATRIX mtxWorld;

	CUICircle::Instance()->Begin();
	CUICircle::Instance()->BeginPass();

	for (auto entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& VtxComp = reg.get<VertexComp>(entity);
		auto& TextureComp = reg.get<TexComp>(entity);

		mtxWorld = TransformComp.GetWorldMatrix();

		// ビューマトリックス取得
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		mtxWorld._11 = mtxView._11;
		mtxWorld._12 = mtxView._21;
		mtxWorld._13 = mtxView._31;
		mtxWorld._21 = mtxView._12;
		mtxWorld._22 = mtxView._22;
		mtxWorld._23 = mtxView._32;
		mtxWorld._31 = mtxView._13;
		mtxWorld._32 = mtxView._23;
		mtxWorld._33 = mtxView._33;

		// テクスチャを設定
		pDevice->SetTexture(0, TextureComp.Tex);
		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		static int test = 0;
		test++;
		float testamount = fabsf(sinf(0.01f * test));
		// パラメータ設定
		D3DXMATRIX Origin = TransformComp.GetWorldMatrix();
		CUICircle::Instance()->SetParameters(mtxWorld, Origin, testamount);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	CUICircle::Instance()->EndPass();
	CUICircle::Instance()->End();
}