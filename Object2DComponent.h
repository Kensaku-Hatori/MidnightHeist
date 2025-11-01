#pragma once

struct Object2DComponent {
    LPDIRECT3DVERTEXBUFFER9 m_pVertex;
    D3DXVECTOR2 Size = { 1.0f, 1.0f };
    D3DXCOLOR Color = { 1,1,1,1 };
    D3DXVECTOR2 UV[4] = { {0.0f,0.0f,},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
    int TexIndex = 0;
};