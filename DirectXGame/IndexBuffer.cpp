#include "IndexBuffer.h"

#include <KamataEngine.h>
#include <d3d12.h>
#include <cassert>

using namespace KamataEngine;

void IndexBuffer::Create(const UINT size, const UINT stride)
{
    // strideの値によって、1つのインデックスをフォーマットを決める
    assert(stride == 2 || stride == 4);  // 2byte or 4byteのみ受け付ける
    DXGI_FORMAT format = (stride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

    // クラス内でdxCommonを利用するために追加
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    /// インデックスリソースの生成 --------------------

    // インデックスリソース用のヒープ設定
    D3D12_HEAP_PROPERTIES uploadHeapPropaties{};
    uploadHeapPropaties.Type = D3D12_HEAP_TYPE_UPLOAD;

    // インデックスリソースの設定
    D3D12_RESOURCE_DESC indexResourceDesc{};
    indexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    indexResourceDesc.Width = size;
    indexResourceDesc.Height = 1;
    indexResourceDesc.DepthOrArraySize = 1;
    indexResourceDesc.MipLevels = 1;
    indexResourceDesc.SampleDesc.Count = 1;
    indexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 実際にインデックスリソースを生成する
    ID3D12Resource* indexResource = nullptr;

    HRESULT hr;
    hr = dxCommon->GetDevice()->CreateCommittedResource(
        &uploadHeapPropaties, D3D12_HEAP_FLAG_NONE, &indexResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexResource)
    );
    assert(SUCCEEDED(hr));

    // 生成したインデックスリソースを取っておく
    indexBuffer_ = indexResource;

    /// IndexBufferViewを作成する --------------------
    D3D12_INDEX_BUFFER_VIEW indexBufferView{};
    indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();  // リソースの先頭アドレスから使う
    indexBufferView.SizeInBytes = size;                                      // 使用するインデックスデータの全サイズ
    indexBufferView.Format = format;                                         // インデックスのフォーマット（インデックスの1つ分のサイズ）

    // IndexBufferViewを取っておく
    indexBufferView_ = indexBufferView;
}

ID3D12Resource* IndexBuffer::Get()
{
    return indexBuffer_;
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetView()
{
    return &indexBufferView_;
}

IndexBuffer::IndexBuffer() {}

IndexBuffer::~IndexBuffer()
{
    if (indexBuffer_)
    {
        indexBuffer_->Release();
        indexBuffer_ = nullptr;
    }
}