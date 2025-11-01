//*************************************
//
//　サウンド処理[sound.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "sound.h"

// サウンドの情報を設定(静的メンバ変数)
std::vector<CEmitter*> CSound::m_vEmitter;
IXAudio2* CSound::m_pXAudio2;
IXAudio2SourceVoice* CSound::m_apSourceVoice[LABEL_MAX];
WAVEFORMATEXTENSIBLE CSound::m_aWaveFMT[LABEL_MAX];
IXAudio2MasteringVoice* CSound::m_pMasteringVoice;
BYTE* CSound::m_apDataAudio[LABEL_MAX];
DWORD CSound::m_aSizeAudio[LABEL_MAX];
X3DAUDIO_HANDLE CSound::m_x3dInstance;
X3DAUDIO_LISTENER CSound::m_Listener;
CSound::SOUNDINFO CSound::m_aSoundInfo[CSound::LABEL_MAX] =
{
	// 0がループしない、-1だったらループする
	{"data/SOUND/BGM/BGM.wav", -1},			// BGM1
	{"data/SOUND/SE/ArrowShot.wav", 0},		// SE1
	{"data/SOUND/SE/StuckArrow.wav", 0},	// SE1
	{"data/SOUND/SE/WaterFall.wav", -1},	// SE1
	{"data/SOUND/SE/MoveSelect.wav", 0}	,	// SE1
	{"data/SOUND/SE/On_Ground.wav", 0}	,	// SE1
	{"data/SOUND/SE/On_Grass.wav", 0}	,	// SE1
	{"data/SOUND/SE/On_Water.wav", 0}	,	// SE1
	{"data/SOUND/SE/Enter.wav", 0}	,		// SE1
};	

//*********************************************
// コンストラクタ
//*********************************************
CSound::CSound()
{
	for (int nCount = 0; nCount < LABEL_MAX; nCount++)
	{
		m_apDataAudio[nCount] = NULL;
		m_apSourceVoice[nCount] = NULL;
		m_aSizeAudio[nCount] = NULL;
	}
	m_pMasteringVoice = NULL;
	m_pXAudio2 = NULL;
}

//*********************************************
// デストラクタ
//*********************************************
CSound::~CSound()
{

}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// X3DAudio初期化（マスターボイス生成後）
	// スピーカー設定を取得して初期化
	XAUDIO2_DEVICE_DETAILS details;
	m_pXAudio2->GetDeviceDetails(0, &details);
	X3DAudioInitialize(details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dInstance);

	// リスナー初期化
	ZeroMemory(&m_Listener, sizeof(X3DAUDIO_LISTENER));
	m_Listener.OrientFront = { 0.0f, 0.0f, -1.0f };
	m_Listener.OrientTop = { 0.0f, 1.0f, 0.0f };
	m_Listener.Position = { 0.0f, 0.0f, 0.0f };
	m_Listener.Velocity = { 0.0f, 0.0f, 0.0f };

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&m_aWaveFMT[nCntSound], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &m_aWaveFMT[nCntSound], dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(m_aWaveFMT[nCntSound].Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	for (auto emitter : m_vEmitter)
	{
		if (emitter) emitter->Uninit();
	}
	m_vEmitter.clear();

	// COMライブラリの終了処理
	CoUninitialize();
}

void CSound::SetListenerPos(D3DXVECTOR3 Pos)
{
	m_Listener.Position = Pos;
}

//*********************************************
// 音を流す
//*********************************************
HRESULT CSound::Play(CSound::LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//*********************************************
// ボリュームを指定して
//*********************************************
HRESULT CSound::Play(LABEL label, const float Distance,const float Range)
{
	// 範囲からの差分
	float LocalVolume = Range - Distance;

	// 範囲外だったら早期リターン
	if (LocalVolume < 0.0f) return E_FAIL;

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 割合で音量を計算
	LocalVolume = LocalVolume / Range;

	// 音量設定
	m_apSourceVoice[label]->SetVolume(LocalVolume);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//*********************************************
// 音量を途中で変える
//*********************************************
HRESULT CSound::SetVolume(LABEL label, const float Distance, const float Range)
{
	// 範囲からの差分
	float LocalVolume = Range - Distance;

	// 範囲外だったら早期リターン
	if (LocalVolume < 0.0f)
	{
		// 音量設定
		m_apSourceVoice[label]->SetVolume(0.0f);

		return E_FAIL;
	}

	// 割合で音量を計算
	LocalVolume = LocalVolume / Range;

	// 音量設定
	m_apSourceVoice[label]->SetVolume(LocalVolume);

	return S_OK;
}

//*********************************************
// ラベルを指定して音を止める
//*********************************************
void CSound::Stop(CSound::LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//*********************************************
// 音を全部止める
//*********************************************
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//*********************************************
// 音が流れているかどうか
//*********************************************
bool CSound::IsPlay(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	// 再生中じゃない
	if (xa2state.BuffersQueued == 0)
	{
		return false;
	}
	// 再生中
	return true;
}

//*********************************************
// チャンクのチェック
//*********************************************
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//*********************************************
// チャンクデータの読み込み
//*********************************************
HRESULT CSound::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

std::unique_ptr<CEmitter> CSound::CreateEmitter(LABEL  Label)
{
	auto emitter = std::make_unique<CEmitter>();

	XAUDIO2_VOICE_DETAILS details;
	m_pMasteringVoice->GetVoiceDetails(&details);

	emitter->Init(m_pXAudio2, &m_aWaveFMT[Label], details.InputChannels);

	return emitter; // 所有権を呼び出し側へ渡す}
}

CEmitter::CEmitter()
{
}

CEmitter::~CEmitter()
{
}

HRESULT CEmitter::Init(IXAudio2* XAudio, const WAVEFORMATEXTENSIBLE* pFormat,UINT DstChannelCnt)
{
	HRESULT hr = XAudio->CreateSourceVoice(&m_Emitter.pSourceVoice, (const WAVEFORMATEX*)pFormat);
	if (FAILED(hr)) return hr;

	// エミッター初期化
	ZeroMemory(&m_Emitter.Emitter, sizeof(X3DAUDIO_EMITTER));
	m_Emitter.Emitter.Position = { 0.0f, 0.0f, 0.0f };
	m_Emitter.Emitter.OrientFront = { 0.0f, 0.0f, 1.0f };
	m_Emitter.Emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
	m_Emitter.Emitter.Velocity = { 0.0f, 0.0f, 0.0f };
	m_Emitter.Emitter.ChannelCount = 1;
	m_Emitter.Emitter.InnerRadius = 1.0;        // 距離減衰開始まで非常に短く
	m_Emitter.Emitter.InnerRadiusAngle = X3DAUDIO_PI / 4;   // 正面補正なし（左右差が出る）
	m_Emitter.Emitter.CurveDistanceScaler = 10.0f; // 距離減衰のスケール

	// DSP設定初期化
	ZeroMemory(&m_Emitter.DspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
	m_Emitter.DspSettings.SrcChannelCount = 1;
	m_Emitter.DspSettings.DstChannelCount = DstChannelCnt;
	m_Emitter.DspSettings.pMatrixCoefficients = m_Emitter.m_Mtx;

	return S_OK;
}

void CEmitter::Uninit(void)
{
	if (m_Emitter.pSourceVoice)
	{
		m_Emitter.pSourceVoice->DestroyVoice();
		m_Emitter.pSourceVoice = nullptr;
	}
}

void CEmitter::Stop(void)
{
	if (m_Emitter.pSourceVoice)
	{
		m_Emitter.pSourceVoice->Stop();
		m_Emitter.pSourceVoice->FlushSourceBuffers();
	}
}

void CEmitter::Play(X3DAUDIO_HANDLE Handle, X3DAUDIO_LISTENER& Listener, BYTE* AudioData, DWORD AudioSize)
{
	if (!m_Emitter.pSourceVoice) return;

	// 再生中なら一旦停止
	Stop();

	// 音データをセット
	XAUDIO2_BUFFER buffer = {};
	buffer.AudioBytes = AudioSize;
	buffer.pAudioData = AudioData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	m_Emitter.pSourceVoice->SubmitSourceBuffer(&buffer);

	if (!m_Emitter.DspSettings.pMatrixCoefficients) return;
	if (m_Emitter.DspSettings.SrcChannelCount == 0 || m_Emitter.DspSettings.DstChannelCount == 0) return;

	// --- 立体音響計算 ---
	X3DAudioCalculate(
		Handle,
		&Listener,
		&m_Emitter.Emitter,
		X3DAUDIO_CALCULATE_MATRIX |
		X3DAUDIO_CALCULATE_DOPPLER |
		X3DAUDIO_CALCULATE_LPF_DIRECT,
		&m_Emitter.DspSettings
	);

	// 出力マトリクスを設定（左右定位）
	m_Emitter.pSourceVoice->SetOutputMatrix(
		nullptr,
		m_Emitter.DspSettings.SrcChannelCount,
		m_Emitter.DspSettings.DstChannelCount,
		m_Emitter.DspSettings.pMatrixCoefficients
	);

	// ドップラー効果を反映
	m_Emitter.pSourceVoice->SetFrequencyRatio(m_Emitter.DspSettings.DopplerFactor);

	// 再生開始
	m_Emitter.pSourceVoice->Start();
}

void CEmitter::Update(X3DAUDIO_HANDLE Handle, X3DAUDIO_LISTENER& Listener)
{
	// --- 立体音響計算 ---
	X3DAudioCalculate(
		Handle,
		&Listener,
		&m_Emitter.Emitter,
		X3DAUDIO_CALCULATE_MATRIX |
		X3DAUDIO_CALCULATE_DOPPLER |
		X3DAUDIO_CALCULATE_LPF_DIRECT,
		&m_Emitter.DspSettings
	);

	// 出力マトリクスを設定（左右定位）
	m_Emitter.pSourceVoice->SetOutputMatrix(
		nullptr,
		m_Emitter.DspSettings.SrcChannelCount,
		m_Emitter.DspSettings.DstChannelCount,
		m_Emitter.DspSettings.pMatrixCoefficients
	);

	// ドップラー効果を反映
	m_Emitter.pSourceVoice->SetFrequencyRatio(m_Emitter.DspSettings.DopplerFactor);

	CDebugProc::print("L: %f  R: %f\n",
		m_Emitter.DspSettings.pMatrixCoefficients[0],
		m_Emitter.DspSettings.pMatrixCoefficients[1]);
}

void CEmitter::SetPosition(D3DXVECTOR3 Pos)
{
	m_Emitter.Emitter.Position = { Pos.x, Pos.y, Pos.z };
}