//****************************************************************
//
// マップマネジャーの処理[mapmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "mapmanager.h"
#include "shadowmap.h"
#include "game.h"
#include "Factories.h"

// ネームスペース
using namespace nlohmann;
using namespace std;

//***************************************
// 初期化処理
//***************************************
HRESULT CMapManager::Init(void)
{
	// ベクターをクリア
	m_vMapObject.clear();

	// 正常終了
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CMapManager::Uninit(void)
{
}

//***************************************
// 更新処理
//***************************************
void CMapManager::Update(void)
{
}

//***************************************
// 描画処理
//***************************************
void CMapManager::Draw(void)
{
}

//***************************************
// ステージ読み込み
//***************************************
void CMapManager::Load(string Path)
{
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

	// 今置いてあるオブジェクトの破棄
	for (auto MapObjects = m_vMapObject.begin(); MapObjects != m_vMapObject.end(); MapObjects++)
	{
		MapObjects = m_vMapObject.erase(MapObjects);
		if (MapObjects == m_vMapObject.end()) break;
	}

	// クリア
	m_vMapObject.clear();

	// jsonデータを宣言
	ordered_json jsonData;
	// jsonを読み込む
	ifs >> jsonData;

	// SetStage配列の中にあるデータを代入存在しなかったら終わる
	for (const auto& obj : jsonData)
	{
		bool Static;
		float Mass;
		D3DXVECTOR3 Pos, Scale;
		D3DXQUATERNION Quad;
		std::string LocalPath;

		// IsStaticタグのデータを取り出す
		Static = obj["RigitBody"]["IsStatic"];
		// Massタグのデータを取り出す
		Mass = obj["RigitBody"]["Mass"];

		// Pathタグのデータを取り出す
		LocalPath = obj["Assets"]["Path"];

		// Posタグのデータを取り出す
		Pos.x = obj["Transform"]["Pos"]["x"];
		Pos.y = obj["Transform"]["Pos"]["y"];
		Pos.z = obj["Transform"]["Pos"]["z"];

		// Rotタグのデータを取り出す
		Quad.w = obj["Transform"]["Rot"]["w"];
		Quad.x = obj["Transform"]["Rot"]["x"];
		Quad.y = obj["Transform"]["Rot"]["y"];
		Quad.z = obj["Transform"]["Rot"]["z"];

		// Scaleタグのデータを取り出す
		Scale.x = obj["Transform"]["Scale"]["x"];
		Scale.y = obj["Transform"]["Scale"]["y"];
		Scale.z = obj["Transform"]["Scale"]["z"];

		m_vMapObject.push_back(Factories::makeMapobject(CManager::GetScene()->GetReg(), LocalPath, Pos, Quad,Scale));
	}
}

//***************************************
// 要素を削除
//***************************************
void CMapManager::Erase(entt::entity Erase)
{
	// 削除
	m_vMapObject.erase(
		std::remove(m_vMapObject.begin(), m_vMapObject.end(), Erase),
		m_vMapObject.end()
	);
}