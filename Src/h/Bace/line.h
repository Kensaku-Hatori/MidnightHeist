//****************************************************************
//
// 線描がの処理[Line.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _LINE_H_
#define _LINE_H_

// ラインクラスを定義
class CLine
{
public:
	// コンストラクタ・デストラクタ
	CLine();
	~CLine();

	// メンバ関数
	void Clear(void);
	void AddLine(const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DCOLOR Col);
	void Draw(void);

	// 静的メンバ関数
	static std::unique_ptr<CLine> Create(void);
private:
	// ラインの可変長配列
	std::vector<VERTEX_LINE> m_Vertex;
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
};
#endif // !_LINE_H_