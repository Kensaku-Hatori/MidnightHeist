//****************************************************************
//
// マップに置くオブジェクトの処理[mapobject.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "mapobject.h"
#include "modelmanager.h"
#include "manager.h"
#include "math.h"
#include "object2D.h"
#include "camera.h"
#include "toon.h"
#include "shadowmap.h"
#include "shapeshadow.h"

//***************************************
// コンストラクタ
//***************************************
CMapObject::CMapObject() : CObject(2)
{
	m_Pos = VEC3_NULL;
	m_ModelPath.clear();
}

//***************************************
// デストラクタ
//***************************************
CMapObject::~CMapObject()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CMapObject::Init(void)
{
	m_Mass = 100;
	D3DXQuaternionIdentity(&m_Quad);

	// 状態を初期化
	m_IsStatic = true;

	m_CollisionShape.reset(new btBoxShape(btVector3(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y, m_Size.z * m_Scale.z)));

	btVector3 test = { 0.0f,0.0f,0.0f };
	m_CollisionShape->calculateLocalInertia(0, test);

	btTransform transform,Origin,Offset;
	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	transform.setIdentity();
	rotation = SetQuad(m_Quad);
	Origin.setRotation(rotation);
	Origin.setOrigin(btVector3(m_Pos.x, m_Pos.y, m_Pos.z));
	Offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));
	transform.mult(Origin, Offset);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(0, motionState, m_CollisionShape.get());

	m_RigitBody = std::make_unique<btRigidBody>(info);
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	m_RigitBody->setUserPointer(this);
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	//m_Sound = CSound::CreateEmitter(CSound::LABEL_WATERFALL);
	//m_Sound->SetPosition(VEC3_NULL);
	//m_Sound->Play(
	//	*CSound::Get3DHandle(),
	//	CSound::GetListener(),
	//	CSound::GetAudioData(CSound::LABEL_WATERFALL),
	//	CSound::GetAudioSize(CSound::LABEL_WATERFALL)
	//);

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CMapObject::Uninit(void)
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

	Release();
}

//***************************************
// 更新処理
//***************************************
void CMapObject::Update(void)
{
	if (m_RigitBody == nullptr) return;

	// リジットボディーの更新
	UpdateRB();

	// トランスフォーム
	btTransform trans;

	// ゲット
	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// 回転行列を取得オフセット分掛ける
	// 物理世界での位置からオフセット分ずらした現実世界での位置を計算する用の変数
	btVector3 worldoffet = trans.getBasis() * btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z);

	// 物理世界の位置から回転行列をかけ合わせたオフセットを引く
	btVector3 pos = trans.getOrigin() - worldoffet;

	// 位置に代入
	m_Pos.x = pos.x();
	m_Pos.y = pos.y();
	m_Pos.z = pos.z();
}

//***************************************
// リジットボディーの更新
//***************************************
void CMapObject::UpdateRB(void)
{
	// 剛体の削除
	if (m_RigitBody)
	{
		// 物理世界から削除
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());

		// モーションステートを取得nullチェック
		if (m_RigitBody->getMotionState())
		{
			// モーションステートを削除
			delete m_RigitBody->getMotionState();
		}
		// リジットボディをクリア
		m_RigitBody.reset();
	}

	// 当たり判定を再生成
	m_CollisionShape.reset(new btBoxShape(btVector3(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y, m_Size.z * m_Scale.z)));
	m_RBOffset.y = m_Size.y * m_Scale.y;

	// 質量を宣言
	btScalar Mass = 0;

	if (m_IsStatic == false) Mass = m_Mass;

	// 抗力を代入
	btVector3 Inertia = { 0.0f,0.0f,0.0f };

	// 抗力を設定
	m_CollisionShape->calculateLocalInertia(Mass, Inertia);

	// 物理世界の位置などを取得
	btTransform transform,origin,offset;

	// 初期化
	transform.setIdentity();
	origin.setIdentity();
	offset.setIdentity();

	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	rotation = SetQuad(m_Quad);
	origin.setRotation(rotation);
	origin.setOrigin(btVector3(m_Pos.x, m_Pos.y, m_Pos.z));
	offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));
	transform.mult(origin, offset);

	// インターフェイスを設定
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(Mass, motionState, m_CollisionShape.get());

	// リジットボディーを再生成
	m_RigitBody.reset(new btRigidBody(info));

	// 移動方向を制限
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	m_RigitBody->setUserPointer(this);

	// スリープ状態を設定
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界にリジットボディーを追加
	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
}

//***************************************
// 描画処理
//***************************************
void CMapObject::Draw(void)
{
	//DrawObject();
}

//***************************************
// モデルの描画処理
//***************************************
void CMapObject::DrawObject(void)
{
	// モデルへのインデックスが-1だったら終わる
	if (m_ModelPath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quad);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), NULL,CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		if (col.pTextureFilename == NULL)
		{
			CToon::Instance()->BeginPass(0);
		}
		else
		{
			CToon::Instance()->BeginPass(1);
		}

		// テクスチャの設定
		pDevice->SetTexture(0, CLoadTexture::GetTex(modelinfo.modelinfo.TexPath[nCntMat]));

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}

void CMapObject::DrawShape(void)
{
	// モデルへのインデックスが-1だったら終わる
	if (m_ModelPath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quad);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CShapeShadow::Instance()->SetParameters(m_mtxWorld);

		CShapeShadow::Instance()->BeginPass(0);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShapeShadow::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}

void CMapObject::CastShadow(void)
{
	// モデルへのインデックスが-1だったら終わる
	if (m_ModelPath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quad);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), CLoadTexture::GetTex(modelinfo.modelinfo.TexPath[nCntMat]),CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		if (col.pTextureFilename == NULL)
		{
			CToon::Instance()->BeginPass(0);
		}
		else
		{
			CToon::Instance()->BeginPass(1);
		}

		//// テクスチャの設定
		//pDevice->SetTexture(0, CLoadTexture::GetTex(modelinfo->modelinfo.TexPath[nCntMat]));

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}

void CMapObject::DrawShadow(void)
{
	// モデルへのインデックスが-1だったら終わる
	if (m_ModelPath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quad);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CShadowMap::Instance()->SetParameters(m_mtxWorld);

		CShadowMap::Instance()->BeginPass(0);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShadowMap::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}

//***************************************
// アウトライン描画
//***************************************
void CMapObject::DrawOutline(void)
{
	// モデルへのインデックスが-1だったら終わる
	if (m_ModelPath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quad);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), NULL,CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		CToon::Instance()->BeginPass(2);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}

//***************************************
// 最小値最大値を計算
//***************************************
void CMapObject::CalcSize(void)
{
	// モデルmanagerからインデックスを指定して取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_ModelPath);
	BYTE* pVtxBuff = NULL;
	WORD SizeFVF = (WORD)D3DXGetFVFVertexSize(modelinfo.modelinfo.pMesh->GetFVF());
	int nNumVtx = modelinfo.modelinfo.pMesh->GetNumVertices();
	modelinfo.modelinfo.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	D3DXVECTOR3 Max, Min;
	Max = VEC3_NULL;
	Min = VEC3_NULL;

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3* Size = (D3DXVECTOR3*)pVtxBuff;
		if (Max.x < Size->x)
		{
			Max.x = Size->x;
		}
		if (Max.y < Size->y)
		{
			Max.y = Size->y;
		}
		if (Max.z < Size->z)
		{
			Max.z = Size->z;
		}
		if (Min.x > Size->x)
		{
			Min.x = Size->x;
		}
		if (Min.y > Size->y)
		{
			Min.y = Size->y;
		}
		if (Min.z > Size->z)
		{
			Min.z = Size->z;
		}
		pVtxBuff += SizeFVF;
	}
	m_Size = Max;
	m_Size.y *= 0.5f;
	m_RBOffset = { 0.0f,m_Size.y * m_Scale.y,0.0f };

	modelinfo.modelinfo.pMesh->UnlockVertexBuffer();
}

//***************************************
// 高さを再設定
//***************************************
void CMapObject::UpdateHeight(void)
{
}

//***************************************
// 位置を設定
//***************************************
void CMapObject::SetPos(D3DXVECTOR3 Pos)
{
	// 位置ずらす
	m_Pos = Pos;

	if (m_RigitBody == nullptr) return;
}

//***************************************
// 向きを変更
//***************************************
void CMapObject::SetRot(D3DXVECTOR3 Rot)
{
	if (m_RigitBody == nullptr) return;

	btQuaternion Quad;
	Quad.setEulerZYX(Rot.z, Rot.y, Rot.x);
	m_Quad = SetQuad(Quad);
}

//***************************************
// 向きを変更
//***************************************
void CMapObject::AddRot(D3DXVECTOR3 Vec, float Scaler)
{
	if (m_RigitBody == nullptr) return;

	btTransform trans;
	trans.setIdentity();
	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	btQuaternion add;
	add.setRotation(btVector3(Vec.x, Vec.y, Vec.z), Scaler);
	rotation = SetQuad(m_Quad) * add;
	m_Quad = SetQuad(rotation);
	trans.setRotation(rotation);
	m_RigitBody->setWorldTransform(trans);
	m_RigitBody->getMotionState()->setWorldTransform(trans);
}

//***************************************
// 拡大率を設定
//***************************************
void CMapObject::SetScale(D3DXVECTOR3 Scale)
{
	if (m_RigitBody == nullptr) return;
	if (m_Scale == Scale) return;

	m_Scale = Scale;
}

//***************************************
// クオータニオンを設定
//***************************************
void CMapObject::SetRotFromQuad(btQuaternion Quad)
{
	if (m_RigitBody == nullptr) return;
	m_Quad = SetQuad(Quad);
}

//***************************************
// 静的か動的かを切り替える
//***************************************
void CMapObject::SetStatic(const bool Static)
{
	// フラグが現状と同じだったら
	if(m_RigitBody == nullptr) return;
	if (m_IsStatic == Static) return;

	// 状態を変更
	m_IsStatic = Static;
}

//***************************************
// 質量を適応
//***************************************
void CMapObject::SetMass(const float Mass)
{
	// フラグが現状と同じだったら
	if (m_Mass == Mass) return;
	if (m_RigitBody == nullptr) return;

	// 状態を変更
	m_Mass = Mass;
}

btQuaternion CMapObject::SetQuad(D3DXQUATERNION Set)
{
	return btQuaternion(Set.x, Set.y, Set.z, Set.w);
}

D3DXQUATERNION CMapObject::SetQuad(btQuaternion Set)
{
	return D3DXQUATERNION(Set.x(), Set.y(), Set.z(), Set.w());
}

D3DXVECTOR3 CMapObject::GetRot(void)
{
	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	rotation = SetQuad(m_Quad);
	D3DXVECTOR3 Out;
	rotation.getEulerZYX(Out.z, Out.y, Out.x);
	return Out;
}

//***************************************
// マウスとモデルのメッシュの当たり判定
//***************************************
bool CMapObject::CollisionMousetoMesh(float* Distance)
{
	// モデルのマトリックスの逆行列
	D3DXMATRIX invWorld;

	// 逆行列を設定
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);

	// レイの始点と向きのローカル変数
	D3DXVECTOR3 localRayOrigin, localRayDir;

	// マウスのレイを取得
	CInputMouse::Ray LocalRay = CManager::GetInputMouse()->GetRay();

	// 始点を位置としてマトリックスで変換
	D3DXVec3TransformCoord(&localRayOrigin, &LocalRay.Origin, &invWorld);

	// レイの向きを方向ベクトルとしてマトリックスで変換
	D3DXVec3TransformNormal(&localRayDir, &LocalRay.Dir, &invWorld);

	// 一応正規化
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	// 当たったかどうかの一時変数
	BOOL hit;

	// 当たり判定を実行
	D3DXIntersect(CModelManager::GetModelInfo(m_ModelPath).modelinfo.pMesh, &localRayOrigin, &localRayDir, &hit, NULL, NULL, NULL, Distance, NULL, NULL);

	// 当たったかどうかを返す
	return hit;
}

//***************************************
// レイとモデルの当たり判定
//***************************************
bool CMapObject::CollisionRaytoMesh(D3DXVECTOR3 Origin, D3DXVECTOR3 Dir, float* Distance)
{
	// モデルのマトリックスの逆行列
	D3DXMATRIX invWorld;

	// 逆行列を設定
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);

	// レイの始点と向きのローカル変数
	D3DXVECTOR3 localRayOrigin, localRayDir;

	// マウスのレイを取得
	CInputMouse::Ray LocalRay;
	LocalRay.Origin = Origin;
	LocalRay.Dir = Dir;

	// 始点を位置としてマトリックスで変換
	D3DXVec3TransformCoord(&localRayOrigin, &LocalRay.Origin, &invWorld);

	// レイの向きを方向ベクトルとしてマトリックスで変換
	D3DXVec3TransformNormal(&localRayDir, &LocalRay.Dir, &invWorld);

	// 当たったかどうかの一時変数
	BOOL hit;

	// 当たり判定を実行
	D3DXIntersect(CModelManager::GetModelInfo(m_ModelPath).modelinfo.pMesh, &localRayOrigin, &localRayDir, &hit, NULL, NULL, NULL, Distance, NULL, NULL);

	// 当たったかどうかを返す
	return hit;
}

//***************************************
// 生成
//***************************************
CMapObject* CMapObject::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	CMapObject* pMapObject = NULL;
	// メモリ確保
	pMapObject = new CMapObject;
	// 情報を設定
	pMapObject->SetModelPath(Path);
	pMapObject->SetPos(Pos);
	pMapObject->SetRot(Rot);
	pMapObject->CalcSize();
	// 初期化処理
	pMapObject->Init();
	return pMapObject;
}