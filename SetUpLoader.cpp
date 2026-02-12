//****************************************************************
//
// セットアップ読み込みの処理[SetUpLoader.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "SetUpLoader.h"
#include "Factories.h"
#include "mapmanager.h"

//***************************************
// シーンに対して読み込み
//***************************************
void CSetUpLoader::LoadToScene(entt::registry& Reg, std::string Path)
{
	// 名前空間
	using namespace nlohmann;
	using namespace std;
	using namespace Factories;

	// jsonファイルを開く
	ifstream ifs(Path);

	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}

	// jsonデータを宣言
	ordered_json jsonData;
	// jsonを読み込む
	ifs >> jsonData;

	// SetStage配列の中にあるデータを代入存在しなかったら終わる
	for (const auto& Info : jsonData)
	{
		// 属性判定用変数
		std::string Attribute;
		// 属性取得
		Attribute = Info["Attribute"];

		// 2Dポリゴンなら
		if (Attribute.find("Poly2D") != std::string::npos)
		{
			// 描画順
			int Layer;
			// アセット
			std::string Asset;
			// 位置・大きさ
			D3DXVECTOR2 Pos, Size;

			// 描画順・アセットを読み込み
			Layer = Info["Layer"];
			Asset = Info["Asset"];

			// 位置と大きさを読み込む
			Pos.x = Info["Transform"]["Position"]["x"];
			Pos.y = Info["Transform"]["Position"]["y"];

			Size.x = Info["Transform"]["Size"]["x"];
			Size.y = Info["Transform"]["Size"]["y"];

			// タイトルロゴを生成
			Factories::makeObject2D(Reg, Layer, Asset, Pos, Size);
		}
		// タイトルプレイヤーなら
		else if (Attribute.find("TitlePlayer") != std::string::npos)
		{
			// 位置
			D3DXVECTOR3 Pos;

			// 位置と大きさを読み込む
			Pos.x = Info["Transform"]["Position"]["x"];
			Pos.y = Info["Transform"]["Position"]["y"];
			Pos.z = Info["Transform"]["Position"]["z"];

			// タイトルロゴを生成
			entt::entity Player =  Factories::makeBacePlayer(Reg, Pos);
			// プレイヤーをタイトル用の初期化
			Factories::InitTitlePlayer(Reg, Player);
		}
		// タイトルプレイヤーなら
		else if (Attribute.find("GamePlayer") != std::string::npos)
		{
			// 位置
			D3DXVECTOR3 Pos;

			// 位置と大きさを読み込む
			Pos.x = Info["Transform"]["Position"]["x"];
			Pos.y = Info["Transform"]["Position"]["y"];
			Pos.z = Info["Transform"]["Position"]["z"];

			// タイトルロゴを生成
			entt::entity Player = Factories::makeBacePlayer(Reg, Pos);
			// プレイヤーをタイトル用の初期化
			Factories::InitGamePlayer(Reg, Player);
		}
		// マップオブジェクトなら
		else if (Attribute.find("MapObject") != std::string::npos)
		{
			// アセット
			std::string Asset;
			// アセットを読み込み
			Asset = Info["Asset"];
			// タイトルロゴを生成
			Factories::makeMapobject(Reg, Asset);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("MapManager") != std::string::npos)
		{
			// アセット
			std::string Asset;
			// アセットを読み込み
			Asset = Info["Asset"];
			// タイトルマネージャ生成
			CMapManager::Instance()->Load(Asset);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("PatrolManager") != std::string::npos)
		{
			// アセット
			std::string Asset;
			// アセットを読み込み
			Asset = Info["Asset"];
			// タイトルマネージャ生成
			MeshFactories::makePatrolPointFromFile(Reg, Asset);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("EnemyManager") != std::string::npos)
		{
			// タイトルマネージャ生成
			ManagerFactories::makeEnemyManager(Reg);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("PauseManager") != std::string::npos)
		{
			// タイトルマネージャ生成
			ManagerFactories::makePauseManager(Reg);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("ItemManager") != std::string::npos)
		{
			// タイトルマネージャ生成
			ManagerFactories::makeItemManager(Reg);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("GateManager") != std::string::npos)
		{
			// タイトルマネージャ生成
			ManagerFactories::makeGateManager(Reg);
		}
		// タイトルマネージャーなら
		else if (Attribute.find("TitleManager") != std::string::npos)
		{
			// タイトルマネージャ生成
			ManagerFactories::makeTitleManager(Reg);
		}
		// タイトルマネージャーなら
		//else if (Attribute.find("StutsManager") != std::string::npos)
		//{
		//}
		// メッシュフィールドなら
		else if (Attribute.find("MeshField") != std::string::npos)
		{
			// アセット
			std::string Asset;
			// 分割数・大きさ
			int Horizon, Vertical;
			D3DXVECTOR2 Size;

			// アセットを読み込み
			Asset = Info["Asset"];

			// 位置と大きさを読み込む
			Horizon = Info["Transform"]["Division"]["h"];
			Vertical = Info["Transform"]["Division"]["v"];

			Size.x = Info["Transform"]["Size"]["x"];
			Size.y = Info["Transform"]["Size"]["y"];

			// 地面生成
			MeshFactories::makeMeshField(Reg, Horizon, Vertical, Size, Asset);
		}
		// スカイボックスなら
		else if (Attribute.find("SkyBox") != std::string::npos)
		{
			// スカイボックス生成
			MeshFactories::makeSkyBox(Reg);
		}
	}
}