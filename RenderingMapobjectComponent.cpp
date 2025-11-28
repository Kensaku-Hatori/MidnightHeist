//****************************************************************
//
// マップオブジェクト描画システムの処理[RenderingMapobjectSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingMapobjectComponent.h"
#include "XRenderingComponent.hpp"
#include "TagComp.hpp"
#include "shadowmap.h"
#include "toon.h"

using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingMapobjectSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<MapObjectComponent>();

	// シャドウマップへの書き込みを開始
	CShadowMap::Instance()->Begin();
	CShadowMap::Instance()->WriteMaps();
	for (auto entity : view)
	{
		DrawShadowMap(reg, entity);
	}
	// シャドウマップへの書き込みを終了
	CShadowMap::Instance()->EndMaps();
	CShadowMap::Instance()->End();

	// 影を付けて描画
	CToon::Instance()->Begin();
	for (auto entity : view)
	{
		DrawUseShadowMap(reg, entity);
	}
	CToon::Instance()->End();
}

//*********************************************
// シャドウマップへの書き込み
//*********************************************
void RenderingMapobjectSystem::DrawShadowMap(entt::registry& Reg, entt::entity Entity)
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

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CShadowMap::Instance()->SetParameters(mtxWorld);

		CShadowMap::Instance()->BeginPass(0);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShadowMap::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}

//*********************************************
// シャドウマップを使って描画
//*********************************************
void RenderingMapobjectSystem::DrawUseShadowMap(entt::registry& Reg, entt::entity Entity)
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

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { 1.0f,1.0f,1.0f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), CLoadTexture::GetTex(modelinfo.modelinfo.TexPath[nCntMat]), CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		if (col.pTextureFilename == NULL)
		{
			CToon::Instance()->BeginPass(0);
		}
		else
		{
			CToon::Instance()->BeginPass(1);
		}

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}