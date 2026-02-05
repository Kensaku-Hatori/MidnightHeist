//****************************************************************
//
// 敵の更新処理[UpdateEnemySystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySystem.h"
#include "EnemyAIUtils.hpp"
#include "ColorComponent.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateEnemySystem::Update(entt::registry& reg)
{
	// ビュー取得
	auto view = reg.view<EnemyComponent>();

	// アクセス
	for (auto entity : view)
	{
		// 情報を取得
		auto& Transform = reg.get<Transform3D>(entity);
		auto& FanInfoCmp = reg.get<FanComp>(entity);
		auto& ChildrenCmp = reg.get<ChildrenComp>(entity);
		auto& EnemyAiCmp = reg.get<EnemyAIComp>(entity);
		auto& SoundCmp = reg.get<EnemyListenerComp>(entity);
		auto& StateCmp = reg.get<EnemyAIComp>(entity);
		auto& ChractorCmp = reg.get<CharactorComp>(entity);
		auto& SightColorCmp = reg.get<ColorComp>(ChildrenCmp.Children[1]);
		
		switch (StateCmp.State)
		{
		case EnemyState::ENEMYSTATE::SEARCH:
			SightColorCmp.Col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		default:
			SightColorCmp.Col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
		// プレイヤーが発する音
		SoundCmp.ListenerVolume = EnemyListenerConfig::Bace * EnemyListenerConfig::Scale[static_cast<int>(StateCmp.State)];

		// マトリックスを取得
		D3DXMATRIX myMatrix = Transform.mtxWorld;
		// 扇の位置と向きを設定
		FanInfoCmp.Origin = { myMatrix._41,myMatrix._42,myMatrix._43 };
		FanInfoCmp.Dir = { -myMatrix._31,-myMatrix._32,-myMatrix._33 };

		// エミッターの情報を設定
		EnemyAiCmp.Emitter->SetPos(Transform.Pos);
		EnemyAiCmp.Emitter->SetFront({ -myMatrix._31,-myMatrix._32,-myMatrix._33 });
		EnemyAiCmp.Emitter->Update();

		// 向いている方向を計算
		D3DXVECTOR3 VecUp = VEC_UP;
		D3DXVECTOR3 Dir;
		D3DXQUATERNION SetQuat;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&Dir, &FanInfoCmp.Dir);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(Dir.x, Dir.z);
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);

		// 自身が立てている音の大きさを子往診
		auto& SineCurveCmp = reg.get<VisibleSineCurveComp>(ChildrenCmp.Children[2]);

		SineCurveCmp.Radius = SoundCmp.ListenerVolume;

		// 目標の位置まで補完
		D3DXQuaternionSlerp(&Transform.Quat, &Transform.Quat, &ChractorCmp.QuatDest, ChractorCmp.QuatSpeed);

		// 光線たちの更新
		UpdateRays(reg, entity);
	}
}

//*********************************************
// 剛体更新
//*********************************************
void UpdateEnemySystem::UpdateRB(entt::registry& Reg, entt::entity& Entity)
{
	// 剛体情報を取得
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	// 生成されていなかったら
	if (RBCmp.RigitBody != nullptr) return;

	// 自分の情報を取得
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& ColliderCmp = Reg.get <SingleCollisionShapeComp>(Entity);

	// 当たり判定を生成
	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(15.0f), btScalar(20.0f));

	// 質量を設定する用の変数
	btScalar mass = 1.0f;
	// 回転抗力
	btVector3 inertia(0, 0, 0);
	// 当たり判定に情報を渡す
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	// 剛体のトランスフォームを設定する用の変数
	btTransform transform;
	transform.setIdentity();
	// 原点を設定
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + 20.0f, TransformCmp.Pos.z));

	// モーションステートを生成
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	// 剛体生成時に必要な情報
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	// 生成
	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);

	// 移動制限を設定
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	RBCmp.RigitBody->setUserPointer(this);
	// 剛体の状態を設定
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界に追加
	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_ENEMY, CollisionGroupAndMasks::MASK_ENEMY);
}

//*********************************************
// 光線の更新
//*********************************************
void UpdateEnemySystem::UpdateRays(entt::registry& Reg, entt::entity& Entity)
{
	UpdateToPlayerRay(Reg, Entity);
}

//*********************************************
// プレイヤーまでの光線の更新
//*********************************************
void UpdateEnemySystem::UpdateToPlayerRay(entt::registry& Reg, entt::entity& Entity)
{
	// プレイヤーのビュー
	auto Playerview = Reg.view<PlayerComponent, InGameComp>();
	// プレイヤーが存在しなかったら切り上げ
	if (Playerview.size_hint() == 0) return;
	// プレイヤーのエンティティ
	auto PlayerEntity = *Playerview.begin();
	// 自分自身のコンポーネントを取得
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& State = Reg.get<EnemyAIComp>(Entity);
	// プレイヤーのトランスフォームを取得
	auto& PlayerTransformCmp = Reg.get<Transform3D>(PlayerEntity);
	D3DXVECTOR3 MyPos, PlayerPos;
	MyPos = D3DXVECTOR3(TransformCmp.mtxWorld._41, 0.0f, TransformCmp.mtxWorld._43);
	PlayerPos = D3DXVECTOR3(PlayerTransformCmp.mtxWorld._41, 0.0f, PlayerTransformCmp.mtxWorld._43);

	D3DXVECTOR3 Vec = PlayerPos - MyPos;

	// 正規化されたプレイヤーまでのベクトル
	D3DXVECTOR3 VecNormal;
	D3DXVec3Normalize(&VecNormal, &Vec);
	// プレイヤーまでの光線の情報
	RayComp ToPlayerRay;
	ToPlayerRay.Origin = MyPos;
	ToPlayerRay.Dir = VecNormal;
	// フラグを保存
	State.IsOldBlockedToPlayer = State.IsBlockedToPlayer;
	// フラグを更新
	State.IsBlockedToPlayer = CMath::IsBlockedRayToMeshes(CMapManager::Instance()->GetvMapObject(), ToPlayerRay, CMath::CalcDistance(MyPos, PlayerPos));
}