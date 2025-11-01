//****************************************************************
//
// マップマネジャーの処理[mapmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "mapmanager.h"
#include "mapobject.h"
#include "shadowmap.h"
#include "game.h"
#include "player.h"
#include "laser.h"

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
	// モデルとマウスの当たり判定用の距離
	float Distance, CurrentDistance;

	// 現在の距離を10000.0fにする(ソートするため)
	Distance = 1000000.0f;

	// 昔の距離を初期化
	CurrentDistance = 0.0f;

	// モデルとの当たり判定を開始
	for (auto Models = m_vMapObject.begin(); Models != m_vMapObject.end(); Models++)
	{
		// 当たったら
		if ((*Models)->CollisionRaytoMesh(CGame::GetPlayer()->GetLaser()->GetOrigin(), CGame::GetPlayer()->GetLaser()->GetDir(),&CurrentDistance) == true)
		{
			// 最新の距離と当たったオブジェクトとの距離を比較
			if (CurrentDistance < Distance)
			{
				// 最新の距離を更新
				Distance = CurrentDistance;
			}
		}
	}

	// 距離を設定
	CGame::GetPlayer()->GetLaser()->SetHeight(Distance);
}

//***************************************
// 描画処理
//***************************************
void CMapManager::Draw(void)
{
	// シャドウマップへの書き込みを開始
	CShadowMap::Instance()->Begin();
	CShadowMap::Instance()->WriteMaps();

	for (auto Models = m_vMapObject.begin(); Models != m_vMapObject.end(); Models++)
	{
		(*Models)->DrawShadow();
	}

	// 書き込みを終わる
	CShadowMap::Instance()->EndMaps();
	CShadowMap::Instance()->End();

	// シャドウマップを使って描画
	for (auto Models = m_vMapObject.begin(); Models != m_vMapObject.end(); Models++)
	{
		(*Models)->CastShadow();
	}
}

//***************************************
// オブジェクトの生成
//***************************************
void CMapManager::CreateObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	// 生成、要素に追加
	CMapObject* LocalObject = NULL;
	LocalObject = CMapObject::Create(Pos, Rot, Path);
	m_vMapObject.push_back(LocalObject);
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
		(*MapObjects)->Uninit();
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

		CMapObject* SetInfo = CMapObject::Create(Pos, { Quad.x,Quad.y ,Quad.z }, LocalPath);

		SetInfo->SetRotFromQuad(SetInfo->SetQuad(Quad));
		SetInfo->SetStatic(Static);
		SetInfo->SetPos(Pos);
		SetInfo->SetMass(Mass);
		SetInfo->SetScale(Scale);

		// 連結
		m_vMapObject.push_back(SetInfo);
	}
}