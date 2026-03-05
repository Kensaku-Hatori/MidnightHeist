//****************************************************************
//
// フェードの処理[fade.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "fade.h"
#include "manager.h"
#include "math_T.h"

// 規定値を設定
const D3DXCOLOR CFade::Config::DefoultCol = { 0.0f,0.0f,0.0f,0.0f };

//************************************
// 初期化処理
//************************************
void CFade::Init(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
	}

	//頂点バッファをアンロック
	m_pVertex->Unlock();
}

//************************************
// 終了処理
//************************************
void CFade::Uninit(void)
{
	// 頂点バッファが生成されていたら
	if (m_pVertex != NULL)
	{
		// シングルトンのメンバ変数を破棄
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// シーンをNULLにする
	m_pNexScene = NULL;
}

//************************************
// 更新処理
//************************************
void CFade::Update(void)
{
	// 初期状態以外だったら
	if (m_Fade != FADE_NONE)
	{
		VERTEX_2D* pVtx;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

		// フェードイン
		if (m_Fade == FADE_IN)
		{
			// アルファ値を下げる
			m_colorFade.a = Clamp(m_colorFade.a - Config::Speed, 0.0f, 1.0f);
			// 最小値になったら
			if (m_colorFade.a <= 0.0f)
			{
				m_Fade = FADE_NONE;
			}
		}
		// フェードアウト
		else if (m_Fade == FADE_OUT)
		{
			// アルファ値をあげる
			m_colorFade.a = Clamp(m_colorFade.a + Config::Speed, 0.0f, 1.0f);
			// 最大値になったら
			if (m_colorFade.a >= 1.0f)
			{
				m_Fade = FADE_IN;
				CManager::SetScene(m_pNexScene);
			}
		}
		//頂点カラーの設定
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;

		//頂点バッファをアンロック
		m_pVertex->Unlock();
	}
}

//************************************
// 描画処理
//************************************
void CFade::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//************************************
// フェードと次のシーンを設定
//************************************
void CFade::SetFade(CScene* pNextScene)
{
	// 今のフェードが初期状態じゃなかったら
	if (m_Fade != FADE_NONE)
	{
		// ポインタを破棄し処理を終了
		delete pNextScene;
		pNextScene = nullptr;
		return;
	}
	// 次のシーンがNULLじゃなかったら
	if (m_pNexScene != NULL)
	{
		m_pNexScene = NULL;
	}
	// 次のシーンのポインタとモードを設定
	m_Fade = FADE_OUT;
	m_pNexScene = pNextScene;
}

//************************************
// デストラクタ
//************************************
CFade::~CFade()
{
}