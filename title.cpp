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
#include "Sound2D.h"
#include "Sound3D.h"
#include "math.h"
#include "SetUpLoader.h"
#include "btCollisionConfig.hpp"

// 規定値を設定
// 地面
const btVector3 CTitle::Config::Ground::Size = { 2000.0f,0.0f,1000.0f };
const btVector3 CTitle::Config::Ground::Origin = { 0.0f,0.0f,0.0f };

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
	CSetUpLoader::Instance().LoadToScene(GetReg(), Config::SetUp::Path);
	// タイトルBGMを生成・再生
	CSound2D::Instance()->Play(SoundDevice::LABEL_TITLEBGM);
	// 聞き手の初期化
	CListener::Instance()->Init();

	// タイトルプレイヤーが落ちないために地面の当たり判定を生成
	{
		m_GroundShape = std::make_unique<btBoxShape>(Config::Ground::Size);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(Config::Ground::Origin);

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
	// カメラにシステムを追加
	CManager::GetCamera()->AddSystem(new CTitleCamera);
	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CTitle::Update(void)
{
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

	// BGMを停止
	CSound2D::Instance()->Stop(SoundDevice::LABEL_TITLEBGM);

	// カメラのシステムを終了
	CManager::GetCamera()->EndSystems();
	// エンティティたちをクリア
	GetReg().clear();
	// 自分自身の破棄
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CTitle::Draw(void)
{
}