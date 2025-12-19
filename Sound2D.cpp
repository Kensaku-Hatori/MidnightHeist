//****************************************************************
//
// 2Dサウンドの処理[Sound2D.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Sound2D.h"
#include "SoundDevice.h"

//*********************************************
// 初期化
//*********************************************
HRESULT CSound2D::Init(void)
{
	HRESULT hr;
	for (int nCnt = 0; nCnt < SoundDevice::LABEL_MAX; nCnt++)
	{
		// フォーマット取得
		WAVEFORMATEXTENSIBLE SetFmt = CSoundDevice::Instance()->GetAudioFMT(static_cast<SoundDevice::LABEL>(nCnt));
		// ソースボイスの生成
		hr = CSoundDevice::Instance()->GetAudio2Device()->CreateSourceVoice(&m_pSourceVoice[nCnt], &SetFmt.Format);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = CSoundDevice::Instance()->GetAudioSize(static_cast<SoundDevice::LABEL>(nCnt));
		buffer.pAudioData = CSoundDevice::Instance()->GetAudioData(static_cast<SoundDevice::LABEL>(nCnt));
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = SoundDevice::aSoundInfo[static_cast<SoundDevice::LABEL>(nCnt)].nCntLoop;

		// オーディオバッファの登録
		m_pSourceVoice[nCnt]->SubmitSourceBuffer(&buffer);
	}

	return S_OK;
}

//*********************************************
// 破棄
//*********************************************
void CSound2D::Uninit(void)
{
	for (int nCnt = 0; nCnt < SoundDevice::LABEL_MAX; nCnt++)
	{
		if (m_pSourceVoice[nCnt] != nullptr)
		{
			m_pSourceVoice[nCnt]->Stop();
			m_pSourceVoice[nCnt]->FlushSourceBuffers();
			m_pSourceVoice[nCnt]->DestroyVoice();
			m_pSourceVoice[nCnt] = nullptr;
		}
	}
}

//*********************************************
// 再生
//*********************************************
void CSound2D::Play(SoundDevice::LABEL Label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = CSoundDevice::Instance()->GetAudioSize(Label);
	buffer.pAudioData = CSoundDevice::Instance()->GetAudioData(Label);
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = SoundDevice::aSoundInfo[Label].nCntLoop;

	// 状態取得
	m_pSourceVoice[Label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_pSourceVoice[Label]->Stop(0);

		// オーディオバッファの削除
		m_pSourceVoice[Label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_pSourceVoice[Label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_pSourceVoice[Label]->Start(0);
}