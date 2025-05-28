#include "RootSignature.h"
#include "KamataEngine.h"

using namespace KamataEngine;

RootSignature::RootSignature(){}

RootSignature::~RootSignature() 
{
    if (rootSignature_) {
        rootSignature_->Release();
        rootSignature_ = nullptr;
    }
}

void RootSignature::Create()
{
    // 既にインスタンスがあるなら解放する
    if (rootSignature_) {
        rootSignature_->Release();
        rootSignature_ = nullptr;
    }

    // クラス内で取得するために追加
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    /// RootSignatureを作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
    descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* signatureBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr= D3D12SerializeRootSignature(
        &descriptionRootSignature,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signatureBlob,
        &errorBlob
    );

    if (FAILED(hr)) {
        DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
        assert(false);
    }

    // バイナリをもとに生成
    ID3D12RootSignature* rootSignature = nullptr;
    hr=dxCommon->GetDevice()->CreateRootSignature(0,signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

    //  signatureBlobはRootSignatureの生成後解放する
    signatureBlob->Release();

    // 生成したRootSignatureを保持
    rootSignature_ = rootSignature;
}

ID3D12RootSignature* RootSignature::Get()
{
    return rootSignature_;
}
