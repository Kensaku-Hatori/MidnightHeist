//****************************************************************
//
// ３Dエフェクトの処理[effect3D.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "effect3d.h"
#include "texmanager.h"

//*********************************************
// コンストラクタ
//*********************************************
CEffect3D::CEffect3D(const int Priority) : CObjectBillBoard(Priority)
{
	// エフェクトの挙動を初期化
	m_Module.Move = VEC3_NULL;
	m_Module.nLife = NULL;
	m_Module.fAlphaDef = NULL;
	m_Module.fSizeDef = NULL;
}

//*********************************************
// デストラクタ
//*********************************************
CEffect3D::~CEffect3D()
{
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CEffect3D::Init(void)
{
	// 一フレームに下がるアルファ値とサイズを設定
	m_Module.fAlphaDef = CObjectBillBoard::GetCol().a / m_Module.nLife;
	m_Module.fSizeDef = CObjectBillBoard::GetSize().x / m_Module.nLife;

	// ビルボードの初期化処理
	return CObjectBillBoard::Init();
}

//*********************************************
// 終了処理
//*********************************************
void CEffect3D::Uninit(void)
{
	// ビルボードの終了処理(メモリを破棄)
	CObjectBillBoard::Uninit();
}

//*********************************************
// 更新処理
//*********************************************
void CEffect3D::Update(void)
{
	// 寿命を減らす
	m_Module.nLife--;

	// 寿命が尽きたら
	if (m_Module.nLife < 0)
	{
		// 終了処理
		Uninit();

		// 処理を切り上げる
		return;
	}

	// 色を減らす
	D3DXCOLOR Col = CObjectBillBoard::GetCol();
	Col.a -= m_Module.fAlphaDef;

	// サイズを減らす
	D3DXVECTOR2 Size = CObjectBillBoard::GetSize();
	Size.x -= m_Module.fSizeDef;
	Size.y -= m_Module.fSizeDef;

	// 位置を動かす
	D3DXVECTOR3 Pos = CObjectBillBoard::GetPos();
	m_Module.Move.y += m_Module.fGravity;
	Pos += m_Module.Move;

	// 設定
	CObjectBillBoard::SetCol(Col);
	CObjectBillBoard::SetSize(Size);
	CObjectBillBoard::SetPosition(Pos);
	CObjectBillBoard::Update();
}

//*********************************************
// 描画処理
//*********************************************
void CEffect3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォグを切る
	CManager::GetRenderer()->offFog();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// テクスチャを設定
	pDevice->SetTexture(0, CLoadTexture::GetTex(m_Module.FilePath));

	// 描画
	CObjectBillBoard::Draw();

	//ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// フォグをつける
	CManager::GetRenderer()->onFog();
}

//*********************************************
// 生成
//*********************************************
CEffect3D* CEffect3D::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXCOLOR Col, D3DXVECTOR2 Size, float Gravity, int Life, const char* FilePath,const int Priority)
{
	// インスタンスを宣言
	CEffect3D* pEffect = NULL;

	// メモリ確保
	pEffect = new CEffect3D(Priority);

	// 情報を代入
	pEffect->m_Module.Move = Move;
	pEffect->m_Module.nLife = Life;
	pEffect->SetPosition(Pos);
	pEffect->SetCol(Col);
	pEffect->SetSize(Size);
	pEffect->m_Module.fGravity = Gravity;
	pEffect->m_Module.FilePath = FilePath;

	// 初期化処理
	pEffect->Init();

	// インスタンスを返す
	return pEffect;
}