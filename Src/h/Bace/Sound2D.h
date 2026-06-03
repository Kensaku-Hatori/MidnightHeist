//================================================================
//
// 2Dサウンドの処理[Sound2D.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "SoundDevice.h"

//****************************************************************
// クラスを定義
//****************************************************************
class CSound2D
{
public:
	// コンストラクタ・デストラクタ
	CSound2D() = default;
	~CSound2D() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	HRESULT Init(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// ラベルを指定して再生
	/// </summary>
	/// <param name="Label"></param>
	void Play(SoundDevice::LABEL Label);
	/// <summary>
	/// ラベルを指定して停止
	/// </summary>
	/// <param name="Label"></param>
	void Stop(SoundDevice::LABEL Label);
	/// <summary>
	/// すべて停止
	/// </summary>
	/// <param name=""></param>
	void StopAll(void);
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CSound2D& Instance(void) {
		static CSound2D Instance;
		return Instance;
	}
private:
	// メンバ変数
	// ソースボイスの配列
	IXAudio2SourceVoice* m_pSourceVoice[SoundDevice::LABEL_MAX];
};