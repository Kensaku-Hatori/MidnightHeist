//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyChase.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "LaserCollisionFragComp.hpp"
#include "ParentComponent.hpp"
#include "EnemyPatrolPointComp.hpp"
#include "RayComponent.hpp"
#include "math.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemyChaseSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<EnemyComponent, EnemtAIComp>();

	// プレイヤーのビューを取得
	auto PlayerView = reg.view<PlayerComponent>();
	auto PatrolManagerview = reg.view<PatrolPointManager>();

	// プレイヤーが存在しないまたはパトロールポイントマネージャーが存在しなかったらスキップ
	if (PlayerView.empty() || PatrolManagerview.empty()) return;

	// Entityを取得
	auto PatrolManagerEneity = *PatrolManagerview.begin();
	auto playerEntity = *PlayerView.begin();

	// コンテナにアクセス
	for (auto [Entity,State] : view.each())
	{
		// 敵がCHASE以外の状態ならスキップ
		if (State.State != EnemyState::ENEMYSTATE::CHASE) continue;

		// プレイヤーとパトロールポイントマネージャーのコンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
		auto& PlayerTransCmp = reg.get<Transform3D>(playerEntity);

		// 自分のコンポーネントを取得
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& Laser = reg.get<SingleParentComp>(Entity);
		// レーザーのコリジョンフラグを取得
		auto& CollisionInfo = reg.get<LaserCollisionInfoComp>(Laser.Parent);

		// プレイヤーとの間にオブジェクトがあったら
		if (CollisionInfo.IsRayCollision == true)
		{
			// 捜索ステートにする
			State.State = EnemyState::ENEMYSTATE::SEARCH;
			// 最後に見たプレイヤーの位置を保存
			State.LastLookPlayerPosition = PlayerTransCmp.Pos;

			// マップオブジェクトのビュー
			auto MapObjectView = reg.view<MapObjectComponent>();

			// ソートするための比較用変数
			float CurrentDistance = 100000.0f;

			// 一番近くの障害物をまたがないポイントへのIdx
			int BestPoint = -1;

			// パトロールポイントへアクセス
			for (int nCnt = 0; nCnt < static_cast<int>(PatrolPointCmp.PatrolPoint.size()); nCnt++)
			{
				// ポイントへのレイ
				RayComp ToPointRay;
				// 例の向きを正規化するようの変数
				D3DXVECTOR3 NormalizeToPointVec;
				// ベクトルを引く
				D3DXVECTOR3 ToPointVec = PatrolPointCmp.PatrolPoint[nCnt].Point - TransformCmp.Pos;
				// Y成分を無くす
				ToPointVec.y = 0.0f;
				// 正規化した結果を代入
				D3DXVec3Normalize(&NormalizeToPointVec, &ToPointVec);

				// レイの位置と向きを設定
				ToPointRay.Origin = TransformCmp.Pos;
				ToPointRay.Dir = NormalizeToPointVec;

				// 次に判定をとるオブジェクトへの距離
				float NowDistance = 0.0f;

				// 当たったかどうか
				bool CantMove = false;

				// マップオブジェクトへアクセス
				for (auto MapObject : MapObjectView)
				{
					// 当たり判定に必要なコンポーネントを取得
					auto& XRenderingCmp = reg.get<XRenderingComp>(MapObject);
					auto& MapObjectTransCmp = reg.get<Transform3D>(MapObject);

					// 当たったら
					if (CMath::IsMeshOnTheRay(XRenderingCmp.Info.modelinfo.pMesh, MapObjectTransCmp.GetWorldMatrix(), ToPointRay, &NowDistance) == true)
					{
						// 距離が現在位置からポイントへの距離より多かったら障害物があると判定
						float dist = D3DXVec3Length(&ToPointVec);
						if (dist > NowDistance)
						{
							// そのポイントへの移動は不可
							CantMove = true;
							// 切り上げ
							break;
						}
					}
				}
				// 移動可能なポイントだったら
				if (!CantMove)
				{
					// 距離を計算
					float dist = D3DXVec3Length(&ToPointVec);
					// ソート
					if (dist < CurrentDistance)
					{
						CurrentDistance = dist;
						// Idxを記録
						BestPoint = nCnt;
					}
				}
			}
			// 初期値から動いていたら
			if (BestPoint != -1)
			{
				// 目的地を設定
				State.NowPatrolPoint = BestPoint;
				State.DestPatrolPoint = BestPoint;
			}
			// ステートを切り替えたので切り上げ
			continue;
		}
		UpdateMove(reg, Entity, playerEntity);
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemyChaseSystem::UpdateMove(entt::registry& Reg, entt::entity& Entity, entt::entity& PlayerEntity)
{
	// 自分のコンポーネントを取得
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComp>(Entity);
	// プレイヤーのコンポーネントを取得
	auto& PlayerTransCmp = Reg.get<Transform3D>(PlayerEntity);

	if (RBCmp.RigitBody != nullptr)
	{
		D3DXVECTOR3 ToPlayer = PlayerTransCmp.Pos - TransformCmp.Pos;
		D3DXVec3Normalize(&ToPlayer, &ToPlayer);
		VelocityCmp.Velocity = { ToPlayer.x, ToPlayer.y, ToPlayer.z };
		VelocityCmp.Velocity.y = RBCmp.RigitBody->getLinearVelocity().y();
		// 設定
		RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity * 9));

		// トランスフォームを取得
		btTransform trans;
		RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

		// 描画モデルの位置
		btVector3 newPos;

		// 位置を取得
		newPos = trans.getOrigin();

		D3DXVECTOR3 Axis = VelocityCmp.Velocity;
		D3DXVECTOR3 VecFront = { 0.0f,0.0f,-1.0f };
		D3DXVECTOR3 VecUp = VEC_UP;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&TransformCmp.Quat, &VecUp, angle);

		// 位置を計算、設定
		TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));
	}
}