#pragma once

struct Transform {
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };
	D3DXQUATERNION Quat = { 0.0f,0.0f,0.0f,1.0f };
	D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f };

	void SetPosition(const D3DXVECTOR3& pos) { Pos = pos; }
	void SetScale(const D3DXVECTOR3& scale) { Scale = scale; }
	void SetRotation(const D3DXQUATERNION& quat) { Quat = quat; }
	const D3DXVECTOR3& GetPosition() const { return Pos; }
	D3DXMATRIX GetWorldMatrix() const {
		D3DXMATRIX mtxScale, mtxRotation, mtxTransform;
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixRotationQuaternion(&mtxRotation, &Quat);
		D3DXMatrixTranslation(&mtxTransform, Pos.x, Pos.y, Pos.z);
		return mtxScale * mtxRotation * mtxTransform;
	}
};