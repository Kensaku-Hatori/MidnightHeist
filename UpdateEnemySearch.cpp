//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySearch.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "EnemyPatrolPointComp.hpp"
#include "FanInfoComponent.hpp"
#include "ParentComponent.hpp"
#include "LaserCollisionFragComp.hpp"
#include "PlayerSoundVolumeComp.hpp"
#include "mapmanager.h"
#include "EnemySoundListener.hpp"
#include "math.h"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemySearchSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<EnemyComponent, EnemyAIComp>();

	// コンテナにアクセス
	for (auto [Entity, State] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (State.State != EnemyState::ENEMYSTATE::SEARCH) continue;

		// パトロールポイントとプレイヤーのビュー
		auto PatrolManagerview = reg.view<PatrolPointManager>();
		auto Playerview = reg.view<PlayerComponent,InGameComp>();

		// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
		if (PatrolManagerview.empty()) continue;

		// 扇情報を取得
		auto& FanInfoCmp = reg.get<FanComp>(Entity);
		// Entityを取得
		auto PatrolManagerEneity = *PatrolManagerview.begin();
		auto PlayerEneity = *Playerview.begin();
		// コンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
		auto& PlayerTransformCmp = reg.get<Transform3D>(PlayerEneity);
		auto& PlayerSoundVolumeCmp = reg.get<PlayerSoundVolumeComp>(PlayerEneity);

		// 自分自身のコンポーネントを取得
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);
		auto& EnemyListenerVolumeCmp = reg.get<EnemyListenerComp>(Entity);

		D3DXVECTOR3 ToPlayer = PlayerTransformCmp.Pos - TransformCmp.Pos;
		float Distance = D3DXVec3Length(&ToPlayer);

		if (Distance < PlayerSoundVolumeCmp.SoundVolume + EnemyListenerVolumeCmp.ListenerVolume)
		{
			// 追いかけモード
			State.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}
		// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
			CMath::IsCanSight(TransformCmp.Pos, PlayerTransformCmp.Pos, CMapManager::Instance()->GetvMapObject()) == true)
		{
			// 追いかけモード
			State.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}

		// 剛体が生成されていたら
		if (RBCmp.RigitBody == nullptr) continue;

		// 目的地へのベクトルを引く
		State.NextIdx = State.IsFinish ? State.NowIdx - 1 : State.NowIdx + 1;
		D3DXVECTOR3 ToDestPos = PatrolPointCmp.PatrolPoint[State.HalfPatrolRoute[State.NextIdx].Idx].Point - TransformCmp.Pos;
		// Y成分を消す
		ToDestPos.y = 0.0f;
		// ベクトルを正規化する用の変数
		D3DXVECTOR3 Normalize;
		// 正規化
		D3DXVec3Normalize(&Normalize, &ToDestPos);

		// 目的地に着いたら
		if (D3DXVec3Length(&ToDestPos) < PatrolPointCmp.PointRadius)
		{
			// 移動量を無くす
			VelocityCmp = VEC3_NULL;
			// カウンタを進める
			State.CoolDownCnt++;

			// クールダウンが終わったら
			if (State.CoolDownCnt >= State.HalfPatrolRoute[State.NextIdx].CoolDown)
			{
				// リセット
				State.CoolDownCnt = 0;
				// 今の位置を目標の位置にする
				State.NowIdx = State.IsFinish ? State.NowIdx - 1 : State.NowIdx + 1;
				// フラグを立てる
				if (static_cast<int>(State.HalfPatrolRoute.capacity() - 1) <= State.NowIdx && State.IsFinish == false)
				{
					State.IsFinish = true;
				}
				if (0 >= State.NowIdx && State.IsFinish == true)
				{
					State.IsFinish = false;
				}
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
// A*の更新
//*********************************************
void UpdateEnemySearchSystem::UpdateAStar(entt::registry& Reg, entt::entity Entity)
{
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemySearchSystem::UpdateMove(entt::registry& Reg, entt::entity Entity)
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