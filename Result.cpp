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

//***************************************
// 初期化処理
//***************************************
HRESULT CResult::Init(void)
{
	// タイトル用のモデル生成
	Factories::makeMapobject(GetReg(), "data/MODEL/Prison.x");

	Factories::makeObjectX(GetReg(), "data/MODEL/testplayer.x");
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