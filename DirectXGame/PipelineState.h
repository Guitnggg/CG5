#pragma once

#include <d3dx12.h>

class PipelineState
{
public:
    // コンストラクタ
    PipelineState();
    
    // デストラクタ
    ~PipelineState();

    // 生成
    void Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);

    // Getter
    ID3D12PipelineState* Get();

private:
    ID3D12PipelineState* pipelineState_ = nullptr;
};

