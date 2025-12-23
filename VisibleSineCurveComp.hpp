//****************************************************************
//
// 音の大きさのコンポーネント[EmitterVolumeComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct VisibleSineCurveComp {
    // 引数アリのコンストラクタ
	VisibleSineCurveComp(const float _SineMacro = 0.1f, const float _Speed = 1.0f, const float _Ripple = 1.0f, const float _Radius = 100.0f) :
		SineMacro(_SineMacro), Speed(_Speed), Ripple(_Ripple), Radius(_Radius), nCntSineCurve(0) {};
    // 波の細かさ
	float SineMacro;
	// 波の速さ
	float Speed;
	// 波の波及率
	float Ripple;
	// 波の大きさ
	float Radius;
	// カウンタ
	int nCntSineCurve;
};