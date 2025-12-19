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
	CEmitter() = default;
	~CEmitter() {
		if (m_pSourceVoice != nullptr)
		{
			m_pSourceVoice->Stop();
			m_pSourceVoice->FlushSourceBuffers();
			m_pSourceVoice->DestroyVoice();
			m_pSourceVoice = nullptr;
		}
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// セッター
	void SetPos(const D3DXVECTOR3 Pos) { m_emitter.Position = Pos; }
	void SetFront(const D3DXVECTOR3 Front) { m_emitter.OrientFront = Front; }
	// 再生
	void Play(void);

	// 生成
	[[nodiscard]] static CEmitter* Create(SoundDevice::LABEL Label, D3DXVECTOR3 Pos = VEC3_NULL);
private:
    // XAudio2
	SoundDevice::LABEL m_Label;
    IXAudio2SourceVoice* m_pSourceVoice;

    X3DAUDIO_DSP_SETTINGS m_dspSettings;
    X3DAUDIO_EMITTER m_emitter;
    X3DAUDIO_CONE m_emitterCone;

	X3DAUDIO_DISTANCE_CURVE_POINT m_Emitter_LFE_CurvePoints[3];
	X3DAUDIO_DISTANCE_CURVE       m_Emitter_LFE_Curve;
	X3DAUDIO_DISTANCE_CURVE_POINT m_Emitter_Reverb_CurvePoints[3];
	X3DAUDIO_DISTANCE_CURVE       m_Emitter_Reverb_Curve;

    FLOAT32 m_emitterAzimuths[SoundDevice::InputChannels];
	FLOAT32 m_matrixCoefficients[SoundDevice::InputChannels * SoundDevice::OutPutChannels];
};

// クラスを定義
class CListener
{
public:
	CListener() = default;
	~CListener() = default;

	HRESULT Init(void);
	void Update(void);
	void SetPos(const D3DXVECTOR3 Pos) { m_listener.Position = Pos; }
	void SetFront(const D3DXVECTOR3 Front) { m_listener.OrientFront = Front; }
	X3DAUDIO_LISTENER GetListener(void) { return m_listener; }
	static CListener* Instance(void) {
		static std::unique_ptr<CListener> Instance = std::make_unique<CListener>();
		return Instance.get();
	}
private:
	// XAudio2
	X3DAUDIO_LISTENER m_listener;
	X3DAUDIO_CONE m_Listener_DirectionalCone;
};