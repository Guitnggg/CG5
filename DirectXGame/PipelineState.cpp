#include "PipelineState.h"
#include "KamataEngine.h"

using namespace KamataEngine;

PipelineState::PipelineState() {}

PipelineState::~PipelineState() 
{
    if (pipelineState_) {
        pipelineState_->Release();
        pipelineState_ = nullptr;
    }
}

void PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
    // クラス内で DirectXCommon のインスタンスを取得
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    ID3D12PipelineState* graphicsPipelineState = nullptr;
    HRESULT hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
        &desc, IID_PPV_ARGS(&graphicsPipelineState));
    assert(SUCCEEDED(hr));

    // 作成したパイプラインステートを保持
    pipelineState_ = graphicsPipelineState;
}

ID3D12PipelineState* PipelineState::Get()
{
    return pipelineState_;
}
