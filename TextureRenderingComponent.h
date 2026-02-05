//****************************************************************
//
// テクスチャ情報のコンポーネント[TextureRenderingComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "texmanager.h"

// テクスチャ情報
struct TexComponent {
	// イニシャライズコンストラクタ
	TexComponent(std::string _Path = {}) : FilePath(_Path) {
		// テクスチャのコピーを取得
		Tex = CLoadTexture::GetTex(FilePath);
	};

	// テクスチャへのパス
	std::string FilePath;
	// テクスチャのコピー
	LPDIRECT3DTEXTURE9 Tex;
};