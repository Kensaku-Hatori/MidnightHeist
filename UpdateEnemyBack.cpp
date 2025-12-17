//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyBack.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "EnemyPatrolPointComp.hpp"
#include "FanInfoComponent.hpp"
#include "ParentComponent.hpp"
#include "LaserCollisionFragComp.hpp"
#include "Factories.h"
#include "mapmanager.h"
#include "math.h"
#include "math_T.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemyBackSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<EnemyComponent, EnemtAIComp>();

	// コンテナにアクセス
	for (auto [Entity, State] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (State.State != EnemyState::ENEMYSTATE::BACK) continue;

		// パトロールポイントとプレイヤーのビュー
		auto PatrolManagerview = reg.view<PatrolPointManager>();
		auto Playerview = reg.view<PlayerComponent>();

		// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
		if (PatrolManagerview.empty() || Playerview.empty()) continue;

		// 扇情報を取得
		auto& FanInfoCmp = reg.get<FanComp>(Entity);
		// Entityを取得
		auto PatrolManagerEneity = *PatrolManagerview.begin();
		auto PlayerEneity = *Playerview.begin();
		// コンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
		auto& PlayerTransformCmp = reg.get<Transform3D>(PlayerEneity);

		// 自分自身のコンポーネントを取得
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);

		// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
			CMath::IsCanSight(TransformCmp.Pos, PlayerTransformCmp.Pos, CMapManager::Instance()->GetvMapObject()) == false)
		{
			// 追いかけモード
			State.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}

		// 剛体が生成されていたら
		if (RBCmp.RigitBody == nullptr) continue;

		D3DXVECTOR3 ToDestPos = State.DestPos - TransformCmp.Pos;
		// Y成分を消す
		ToDestPos.y = 0.0f;
		// ベクトルを正規化する用の変数
		D3DXVECTOR3 Normalize;
		// 正規化
		D3DXVec3Normalize(&Normalize, &ToDestPos);

		// 目的地に着いたら
		if (D3DXVec3Length(&ToDestPos) < PatrolPointCmp.PointRadius)
		{
			// 今の位置を目標の位置にする
			State.BackIdx++;
			// フラグを立てる
			if (State.BackIdx > static_cast<int>(State.AStarRoute.size() - 1))
			{
				State.State = EnemyState::ENEMYSTATE::SEARCH;
				State.CoolDownCnt = 0;
				State.BackIdx = 0;
				continue;
			}
			else
			{
				// 目標の位置を設定
				State.DestPos = PatrolPointCmp.PatrolPoint[State.AStarRoute[State.BackIdx]].Point;
			}
		}
		else
		{
			// 移動方向を代入
			VelocityCmp.Velocity = Normalize;
			// 速さを掛ける
			VelocityCmp.Velocity *= 8;
			// Y成分を取り除く
			VelocityCmp.Velocity.y = RBCmp.RigitBody->getLinearVelocity().y();
		}
		// 移動
		UpdateMove(reg, Entity);
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemyBackSystem::UpdateMove(entt::registry& Reg, entt::entity& Entity)
{
	// 自分自身のコンポーネントを取得
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComp>(Entity);

	// 設定
	RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity));;

	// トランスフォームを取得
	btTransform trans;
	RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

	// 描画モデルの位置
	btVector3 newPos;

	// 位置を取得
	newPos = trans.getOrigin();

	// 位置を計算、設定
	TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));

	if (VelocityCmp.Velocity == VEC3_NULL) return;

	D3DXVECTOR3 VecUp = VEC_UP;
	D3DXQUATERNION SetQuat;
	// 移動値を方向ベクトルに変換
	D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

	// 方向ベクトルのZX平面上での角度を求める
	float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
	angle += D3DX_PI;

	D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);
	TransformCmp.QuatDest = SetQuat;
}