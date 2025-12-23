//****************************************************************
//
// 3Dサウンドの処理[Sound2D.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Sound3D.h"

// 定数
// 
//*********************************************
// 初期化
//*********************************************
HRESULT CEmitter::Init(void)
{
	HRESULT hr;

	// LFEカーブ
	m_Emitter_LFE_CurvePoints[0] = { 0.0f,1.0f };
	m_Emitter_LFE_CurvePoints[1] = { 0.25f,0.0f };
	m_Emitter_LFE_CurvePoints[2] = { 1.0f,0.0f };
	m_Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&m_Emitter_LFE_CurvePoints[0], 3 };

	// REVERBカーブ
	m_Emitter_Reverb_CurvePoints[0] = { 0.0f,1.0f };
	m_Emitter_Reverb_CurvePoints[1] = { 1.0f,0.0f };
	m_Emitter_Reverb_Curve = { m_Emitter_Reverb_CurvePoints, 2 };

	// リスナー取得
	X3DAUDIO_LISTENER Listener = CListener::Instance()->GetListener();
	// マスターボイス・サブミックスボイスを取得
	IXAudio2MasteringVoice* MasteringVoice = CSoundDevice::Instance()->GetMasteringVoice();
	IXAudio2SubmixVoice* SubMixVoice = CSoundDevice::Instance()->GetSubMixVoice(SoundDevice::BUS_DEFAULT);
	// フォーマット取得
	WAVEFORMATEXTENSIBLE SetFmt = CSoundDevice::Instance()->GetAudioFMT(m_Label);
	m_SourceChannels = SetFmt.Format.nChannels;

	XAUDIO2_SEND_DESCRIPTOR sendDesc[2] = {
		{ XAUDIO2_SEND_USEFILTER, MasteringVoice },
		{ XAUDIO2_SEND_USEFILTER, SubMixVoice }
	};
	XAUDIO2_VOICE_SENDS sendList = { 2, sendDesc };

	// ソースボイスの生成
	hr = CSoundDevice::Instance()->GetAudio2Device()->CreateSourceVoice(&m_pSourceVoice, &SetFmt.Format, XAUDIO2_VOICE_USEFILTER,
		XAUDIO2_DEFAULT_FREQ_RATIO, NULL, &sendList);
	if (FAILED(hr))
	{
		return S_FALSE;
	}

	// 出力フォーマットを取得
	XAUDIO2_VOICE_DETAILS details;
	MasteringVoice->GetVoiceDetails(&details);

	int AzimuthNum = SetFmt.Format.nChannels;
	int CoefficientNum = SetFmt.Format.nChannels * details.InputChannels;

	m_emitterAzimuths.reserve(AzimuthNum);
	m_matrixCoefficients.reserve(CoefficientNum);

	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = CSoundDevice::Instance()->GetAudioSize(m_Label);
	buffer.pAudioData = CSoundDevice::Instance()->GetAudioData(m_Label);
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = SoundDevice::aSoundInfo[m_Label].nCntLoop;

	// オーディオバッファの登録
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	m_emitter.pCone = &m_emitterCone;
	m_emitter.pCone->InnerAngle = 0.0f;
	// Setting the inner cone angles to X3DAUDIO_2PI and
	// outer cone other than 0 causes
	// the emitter to act like a point emitter using the
	// INNER cone settings only.
	m_emitter.pCone->OuterAngle = 0.0f;
	// Setting the outer cone angles to zero causes
	// the emitter to act like a point emitter using the
	// OUTER cone settings only.
	m_emitter.pCone->InnerVolume = 0.0f;
	m_emitter.pCone->OuterVolume = 1.0f;
	m_emitter.pCone->InnerLPF = 0.0f;
	m_emitter.pCone->OuterLPF = 1.0f;
	m_emitter.pCone->InnerReverb = 0.0f;
	m_emitter.pCone->OuterReverb = 1.0f;

	m_emitter.OrientFront = D3DXVECTOR3(0, 0, 1);
	m_emitter.OrientTop = D3DXVECTOR3(0, 1, 0);
	m_emitter.ChannelCount = m_SourceChannels;
	m_emitter.ChannelRadius = 1.0f;
	m_emitter.pChannelAzimuths = m_emitterAzimuths.data();

	// Use of Inner radius allows for smoother transitions as
	// a sound travels directly through, above, or below the listener.
	// It also may be used to give elevation cues.
	m_emitter.InnerRadius = 2.0f;
	m_emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;;

	m_emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	m_emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&m_Emitter_LFE_Curve;
	m_emitter.pLPFDirectCurve = NULL; // use default curve
	m_emitter.pLPFReverbCurve = NULL; // use default curve

	m_emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&m_Emitter_Reverb_Curve;
	m_emitter.CurveDistanceScaler = 300.0f;
	m_emitter.DopplerScaler = 1.0f;

	m_dspSettings.SrcChannelCount = m_SourceChannels;
	m_dspSettings.DstChannelCount = CSoundDevice::Instance()->GetChannels();
	m_dspSettings.pMatrixCoefficients = m_matrixCoefficients.data();
	m_dspSettings.DopplerFactor = 1.0f;
	m_dspSettings.ReverbLevel = 1.0f;

	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CEmitter::Uninit(void)
{
	if (m_pSourceVoice != nullptr)
	{
		m_pSourceVoice->Stop();
		m_pSourceVoice->FlushSourceBuffers();
		m_pSourceVoice->DestroyVoice();
		// 別スレッドで完全に破棄されるまで待つ
		Sleep(100);
		m_pSourceVoice = nullptr;
	}
	m_matrixCoefficients.clear();
	m_emitterAzimuths.clear();
	delete this;
}

//*********************************************
// 更新処理
//*********************************************
void CEmitter::Update(void)
{
	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
		| X3DAUDIO_CALCULATE_REVERB;
	//if (m_AudioState.fUseRedirectToLFE)
	//{
	//	// On devices with an LFE channel, allow the mono source data
	//	// to be routed to the LFE destination channel.
	//	dwCalcFlags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
	//}

	if (m_dspSettings.pMatrixCoefficients == nullptr) return;

	X3DAUDIO_HANDLE* AudioHandle = CSoundDevice::Instance()->GetAudio3Handle();
	X3DAUDIO_LISTENER Listener = CListener::Instance()->GetListener();
	IXAudio2MasteringVoice* MasteringVoice = CSoundDevice::Instance()->GetMasteringVoice();
	IXAudio2SubmixVoice* SubMixVoice = CSoundDevice::Instance()->GetSubMixVoice(SoundDevice::BUS_DEFAULT);

	X3DAudioCalculate(*AudioHandle, &Listener, &m_emitter, dwCalcFlags,
		&m_dspSettings);

	if (m_pSourceVoice != nullptr)
	{
		// Apply X3DAudio generated DSP settings to XAudio2
		m_pSourceVoice->SetFrequencyRatio(m_dspSettings.DopplerFactor);
		m_pSourceVoice->SetOutputMatrix(MasteringVoice, m_SourceChannels, CSoundDevice::Instance()->GetChannels(),
			m_matrixCoefficients.data());

		m_pSourceVoice->SetOutputMatrix(
			SubMixVoice,
			m_SourceChannels,
			1,
			&m_dspSettings.ReverbLevel
		);
		XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_dspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		m_pSourceVoice->SetOutputFilterParameters(MasteringVoice, &FilterParametersDirect);
		XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_dspSettings.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		m_pSourceVoice->SetOutputFilterParameters(SubMixVoice, &FilterParametersReverb);
	}
}

//*********************************************
// 再生
//*********************************************
void CEmitter::Play(void)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = CSoundDevice::Instance()->GetAudioSize(m_Label);
	buffer.pAudioData = CSoundDevice::Instance()->GetAudioData(m_Label);
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = SoundDevice::aSoundInfo[m_Label].nCntLoop;

	// 状態取得
	m_pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_pSourceVoice->Stop(0);

		// オーディオバッファの削除
		m_pSourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_pSourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	m_pSourceVoice->Start(0);
}

//*********************************************
// 生成
//*********************************************
CEmitter* CEmitter::Create(SoundDevice::LABEL Label, D3DXVECTOR3 Pos)
{
	CEmitter* Instance = new CEmitter;
	Instance->m_Label = Label;
	Instance->m_emitter.Position = Pos;
	Instance->Init();
	Instance->Update();
	return Instance;
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CListener::Init(void)
{
	m_Listener_DirectionalCone = { X3DAUDIO_PI * 3.0f, X3DAUDIO_PI, 1.0f, 0.8f, 0.0f, 0.2f, 0.0f, 1.0f };
	m_listener.Position = VEC3_NULL;
	m_listener.OrientFront = D3DXVECTOR3(0, 0, 1);
	m_listener.OrientTop = D3DXVECTOR3(0, 1, 0);
	m_listener.pCone = (X3DAUDIO_CONE*)&m_Listener_DirectionalCone;
	return E_NOTIMPL;
}