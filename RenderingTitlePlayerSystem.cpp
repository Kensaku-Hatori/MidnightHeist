//****************************************************************
//
// タイトルプレイヤー描画システムの処理[RenderingTitlePlayerSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingTitlePlayerSystem.h"
#include "XRenderingComponent.hpp"
#include "TagComp.hpp"
#include "shapeshadow.h"
#include "toon.h"
#include "shadowmap.h"

using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 描画
//*********************************************
void RenderingTitlePlayerSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;		// マテリアルへのポインタ

	// エンテティのリストを取得
	auto view = reg.view<PlayerComponent, InTitleComp>();

	// アクセス
	for (auto entity : view)
	{
		// 情報を取得
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& RenderingComp = reg.get<XRenderingComp>(entity);

		// マトリックスを取得
		mtxWorld = TransformComp.GetWorldMatrix();

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)RenderingComp.Info.modelinfo.pBuffMat->GetBufferPointer();

		D3DXMATRIX View, Proj;
		pDevice->GetTransform(D3DTS_VIEW, &View);
		pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

		CToon::Instance()->Begin();

		// マテリアル分回す
		for (int nCntMat = 0; nCntMat < (int)RenderingComp.Info.modelinfo.dwNumMat; nCntMat++)
		{
			D3DXMATERIAL pCol = pMat[nCntMat];
			// マテリアルの設定
			pDevice->SetMaterial(&pCol.MatD3D);
			D3DXVECTOR4 SettCol = { pCol.MatD3D.Diffuse.r,pCol.MatD3D.Diffuse.g,pCol.MatD3D.Diffuse.b,pCol.MatD3D.Diffuse.a };

			CToon::Instance()->SetUseShadowMapParameters(mtxWorld, View, Proj, SettCol, CShadowMap::Instance()->GetTex(), RenderingComp.Info.modelinfo.Tex[nCntMat], CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

			// テクスチャパスがあるかどうか
			if (pCol.pTextureFilename == NULL)
			{
				CToon::Instance()->BeginPass(0);
			}
			else
			{
				CToon::Instance()->BeginPass(1);
			}
			// モデル(パーツ)の描画
			RenderingComp.Info.modelinfo.pMesh->DrawSubset(nCntMat);
			CToon::Instance()->EndPass();
		}
		CToon::Instance()->End();
		// 既存のマテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}