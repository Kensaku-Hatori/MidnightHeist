//================================================================
//
//　プリコンパイル済みヘッター[pch.h]
//　Author:Hatori Kensaku
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// ビルド時警告対処マクロ
//****************************************************************
#define _CRT_SECURE_NO_WARNINGS

//****************************************************************
// インクルード
//****************************************************************
#include<Windows.h>
#include "d3dx9.h"

//****************************************************************
// ビルド時警告対処マクロ
//****************************************************************
#define DIRECTINPUT_VERSION (0x0800)

//****************************************************************
// インクルード
//****************************************************************
#include<stdio.h>

//****************************************************************
// DirectX系
//****************************************************************
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"
#include "xaudio2fx.h"
#include "x3daudio.h"

//****************************************************************
// ライブラリ
//****************************************************************
#include "json.hpp"
#include "entt.hpp"
#include <bullet/btBulletDynamicsCommon.h>

//****************************************************************
// std系
//****************************************************************
#include "string.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <shlwapi.h>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>

//****************************************************************
// DirecX系のライブラリのリンク
//****************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib, "XAudio2.lib")
#pragma comment(lib, "X3DAudio.lib")
#pragma comment(lib, "shlwapi.lib")

//****************************************************************
// マクロ定義
//****************************************************************
// 頂点情報のストリームをマクロ定義
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 位置、座標変換用変数、色、テクスチャ
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 座標・法線・カラー・テクスチャ
#define FVF_VERTEX_LINE (D3DFVF_XYZ | D3DFVF_DIFFUSE)								// 座標・カラー

// ゲームに使うマクロ
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "MidNightHeist"

// ウィンドウ
#define SCREEN_WIDTH UINT(1280)
#define SCREEN_HEIGHT UINT(720)
#define SCREEN_CENTER D3DXVECTOR2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f)

// 最大値
#define MAX_WORD (4096)

// 型の初期値
#define QUAT_NULL D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f)
#define VEC3_NULL D3DXVECTOR3(0.0f,0.0f,0.0f)
#define VEC_UP D3DXVECTOR3(0.0f,1.0f,0.0f)
#define VEC2_NULL D3DXVECTOR2(0.0f,0.0f)
#define VEC3 D3DXVECTOR3
#define VEC2 D3DXVECTOR2

// 色
#define FOG D3DXCOLOR(0.8f,0.8f,0.8f,0.6f)
#define WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
#define BLACK D3DXCOLOR(0.0f,0.0f,0.0f,1.0f)
#define BURNTSCUM D3DXCOLOR(0.4f,0.4f,0.4f,1.0f)
#define RED D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)
#define GREEN D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)
#define BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)
#define BROWN D3DXCOLOR(0.62f,0.28f,0.16f,1.0f)
#define PURPLE D3DXCOLOR(0.8f,0.0f,0.9f,1.0f)
#define FIRE D3DXCOLOR(1.0f,0.3f,0.1f,1.0f)
#define WATERRIPPLE D3DXCOLOR(0.7f,0.7f,0.8f,0.7f)
#define WATERRIPPLEOUT D3DXCOLOR(0.7f,0.7f,0.8f,1.0f)
#define FIRESMOKE D3DXCOLOR(0.5f,0.5f,0.5f,1.0f)
#define ARROWORBIT D3DXCOLOR(0.9f,0.8f,0.0f,1.0f)

// 変換
#define TOSECOND (60)
#define TOPI (float)(0.0174f)

//****************************************************************
// 前方宣言
//****************************************************************
class CRenderer;
class CObject;

//****************************************************************
//頂点情報(2D)の構造体
//****************************************************************
typedef struct
{
	D3DXVECTOR3 pos;// 頂点座標
	float rhw;		// 座標変換用係数
	D3DCOLOR col;	// 頂点カラー
	D3DXVECTOR2 tex;// テクスチャ座標
}VERTEX_2D;

//****************************************************************
//頂点情報(3D)の構造体
//****************************************************************
typedef struct
{
	D3DXVECTOR3 pos;// 頂点座標
	D3DXVECTOR3 nor;// 法線ベクトル
	D3DCOLOR col;	// 頂点カラー
	D3DXVECTOR2 tex;// テクスチャ座標
}VERTEX_3D;

//****************************************************************
//頂点情報(Line)の構造体
//****************************************************************
typedef struct
{
	D3DXVECTOR3 pos;// 頂点座標
	D3DCOLOR col;	// 頂点カラー
}VERTEX_LINE;