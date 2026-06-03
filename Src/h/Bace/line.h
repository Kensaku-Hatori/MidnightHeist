//================================================================
//
// 線描がの処理[Line.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************

//****************************************************************
// ラインクラスを定義
//****************************************************************
class CLine
{
public:
	// コンストラクタ・デストラクタ
	CLine();
	~CLine();

	/// <summary>
	/// クリア
	/// </summary>
	/// <param name=""></param>
	void Clear(void);
	/// <summary>
	/// ラインを追加
	/// </summary>
	/// <param name="P1">視点</param>
	/// <param name="P2">終点</param>
	/// <param name="Col">色</param>
	void AddLine(const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DCOLOR Col);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static CLine& Instance(void) {
		static CLine Instance;
		return Instance;
	}
private:
	// ラインの可変長配列
	std::vector<VERTEX_LINE> m_Vertex;
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
};