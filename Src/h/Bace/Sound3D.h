//================================================================
//
// 3Dサウンドの処理[Sound2D.h]
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
class CEmitter
{
public:
	// コンストラクタ・デストラクタ
	CEmitter() = default;
	~CEmitter() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 発生源の位置を設定
	/// </summary>
	/// <param name="Pos">設定したい位置</param>
	inline void SetPos(const D3DXVECTOR3 Pos) { m_emitter.Position = Pos; }
	/// <summary>
	/// 発生源の大の向きを設定
	/// </summary>
	/// <param name="Front">向き</param>
	inline void SetFront(const D3DXVECTOR3 Front) { m_emitter.OrientFront = Front; }
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name=""></param>
	void Play(void);
	/// <summary>
	/// 音源を生成
	/// </summary>
	/// <param name="Label">音の種類</param>
	/// <param name="Pos">初期位置</param>
	/// <returns>音源(戻り値を強制)</returns>
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
	// 破棄されたかどうか
	bool m_IsDestory;
};

//****************************************************************
// クラスを定義
//****************************************************************
class CListener
{
public:
	// コンストラクタ・デストラクタ
	CListener() = default;
	~CListener() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 聞き手の位置を設定
	/// </summary>
	/// <param name="Pos">設定先の位置</param>
	inline void SetPos(const D3DXVECTOR3 Pos) { m_listener.Position = Pos; }
	/// <summary>
	/// 聞き手の向きを設定
	/// </summary>
	/// <param name="Front">向き</param>
	inline void SetFront(const D3DXVECTOR3 Front) { m_listener.OrientFront = Front; }
	/// <summary>
	/// 情報を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline X3DAUDIO_LISTENER GetListener(void) { return m_listener; }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static CListener& Instance(void) {
		static CListener Instance;
		return Instance;
	}
private:
	// メンバ変数
	// 聞き手の情報
	X3DAUDIO_LISTENER m_listener;
	// 聞き手の向き
	X3DAUDIO_CONE m_Listener_DirectionalCone;
};