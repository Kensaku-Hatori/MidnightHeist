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
	// コンストラクタ・デストラクタ
	CSound2D() = default;
	~CSound2D() = default;

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);

	// 再生
	void Play(SoundDevice::LABEL Label);
	// 停止
	void Stop(SoundDevice::LABEL Label);
	void StopAll(void);

	// 静的メンバ関数
	static CSound2D* Instance(void) {
		static std::unique_ptr<CSound2D> Instance = std::make_unique<CSound2D>();
		return Instance.get();
	}
private:
	// メンバ変数
	// ソースボイスの配列
	IXAudio2SourceVoice* m_pSourceVoice[SoundDevice::LABEL_MAX];
};