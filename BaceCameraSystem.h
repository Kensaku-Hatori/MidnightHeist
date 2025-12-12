//****************************************************************
//
// カメラシステムの処理[基本のカメラシステムcamera.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード

// 前方宣言
class CCamera;

// クラス定義
class CBaceSystem
{
public:
	// コンストラクタ
	CBaceSystem() = default;
	// デストラクタ
	virtual ~CBaceSystem() = default;
	// 純粋仮想関数
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Uninit(void) = 0;
	void SetOwner(CCamera* _Owner) { m_pOwner = _Owner; }
protected:
	// オーナー
	CCamera* m_pOwner;
};