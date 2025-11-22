#include "player.h"
#include "manager.h"
#include "laser.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Init(void)
{
	SetPosition({ 0.0f,100.0f,0.0f });

	m_CollisionShape = std::make_unique<btCapsuleShape>(btScalar(7.0f), btScalar(20.0f));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPosition().x, GetPosition().y + 20.0f, GetPosition().z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	m_RigitBody = std::make_unique<btRigidBody>(info);
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	m_RigitBody->setUserPointer(this);
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	CObjectX::Init();
	CObjectX::SetPath("data\\MODEL\\testplayer1.x");
	m_Lasertest = CLaser::Create(8, 2);

	return E_NOTIMPL;
}

void CPlayer::Uninit(void)
{
	// 剛体の削除
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
	}

	// 衝突形状の削除
	m_CollisionShape.reset();

	CObjectX::Uninit();
}

void CPlayer::Update(void)
{
	// 早期リターン
	if (m_RigitBody == nullptr) return;
	if (m_RigitBody->getMotionState() == nullptr) return;

	// 移動量を設定するための変数
	btVector3 moveDir(0, 0, 0);

	// 描画モデルの位置
	btVector3 newPos;

	// カメラから見た移動量を計算
	D3DXVECTOR3 V = CManager::GetCamera()->GetPosR() - CManager::GetCamera()->GetPosV();
	D3DXVECTOR3 H;
	D3DXVECTOR3 VecU = VEC_UP;
	V.y = 0.0f;

	// 外積でX軸を計算
	D3DXVec3Cross(&H, &V, &VecU);
	D3DXVec3Normalize(&V, &V);
	D3DXVec3Normalize(&H, &H);

	// 素早さ
	const float Speed = 10;

	// キーボード検知
	if (CManager::GetInputKeyboard()->GetPress(DIK_W))  moveDir += btVector3(V.x, V.y, V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_S)) moveDir += btVector3(-V.x, -V.y, -V.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_A)) moveDir += btVector3(H.x, H.y, H.z);
	if (CManager::GetInputKeyboard()->GetPress(DIK_D)) moveDir += btVector3(-H.x, -H.y, -H.z);

	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{
		// 設定する用の向き
		D3DXVECTOR3 Rot;

		// 取得
		Rot = GetRotasion();

		// Yの角度だけ変える
		Rot.y -= 0.01f;

		// 角度の正規化
		if (Rot.y > D3DX_PI)
		{
			Rot.y += -D3DX_PI * 2;
		}
		else if (Rot.y < -D3DX_PI)
		{
			Rot.y += D3DX_PI * 2;
		}

		// 設定
		SetRotasion(Rot);
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{
		// 設定する用の向き
		D3DXVECTOR3 Rot;

		// 取得
		Rot = GetRotasion();

		// Yの角度だけ変える
		Rot.y += 0.01f;

		// 角度の正規化
		if (Rot.y > D3DX_PI)
		{
			Rot.y += -D3DX_PI * 2;
		}
		else if (Rot.y < -D3DX_PI)
		{
			Rot.y += D3DX_PI * 2;
		}

		// 設定
		SetRotasion(Rot);
	}

	// 移動していたら
	if (moveDir.length2() > 0.0f)
	{
		// 移動値を方向ベクトルに変換
		moveDir.normalize();

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(moveDir.x(), moveDir.z());

		// 設定する用の向き
		D3DXVECTOR3 Rot;

		// 取得
		Rot = GetRotasion();

		// Yの角度だけ変える
		Rot.y = angle + D3DX_PI;

		// 角度の正規化
		if (Rot.y > D3DX_PI)
		{
			Rot.y += -D3DX_PI * 2;
		}
		else if (Rot.y < -D3DX_PI)
		{
			Rot.y += D3DX_PI * 2;
		}

		// 設定
		SetRotasion(Rot);

		// スピードを掛ける
		moveDir *= Speed;
	}

	// Yの移動量を取得(重力)
	moveDir.setY(m_RigitBody->getLinearVelocity().y());

	// 設定
	m_RigitBody->setLinearVelocity(moveDir);

	// トランスフォームを取得
	btTransform trans;
	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// 位置を取得
	newPos = trans.getOrigin();

	// 位置を計算、設定
	SetPosition(D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));

	// 更新
	CObjectX::Update();

	D3DXVECTOR3 LaserPos = GetPosition();
	LaserPos.y += 100.0f;
	m_Lasertest->SetOrigin(LaserPos);
	D3DXMATRIX mtxWorld = GetmtxWorld();
	D3DXVECTOR3 Front = { mtxWorld._11,mtxWorld._12,mtxWorld._13 };
	D3DXVec3Normalize(&Front, &Front);
	m_Lasertest->SetFront(Front);
}

void CPlayer::Draw(void)
{
	CObjectX::DrawOutline();
	CObjectX::CastShadow();
}

void CPlayer::DrawShape(void)
{
	CObjectX::DrawShape();
}

void CPlayer::CastShadow(void)
{
	CObjectX::CastShadow();
}

CPlayer* CPlayer::Create(void)
{
	CPlayer* pPlayer = NULL;
	pPlayer = new CPlayer;
	pPlayer->Init();
	return pPlayer;
}
