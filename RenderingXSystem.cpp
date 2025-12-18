//****************************************************************
//
// モデル描画基本システムのクラスの処理[Xrenderingbace.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "manager.h"
#include "modelmanager.h"
#include "TagComp.hpp"
#include "XRenderingComponent.hpp"
#include "TransformComponent.hpp"
#include "texmanager.h"
#include "RenderingXSystem.h"
#include "LayerManager.hpp"

using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderXSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;		// マテリアルへのポインタ

	// エンテティのリストを取得
	auto view = reg.view<ObjectXComponent>();

	for (auto entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& RenderingComp = reg.get<XRenderingComp>(entity);

		mtxWorld = TransformComp.GetWorldMatrix();

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)RenderingComp.Info.modelinfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)RenderingComp.Info.modelinfo.dwNumMat; nCntMat++)
		{
			D3DXMATERIAL pCol = pMat[nCntMat];
			// マテリアルの設定
			pDevice->SetMaterial(&pCol.MatD3D);

			if (RenderingComp.Info.modelinfo.Tex[nCntMat] != nullptr)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, RenderingComp.Info.modelinfo.Tex[nCntMat]);
			}
			else
			{
				// テクスチャの設定
				pDevice->SetTexture(0, nullptr);
			}
			// モデル(パーツ)の描画
			RenderingComp.Info.modelinfo.pMesh->DrawSubset(nCntMat);
		}
		pDevice->SetMaterial(&matDef);
	}
}