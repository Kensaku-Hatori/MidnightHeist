//****************************************************************
//
// 2Dサウンドの処理[Sound2D.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "SoundDevice.h"

// クラスを定義
class CSound2D
{
public:
	CSound2D() = default;
	~CSound2D() {
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
	};
	HRESULT Init(void);
	void Uninit(void);
	void Play(SoundDevice::LABEL Label);
	void Stop(SoundDevice::LABEL Label);
	static CSound2D* Instance(void) {
		static std::unique_ptr<CSound2D> Instance = std::make_unique<CSound2D>();
		return Instance.get();
	}
private:
	// メンバ変数
	IXAudio2SourceVoice* m_pSourceVoice[SoundDevice::LABEL_MAX];
};