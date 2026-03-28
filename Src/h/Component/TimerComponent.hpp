//****************************************************************
//
// タイマーのコンポーネント[TimerComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct TimerComponent {
	// 引数アリのコンストラクタ
	TimerComponent(const int _nData = 1, const D3DXVECTOR2 _DigitSize = { 50.0f,100.0f }, const D3DXVECTOR2 _DigitOffset = { 100.0f,0.0f }) :
		nDigitNum(0), nData(_nData), DigitSize(_DigitSize), DigitOffset(_DigitOffset) {};
	// 桁数
	int nDigitNum;
	// タイマー(フレーム数)
	int nData;
	// 桁の大きさ
	D3DXVECTOR2 DigitSize;
	// 桁の間隔
	D3DXVECTOR2 DigitOffset;
};