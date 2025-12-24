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

	// 最大数分繰り返す
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

		// サブミックスボイス登録用のバッファ
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
	// 最大数分繰り返す
	for (int nCnt = 0; nCnt < SoundDevice::LABEL_MAX; nCnt++)
	{
		// 作られていたら
		if (m_pSourceVoice[nCnt] != nullptr)
		{
			// 破棄
			m_pSourceVoice[nCnt]->Stop();
			m_pSourceVoice[nCnt]->FlushSourceBuffers();
			m_pSourceVoice[nCnt]->DestroyVoice();
			m_pSourceVoice[nCnt] = nullptr;
		}
	}
	// 別スレッドで完全に破棄されるまで待つ
	Sleep(100 * SoundDevice::LABEL_MAX);
}

//*********************************************
// 再生
//*********************************************
void CSound2D::Play(SoundDevice::LABEL Label)
{
	// ソースボイスのステート取得用変数
	XAUDIO2_VOICE_STATE xa2state;
	// サブミックスボイス登録用バッファ
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

//*********************************************
// 停止
//*********************************************
void CSound2D::Stop(SoundDevice::LABEL Label)
{
	// ソースボイスのステート取得用変数
	XAUDIO2_VOICE_STATE xa2state;
	// 状態取得
	m_pSourceVoice[Label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_pSourceVoice[Label]->Stop(0);
	}
}

//*********************************************
// すべて停止
//*********************************************
void CSound2D::StopAll(void)
{
	// ソースボイスのステート取得用変数
	XAUDIO2_VOICE_STATE xa2state;
	// 最大数分繰り返す
	for (int nCnt = 0; nCnt < SoundDevice::LABEL_MAX; nCnt++)
	{
		// 状態取得
		m_pSourceVoice[nCnt]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_pSourceVoice[nCnt]->Stop(0);
		}
	}
}