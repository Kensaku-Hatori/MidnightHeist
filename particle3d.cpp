//****************************************************************
//
// 3Dparticleの処理[particle3D.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "particle3d.h"
#include "effect3d.h"
#include "math.h"

//*********************************************
// コンストラクタ
//*********************************************
CParticle3D::CParticle3D()
{
	// パーティクルの情報を初期化
	m_ParticleInfo = {};
}

//*********************************************
// デストラクタ
//*********************************************
CParticle3D::~CParticle3D()
{
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CParticle3D::Init(void)
{
	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CParticle3D::Uninit(void)
{
	// 自分自身の破棄
	Release();
}

//*********************************************
// 更新処理
//*********************************************
void CParticle3D::Update(void)
{
	// カウンターを進める
	m_ParticleInfo.Bece.nCounter++;

	// クールダウンを超えたら
	if (m_ParticleInfo.Bece.nCounter > m_ParticleInfo.Bece.nCoolDown)
	{
		// 寿命が尽きていなかったら、ループする場合なら
		if (m_ParticleInfo.Bece.nLife > 0 || m_ParticleInfo.Bece.bLoop == true)
		{
			// 指定された個数分生成
			for (int nCount = 0; nCount < m_ParticleInfo.Bece.nNumEffect; nCount++)
			{
				// <情報をランダムで決める>

				// 寿命
				int Life = rand() % m_ParticleInfo.Bece.nMaxLife + m_ParticleInfo.Bece.nMinLife;

				// 速さ
				float Speed = CMath::randf(m_ParticleInfo.Bece.fMaxSpeed, m_ParticleInfo.Bece.fMinSpeed);

				// 大きさ
				float fRadius = CMath::randf(m_ParticleInfo.MaxRadius, m_ParticleInfo.MinRadius);

				// 重力
				float Gravity = m_ParticleInfo.Bece.Gravity;

				// 移動量を計算
				D3DXVECTOR3 Move;

				// 方向 * 速さ
				Move = CMath::RandVector(m_ParticleInfo.Bece.MaxDir, m_ParticleInfo.Bece.MinDir);
				Move *= Speed;

				// プライオリティが不正な値だったらデフォルトに固定
				if (m_ParticleInfo.Bece.nPriority < 0 || m_ParticleInfo.Bece.nPriority > CObject::Config::Priority) m_ParticleInfo.Bece.nPriority = 3;

				// 生成
				CEffect3D::Create(m_ParticleInfo.Bece.Pos, Move, m_ParticleInfo.Bece.Col, D3DXVECTOR2(fRadius, fRadius), Gravity, Life, m_ParticleInfo.Bece.FilePath,m_ParticleInfo.Bece.nPriority);
			}
		}
		else
		{
			// 自分自身の破棄
			Uninit();

			// 処理の切り上げ
			return;
		}
		// カウンターをリセット、寿命を減らす
		m_ParticleInfo.Bece.nCounter = 0;
		m_ParticleInfo.Bece.nLife--;
	}
}

//*********************************************
// 生成
//*********************************************
CParticle3D* CParticle3D::Create(DefoultEffectInfo ParticleInfo)
{
	// インスタンスを宣言
	CParticle3D* pParticle = NULL;

	// メモリ確保
	pParticle = new CParticle3D;

	// 初期化処理
	pParticle->Init();

	// 情報の設定
	pParticle->m_ParticleInfo = ParticleInfo;
	pParticle->m_ParticleInfo.Bece.nCounter = ParticleInfo.Bece.nCoolDown;

	// インスタンスを返す
	return pParticle;
}


//*********************************************
// チャージ中のエフェクト
//*********************************************
//*********************************************
// コンストラクタ
//*********************************************
CZoneParticle3D::CZoneParticle3D()
{
}

//*********************************************
// デストラクタ
//*********************************************
CZoneParticle3D::~CZoneParticle3D()
{
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CZoneParticle3D::Init(void)
{
	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CZoneParticle3D::Uninit(void)
{
	// 自分自身の破棄
	Release();
}

//*********************************************
// 更新処理
//*********************************************
void CZoneParticle3D::Update(void)
{
	// カウンターを進める
	m_ParticleInfo.ParticleInfo.Bece.nCounter++;

	// クールダウンを超えたら
	if (m_ParticleInfo.ParticleInfo.Bece.nCounter > m_ParticleInfo.ParticleInfo.Bece.nCoolDown)
	{
		// 寿命が尽きていなかったら、ループする場合なら
		if (m_ParticleInfo.ParticleInfo.Bece.nLife > 0 || m_ParticleInfo.ParticleInfo.Bece.bLoop == true)
		{
			// 指定された回数分繰り返す
			for (int nCount = 0; nCount < m_ParticleInfo.ParticleInfo.Bece.nNumEffect; nCount++)
			{
				// <情報をランダムで決める>

				// 位置と方向をランダムで決めるための一時変数
				D3DXVECTOR3 Pos,Dir;

				// 移動方向
				Dir = CMath::RandVector(m_ParticleInfo.ParticleInfo.Bece.MaxDir, m_ParticleInfo.ParticleInfo.Bece.MinDir);

				// 寿命
				int Life = rand() % m_ParticleInfo.ParticleInfo.Bece.nMaxLife + m_ParticleInfo.ParticleInfo.Bece.nMinLife;

				// 速さ
				float Speed = CMath::randf(m_ParticleInfo.ParticleInfo.Bece.fMaxSpeed, m_ParticleInfo.ParticleInfo.Bece.fMinSpeed);

				// 発生位置からの距離
				float Radius = CMath::randf(m_ParticleInfo.fMaxRadius, m_ParticleInfo.fMinRadius);

				// 粒子の大きさ
				float fRadius = CMath::randf(m_ParticleInfo.ParticleInfo.MaxRadius, m_ParticleInfo.ParticleInfo.MinRadius);

				// 重力
				float Gravity = m_ParticleInfo.ParticleInfo.Bece.Gravity;

				// 初期位置を計算
				// 発生位置 + (向き * 距離)
				Pos = m_ParticleInfo.ParticleInfo.Bece.Pos + (Dir * Radius);

				// 移動量とその計算に必要な一時変数を宣言
				D3DXVECTOR3 Move,Vec;

				// 移動方向を計算
				Vec = m_ParticleInfo.DestPos - Pos;

				// 正規化
				D3DXVec3Normalize(&Vec, &Vec);

				// 移動量を計算
				// 方向 * 速さ
				Move = Vec * Speed;

				// 生成
				CEffect3D::Create(Pos, Move, m_ParticleInfo.ParticleInfo.Bece.Col, D3DXVECTOR2(fRadius, fRadius), Gravity, Life, m_ParticleInfo.ParticleInfo.Bece.FilePath);
			}
		}
		else
		{
			// 終了
			Uninit();

			// 処理を切り上げる
			return;
		}
		// カウンターをリセット、寿命を減らす
		m_ParticleInfo.ParticleInfo.Bece.nCounter = 0;
		m_ParticleInfo.ParticleInfo.Bece.nLife--;
	}
}

//*********************************************
// 生成
//*********************************************
CZoneParticle3D* CZoneParticle3D::Create(ZoneEffectInfo ParticleInfo)
{
	// インスタンスを宣言
	CZoneParticle3D* pParticle = NULL;

	// メモリ確保
	pParticle = new CZoneParticle3D;

	// 初期処理
	pParticle->Init();

	// 情報の設定
	pParticle->m_ParticleInfo = ParticleInfo;
	pParticle->m_ParticleInfo.ParticleInfo.Bece.nCounter = ParticleInfo.ParticleInfo.Bece.nCoolDown;

	// インスタンスを返す
	return pParticle;
}