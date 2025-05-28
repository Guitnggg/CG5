#pragma once

#include <d3dx12.h>

class VertexBuffer
{
public:
    // コンストラクタ
    VertexBuffer();

    // デストラクタ
    ~VertexBuffer();

    // 生成
    void Create(const UINT size, const UINT stride);

    // Getter
    ID3D12Resource* Get();
    D3D12_VERTEX_BUFFER_VIEW* GetView();

private:
    ID3D12Resource* vertexBuffer_ = nullptr;       // 頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};  // 頂点バッファビュー
};

