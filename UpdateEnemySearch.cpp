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
	auto view = reg.view<Enemy, AIComponent,EnemyStateComponent>();

	// パトロールポイントとプレイヤーのビュー
	auto PatrolManagerview = reg.view<PatrolPointManager>();
	auto Playerview = reg.view<Player, InGame>();

	// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
	if (PatrolManagerview.empty()) return;

	// コンテナにアクセス
	for (auto [Entity, AICmp,StateCmp] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (StateCmp.State != EnemyState::ENEMYSTATE::SEARCH) continue;

		// 扇情報を取得
		auto& FanInfoCmp = reg.get<FanComponent>(Entity);
		// Entityを取得
		auto PatrolManagerEneity = *PatrolManagerview.begin();
		auto PlayerEneity = *Playerview.begin();
		// コンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComponent>(PatrolManagerEneity);
		auto& PlayerTransformCmp = reg.get<Transform3D>(PlayerEneity);
		auto& PlayerSoundVolumeCmp = reg.get<PlayerSoundVolumeComponent>(PlayerEneity);

		// 自分自身のコンポーネントを取得
		auto& RBCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComponent>(Entity);
		auto& EnemyListenerVolumeCmp = reg.get<EnemyListenerComponent>(Entity);

		// プレイヤーまでのベクトル
		D3DXVECTOR3 ToPlayer = PlayerTransformCmp.Pos - TransformCmp.Pos;
		// ベクトルの距離
		float Distance = D3DXVec3Length(&ToPlayer);

		// 音が聞こえていたらかつプレイヤーとの間にオブジェクトがなかったら
		if (Distance < PlayerSoundVolumeCmp.SoundVolume + EnemyListenerVolumeCmp.ListenerVolume &&
			AICmp.IsBlockedToPlayer == false)
		{
			// 見つかった回数をインクリメント
			CGame::AddEnCount();
			// 追いかけモード
			StateCmp.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}
		// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
			AICmp.IsBlockedToPlayer == false)
		{
			CGame::AddEnCount();
			// 追いかけモード
			StateCmp.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}

		// 剛体が生成されていたら
		if (RBCmp.Body == nullptr) continue;

		// 目的地へのベクトルを引く
		AICmp.NextIdx = AICmp.IsFinish ? AICmp.NowIdx - 1 : AICmp.NowIdx + 1;
		D3DXVECTOR3 ToDestPos = PatrolPointCmp.PatrolPoint[AICmp.HalfPatrolRoute[AICmp.NextIdx].Idx].Point - TransformCmp.Pos;
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
			AICmp.CoolDownCnt++;

			// クールダウンが終わったら
			if (AICmp.CoolDownCnt >= AICmp.HalfPatrolRoute[AICmp.NextIdx].CoolDown)
			{
				// リセット
				AICmp.CoolDownCnt = 0;
				// 今の位置を目標の位置にする
				AICmp.NowIdx = AICmp.IsFinish ? AICmp.NowIdx - 1 : AICmp.NowIdx + 1;
				// フラグを立てる
				if (static_cast<int>(AICmp.HalfPatrolRoute.capacity() - 1) <= AICmp.NowIdx && AICmp.IsFinish == false)
				{
					AICmp.IsFinish = true;
				}
				if (0 >= AICmp.NowIdx && AICmp.IsFinish == true)
				{
					AICmp.IsFinish = false;
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
	auto& VelocityCmp = Reg.get<VelocityComponent>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComponent>(Entity);

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