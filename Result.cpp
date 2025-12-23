//****************************************************************
//
// リザルトの処理[Result.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Result.h"
#include "manager.h"
#include "Factories.h"
#include "ResultCameraSystem.h"
#include "ColorComponent.hpp"

//***************************************
// 初期化処理
//***************************************
HRESULT CResult::Init(void)
{
	// タイトル用のモデル生成
	Factories::makeMapobject(GetReg(), "data/MODEL/Prison.x");
	// テスト用のプレイヤー生成
	Factories::makeObjectX(GetReg(), "data/MODEL/testplayer.x");

	// 黒い半透明ポリゴン
	entt::entity BlackBoard = Factories::makeObject2D(GetReg(), 3, "", D3DXVECTOR2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f), D3DXVECTOR2(SCREEN_WIDTH * 0.3f, SCREEN_WIDTH * 0.5f));
	auto& ColorCmp = GetReg().get<ColorComp>(BlackBoard);
	ColorCmp.Col = BLACK;
	ColorCmp.Col.a = 0.5f;

	// スタッツ情報
	Factories::makeObject2D(GetReg(), 4, "data/TEXTURE/Success.png", D3DXVECTOR2(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.2f), D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.1f));
	Factories::makeObject2D(GetReg(), 4, "data/TEXTURE/Time.png", D3DXVECTOR2(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.4f), D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.05f));
	Factories::makeObject2D(GetReg(), 4, "data/TEXTURE/LockingTime.png", D3DXVECTOR2(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.55f), D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.05f));
	Factories::makeObject2D(GetReg(), 4, "data/TEXTURE/SmartPoint.png", D3DXVECTOR2(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.7f), D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.05f));

	// カメラにシステムを追加
	CManager::GetCamera()->AddSystem(new CResultCamera);

    return E_NOTIMPL;
}

//***************************************
// 終了処理
//***************************************
void CResult::Uninit(void)
{
	// カメラのシステムを終了
	CManager::GetCamera()->EndSystems();
	// エンティティたちをクリア
	GetReg().clear();
    delete this;
}

//***************************************
// 更新処理
//***************************************
void CResult::Update(void)
{
}

//***************************************
// 描画
//***************************************
void CResult::Draw(void)
{
}