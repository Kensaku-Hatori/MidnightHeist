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
#include "fade.h"
#include "title.h"
#include "Sound2D.h"

//***************************************
// 初期化処理
//***************************************
HRESULT CResult::Init(void)
{
	std::string Path;
	const bool IsClear = CManager::GetIsClear();

	if (IsClear == true)Path = "data/MODEL/ClockTower.x";
	else Path = "data/MODEL/Prison.x";

	// タイトル用のモデル生成
	Factories::makeMapobject(GetReg(), Path);
	// テスト用のプレイヤー生成
	Factories::makeMapobject(GetReg(), "data/MODEL/testplayer.x");
	MeshFactories::makeSkyBox(GetReg());

	// 黒い半透明ポリゴン
	entt::entity BlackBoard = Factories::makeObject2D(GetReg(), 3, "", D3DXVECTOR2(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.5f), D3DXVECTOR2(SCREEN_WIDTH * 0.2f, SCREEN_WIDTH * 0.5f));
	auto& ColorCmp = GetReg().get<ColorComp>(BlackBoard);
	ColorCmp.Col = BLACK;
	ColorCmp.Col.a = 0.5f;

	// スタッツ情報
	ManagerFactories::makeStutsManager(GetReg());

	// カメラにシステムを追加
	if (IsClear == true)CManager::GetCamera()->AddSystem(new CClearCamera);
	else CManager::GetCamera()->AddSystem(new CFailedCamera);
	if (IsClear == true)CSound2D::Instance()->Play(SoundDevice::LABEL_GAMECLEARBGM);
	else CSound2D::Instance()->Play(SoundDevice::LABEL_GAMEOVERBGM);

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
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CManager::GetFade()->SetFade(new CTitle);
	}
}

//***************************************
// 描画
//***************************************
void CResult::Draw(void)
{
}