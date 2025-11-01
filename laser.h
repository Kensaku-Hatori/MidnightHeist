#include "object.h"

class CLaser : public CObject
{
public:
	CLaser();
	~CLaser();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetOrigin(D3DXVECTOR3 Pos);
	void SetFront(D3DXVECTOR3 YAxis);
	void SetHeight(const float Height);

	D3DXVECTOR3 GetDir(void) { return m_Dir; }
	D3DXVECTOR3 GetOrigin(void) { return m_Origin; }

	static CLaser* Create(const int DivX,const int DivY);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXQUATERNION m_Quat;				// クォータニオン
	D3DXVECTOR3 m_Origin;				// 始点
	D3DXVECTOR3 m_Dir;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXCOLOR m_Col;					// 色
	float m_fRadius;					// 円の大きさ
	float m_fLength = 1000.0f;			// 長さ
	int m_nVertexNum;					// 頂点数
	int m_nIndxBuffNum;					// インデックスバッファの数
	int m_nPolyNum;						// 描画するポリゴン数
	int m_nDivX;						// 分割数X
	int m_nDivY;						// 分割数Y
};