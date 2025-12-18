//****************************************************************
//
// プレイヤー描画システムの処理[RenderingPlayerSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

#include "RenderingGamePlayerSystem.h"
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
void RenderingGameSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;		// マテリアルへのポインタ

	// エンテティのリストを取得
	auto view = reg.view<PlayerComponent,InGameComp>();

	// 物陰マップに書き込む
	CShapeShadow::Instance()->Begin();
	CShapeShadow::Instance()->BeginScene();
	// アクセス
	for (auto Entity : view)
	{
		// 描画
		RenderingShape(reg, Entity);
	}
	// 書き込みを終了する
	CShapeShadow::Instance()->EndTexs();
	CShapeShadow::Instance()->End();

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

//*********************************************
// プレイヤーのシルエット描画
//*********************************************
void RenderingGameSystem::RenderingShape(entt::registry& Reg, entt::entity Entity)
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