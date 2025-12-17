//****************************************************************
//
// マップマネジャーの処理[mapmanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

// 前方宣言
class CMapObject;

// クラスを定義
class CMapManager
{
public:
	// デストラクタ
	~CMapManager() { m_vMapObject.clear(); }

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 読み込み
	void Load(std::string Path);

	// ベクター用
	void Erase(entt::entity Erase);
	// ゲッター
	std::vector<entt::entity>& GetvMapObject(void) { return m_vMapObject; }
	// 静的メンバ関数
	// シングルトン
	static CMapManager* Instance(void) {
		static std::unique_ptr<CMapManager> Instance(new CMapManager);
		return Instance.get();
	};
private:
	// コンストラクタ
	CMapManager() { Init(); }
	// メンバ変数
	std::vector<entt::entity> m_vMapObject;			// マップに置くオブジェクト
};

#endif // !_MAPMANAGER_H_