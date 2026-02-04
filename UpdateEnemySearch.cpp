//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySearch.h"
#include "EnemyAIUtils.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateEnemySearchSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<EnemyComponent, EnemyAIComp>();

	// パトロールポイントとプレイヤーのビュー
	auto PatrolManagerview = reg.view<PatrolPointManager>();
	auto Playerview = reg.view<PlayerComponent, InGameComp>();

	// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
	if (PatrolManagerview.empty()) return;

	// コンテナにアクセス
	for (auto [Entity, State] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (State.State != EnemyState::ENEMYSTATE::SEARCH) continue;

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
		auto& RBCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);
		auto& EnemyListenerVolumeCmp = reg.get<EnemyListenerComp>(Entity);

		// プレイヤーまでのベクトル
		D3DXVECTOR3 ToPlayer = PlayerTransformCmp.Pos - TransformCmp.Pos;
		// ベクトルの距離
		float Distance = D3DXVec3Length(&ToPlayer);

		//// 音が聞こえていたらかつプレイヤーとの間にオブジェクトがなかったら
		//if (Distance < PlayerSoundVolumeCmp.SoundVolume + EnemyListenerVolumeCmp.ListenerVolume &&
		//	State.IsBlockedToPlayer == false)
		//{
		//	// 見つかった回数をインクリメント
		//	CGame::AddEnCount();
		//	// 追いかけモード
		//	State.State = EnemyState::ENEMYSTATE::CHASE;
		//	continue;
		//}
		//// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		//if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
		//	State.IsBlockedToPlayer == false)
		//{
		//	CGame::AddEnCount();
		//	// 追いかけモード
		//	State.State = EnemyState::ENEMYSTATE::CHASE;
		//	continue;
		//}

		// 剛体が生成されていたら
		if (RBCmp.Body == nullptr) continue;

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
			VelocityCmp.Velocity.y = RBCmp.Body->getLinearVelocity().y();
		}
		// 移動
		UpdateMove(reg, Entity);
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemySearchSystem::UpdateMove(entt::registry& Reg, entt::entity Entity)
{
	// 自分自身のコンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComp>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComp>(Entity);

	// 設定
	RBCmp.Body->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity));;

	// 移動量がなかったら
	if (VelocityCmp.Velocity == VEC3_NULL) return;

	// クォータニオンを求める
	// 真上ベクトル
	D3DXVECTOR3 VecUp = VEC_UP;
	// 設定用の変数
	D3DXQUATERNION SetQuat;
	// 移動値を方向ベクトルに変換
	D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

	// 方向ベクトルのZX平面上での角度を求める
	float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
	angle += D3DX_PI;

	// クォータニオンを求める
	D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);
	// 設定
	CharactorCmp.QuatDest = SetQuat;
}