//****************************************************************
//
// ファクトリ[Factories.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "FactorieUtils.h"

// 名前空間
using namespace Tag;

//*********************************************
// オブジェクト2Dの生成
//*********************************************
entt::entity Factories::makeObject2D(entt::registry& Reg, const int Layer, const std::string& Path, D3DXVECTOR2 Pos, D3DXVECTOR2 Size)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform2D>(myEntity, Pos);
	Reg.emplace<Object2DComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, Path);
	Reg.emplace<SizeComp>(myEntity, Size);
	Reg.emplace<ColorComp>(myEntity);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<LayerComp>(myEntity, Layer);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VertexComp& myVtx = CManager::GetScene()->GetReg().get<VertexComp>(myEntity);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&myVtx.pVertex,
		NULL);

	return myEntity;
}

//*********************************************
// オブジェクト3Dの生成
//*********************************************
entt::entity Factories::makeObject3D(entt::registry& Reg)
{
	static int test;
	test++;
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, 1.0f, D3DXVECTOR3(100.0f * test, 0.0f, 0.0f));
	Reg.emplace<Object3DComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "data\\TEXTURE\\floor.jpg");
	Reg.emplace<SizeComp>(myEntity, D3DXVECTOR2(100.0f, 100.0f));
	Reg.emplace<ColorComp>(myEntity);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<NorComp>(myEntity, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	Reg.emplace<LayerComp>(myEntity, 3);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VertexComp& myVtx = CManager::GetScene()->GetReg().get<VertexComp>(myEntity);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&myVtx.pVertex,
		NULL);

	return myEntity;
}

//*********************************************
// オブジェクトXの生成
//*********************************************
entt::entity Factories::makeObjectX(entt::registry& Reg, const std::string& Path)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity);
	Reg.emplace<ObjectXComponent>(myEntity);
	Reg.emplace<LayerComp>(myEntity,3);
	Reg.emplace<XRenderingComp>(myEntity, Path);

	return myEntity;
}

//*********************************************
// オブジェクトPlayerの生成
//*********************************************
entt::entity Factories::makePlayer(entt::registry& Reg)
{
	entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, 0.1f, D3DXVECTOR3(1200.0f, 300.0f, 375.0f));
	Reg.emplace<PlayerComponent>(myEntity);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<RenderingOutLine>(myEntity);
	Reg.emplace<OutLineComp>(myEntity, 6.0f, D3DXVECTOR4(1.0f, 0.3f, 0.5f, 1.0f), CMath::CalcModelSize("data\\MODEL\\testplayer1.x").y * 2.0f);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<PlayerAnimComp>(myEntity);
	Reg.emplace<CapsuleComp>(myEntity, CMath::CalcModelSize("data\\MODEL\\testplayer1.x").y * 2.0f, 7.0f, CMath::CalcModelSize("data\\MODEL\\testplayer1.x").y);
	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\testplayer1.x");

	InitPlayer(Reg, myEntity);

	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		entt::entity LockOnEntity = Reg.emplace<SingleParentComp>(myEntity, makeObject2D(Reg, 3, "data/TEXTURE/lockon01.png", D3DXVECTOR2(FLT_MAX, FLT_MAX))).Parent;
		Reg.emplace<LockOnAnimComp>(LockOnEntity, VEC2_NULL, 60, 120, 60).ApperColor = RED;
	}

	return myEntity;
}

//*********************************************
// プレイヤーの初期化
//*********************************************
void Factories::InitPlayer(entt::registry& Reg, entt::entity& Entity)
{
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	auto& ColliderCmp = Reg.get<SingleCollisionShapeComp>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& CapsuleCmp = Reg.get<CapsuleComp>(Entity);

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(CapsuleCmp.Radius), btScalar(CapsuleCmp.AllHeight));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + CapsuleCmp.ToCenterOffset, TransformCmp.Pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_PLAYER, CollisionGroupAndMasks::MASK_PLAYER);
}

//*********************************************
// オブジェクトEnemyの生成
//*********************************************
entt::entity Factories::makeEnemy(entt::registry& Reg, D3DXVECTOR3 Pos, std::vector<EnemyState::PatrolMap>& PointList)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, 0.01f, Pos);
	auto& Trans = Reg.get<Transform3D>(myEntity);
	D3DXMATRIX Basis = Trans.GetBasis();
	D3DXVECTOR3 FrontVec = { Basis._31,Basis._32,Basis._33 };
	D3DXQuaternionRotationYawPitchRoll(&Trans.Quat, D3DX_PI, 0.0f, 0.0f);
	Reg.emplace<VelocityComp>(myEntity);
	Reg.emplace<FanComp>(myEntity, Trans.Pos, FrontVec, 90.0f, 200.0f);
	Reg.emplace<EnemyComponent>(myEntity);
	Reg.emplace<EnemtAIComp>(myEntity, EnemyState::ENEMYSTATE::SEARCH, PointList);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\serchrobot.x");
	Reg.emplace<SingleParentComp>(myEntity, MeshFactories::makeLaser(Reg, myEntity));
	Reg.emplace<EnemtAIAstarComp>(myEntity);

	return myEntity;
}

//*********************************************
// オブジェクトMapobjectの生成
//*********************************************
entt::entity Factories::makeMapobject(entt::registry& Reg, const std::string& Path,const D3DXVECTOR3& Pos, const D3DXQUATERNION& Quat, const D3DXVECTOR3& Scale)
{
	entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, 1.0f, Pos, Scale, Quat);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<CastShapeShadow>(myEntity);
	Reg.emplace<MapObjectComponent>(myEntity);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<Size3DComp>(myEntity, Path);
	Reg.get<SingleCollisionShapeComp>(myEntity).Offset.y = Reg.get<Size3DComp>(myEntity).Size.y;
	Reg.emplace<XRenderingComp>(myEntity, Path);

	// コンポーネントをマッピング
	MappingModelPathToComponent(Reg, myEntity, Path);

	return myEntity;
}

//*********************************************
// パスをもとにコンポーネントをマッピング
//*********************************************
void Factories::MappingModelPathToComponent(entt::registry& Reg, entt::entity& Entity, std::string Path)
{
	// アイテムなら
	if (Path.find("item01.x") != std::string::npos)
	{
		Reg.emplace<RenderingOutLine>(Entity);
		Reg.emplace<OutLineComp>(Entity, 6.0f, D3DXVECTOR4(1.0f, 0.7f, 0.5f, 1.0f), CMath::CalcModelSize(Path).y * 2.0f);
		Reg.emplace<ItemComponent>(Entity);
		entt::entity LockOnEntity = Reg.emplace<SingleParentComp>(Entity, makeObject2D(Reg, 3, "data/TEXTURE/lockon.png", D3DXVECTOR2(FLT_MAX, FLT_MAX))).Parent;
		Reg.emplace<LockOnAnimComp>(LockOnEntity, VEC2_NULL, 60, 60, 60);
	}
}

//*********************************************
// タイトルマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeTitleManager(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<TitleManagerComponent>(myEntity);
	Reg.emplace<Select2DComp>(myEntity, TitleMenu::MENUTYPE::START);

	InitTitleManager(Reg);

	return myEntity;
}

//*********************************************
// タイトルマネージャー初期化
//*********************************************
void ManagerFactories::InitTitleManager(entt::registry& Reg)
{
	for (int nCnt = 0; nCnt < static_cast<unsigned int>(TitleMenu::MENUTYPE::MAX); nCnt++)
	{
		const entt::entity menuEntity = Factories::makeObject2D(Reg, 1, TitleMenu::PathList[nCnt], { 300.0f,360.0f + (150.0f * nCnt) }, { 200.0f,50.0f });
		Reg.emplace<TitleMenuComponent>(menuEntity);
		Reg.emplace<Menu2DComp>(menuEntity, nCnt);
	}
}

//*********************************************
// ポーズマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makePauseManager(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<PauseManagerComponent>(myEntity);
	Reg.emplace<Select2DComp>(myEntity, PauseMenu::MENUTYPE::CONTINUE);

	InitPauseManager(Reg, myEntity);

	return myEntity;
}

//*********************************************
// ポーズマネージャー初期化
//*********************************************
void ManagerFactories::InitPauseManager(entt::registry& Reg, entt::entity Parent)
{
	for (int nCnt = 0; nCnt < static_cast<unsigned int>(PauseMenu::MENUTYPE::MAX); nCnt++)
	{
		const entt::entity menuEntity = Factories::makeObject2D(Reg, 1, PauseMenu::PathList[nCnt], { 300.0f,360.0f + (150.0f * nCnt) }, { 200.0f,50.0f });
		Reg.remove<Object2DComponent>(menuEntity);
		Reg.emplace<PauseMenuComponent>(menuEntity);
		Reg.emplace<SingleParentComp>(menuEntity, Parent);
		Reg.emplace<Menu2DComp>(menuEntity, nCnt);
	}
}

//*********************************************
// エネミーマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeEnemyManager(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<EnemyManagerComp>(myEntity, "data/TEXT/EnemyManager.json");
	return myEntity;
}

//*********************************************
// メッシュフィールドの生成
//*********************************************
entt::entity MeshFactories::makeMeshField(entt::registry& Reg, const int DivH, const int DivV, const D3DXVECTOR2& Size)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity);
	Reg.emplace<MeshFieldComponent>(myEntity);

	Reg.emplace<DivisionComp>(myEntity,DivH,DivV);

	Reg.emplace<MeshInfoComp>(myEntity, DivH, DivV);

	Reg.emplace<SizeComp>(myEntity, Size);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "data\\TEXTURE\\field.jpg");

	InitMeshField(Reg, myEntity);

	return myEntity;
}

//*********************************************
// メッシュフィールドの初期化
//*********************************************
HRESULT MeshFactories::InitMeshField(entt::registry& Reg, const entt::entity& Entity)
{
	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	auto& MeshInfoCmp = Reg.get<MeshInfoComp>(Entity);

	auto& VtxCmp = Reg.get<VertexComp>(Entity);
	auto& IdxBuffCmp = Reg.get<IndexBufferComp>(Entity);

	auto& SizeCmp = Reg.get<SizeComp>(Entity);

	auto& DivCmp = Reg.get<DivisionComp>(Entity);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MeshInfoCmp.nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&VtxCmp.pVertex,
		NULL);

	//インデックスへのポインタ
	WORD* pIdx = NULL;

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	//中央へずらす
	float centerX = SizeCmp.Size.x * (DivCmp.nDivHorizontal - 2) * 0.5f;
	float centerY = SizeCmp.Size.y * (DivCmp.nDivVertical - 2) * 0.5f;

	//頂点情報の設定
	for (int nCntZ = 0; nCntZ <= DivCmp.nDivVertical; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= DivCmp.nDivHorizontal; nCntX++)
		{
			//頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3((float)(-SizeCmp.Size.x + (SizeCmp.Size.x * nCntX) - centerX),
				0.0f,
				(float)(SizeCmp.Size.y - (SizeCmp.Size.y * nCntZ) + centerY));

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntVtx].col = WHITE;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			nCntVtx++;
		}
	}

	//頂点バッファをアンロック　
	VtxCmp.pVertex->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MeshInfoCmp.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IdxBuffCmp.pIdx,
		NULL);

	// インデックスバッファをロック
	IdxBuffCmp.pIdx->Lock(0, 0, (void**)&pIdx, 0);
	int nIdxA = DivCmp.nDivHorizontal + 1;	// 上側インデックス
	int nIdxB = 0;				// 今のインデックス
	int nCntIdx = 0;			// インデックスカウンター

	// インデックスを求める
	for (int nCntA = 0; nCntA < DivCmp.nDivVertical; nCntA++)
	{// Zの分割数分回す
		for (int nCntB = 0; nCntB <= DivCmp.nDivHorizontal; nCntB++)
		{// Xの分割数分回す
			pIdx[nCntIdx] = (WORD)nIdxA;
			pIdx[nCntIdx + 1] = (WORD)nIdxB;
			nCntIdx += 2;
			nIdxA++;
			nIdxB++;
		}
		if (nCntA < DivCmp.nDivVertical - 1)
		{// 安全装置
			pIdx[nCntIdx] = (WORD)nIdxB - 1;
			pIdx[nCntIdx + 1] = (WORD)nIdxA;
			nCntIdx += 2;
		}
	}
	// インデックスバッファのアンロック
	IdxBuffCmp.pIdx->Unlock();

	return S_OK;
}

//*********************************************
// メッシュレーザーの生成
//*********************************************
entt::entity MeshFactories::makeLaser(entt::registry& Reg, entt::entity Parent)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, 1.0f, D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	Reg.emplace<LaserComponent>(myEntity);
	Reg.emplace<SingleParentComp>(myEntity, Parent);
	Reg.emplace <LaserCollisionInfoComp>(myEntity);

	Reg.emplace<DivisionComp>(myEntity, 2, 8);

	Reg.emplace<MeshInfoComp>(myEntity, 2, 8);

	Reg.emplace<SizeComp>(myEntity, D3DXVECTOR2(1.0f, 100000.0f));
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);

	InitLaserMesh(Reg, myEntity);

	return myEntity;
}

//*********************************************
// メッシュレーザーの初期化
//*********************************************
HRESULT MeshFactories::InitLaserMesh(entt::registry& Reg, const entt::entity& Entity)
{
	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	auto& MeshInfoCmp = Reg.get<MeshInfoComp>(Entity);

	auto& VtxCmp = Reg.get<VertexComp>(Entity);
	auto& IdxBuffCmp = Reg.get<IndexBufferComp>(Entity);

	auto& SizeCmp = Reg.get<SizeComp>(Entity);

	auto& DivCmp = Reg.get<DivisionComp>(Entity);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MeshInfoCmp.nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&VtxCmp.pVertex,
		NULL);

	//インデックスへのポインタ
	WORD* pIdx = NULL;

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
			D3DXVECTOR3 CenterVec = D3DXVECTOR3(-pVtx[nCntVtx].pos.x, 0.0f, -pVtx[nCntVtx].pos.z);
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

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MeshInfoCmp.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IdxBuffCmp.pIdx,
		NULL);

	// インデックスバッファをロック
	IdxBuffCmp.pIdx->Lock(0, 0, (void**)&pIdx, 0);
	int nIdxA = DivCmp.nDivVertical + 1;	// 上側インデックス
	int nIdxB = 0;				// 今のインデックス
	int nCntIdx = 0;			// インデックスカウンター

	// インデックスを求める
	for (int nCntA = 0; nCntA < DivCmp.nDivHorizontal; nCntA++)
	{// Zの分割数分回す
		for (int nCntB = 0; nCntB <= DivCmp.nDivVertical; nCntB++)
		{// Xの分割数分回す
			pIdx[nCntIdx] = (WORD)nIdxA;
			pIdx[nCntIdx + 1] = (WORD)nIdxB;
			nCntIdx += 2;
			nIdxA++;
			nIdxB++;
		}
		if (nCntA < DivCmp.nDivHorizontal - 1)
		{// 安全装置
			pIdx[nCntIdx] = (WORD)nIdxB - 1;
			pIdx[nCntIdx + 1] = (WORD)nIdxA;
			nCntIdx += 2;
		}
	}
	// インデックスバッファのアンロック
	IdxBuffCmp.pIdx->Unlock();

	return S_OK;
}

//*********************************************
// 巡回ポイントの作成
//*********************************************
entt::entity MeshFactories::makePatrolPointFromFile(entt::registry& Reg, std::string Path)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<PatrolPointComp>(myEntity, Path, 10.0f);

	Reg.emplace<PatrolPointManager>(myEntity);

	return myEntity;
}

//*********************************************
// スカイボックスの作成
//*********************************************
entt::entity MeshFactories::makeSkyBox(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<SkyBoxComponent>(myEntity);

	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);
	Reg.emplace<SkyBoxComp>(myEntity, "data\\SKYBOX");

	InitMeshCube(Reg, myEntity);
	return myEntity;
}

//*********************************************
// メッシュ矩形の初期化
//*********************************************
HRESULT MeshFactories::InitMeshCube(entt::registry& Reg, const entt::entity& Entity)
{
	auto& VertexCmp = Reg.get<VertexComp>(Entity);
	auto& IdxBuffCmp = Reg.get<IndexBufferComp>(Entity);

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&VertexCmp.pVertex,
		NULL);

	//インデックスへのポインタ
	WORD* pIdx = NULL;

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	VertexCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);

	float Size = 50;

	pVtx[0].pos = D3DXVECTOR3(-Size, Size, -Size);
	pVtx[0].col = WHITE;

	pVtx[1].pos = D3DXVECTOR3(Size, Size, -Size);
	pVtx[1].col = WHITE;

	pVtx[2].pos = D3DXVECTOR3(Size, -Size, -Size);
	pVtx[2].col = WHITE;

	pVtx[3].pos = D3DXVECTOR3(-Size, -Size, -Size);
	pVtx[3].col = WHITE;

	pVtx[4].pos = D3DXVECTOR3(Size, Size, Size);
	pVtx[4].col = WHITE;

	pVtx[5].pos = D3DXVECTOR3(-Size, Size, Size);
	pVtx[5].col = WHITE;

	pVtx[6].pos = D3DXVECTOR3(-Size, -Size, Size);
	pVtx[6].col = WHITE;

	pVtx[7].pos = D3DXVECTOR3(Size, -Size, Size);
	pVtx[7].col = WHITE;

	//頂点バッファをアンロック　
	VertexCmp.pVertex->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IdxBuffCmp.pIdx,
		NULL);

	// インデックスバッファをロック
	IdxBuffCmp.pIdx->Lock(0, 0, (void**)&pIdx, 0);

	// --- 前面 ---
	pIdx[0] = 0; pIdx[1] = 1; pIdx[2] = 2;
	pIdx[3] = 0; pIdx[4] = 2; pIdx[5] = 3;

	// --- 背面 ---
	pIdx[6] = 4; pIdx[7] = 5; pIdx[8] = 6;
	pIdx[9] = 4; pIdx[10] = 6; pIdx[11] = 7;

	// --- 左面 ---
	pIdx[12] = 5; pIdx[13] = 0; pIdx[14] = 3;
	pIdx[15] = 5; pIdx[16] = 3; pIdx[17] = 6;

	// --- 右面 ---
	pIdx[18] = 1; pIdx[19] = 4; pIdx[20] = 7;
	pIdx[21] = 1; pIdx[22] = 7; pIdx[23] = 2;

	// --- 上面 ---
	pIdx[24] = 5; pIdx[25] = 4; pIdx[26] = 1;
	pIdx[27] = 5; pIdx[28] = 1; pIdx[29] = 0;

	// --- 底面 ---
	pIdx[30] = 3; pIdx[31] = 2; pIdx[32] = 7;
	pIdx[33] = 3; pIdx[34] = 7; pIdx[35] = 6;

	// インデックスバッファのアンロック
	IdxBuffCmp.pIdx->Unlock();

	return S_OK;
}