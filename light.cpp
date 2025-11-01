//****************************************************************
//
// ライトの処理[ligt.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "light.h"
#include "manager.h"

const D3DXVECTOR3 CLight::Config::DefoultDir = { 0.2f,-0.8f,-0.4f };
const D3DXCOLOR CLight::Config::Evening = { 0.7f,0.5f,0.3f ,1.0f };

//***************************************
// コンストラクタ
//***************************************
CLight::CLight()
{
	//ライトをクリアする
	ZeroMemory(&m_Light, sizeof(m_Light));
}

//***************************************
// デストラクタ
//***************************************
CLight::~CLight()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	m_nCntLight = 0;

	D3DXVECTOR3 vecDir[Config::NumLIght];

	for (int nCnt = 0; nCnt < Config::NumLIght; nCnt++)
	{
		// ライトの種類を設定
		// 平行光源
		m_Light[nCnt].Type = D3DLIGHT_DIRECTIONAL;
		// ライトの拡散光を設定
		// ライトの色
		m_Light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//// ライトの方向を設定
		//vecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, 0.8f);
		//// ライトの方向を設定
		//vecDir[2] = D3DXVECTOR3(-1, -0.57f, -0.3f);

		//正規化する（大きさ１のベクトルにする）
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_Light[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_Light[nCnt]);

		//ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}

	// カウンタをインクリメント
	m_nCntLight = Config::NumLIght - 1;

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CLight::Uninit(void)
{
}

//***************************************
// 更新処理
//***************************************
void CLight::Update(void)
{

}

//***************************************
// ライトの設定処理
//***************************************
int CLight::SetLight(D3DLIGHT9 Setter)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ライトを加算
	m_nCntLight++;

	//ライトを設定
	pDevice->SetLight(m_nCntLight, &Setter);

	//ライトを有効にする
	pDevice->LightEnable(m_nCntLight, TRUE);

	// インデックスを返す
	return m_nCntLight;
}

//***************************************
// ライトの削除処理
//***************************************
void CLight::DeleteLight(const int Indx)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライトを有効にする
	pDevice->LightEnable(Indx, FALSE);

	// 総数を減らす
	m_nCntLight--;
}

//***************************************
// 平行光源の色をリセット
//***************************************
void CLight::ResetCol(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir[Config::NumLIght];

	for (int nCnt = 0; nCnt < Config::NumLIght; nCnt++)
	{
		// ライトの種類を設定
		// 平行光源
		m_Light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		// ライトの色
		m_Light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		//// ライトの方向を設定
		//vecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, 0.8f);
		//// ライトの方向を設定
		//vecDir[2] = D3DXVECTOR3(-1, -0.57f, -0.3f);

		//正規化する（大きさ１のベクトルにする）
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_Light[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_Light[nCnt]);

		//ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}
}