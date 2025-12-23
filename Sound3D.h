//****************************************************************
//
// 3Dサウンドの処理[Sound2D.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "SoundDevice.h"

// クラスを定義
class CEmitter
{
public:
	// コンストラクタ・デストラクタ
	CEmitter() = default;
	~CEmitter() = default;

	//  メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// セッター
	void SetPos(const D3DXVECTOR3 Pos) { m_emitter.Position = Pos; }
	void SetFront(const D3DXVECTOR3 Front) { m_emitter.OrientFront = Front; }
	// 再生
	void Play(void);

	// 生成(戻り値を強制)
	[[nodiscard]] static CEmitter* Create(SoundDevice::LABEL Label, D3DXVECTOR3 Pos = VEC3_NULL);
private:
	// 音の減衰量
	X3DAUDIO_DISTANCE_CURVE_POINT m_Emitter_LFE_CurvePoints[3];
	X3DAUDIO_DISTANCE_CURVE       m_Emitter_LFE_Curve;
	X3DAUDIO_DISTANCE_CURVE_POINT m_Emitter_Reverb_CurvePoints[2];
	X3DAUDIO_DISTANCE_CURVE       m_Emitter_Reverb_Curve;
    // メンバ変数
	// 自分の音源へのラベル
	SoundDevice::LABEL m_Label;
	// ソースボイス
    IXAudio2SourceVoice* m_pSourceVoice;
	// どのように音が聞こえるかの設定
    X3DAUDIO_DSP_SETTINGS m_dspSettings;
	// 音の発生源
    X3DAUDIO_EMITTER m_emitter;
	//発生源の向き
    X3DAUDIO_CONE m_emitterCone;
	// 発生源のチャンネルごとの向き
	std::vector<FLOAT32> m_emitterAzimuths;
	// 計算結果を出力デバイスに送る用
	std::vector<FLOAT32> m_matrixCoefficients;
	// ソースボイスのチャンネル数
	UINT32 m_SourceChannels;
};

// クラスを定義
class CListener
{
public:
	// コンストラクタ・デストラクタ
	CListener() = default;
	~CListener() = default;

	//  メンバ関数
	HRESULT Init(void);
	void Update(void);

	// セッター
	void SetPos(const D3DXVECTOR3 Pos) { m_listener.Position = Pos; }
	void SetFront(const D3DXVECTOR3 Front) { m_listener.OrientFront = Front; }

	// ゲッター
	X3DAUDIO_LISTENER GetListener(void) { return m_listener; }

	// 静的メンバ関数
	static CListener* Instance(void) {
		static std::unique_ptr<CListener> Instance = std::make_unique<CListener>();
		return Instance.get();
	}
private:
	// メンバ変数
	// 聞き手の情報
	X3DAUDIO_LISTENER m_listener;
	// 聞き手の向き
	X3DAUDIO_CONE m_Listener_DirectionalCone;
};