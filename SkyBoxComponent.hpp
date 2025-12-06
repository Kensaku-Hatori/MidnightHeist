//****************************************************************
//
// スカイボックスのコンポーネント[SkyBoxComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "manager.h"

// スカイボックスの情報
struct SkyBoxComp {
	// コンスタント
	SkyBoxComp(const std::string& FileDir) {
		// テクスチャがまとめられているファイルを開く
		std::string Path;
		Path = FileDir;
		WIN32_FIND_DATA findFileData;
		HANDLE handle;
		handle = FindFirstFile((Path + "\\*.*").c_str(), &findFileData);

		// ファイルの中を走査
		if (handle != INVALID_HANDLE_VALUE) {
			// 繰り返し
			do {
				// ファイル名が存在しなかったら
				if (strcmp(findFileData.cFileName, ".") == 0 ||
					strcmp(findFileData.cFileName, "..") == 0)
					continue;

				// 相対パスに変換
				std::string FullPath = Path + "\\" + findFileData.cFileName;

				// 面を判断
				if (FullPath.find("dds") == std::string::npos && FullPath.find("png") == std::string::npos && FullPath.find("jpg") == std::string::npos) return;
				if (FullPath.find("negx") != std::string::npos || FullPath.find("xneg") != std::string::npos || FullPath.find("lf") != std::string::npos || FullPath.find("left") != std::string::npos) Faces[1] = FullPath;
				if (FullPath.find("posx") != std::string::npos || FullPath.find("xpos") != std::string::npos || FullPath.find("rt") != std::string::npos || FullPath.find("right") != std::string::npos) Faces[0] = FullPath;
				if (FullPath.find("negy") != std::string::npos || FullPath.find("yneg") != std::string::npos || FullPath.find("dn") != std::string::npos || FullPath.find("down") != std::string::npos || FullPath.find("bottom") != std::string::npos) Faces[3] = FullPath;
				if (FullPath.find("posy") != std::string::npos || FullPath.find("ypos") != std::string::npos || FullPath.find("up") != std::string::npos || FullPath.find("top") != std::string::npos) Faces[2] = FullPath;
				if (FullPath.find("negz") != std::string::npos || FullPath.find("zneg") != std::string::npos || FullPath.find("bk") != std::string::npos || FullPath.find("back") != std::string::npos) Faces[5] = FullPath;
				if (FullPath.find("posz") != std::string::npos || FullPath.find("zpos") != std::string::npos || FullPath.find("ft") != std::string::npos || FullPath.find("front") != std::string::npos) Faces[4] = FullPath;

			} while (FindNextFile(handle, &findFileData));

			// 閉じる
			FindClose(handle);
		}

		// まず1枚のテクスチャからサイズを取得
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(Faces[0].c_str(), &info);

		CRenderer* pRenderer;
		pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		// １枚目のテクスチャの解像度でキューブマップを作成
		pDevice->CreateCubeTexture(
			info.Width,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&pCubeTex,
			NULL
		);

		// 面のテクスチャ
		LPDIRECT3DTEXTURE9 srcTex = nullptr;

		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			// 一面の画像読み込み
			D3DXCreateTextureFromFile(pDevice, Faces[nCnt].c_str(), &srcTex);

			// サーフェイス取得
			IDirect3DSurface9* srcSurf = nullptr;
			srcTex->GetSurfaceLevel(0, &srcSurf);

			// フェイス取得
			IDirect3DSurface9* dstSurf = nullptr;
			pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)nCnt, 0, &dstSurf);

			// 流し込み
			D3DXLoadSurfaceFromSurface(
				dstSurf,
				NULL,
				NULL,
				srcSurf,
				NULL,
				NULL,
				D3DX_DEFAULT,
				0
			);

			// リソース開放
			srcSurf->Release();
			dstSurf->Release();
			srcTex->Release();
		}
	};
	~SkyBoxComp() {
		if (pCubeTex != nullptr)
		{
			pCubeTex->Release();
			pCubeTex = nullptr;
		}
	}
	std::string Faces[6];
	LPDIRECT3DCUBETEXTURE9 pCubeTex;	// テクスチャ
};