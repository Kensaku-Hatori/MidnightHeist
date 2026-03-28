//****************************************************************
//
// ゲートマネージャーコンポーネント[GateManagerComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// ゲートマネージャーの情報
struct GateManagerComponent {
	// コンストラクタ
	GateManagerComponent() : m_IsOpenTrigger(false) {};
	// カプセル情報
	bool m_IsOpenTrigger;
};