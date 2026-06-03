//================================================================
//
// マップマネジャーの処理[mapmanager.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// 前方宣言
//****************************************************************

//****************************************************************
// クラスを定義
//****************************************************************
class CMapManager
{
public:
	// デストラクタ
	~CMapManager() { m_vMapObject.clear(); }

	// メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>結果(一応付けている)</returns>
	HRESULT Init(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	void Uninit(void);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);
	/// <summary>
	/// ステージの読み込み
	/// </summary>
	/// <param name="Path">読み込むステージのファイルパス</param>
	void Load(std::string Path);
	/// <summary>
	/// 外部から要素を消すときのヘルパー関数
	/// </summary>
	/// <param name="Erase">消す対象</param>
	void Erase(entt::entity Erase);
	/// <summary>
	/// マップのオブジェクトリストを取得
	/// </summary>
	/// <returns>オブジェクトリスト</returns>
	inline std::vector<entt::entity>& GetvMapObject(void) { return m_vMapObject; }
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns>インスタンス</returns>
	inline static CMapManager& Instance(void) {
		static CMapManager Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CMapManager() { Init(); }
	// メンバ変数
	std::vector<entt::entity> m_vMapObject;			// マップに置くオブジェクト
};