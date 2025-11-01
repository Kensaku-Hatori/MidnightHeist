//****************************************************************
//
// 線描がの処理[Line.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "line.h"
#include "manager.h"

// 名前空間
using namespace std;

//***************************************
// コンストラクタ
//***************************************
CLine::CLine()
{
	// 配列の中を空にする
	m_Vertex.clear();
}

//***************************************
// デストラクタ
//***************************************
CLine::~CLine()
{
}

//***************************************
// 配列のリセット処理
//***************************************
void CLine::Clear(void)
{
	// 配列の中を空にする
	m_Vertex.clear();
}

//***************************************
// 線を追加
//***************************************
void CLine::AddLine(const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DCOLOR Col)
{
	// 後ろから配列に追加
	m_Vertex.push_back({ P1, Col });
	m_Vertex.push_back({ P2, Col });
}

//***************************************
// 描画処理
//***************************************
void CLine::Draw(void)
{
	// 配列の中身が存在しなかったら
	if (m_Vertex.empty()) return;

	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 頂点情報を流し込む
	pDevice->SetFVF(FVF_VERTEX_LINE);

	// 描画
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, (UINT)(m_Vertex.size() * 0.5f), m_Vertex.data(), sizeof(VERTEX_LINE));

	// ライトを有効に治す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//***************************************
// インスタンスの生成
//***************************************
unique_ptr<CLine> CLine::Create(void)
{
	// unique_ptr で直接生成して返す
	return make_unique<CLine>();
}