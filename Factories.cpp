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
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(100.0f * test, 0.0f, 0.0f));
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
// タイトルマネージャーの生成
//*********************************************
entt::entity Factories::makeTitleManager(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<TitleManagerComponent>(myEntity);
	Reg.emplace<TitleSelectComp>(myEntity);
	Reg.emplace<LayerComp>(myEntity, 3);

	InitTitleManager(Reg);

	return myEntity;
}

//*********************************************
// タイトルマネージャー初期化
//*********************************************
void Factories::InitTitleManager(entt::registry& Reg)
{
	for (int nCnt = 0; nCnt < static_cast<unsigned int>(TitleMenu::MENUTYPE::MAX); nCnt++)
	{
		const entt::entity menuEntity = Factories::makeObject2D(Reg, 3, TitleMenu::PathList[nCnt], { 300.0f,360.0f + (150.0f * nCnt) }, { 200.0f,50.0f });
		Reg.emplace<TitleMenuComponent>(menuEntity);
		Reg.emplace<TitleMenuComp>(menuEntity, static_cast<TitleMenu::MENUTYPE>(nCnt));
	}
}

//*********************************************
// オブジェクトPlayerの生成
//*********************************************
entt::entity Factories::makePlayer(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity).Pos = { 600.0f,200.0f,300.0f };
	Reg.emplace<PlayerComponent>(myEntity);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<RenderingOutLine>(myEntity);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\testplayer1.x");

	return myEntity;
}

//*********************************************
// オブジェクトEnemyの生成
//*********************************************
entt::entity Factories::makeEnemy(entt::registry& Reg)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity).Pos = { -600.0f,200.0f,-280.0f };
	auto& Trans = Reg.get<Transform3D>(myEntity);
	D3DXMATRIX Basis = Trans.GetBasis();
	D3DXVECTOR3 FrontVec = { Basis._31,Basis._32,Basis._33 };
	D3DXQuaternionRotationYawPitchRoll(&Trans.Quat, D3DX_PI, 0.0f, 0.0f);
	Reg.emplace<VelocityComp>(myEntity);
	Reg.emplace<FanComp>(myEntity, Trans.Pos, FrontVec, 90.0f, 200.0f);
	Reg.emplace<EnemyComponent>(myEntity);
	Reg.emplace<EnemtAIComp>(myEntity, EnemyState::ENEMYSTATE::SEARCH);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<XRenderingComp>(myEntity, "data\\MODEL\\serchrobot.x");
	Reg.emplace<SingleParentComp>(myEntity, MeshFactories::makeLaser(Reg, myEntity));

	return myEntity;
}

//*********************************************
// オブジェクトMapobjectの生成
//*********************************************
entt::entity Factories::makeMapobject(entt::registry& Reg, const std::string& Path,const D3DXVECTOR3& Pos, const D3DXQUATERNION& Quat, const D3DXVECTOR3& Scale)
{
	const entt::entity myEntity = Reg.create();
	Reg.emplace<Transform3D>(myEntity, Pos, Quat, Scale);
	Reg.emplace<CastShadow>(myEntity);
	Reg.emplace<CastShapeShadow>(myEntity);
	Reg.emplace<MapObjectComponent>(myEntity);
	Reg.emplace<SingleCollisionShapeComp>(myEntity);
	Reg.emplace<RigitBodyComp>(myEntity);
	Reg.emplace<Size3DComp>(myEntity, Path);
	if (Path.find("item01.x") != std::string::npos)Reg.emplace<RenderingOutLine>(myEntity);
	Reg.get<SingleCollisionShapeComp>(myEntity).Offset.y = Reg.get<Size3DComp>(myEntity).Size.y;
	Reg.emplace<XRenderingComp>(myEntity, Path);

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
	Reg.emplace<Transform3D>(myEntity, D3DXVECTOR3(0.0f, 30.0f, 0.0f));
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

	InitPatrolPointFromFile(Reg, myEntity);

	return myEntity;
}

//*********************************************
// 巡回ポイントの初期化
//*********************************************
HRESULT MeshFactories::InitPatrolPointFromFile(entt::registry& Reg, const entt::entity& Entity)
{
	return S_OK;
}