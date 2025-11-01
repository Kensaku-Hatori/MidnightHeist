//*************************************
//
//　サウンド処理[sound.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二十インクルード防止
#ifndef _SOUND_H_
#define _SOUND_H_

// インクルード
#include "renderer.h"

// サウンドクラスを定義
class CEmitter
{
public:
	struct Emitter {
		X3DAUDIO_EMITTER Emitter;			// 音源（敵など）
		X3DAUDIO_DSP_SETTINGS DspSettings = {};	// DSP設定（定位・ドップラーなど）
		IXAudio2SourceVoice* pSourceVoice = nullptr;
		FLOAT32 m_Mtx[XAUDIO2_MAX_AUDIO_CHANNELS * XAUDIO2_MAX_AUDIO_CHANNELS] = {};
	};

	// コンストラクタ・デストラクタ
	CEmitter();
	~CEmitter();

	// メンバ関数
	HRESULT Init(IXAudio2* XAudio, const WAVEFORMATEXTENSIBLE* pFormat, UINT DstChannelCnt);
	void Uninit(void);
	void Update(X3DAUDIO_HANDLE Handle, X3DAUDIO_LISTENER& Listener);
	void Play(X3DAUDIO_HANDLE Handle, X3DAUDIO_LISTENER& Listener, BYTE* AudioData, DWORD AudioSize);
	void Stop(void);
	void SetPosition(D3DXVECTOR3 Pos);
private:
	// メンバ変数
	// 3D音響用
	Emitter m_Emitter;
};

// サウンドクラスを定義
class CSound
{
public:
	// サウンドの列挙型
	typedef enum
	{
		LABEL_BGM = 0,		// BGM
		LABEL_SHOT,			// 放つ音
		LABEL_STUCK,		// 刺さる音
		LABEL_WATERFALL,	// 滝
		LABEL_MOVESELECT,	// 選択肢を移動させたとき
		LABEL_MOVEGROUND,	// 地面上を移動したとき
		LABEL_MOVEGRASS,	// 叢を移動したとき
		LABEL_MOVEWATER,	// 水の上を移動したとき
		LABEL_ENTER,		// 決定音
		LABEL_MAX			// 最大値
	}LABEL;

	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	// コンストラクタ・デストラクタ
	CSound();
	~CSound();

	// メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void SetListenerPos(D3DXVECTOR3 Pos);

	// セッター
	HRESULT Play(LABEL label);
	HRESULT Play(LABEL label, const float Distance, const float Range);
	HRESULT SetVolume(LABEL label, const float Distance, const float Range);
	void Stop(LABEL label);
	void Stop(void);

	// ゲッター
	bool IsPlay(LABEL label);

	// 静的メンバ関数
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static X3DAUDIO_HANDLE* Get3DHandle(void) { return &m_x3dInstance; }
	static X3DAUDIO_LISTENER& GetListener(void) { return m_Listener; }
	static BYTE* GetAudioData(LABEL Label) { return m_apDataAudio[Label]; }
	static DWORD GetAudioSize(LABEL Label) { return m_aSizeAudio[Label]; }
	[[nodiscard]] static std::unique_ptr<CEmitter> CreateEmitter(LABEL  Label);
private:
	// 静的メンバ変数
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];				// サウンドの情報
	// メンバ変数
	static X3DAUDIO_HANDLE m_x3dInstance;						// X3DAudioのインスタンス
	static X3DAUDIO_LISTENER m_Listener;						// プレイヤー（聞き手）
	static std::vector<CEmitter*> m_vEmitter;
	static IXAudio2* m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice* m_pMasteringVoice;		// マスターボイス
	static IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX];	// ソースボイス
	static WAVEFORMATEXTENSIBLE m_aWaveFMT[LABEL_MAX];
	static BYTE* m_apDataAudio[LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
};
#endif // !_SOUND_H_