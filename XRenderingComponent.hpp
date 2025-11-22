//****************************************************************
//
// モデル情報のコンポーネント[XRenderingComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "modelmanager.h"

// X情報
struct XRenderingComp {
	// イニシャライズコンストラクタ
	XRenderingComp(std::string _Path = {}) : FilePath(_Path) {
		Info = CModelManager::GetModelInfo(FilePath);
	};
	// モデル情報へのパス
	std::string FilePath;
	CModelManager::MapObject Info;
};