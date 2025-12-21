//****************************************************************
//
// サウンドデバイスの処理[SoundDevice.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 念のため
namespace SoundDevice {
	static constexpr int InputChannels = 1;
	static constexpr int OutPutChannels = 8;
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
		LABEL_SHOT,			// 放つ音
		LABEL_STUCK,		// 刺さる音
		LABEL_WATERFALL,	// 滝
		LABEL_MOVESELECT,	// 選択肢を移動させたとき
		LABEL_MOVEGROUND,	// 地面上を移動したとき
		LABEL_MOVEGRASS,	// 叢を移動したとき
		LABEL_MOVEWATER,	// 水の上を移動したとき
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

// クラスを定義
class CSoundDevice
{
public:
	CSoundDevice() = default;
	~CSoundDevice() = default;

	HRESULT Init(void);
	void Uninit(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* GetAudio2Device(void) { return m_pXAudio2; }
	X3DAUDIO_HANDLE* GetAudio3Handle(void) { return &m_x3DInstance; }
	IXAudio2SubmixVoice* GetSubMixVoice(SoundDevice::BUS Bus) { return m_pSubmixVoices[Bus]; }
	IXAudio2MasteringVoice* GetMasteringVoice(void) { return m_pMasteringVoice; }
	IUnknown* GetReverbEffect(void) { return m_pReverbEffect; }
	BYTE* GetAudioData(SoundDevice::LABEL Label) { return m_apDataAudio[Label]; }
	DWORD GetAudioSize(SoundDevice::LABEL Label) { return m_aSizeAudio[Label]; }
	DWORD GetChannelMask(void) { return m_dwChannelMask; }
	UINT32 GetChannels(void) { return m_nChannels; }
	WAVEFORMATEXTENSIBLE GetAudioFMT(SoundDevice::LABEL Label) { return m_aWaveFMT[Label]; }
	static CSoundDevice* Instance(void) {
		static std::unique_ptr<CSoundDevice> Instance = std::make_unique<CSoundDevice>();
		return Instance.get();
	}
private:
    // XAudio2
    IXAudio2* m_pXAudio2;
    IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SubmixVoice* m_pSubmixVoices[SoundDevice::BUS_MAX];
	IUnknown* m_pReverbEffect;
    // 3D
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