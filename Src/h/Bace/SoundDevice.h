//================================================================
//
// サウンドデバイスの処理[SoundDevice.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// ネームスペース
//****************************************************************
namespace SoundDevice {
	// 定数
	// リバーブのプリセットの数
	static constexpr int NumPreset = 1;
	// リバーブのプリセット
	static const XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS[NumPreset] = {
		XAUDIO2FX_I3DL2_PRESET_DEFAULT,
	};
	// リバーブの種類
	typedef enum
	{
		BUS_DEFAULT = 0,
		BUS_MAX
	}BUS;

	// サウンドの列挙型
	typedef enum
	{
		LABEL_TITLEBGM = 0,	// BGM
		LABEL_GAMEBGM,		// ゲームBGM
		LABEL_GAMECLEARBGM,	// ゲームBGM
		LABEL_GAMEOVERBGM,	// ゲームBGM
		LABEL_PICKING,		// ピッキング
		LABEL_MOVESELECT,	// 選択肢を移動させたとき
		LABEL_ENTER,		// 決定音
		LABEL_ENEMYMOVE,	// 敵の移動音
		LABEL_MAX			// 最大値
	}LABEL;

	// 音のデータ
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	// 音源のデータ
	extern const SOUNDINFO aSoundInfo[SoundDevice::LABEL_MAX];
}

//****************************************************************
// クラスを定義
//****************************************************************
class CSoundDevice
{
public:
	CSoundDevice() = default;
	~CSoundDevice() = default;

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
	/// チャンクチェック
	/// </summary>
	/// <param name="hFile">ファイルハンドル</param>
	/// <param name="format">フォーマット</param>
	/// <param name="pChunkSize">ちゃんくのおおきさ</param>
	/// <param name="pChunkDataPosition">チャンクの位置</param>
	/// <returns>結果</returns>
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	/// <summary>
	/// チャンクを読み込み
	/// </summary>
	/// <param name="hFile">ファイルハンドル</param>
	/// <param name="pBuffer">バッファ</param>
	/// <param name="dwBuffersize">バッファの大きさ</param>
	/// <param name="dwBufferoffset">バッファのオフセット</param>
	/// <returns>結果</returns>
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// ゲッター

	/// <summary>
	/// 2Dサウンドのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline IXAudio2* GetAudio2Device(void) { return m_pXAudio2; }
	/// <summary>
	/// 3Dサウンドのハンドルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ハンドル</returns>
	inline X3DAUDIO_HANDLE* GetAudio3Handle(void) { return &m_x3DInstance; }
	/// <summary>
	/// サブミックスボイスを取得
	/// </summary>
	/// <param name="Bus">プール</param>
	/// <returns>ボイス</returns>
	inline IXAudio2SubmixVoice* GetSubMixVoice(SoundDevice::BUS Bus) { return m_pSubmixVoices[Bus]; }
	/// <summary>
	/// マスターボイスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ボイス</returns>
	inline IXAudio2MasteringVoice* GetMasteringVoice(void) { return m_pMasteringVoice; }
	/// <summary>
	/// リバーブエフェクトを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>リバーブエフェクト</returns>
	inline IUnknown* GetReverbEffect(void) { return m_pReverbEffect; }
	/// <summary>
	/// 音のデータを取得
	/// </summary>
	/// <param name="Label">ラベル</param>
	/// <returns>データ</returns>
	inline BYTE* GetAudioData(SoundDevice::LABEL Label) { return m_apDataAudio[Label]; }
	/// <summary>
	/// 音のサイズを取得
	/// </summary>
	/// <param name="Label">ラベル</param>
	/// <returns>サイズ</returns>
	inline DWORD GetAudioSize(SoundDevice::LABEL Label) { return m_aSizeAudio[Label]; }
	/// <summary>
	/// チャンクマスクを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マスク</returns>
	inline DWORD GetChannelMask(void) { return m_dwChannelMask; }
	/// <summary>
	/// チャンネル数を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャンネル数</returns>
	inline UINT32 GetChannels(void) { return m_nChannels; }
	/// <summary>
	/// 音のフォーマットを取得
	/// </summary>
	/// <param name="Label">ラベル</param>
	/// <returns>フォーマット</returns>
	inline WAVEFORMATEXTENSIBLE GetAudioFMT(SoundDevice::LABEL Label) { return m_aWaveFMT[Label]; }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CSoundDevice& Instance(void) {
		static CSoundDevice Instance;
		return Instance;
	}
private:
    // XAudio2
	// インスタンス
    IXAudio2* m_pXAudio2;
	// マスターボイス
    IXAudio2MasteringVoice* m_pMasteringVoice;
	// サブミックスボイス
	IXAudio2SubmixVoice* m_pSubmixVoices[SoundDevice::BUS_MAX];
	// リバーブ
	IUnknown* m_pReverbEffect;
    // 3D
	// インスタンス
    X3DAUDIO_HANDLE m_x3DInstance;
	// チャンネル系
    DWORD m_dwChannelMask;
    UINT32 m_nChannels;
	// サウンドの情報
	WAVEFORMATEXTENSIBLE m_aWaveFMT[SoundDevice::LABEL_MAX];		// フォーマット
	BYTE* m_apDataAudio[SoundDevice::LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SoundDevice::LABEL_MAX];						// オーディオデータサイズ
	// 初期化フラグ
	bool m_IsInitialized;
};