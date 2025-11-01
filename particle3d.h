//****************************************************************
//
// 3Dparticleの処理[particle3D.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

// インクルード
#include "object.h"

// 3Dparticleクラスを定義
class CParticle3D : public CObject
{
public:
	// デフォルトparticleの構造体
	struct BaceEffectInfo
	{
		D3DXVECTOR3 MaxDir;				// 正の方向の方向ベクトル
		D3DXVECTOR3 MinDir;				// 負の方向ベクトル
		D3DXVECTOR3 Pos;				// 位置
		D3DXCOLOR Col;					// 色
		float fMaxSpeed, fMinSpeed;		// particleの速さ
		float Gravity;					// particleの重力
		int nNumEffect;					// エフェクトの数
		int nMaxLife,nMinLife;			// particleエフェクトの寿命
		int nLife;						// particleの寿命
		int nCoolDown;					// クールダウン
		int nCounter;					// 出現してからのカウンタ
		int nPriority = 3;				// 描画順
		bool bLoop;						// ループするか
		char FilePath[MAX_PATH];		// テクスチャパス
	};

	// デフォルトparticleの構造体
	struct DefoultEffectInfo
	{
		BaceEffectInfo Bece;	// 基盤のエフェクトの情報
		float MaxRadius;		// particleの最大の大きさ
		float MinRadius;		// particleの最小の大きさ
	};

	// コンストラクタ・デストラクタ
	CParticle3D();
	~CParticle3D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	// セッター
	void End(void) { m_ParticleInfo.Bece.bLoop = false; }
	void SetInfo(DefoultEffectInfo Info) { m_ParticleInfo = Info; }
	// ゲッター
	DefoultEffectInfo GetInfo(void) { return m_ParticleInfo; }
	// 静的メンバ関数
	// 生成
	static CParticle3D* Create(DefoultEffectInfo ParticleInfo);
private:
	// メンバ変数
	DefoultEffectInfo m_ParticleInfo;
};

// 3Dparticleクラスを定義
class CZoneParticle3D : public CObject
{
public:
	// デフォルトparticleの構造体
	struct ZoneEffectInfo
	{
		CParticle3D::DefoultEffectInfo ParticleInfo;	// 基本となるエフェクトの情報
		D3DXVECTOR3 DestPos;							// 目標の位置
		float fMaxRadius, fMinRadius;					// 半径
	};

	// コンストラクタ・デストラクタ
	CZoneParticle3D();
	~CZoneParticle3D();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	// 静的メンバ関数
	// 生成
	static CZoneParticle3D* Create(ZoneEffectInfo ParticleInfo);
private:
	// メンバ変数
	ZoneEffectInfo m_ParticleInfo;
};
#endif // !_PARTICLE_H_