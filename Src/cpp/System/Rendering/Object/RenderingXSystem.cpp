//****************************************************************
//
// モデル描画基本システムのクラスの処理[Xrenderingbace.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Bace/manager.h"
#include "Manager/modelmanager.h"
#include "TagComp.hpp"
#include "Component/XRenderingComponent.hpp"
#include "Component/TransformComponent.hpp"
#include "Manager/texmanager.h"
#include "System/Rendering/Obj/RenderingXSystem.h"
#include "System/LayerManager.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderXSystem::Rendering(entt::registry& Reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;		// マテリアルへのポインタ

	// エンテティのリストを取得
	auto view = Reg.view<ObjectX>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformComp = Reg.get<Transform3D>(entity);
		auto& RenderingComp = Reg.get<XRenderingComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &TransformComp.mtxWorld);
		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)RenderingComp.Info.modelinfo.pBuffMat->GetBufferPointer();

		// マテリアル分繰り返す
		for (int nCntMat = 0; nCntMat < (int)RenderingComp.Info.modelinfo.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャを使っているかどうか
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
		// マテリアルをもとに戻す
		pDevice->SetMaterial(&matDef);
	}
}