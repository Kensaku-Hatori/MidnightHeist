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
#include "UICircleComp.hpp"
#include "UICircle.h"
#include "RenderFragComp.hpp"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void RenderingUICircleSystem::Rendering(entt::registry& reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = reg.view<UICircleComponent>();

	// ワールドマトリックス
	D3DXMATRIX mtxWorld;

	// シェーダ起動
	CUICircle::Instance()->Begin();
	CUICircle::Instance()->BeginPass();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& TextureCmp = reg.get<TexComp>(entity);
		auto& UICircleCmp = reg.get<UICircleComp>(entity);
		auto& RenderFragCmp = reg.get<RenderFragComp>(entity);

		// 描画フラグが立っていなかったら
		if (RenderFragCmp.IsRendering == false) continue;

		// ワールドマトリックスを取得
		mtxWorld = TransformCmp.GetWorldMatrix();

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
		pDevice->SetTexture(0, TextureCmp.Tex);
		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxCmp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		// パラメータ設定
		D3DXMATRIX Origin = TransformCmp.GetWorldMatrix();
		CUICircle::Instance()->SetParameters(mtxWorld, Origin, UICircleCmp.FillAmount, UICircleCmp.Radius, UICircleCmp.MaxFillAngle);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	// シェーダ終了
	CUICircle::Instance()->EndPass();
	CUICircle::Instance()->End();
}