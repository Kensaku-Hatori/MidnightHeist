//****************************************************************
//
// ファクトリ[Factories.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "FactorieUtils.h"

// 名前空間
using ordered_json = nlohmann::ordered_json;
using namespace std;
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// オブジェクト2Dの生成
//*********************************************
entt::entity Factories::makeObject2D(entt::registry& Reg, const int Layer, const std::string& Path, D3DXVECTOR2 Pos, D3DXVECTOR2 Size, entt::entity Parent)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform2D>(myEntity, Pos);
	Reg.emplace<Object2DComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, Path);
	Reg.emplace<SizeComp>(myEntity, Size);
	Reg.emplace<ColorComp>(myEntity);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<LayerComp>(myEntity, Layer);
	Reg.emplace<ParentComp>(myEntity, Parent);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントを取得
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
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 0.0f, 800.0f));
	Reg.emplace<Object3DComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "data\\TEXTURE\\floor.jpg");
	Reg.emplace<SizeComp>(myEntity, D3DXVECTOR2(100.0f, 100.0f));
	Reg.emplace<ColorComp>(myEntity);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<NorComp>(myEntity, D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	Reg.emplace<LayerComp>(myEntity, 3);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントを取得
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
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	Reg.emplace<ObjectXComponent>(myEntity);
	Reg.emplace<LayerComp>(myEntity,3);
	Reg.emplace<XRenderingComp>(myEntity, Path);

	return myEntity;
}

//*********************************************
// 3Dの視界を生成
//*********************************************
entt::entity Factories::make3DSightFan(entt::registry& Reg, entt::entity Parent)
{
	// エンティティを生成
	entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	Reg.emplace<SightFanComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "data\\TEXTURE\\EnemySight.png");
	Reg.emplace<FanComp>(myEntity, VEC3_NULL, VEC3_NULL, 90.0f, 200.0f);
	Reg.emplace<ColorComp>(myEntity,RED);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<NorComp>(myEntity, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Reg.emplace<ParentComp>(myEntity, Parent);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントを取得
	VertexComp& myVtx = CManager::GetScene()->GetReg().get<VertexComp>(myEntity);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&myVtx.pVertex,
		NULL);

	// 扇形ポリゴン用の初期化
	InitSightFan(Reg, myEntity);

	return myEntity;
}

//*********************************************
// 3D視界を初期化
//*********************************************
void Factories::InitSightFan(entt::registry& Reg, entt::entity& Entity)
{
	// コンポーネントを取得
	auto& VtxCmp = Reg.get<VertexComp>(Entity);
	auto& FanInfoCmp = Reg.get<FanComp>(Entity);
	auto& ColCmp = Reg.get<ColorComp>(Entity);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	VtxCmp.pVertex->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラーの設定
	pVtx[0].col = ColCmp.Col;
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);

	//頂点座標の設定
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DXToRadian(FanInfoCmp.RangeDegree / 2.0f)) * FanInfoCmp.Length,
		0.0f, 
		-cosf(D3DXToRadian(FanInfoCmp.RangeDegree / 2.0f)) * FanInfoCmp.Length);
	//法線ベクトルの設定
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラーの設定
	pVtx[1].col = D3DXCOLOR(ColCmp.Col.r, ColCmp.Col.g, ColCmp.Col.b, 0.0f);
	//テクスチャ座標の設定
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);

	//頂点座標の設定
	pVtx[2].pos = D3DXVECTOR3(-sinf(D3DXToRadian(FanInfoCmp.RangeDegree / 2.0f)) * FanInfoCmp.Length, 
		0.0f, 
		-cosf(D3DXToRadian(FanInfoCmp.RangeDegree / 2.0f)) * FanInfoCmp.Length);
	//法線ベクトルの設定
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラーの設定
	pVtx[2].col = D3DXCOLOR(ColCmp.Col.r, ColCmp.Col.g, ColCmp.Col.b, 0.0f);
	//テクスチャ座標の設定
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック　
	VtxCmp.pVertex->Unlock();
}

//*********************************************
// 音の大きさを可視化オブジェクトを生成
//*********************************************
entt::entity Factories::makeEmitterVolume(entt::registry& Reg, const D3DXCOLOR Col, entt::entity Parent)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity);
	Reg.emplace<VisibleSound>(myEntity);
	Reg.emplace<VisibleSineCurveComp>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "data\\TEXTURE\\floor.jpg");
	Reg.emplace<ColorComp>(myEntity, Col);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<ParentComp>(myEntity, Parent);

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// コンポーネントを取得
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
// 円形UIの生成
//*********************************************
entt::entity Factories::makeUICircle(entt::registry& Reg, entt::entity Parent)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	Reg.emplace<UICircleComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, "");
	Reg.emplace<SizeComp>(myEntity, D3DXVECTOR2(25.0f, 25.0f));
	Reg.emplace<ColorComp>(myEntity);
	Reg.emplace<UVComp>(myEntity);
	Reg.emplace<NorComp>(myEntity, D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	Reg.emplace<UICircleComp>(myEntity, 25.0f);
	Reg.emplace<RenderFragComp>(myEntity);
	Reg.emplace<LayerComp>(myEntity, 3);
	Reg.emplace<ParentComp>(myEntity, Parent);

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
// オブジェクトPlayerの生成
//*********************************************
entt::entity Factories::makeBacePlayer(entt::registry& Reg, const D3DXVECTOR3& Pos)
{
	// エンティティを生成
	entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, Pos);
	Reg.emplace<PlayerComponent>(myEntity);
	// モデルの大きさを基準に当たり判定を作成
	auto& Collider = Reg.emplace<CapsuleColliderComponent>(myEntity);
	Collider.Height = CMath::CalcModelSize("data\\MODEL\\testplayer1.x").y * 2.0f;
	Collider.Radius = 20.0f;
	Collider.LocalPos = D3DXVECTOR3(0.0f, (Collider.Height * 0.5f) + Collider.Radius, 0.0f);
	Collider.LocalQuat = QUAT_NULL;

	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\testplayer1.x");
	Reg.emplace<Pysics::RigitBody>(myEntity);
	Reg.emplace<RigidBodyComponent>(myEntity, 1.0f, CollisionGroupAndMasks::GROUP_PLAYER, CollisionGroupAndMasks::MASK_PLAYER);
	Reg.emplace<CharactorComp>(myEntity, 0.1f);

	// 基礎プレイヤー用の初期化処理
	//InitBacePlayer(Reg, myEntity);

	return myEntity;
}

//*********************************************
// オブジェクトPlayerの初期化
//*********************************************
void Factories::InitBacePlayer(entt::registry& Reg, entt::entity& Entity)
{
	// コンポーネントを取得
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
// ゲームプレイヤーの初期化
//*********************************************
void Factories::InitGamePlayer(entt::registry& Reg, entt::entity& Entity)
{
	// コンポーネントを追加
	Reg.emplace<InGameComp>(Entity);
	Reg.emplace<CastShadow>(Entity);
	Reg.emplace<PlayerSoundVolumeComp>(Entity);
	Reg.emplace<RenderingOutLine>(Entity);
	// アウトライン用の情報を設定
	Reg.emplace<OutLineComp>(Entity, 6.0f, D3DXVECTOR4(1.0f, 0.3f, 0.5f, 1.0f), CMath::CalcModelSize("data\\MODEL\\testplayer1.x").y * 2.0f);
	Reg.emplace<PlayerStateComp>(Entity);
	Reg.emplace<PlayerAnimComp>(Entity);
	// 子供エンティティリスト用の変数
	std::vector<entt::entity> Children;
	// 子供エンティティを生成
	Children.push_back(makeObject2D(Reg, 3, "data/TEXTURE/lockon01.png", D3DXVECTOR2(FLT_MAX, FLT_MAX), D3DXVECTOR2(100.0f, 100.0f), Entity));
	Children.push_back(makeUICircle(Reg, Entity));
	Children.push_back(makeEmitterVolume(Reg, WHITE, Entity));
	// ロックオンエンティティを取得、子供コンポーネントを追加
	entt::entity LockOnEntity = Reg.emplace<ChildrenComp>(Entity, Children).Children[0];
	// ロックオンエンティティにロックオンアニメーションコンポーネントを追加
	Reg.emplace<LockOnAnimComp>(LockOnEntity, VEC2_NULL, 60, 120, 60).ApperColor = RED;
}

//*********************************************
// タイトルプレイヤーの初期化
//*********************************************
void Factories::InitTitlePlayer(entt::registry& Reg, entt::entity& Entity)
{
	// プレイヤーにタグを追加する
	Reg.emplace<InTitleComp>(Entity);
}

//*********************************************
// 数字ポリゴンの生成
//*********************************************
entt::entity Factories::makeNumber(entt::registry& Reg, const D3DXVECTOR2 Origin, const int Value, const D3DXVECTOR2 DigitSize, const D3DXVECTOR2 DigitOffset)
{
	// エンティティを生成
	entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<TimerComponent>(myEntity);
	Reg.emplace<Transform2D>(myEntity, Origin);
	Reg.emplace<TimerComp>(myEntity, Value, DigitSize, DigitOffset);
	Reg.emplace<RenderFragComp>(myEntity, false);
	// 親子関係コンポーネントを追加
	Reg.emplace<ChildrenComp>(myEntity);

	return myEntity;
}

//*********************************************
// オブジェクトEnemyの生成
//*********************************************
entt::entity Factories::makeEnemy(entt::registry& Reg, D3DXVECTOR3 Pos, std::vector<EnemyState::PatrolMap>& PointList)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, Pos);
	// トランスフォームコンポーネントを取得
	auto& Trans = Reg.get<Transform3D>(myEntity);
	// 敵の視界ポリゴン設定用の回転行列
	D3DXMATRIX Basis = Trans.mtxBasis;
	D3DXVECTOR3 FrontVec = { Basis._31,Basis._32,Basis._33 };
	// コンポーネントを追加
	Reg.emplace<VelocityComp>(myEntity);
	// 視界の情報を設定
	Reg.emplace<FanComp>(myEntity, Trans.Pos, FrontVec, 90.0f, 200.0f);
	Reg.emplace<EnemyComponent>(myEntity);
	Reg.emplace<EnemyListenerComp>(myEntity);
	Reg.emplace<EnemyAIComp>(myEntity, EnemyState::ENEMYSTATE::SEARCH, PointList);
	Reg.emplace<CharactorComp>(myEntity,0.1f);
	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\serchrobot.x");
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<EnemyAIAstarComp>(myEntity);
	// モデルの大きさを基準に当たり判定を作成
	auto& Collider = Reg.emplace<CapsuleColliderComponent>(myEntity);
	Collider.Height = CMath::CalcModelSize("data\\MODEL\\serchrobot.x").y * 2.0f;
	Collider.Radius = 21.0f;
	Collider.LocalPos = D3DXVECTOR3(0.0f, (Collider.Height * 0.5f) + 20.0f, 0.0f);
	Collider.LocalQuat = QUAT_NULL;

	Reg.emplace<Pysics::RigitBody>(myEntity);
	Reg.emplace<RigidBodyComponent>(myEntity, 1.0f, CollisionGroupAndMasks::GROUP_ENEMY, CollisionGroupAndMasks::MASK_ENEMY);

	// エミッタを生成再生
	auto& AI = Reg.get<EnemyAIComp>(myEntity);
	AI.Emitter = CEmitter::Create(SoundDevice::LABEL_ENEMYMOVE, Pos);
	AI.Emitter->Play();

	// 親子関係コンポーネント用のリスト
	std::vector<entt::entity> Children;
	// 子供を作成
	Children.push_back(MeshFactories::makeLaser(Reg, myEntity));
	Children.push_back(make3DSightFan(Reg, myEntity));
	Children.push_back(makeEmitterVolume(Reg,RED, myEntity));
	// 親子関係のコンポーネントを追加
	Reg.emplace<ChildrenComp>(myEntity, Children);

	return myEntity;
}

//*********************************************
// オブジェクトMapobjectの生成
//*********************************************
entt::entity Factories::makeMapobject(entt::registry& Reg, const std::string& Path,const D3DXVECTOR3& Pos, const D3DXQUATERNION& Quat, const D3DXVECTOR3& Scale, const entt::entity& Parent)
{
	// エンティティを生成
	entt::entity myEntity = Reg.create();
	// 親が引数に素材したら
	if (Parent != entt::null)
	{
		Reg.emplace<ParentComp>(myEntity, Parent);
	}
	// コンポーネントを追加
	auto& TransCmp = Reg.emplace<Transform3D>(myEntity, Pos, Scale, Quat);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<CastShapeShadow>(myEntity);
	Reg.emplace<MapObjectComponent>(myEntity);
	auto& SizeCmp = Reg.emplace<Size3DComp>(myEntity, Path);
	auto& BoxColliderCmp = Reg.emplace<BoxColliderComponent>(myEntity);
	BoxColliderCmp.LocalPos = D3DXVECTOR3(0.0f, SizeCmp.Size.y, 0.0f);
	BoxColliderCmp.LocalQuat = QUAT_NULL;
	BoxColliderCmp.HalfSize = D3DXVECTOR3(SizeCmp.Size.x * TransCmp.Scale.x, SizeCmp.Size.y * TransCmp.Scale.y, SizeCmp.Size.z * TransCmp.Scale.z);
	Reg.emplace<Pysics::RigitBody>(myEntity);
	Reg.emplace<RigidBodyComponent>(myEntity, 0.0f, CollisionGroupAndMasks::GROUP_MAPOBJECT, CollisionGroupAndMasks::MASK_MAPOBJECT);
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
		// コンポーネントを追加
		Reg.emplace<RenderingOutLine>(Entity);
		Reg.emplace<ItemComp>(Entity, 100.0f);
		Reg.emplace<OutLineComp>(Entity, 6.0f, D3DXVECTOR4(1.0f, 0.7f, 0.5f, 1.0f), CMath::CalcModelSize(Path).y * 2.0f);
		Reg.emplace<ItemComponent>(Entity);
		// 親子関係コンポーネント用のリスト
		std::vector<entt::entity> Children;
		// 子供を生成
		Children.push_back(makeObject2D(Reg, 3, "data/TEXTURE/lockon.png", D3DXVECTOR2(FLT_MAX, FLT_MAX), D3DXVECTOR2(100.0f, 100.0f), Entity));
		// ロックオンエンティティを取得、子供コンポーネントを追加
		entt::entity LockOnEntity = Reg.emplace<ChildrenComp>(Entity, Children).Children[0];
		Reg.emplace<LockOnAnimComp>(LockOnEntity, VEC2_NULL, 30, 60, 60);
	}
}

//*********************************************
// タイトルマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeTitleManager(entt::registry& Reg)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<TitleManagerComponent>(myEntity);
	Reg.emplace<Select2DComp>(myEntity, TitleMenu::MENUTYPE::START);

	// 初期化
	InitTitleManager(Reg);

	return myEntity;
}

//*********************************************
// タイトルマネージャー初期化
//*********************************************
void ManagerFactories::InitTitleManager(entt::registry& Reg)
{
	// タイトルメニュー用のポリゴンを作成
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
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<PauseManagerComponent>(myEntity);
	Reg.emplace<Select2DComp>(myEntity, PauseMenu::MENUTYPE::CONTINUE);

	// 初期化処理
	InitPauseManager(Reg, myEntity);

	return myEntity;
}

//*********************************************
// ポーズマネージャー初期化
//*********************************************
void ManagerFactories::InitPauseManager(entt::registry& Reg, entt::entity Parent)
{
	// ポーズメニュー用のポリゴンを作成
	for (int nCnt = 0; nCnt < static_cast<unsigned int>(PauseMenu::MENUTYPE::MAX); nCnt++)
	{
		const entt::entity menuEntity = Factories::makeObject2D(Reg, 1, PauseMenu::PathList[nCnt], { 300.0f,360.0f + (150.0f * nCnt) }, { 200.0f,50.0f }, Parent);
		Reg.remove<Object2DComponent>(menuEntity);
		Reg.emplace<PauseMenuComponent>(menuEntity);
		Reg.emplace<Menu2DComp>(menuEntity, nCnt);
	}
}

//*********************************************
// ゲートマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeGateManager(entt::registry& Reg)
{
	// エンティティを生成
	entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(-750.0f, 0.0f, -475.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	Reg.emplace<GateManager>(myEntity);
	Reg.emplace<GateManagerComponent>(myEntity);

	// 基礎プレイヤー用の初期化処理
	InitGateManager(Reg, myEntity);

	return myEntity;
}

//*********************************************
// ゲートマネージャーの初期化
//*********************************************
void ManagerFactories::InitGateManager(entt::registry& Reg, entt::entity& Entity)
{
	// 親子関係コンポーネント用のリスト
	std::vector<entt::entity> Children;
	// 子供を作成
	Children.push_back(Factories::makeMapobject(Reg, "data/MODEL/EXITGate.x", { 30.0f,0.0f,0.0f }, QUAT_NULL, { 1.0f,1.0f,1.0f }, Entity));
	Children.push_back(Factories::makeMapobject(Reg, "data/MODEL/EXITGate.x", { -30.0f,0.0f,0.0f }, QUAT_NULL, { 1.0f,1.0f,1.0f }, Entity));

	// 親子関係のコンポーネントを追加
	Reg.emplace<ChildrenComp>(Entity, Children);
}

//*********************************************
// エネミーマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeEnemyManager(entt::registry& Reg)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<EnemyManagerComp>(myEntity, "data/TEXT/EnemyManager.json");

	return myEntity;
}

//*********************************************
// アイテムマネージャの生成
//*********************************************
entt::entity ManagerFactories::makeItemManager(entt::registry& Reg)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// アイテムリストコンポーネント用のリスト
	std::vector<entt::entity> ItemList;
	// アイテムタグコンポーネントを持っているエンテティのビュー
	auto ItemView = Reg.view<ItemComponent>();
	// アクセス
	for (auto Entity : ItemView)
	{
		// 連結
		ItemList.push_back(Entity);
	}
	// コンポーネントを追加
	Reg.emplace<ItemManagerComp>(myEntity, ItemList);

	return myEntity;
}

//*********************************************
// スタッツマネージャーの生成
//*********************************************
entt::entity ManagerFactories::makeStutsManager(entt::registry& Reg)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// 子供のリスト用変数
	std::vector<entt::entity> ChildrenList;

	// 子供の数分繰り返す
	for (UINT nCnt = 0; nCnt < StutsConfig::STUTSWORD::WORD_TYPE_MAX; nCnt++)
	{
		if (CManager::GetIsClear() == false && nCnt == 0)
		{
			// 連結
			ChildrenList.push_back(Factories::makeObject2D(Reg, 4, "data/TEXTURE/Failed.png", StutsConfig::WordTable[nCnt].Pos, StutsConfig::WordTable[nCnt].Size));
		}
		else
		{
			// 連結
			ChildrenList.push_back(Factories::makeObject2D(Reg, 4, StutsConfig::WordTable[nCnt].Path, StutsConfig::WordTable[nCnt].Pos, StutsConfig::WordTable[nCnt].Size));
		}
	}
	// jsonファイルを開く
	ifstream ifs("data/TEXT/Stuts.json");

	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return {};
	}
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return {};
	}

	// jsonデータを宣言
	ordered_json jsonData;
	// jsonを読み込む
	ifs >> jsonData;

	std::vector<int> Result;
	const auto& obj = jsonData["PlayData"];
	// NAMEタグのデータを取り出す
	Result.push_back(obj["Time"]);
	Result.push_back(obj["EnCount"]);
	Result.push_back(obj["Steal"]);

	// 子供の数分繰り返す
	for (UINT nCnt = 0; nCnt < StutsConfig::STUTSNUMBER::NUM_TYPE_MAX; nCnt++)
	{
		// 連結
		ChildrenList.push_back(Factories::makeNumber(Reg, StutsConfig::ApperNumberTable[nCnt].Origin, Result[nCnt], StutsConfig::ApperNumberTable[nCnt].Size, StutsConfig::ApperNumberTable[nCnt].Offset));
	}
	// コンポーネントを追加
	Reg.emplace<ChildrenComp>(myEntity, ChildrenList);
	Reg.emplace<StutsManagerComp>(myEntity);
	Reg.emplace<StutsManagerComponent>(myEntity);
	// エンティティを返す
	return myEntity;
}

//*********************************************
// メッシュフィールドの生成
//*********************************************
entt::entity MeshFactories::makeMeshField(entt::registry& Reg, const int DivH, const int DivV, const D3DXVECTOR2& Size, const std::string Path)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity);
	Reg.emplace<MeshFieldComponent>(myEntity);
	Reg.emplace<DivisionComp>(myEntity,DivH,DivV);
	Reg.emplace<MeshInfoComp>(myEntity, DivH, DivV);
	Reg.emplace<SizeComp>(myEntity, Size);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);
	Reg.emplace<TexComp>(myEntity, Path);

	// 初期化処理
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

	// コンポーネントを取得
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
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	Reg.emplace<LaserComponent>(myEntity);
	Reg.emplace<ParentComp>(myEntity, Parent);
	Reg.emplace<DivisionComp>(myEntity, 2, 8);
	Reg.emplace<MeshInfoComp>(myEntity, 2, 8);
	Reg.emplace<SizeComp>(myEntity, D3DXVECTOR2(1.0f, 100000.0f));
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);

	// 初期化
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

	// コンポーネントを取得
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
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// パトロールマネージャーコンポーネントを追加
	Reg.emplace<PatrolPointComp>(myEntity, Path, 10.0f);
	// タグコンポーネントを追加
	Reg.emplace<PatrolPointManager>(myEntity);

	return myEntity;
}

//*********************************************
// スカイボックスの作成
//*********************************************
entt::entity MeshFactories::makeSkyBox(entt::registry& Reg)
{
	// エンティティを生成
	const entt::entity myEntity = Reg.create();
	// コンポーネントを追加
	Reg.emplace<SkyBoxComponent>(myEntity);
	Reg.emplace<VertexComp>(myEntity);
	Reg.emplace<IndexBufferComp>(myEntity);
	Reg.emplace<SkyBoxComp>(myEntity, "data\\SKYBOX");

	// 初期化処理
	InitMeshCube(Reg, myEntity);

	return myEntity;
}

//*********************************************
// メッシュ矩形の初期化
//*********************************************
HRESULT MeshFactories::InitMeshCube(entt::registry& Reg, const entt::entity& Entity)
{
	// コンポーネントを取得
	auto& VertexCmp = Reg.get<VertexComp>(Entity);
	auto& IdxBuffCmp = Reg.get<IndexBufferComp>(Entity);

	// デバイス取得
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

	// 大きさ
	float Size = 50;

	// 各頂点を設定
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