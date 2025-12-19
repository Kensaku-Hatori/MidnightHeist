//****************************************************************
//
// サウンドデバイスの処理[SoundDevice.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "SoundDevice.h"

namespace SoundDevice {
    const SOUNDINFO aSoundInfo[LABEL_MAX] =
    {
        {"data/SOUND/BGM/BGM.wav", XAUDIO2_LOOP_INFINITE},
        {"data/SOUND/SE/ArrowShot.wav", 0},
        {"data/SOUND/SE/StuckArrow.wav", 0},
        {"data/SOUND/SE/WaterFall.wav", XAUDIO2_LOOP_INFINITE},
        {"data/SOUND/SE/MoveSelect.wav", 0},
        {"data/SOUND/SE/On_Ground.wav", 0},
        {"data/SOUND/SE/On_Grass.wav", 0},
        {"data/SOUND/SE/On_Water.wav", 0},
        {"data/SOUND/SE/Enter.wav", 0},
    };
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CSoundDevice::Init(void)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    UINT32 flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    HRESULT hr;

    if (FAILED(hr = XAudio2Create(&m_pXAudio2, flags)))
        return hr;

    if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
    {
        return hr;
    }

    XAUDIO2_DEVICE_DETAILS details;
    if (FAILED(hr = m_pXAudio2->GetDeviceDetails(0, &details)))
    {
        return hr;
    }

    if (details.OutputFormat.Format.nChannels > SoundDevice::OutPutChannels)
    {
        return E_FAIL;
    }

    m_dwChannelMask = details.OutputFormat.dwChannelMask;
    m_nChannels = details.OutputFormat.Format.nChannels;

    flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2FX_DEBUG;
#endif

    if (FAILED(hr = XAudio2CreateReverb(&m_pReverbEffect, flags)))
    {
        return hr;
    }

    XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { m_pReverbEffect, TRUE, 1 } };
    XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

    for (int nCnt = 0; nCnt < SoundDevice::BUS::BUS_MAX; nCnt++)
    {
        if (FAILED(hr = m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoices[nCnt], 1,
            details.OutputFormat.Format.nSamplesPerSec, XAUDIO2_VOICE_USEFILTER, 0,
            NULL, &effectChain)))
        {
            return hr;
        }
    }

    const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

    X3DAudioInitialize(details.OutputFormat.dwChannelMask, SPEEDOFSOUND, m_x3DInstance);

    HANDLE hFile;
    DWORD dwChunkSize = 0;
    DWORD dwChunkPosition = 0;
    DWORD dwFiletype;
    XAUDIO2_BUFFER buffer;

    for (int nCnt = 0; nCnt < SoundDevice::LABEL_MAX; nCnt++)
    {
        // バッファのクリア
        memset(&m_aWaveFMT[nCnt], 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(SoundDevice::aSoundInfo[nCnt].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &m_aWaveFMT[nCnt], dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCnt], &dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }
        m_apDataAudio[nCnt] = (BYTE*)malloc(m_aSizeAudio[nCnt]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCnt], m_aSizeAudio[nCnt], dwChunkPosition);
        if (FAILED(hr))
        {
            return S_FALSE;
        }

        // ファイルをクローズ
        CloseHandle(hFile);
    }

     m_IsInitialized = true;

    return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CSoundDevice::Uninit(void)
{
    if (!m_IsInitialized)
        return;

    // 一時停止
    for (int nCntSound = 0; nCntSound < SoundDevice::LABEL_MAX; nCntSound++)
    {
        if (m_apDataAudio[nCntSound] != NULL)
        {
            // オーディオデータの開放
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    for (int nCnt = 0; nCnt < SoundDevice::BUS_MAX; nCnt++)
    {
        if (m_pSubmixVoices[nCnt] != nullptr)
        {
            m_pSubmixVoices[nCnt]->DestroyVoice();
            m_pSubmixVoices[nCnt] = nullptr;
        }
    }
    if (m_pMasteringVoice != nullptr)
    {
        // マスターボイスの破棄
        m_pMasteringVoice->DestroyVoice();
        m_pMasteringVoice = nullptr;
    }
    if (m_pXAudio2 != nullptr)
    {
        m_pXAudio2->StopEngine();
        m_pXAudio2->Release();
        m_pXAudio2 = nullptr;
    }
    if (m_pReverbEffect != nullptr)
    {
        m_pReverbEffect->Release();
        m_pReverbEffect = nullptr;
    }
    for (int nCntSound = 0; nCntSound < SoundDevice::LABEL_MAX; nCntSound++)
    {
        if (m_apDataAudio[nCntSound] != nullptr)
        {
            delete m_apDataAudio[nCntSound];
            m_apDataAudio[nCntSound] = nullptr;
        }
    }
    // COMライブラリの終了処理
    CoUninitialize();

    m_IsInitialized = false;
}

//*********************************************
// チャンクのチェック
//*********************************************
HRESULT CSoundDevice::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
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
HRESULT CSoundDevice::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
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