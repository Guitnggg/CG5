#pragma once

#include <d3dx12.h>

class RootSignature
{
public:
    // コンストラクタ
    RootSignature();

    // デストラクタ
    ~RootSignature();

    // 生成
    void Create();

    // ゲッター
    ID3D12RootSignature* Get();

private:
    ID3D12RootSignature* rootSignature_ = nullptr;

};

