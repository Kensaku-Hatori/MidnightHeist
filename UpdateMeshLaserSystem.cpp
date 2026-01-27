//****************************************************************
//
// メッシュレーザーの更新処理[UpdateMeshLaserSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "ParentComponent.hpp"
#include "UpdateMeshLaserSystem.h"
#include "XRenderingComponent.hpp"
#include "SizeComponent.hpp"
#include "input.h"
#include "MeshInfoComponent.hpp"
#include "VertexRenderingComponent.hpp"
#include "DivisionComponent.hpp"
#include "fade.h"
#include "Result.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateMeshLaserSystem::Update(entt::registry& Reg)
{
	// レーザのビューを生成
	auto view = Reg.view<LaserComponent>();
	// マップオブジェクトのビューを生成
	auto viewMapObject = Reg.view<MapObjectComponent>();

	// レーザーにアクセス
	for (auto entity : view)
	{
		// 親がいなかったら
		if (Reg.get<ParentComp>(entity).Parent == entt::null) continue;
		// コンポーネントを取得
		auto& SizeCmp = Reg.get<SizeComp>(entity);

		// モデルとマウスの当たり判定用の距離
		float Distance, CurrentDistance;

		// 現在の距離を10000.0fにする(ソートするため)
		Distance = 100.0f;
		const float DefaultDist = Distance;
		float ToPlayer = -1;
		// 昔の距離を初期化
		CurrentDistance = 0.0f;

		auto PlayerView = Reg.view<PlayerComponent>();
		// プレイヤーが存在したら
		if (!PlayerView.empty() && CManager::GetScene()->GetMode() == CScene::MODE_GAME)
		{
			// Entityを取得
			auto playerEntity = *PlayerView.begin();
			// 当たったら
			if (CollisionEntity(Reg, entity, playerEntity, ToPlayer) == true && ToPlayer < Distance && CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(new CResult);
			}
		}

		// マップオブジェクトにアクセス
		for (auto entityMapObject : viewMapObject)
		{
			// 当たったら
			if (CollisionEntity(Reg, entity, entityMapObject, CurrentDistance) == true)
			{
				// 最新の距離と当たったオブジェクトとの距離を比較
				if (CurrentDistance < Distance)
				{
					// 最新の距離を更新
					Distance = CurrentDistance;
				}
			}
		}
		// 情報を設定
		SizeCmp.Size.y = Distance;
		// ちょうてんじょうほうを更新
		UpdateVertex(Reg, entity);
	}
}

//*********************************************
// 頂点情報の更新
//*********************************************
void UpdateMeshLaserSystem::UpdateVertex(entt::registry& Reg, entt::entity Entity)
{
	// コンポーネントを取得
	auto& VtxCmp = Reg.get<VertexComp>(Entity);
	auto& DivCmp = Reg.get<DivisionComp>(Entity);
	auto& SizeCmp = Reg.get<SizeComp>(Entity);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// スタートの高さ
	float StartHeight = 0.0f;

	// 高さを下げる割合
	float HeightRatio = SizeCmp.Size.y / DivCmp.nDivHorizontal;

	// 角度の割合
	float fRatio = (D3DX_PI * 2) / DivCmp.nDivVertical;

	//頂点情報の設定
	for (int nCntZ = 0; nCntZ <= DivCmp.nDivHorizontal; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= DivCmp.nDivVertical; nCntX++)
		{
			//頂点座標の設定
			pVtx[nCntVtx].pos.x = sinf(fRatio * nCntX) * SizeCmp.Size.x;
			pVtx[nCntVtx].pos.y = cosf(fRatio * nCntX) * SizeCmp.Size.x;
			pVtx[nCntVtx].pos.z = StartHeight - (HeightRatio * nCntZ);

			// 法線を計算
			D3DXVECTOR3 CenterVec = D3DXVECTOR3(-pVtx[nCntVtx].pos.x, -pVtx[nCntVtx].pos.y, 0.0f);
			D3DXVec3Normalize(&CenterVec, &CenterVec);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = CenterVec;

			//頂点カラーの設定
			pVtx[nCntVtx].col = RED;

			float TexX = 1.0f / (DivCmp.nDivVertical);
			float TexY = 1.0f / DivCmp.nDivHorizontal;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((float)TexX * nCntX, (float)TexY * nCntZ);

			nCntVtx++;
		}
	}
	//頂点バッファをアンロック
	VtxCmp.pVertex->Unlock();
}

//*********************************************
// ほかのエンティティとの当たり判定
//*********************************************
bool UpdateMeshLaserSystem::CollisionEntity(entt::registry& Reg, entt::entity Entity, entt::entity ToEntity, float& Distance)
{
	// マップオブジェクトのコンポーネントを取得
	auto& TransformCmp = Reg.get<Transform3D>(ToEntity);
	auto& ModelInfo = Reg.get<XRenderingComp>(ToEntity);

	// 自分自身のコンポーネントを取得
	auto& LaserTrans = Reg.get<Transform3D>(Entity);
	auto& Parent = Reg.get<ParentComp>(Entity);
	auto& EnemyTranf = Reg.get<Transform3D>(Parent.Parent);

	// 親のワールドマトリックスとかけ合わせたマトリックス
	D3DXMATRIX Mulmtx = CMath::CalcMultiplyMtxWorld(LaserTrans.Pos, LaserTrans.Scale, LaserTrans.Quat, EnemyTranf.mtxWorld);

	// モデルのマトリックスの逆行列
	D3DXMATRIX invWorld = CMath::CalcInverseMtxWorld(TransformCmp.Pos, TransformCmp.Scale, TransformCmp.Quat);

	// レイの始点と向きのローカル変数
	D3DXVECTOR3 localRayOrigin, localRayDir;

	// マウスのレイを取得
	D3DXVECTOR3 LaserPos = { Mulmtx._41 ,Mulmtx._42 ,Mulmtx._43 };
	D3DXVECTOR3 VecFront = { Mulmtx._31 ,Mulmtx._32 ,Mulmtx._33 };

	CInputMouse::Ray LocalRay;
	LocalRay.Origin = LaserPos;
	LocalRay.Dir = -VecFront;

	// 始点を位置としてマトリックスで変換
	D3DXVec3TransformCoord(&localRayOrigin, &LocalRay.Origin, &invWorld);

	// レイの向きを方向ベクトルとしてマトリックスで変換
	D3DXVec3TransformNormal(&localRayDir, &LocalRay.Dir, &invWorld);

	// 当たったかどうかの一時変数
	BOOL hit;

	// 当たり判定を実行
	D3DXIntersect(ModelInfo.Info.modelinfo.pMesh, &localRayOrigin, &localRayDir, &hit, NULL, NULL, NULL, &Distance, NULL, NULL);

	// 当たったかどうかを返す
	return hit;
}