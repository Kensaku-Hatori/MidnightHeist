//****************************************************************
//
// 敵オブジェクト描画システムの処理[RenderingEnemySystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingEnemySystem.h"
#include "TagComp.hpp"
#include "shadowmap.h"
#include "shapeshadow.h"
#include "TransformComponent.hpp"
#include "XRenderingComponent.hpp"
#include "shadowmap.h"
#include "toon.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingEnemySystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<EnemyComponent>();

	CShapeShadow::Instance()->Begin();
	CShapeShadow::Instance()->BeginScene();
	for (auto entity : view)
	{
		DrawShapeShadowMap(reg, entity);
	}
	CShapeShadow::Instance()->EndTexs();
	CShapeShadow::Instance()->End();

	for (auto Entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(Entity);
		auto& RenderingComp = reg.get<XRenderingComp>(Entity);

		CRenderer* pRenderer;
		pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		D3DXMATRIX mtxWorld;						// 計算用マトリックス
		D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
		D3DXMATERIAL* pMat;							// マテリアルへのポインタ

		mtxWorld = TransformComp.GetWorldMatrix();

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		D3DXMATRIX View, Proj;
		pDevice->GetTransform(D3DTS_VIEW, &View);
		pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)RenderingComp.Info.modelinfo.pBuffMat->GetBufferPointer();

		CToon::Instance()->Begin();

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
		pDevice->SetMaterial(&matDef);
	}
}

//*********************************************
// 物陰として描画
//*********************************************
void RenderingEnemySystem::DrawShapeShadowMap(entt::registry& Reg, entt::entity Entity)
{
	auto& TransformComp = Reg.get<Transform3D>(Entity);
	auto& RenderingComp = Reg.get<XRenderingComp>(Entity);

	// モデルへのインデックスが-1だったら終わる
	if (RenderingComp.FilePath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;						// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = RenderingComp.Info;

	mtxWorld = TransformComp.GetWorldMatrix();

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.Tex.size(); nCntMat++)
	{
		CShapeShadow::Instance()->SetParameters(mtxWorld);

		CShapeShadow::Instance()->BeginPass(1);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShapeShadow::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}