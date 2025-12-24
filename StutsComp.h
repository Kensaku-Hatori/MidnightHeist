//****************************************************************
//
// スタッツマネージャーのコンポーネント[StutsComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "PlayerStateComp.hpp"

// 念のため
namespace StutsConfig {
	// スタッツの列挙型
	typedef enum {
		WORD_TYPE_RESULT = 0,	// 成功か失敗か
		WORD_TYPE_TIMERLOGO,	// タイマー用のろご
		WORD_TYPE_ENCOUNTLOGO,	// エンカウント回数のロゴ
		WORD_TYPE_STEPLOGO,		// ステップ数のロゴ
		WORD_TYPE_MAX			// 最大数
	}STUTSWORD;
	// スタッツの列挙型
	typedef enum {
		NUM_TYPE_TIME,			// タイマー
		NUM_TYPE_ENCOUNT,		// エンカウント回数
		NUM_TYPE_STEP,			// ステップ数
		NUM_TYPE_MAX			// 最大数
	}STUTSNUMBER;

	// スタッツの設定情報
	typedef struct {
		const char* Path;
		D3DXVECTOR2 Pos;
		D3DXVECTOR2 Size;
	}StutsWordInfo;
	// スタッツの設定情報
	typedef struct {
		D3DXVECTOR2 Origin;
		D3DXVECTOR2 Size;
		D3DXVECTOR2 Offset;
	}StutsNumberInfo;

	inline static const StutsWordInfo WordTable[STUTSWORD::WORD_TYPE_MAX] = {
		{"data/TEXTURE/Success.png",{SCREEN_WIDTH * 0.85f,SCREEN_HEIGHT * 0.2f},{100.0f,100.0f}},
		{"data/TEXTURE/Time.png",{SCREEN_WIDTH * 0.8f,SCREEN_HEIGHT * 0.4f},{50.0f,50.0f}},
		{"data/TEXTURE/LockingTime.png",{SCREEN_WIDTH * 0.8f,SCREEN_HEIGHT * 0.5f},{50.0f,50.0f}},
		{"data/TEXTURE/StealNum.png",{SCREEN_WIDTH * 0.8f,SCREEN_HEIGHT * 0.6f},{50.0f,50.0f}},
	};
	inline static const StutsNumberInfo NumberTable[STUTSNUMBER::NUM_TYPE_MAX] = {
	{{SCREEN_WIDTH * 0.95f,SCREEN_HEIGHT * 0.4f},{25.0f,40.0f},{50.0f,0.0f}},
	{{SCREEN_WIDTH * 0.95f,SCREEN_HEIGHT * 0.5f},{25.0f,40.0f},{50.0f,0.0f}},
	{{SCREEN_WIDTH * 0.95f,SCREEN_HEIGHT * 0.6f},{25.0f,40.0f},{50.0f,0.0f}},
	};
}