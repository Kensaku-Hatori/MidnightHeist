//************************************************************
//
//　タイトルの処理[title.cpp]
//　Author:Hatori Kensaku
//
//************************************************************

// インクルード
#include "title.h"
#include "manager.h"
#include "modelmanager.h"
#include "Factories.h"
#include "fade.h"
#include "game.h"
#include "TitleCamera.h"
#include "RigitBodyComponent.hpp"
#include "Sound2D.h"
#include "Sound3D.h"
#include "math.h"

// 規定値を設定

// プレイヤー
const D3DXVECTOR3 CTitle::Config::Player::Pos = { -340.0f,0.0f,-1045.0f };

//***************************************
// コンストラクタ
//***************************************
CTitle::CTitle() : CScene(MODE_TITLE)
{
}

//***************************************
// デストラクタ
//***************************************
CTitle::~CTitle()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitle::Init(void)
{
	// 平行光源の色をリセット
	CManager::GetLight()->ResetCol();

	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/Title/MidNightHeist.png", { 340.0f,160.0f }, { 200.0f,100.0f });
	Factories::makeObject3D(GetReg());

	entt::entity Player = Factories::makeBacePlayer(GetReg());
	Factories::InitTitlePlayer(GetReg(), Player);

	Factories::makeMapobject(GetReg(), "data\\MODEL\\Museum.x");

	ManagerFactories::makeTitleManager(GetReg());
	MeshFactories::makeMeshField(GetReg(), 100, 100, { 100.0f,100.0f });
	MeshFactories::makeSkyBox(GetReg());

	CSound2D::Instance()->Play(SoundDevice::LABEL_WATERFALL);

	m_test = CEmitter::Create(SoundDevice::LABEL_WATERFALL);
	m_test->Play();
	CListener::Instance()->Init();

	{
		m_GroundShape = std::make_unique<btBoxShape>(btVector3(btScalar(2000.0f), btScalar(10), btScalar(1000.0f)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -5, 0));

		btScalar mass(0);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			m_GroundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_GroundShape.get(), localInertia);
		m_GroundRB = std::make_unique<btRigidBody>(rbInfo);

		//add the body to the dynamics world
		CManager::GetDynamicsWorld()->addRigidBody(m_GroundRB.get(), CollisionGroupAndMasks::GROUP_MAPOBJECT, CollisionGroupAndMasks::MASK_MAPOBJECT);
	}

	CManager::GetCamera()->AddSystem(new CTitleCamera);
	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CTitle::Update(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true)
	{
		Factories::makeObject3D(GetReg());
	}
	m_test->Update();
}

//***************************************
// 終了処理
//***************************************
void CTitle::Uninit(void)
{
	// 剛体の削除
	if (m_GroundRB)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_GroundRB.get());
		if (m_GroundRB->getMotionState())
		{
			delete m_GroundRB->getMotionState();
		}
		m_GroundRB.reset();
	}
	m_GroundShape.reset();

	m_test->Uninit();

	CManager::GetCamera()->EndSystems();
	GetReg().clear();
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CTitle::Draw(void)
{
}