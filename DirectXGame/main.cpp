#include <Windows.h>
#include <d3dcompiler.h>
#include <cassert>

#include "KamataEngine.h"
#include "Shader.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "VertexBuffer.h"

using namespace KamataEngine;

// 関数プロトタイプ宣言
void SetupPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    // KamataEngineの初期化
    KamataEngine::Initialize(L"LE3C_14_タカキ_ケンゴ");

    // DirectXCommonのインスタンスの取得
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // DirectXCommonクラスが管理している、ウィンドウの幅と高さの値を取得
    int32_t w = dxCommon->GetBackBufferWidth();
    int32_t h = dxCommon->GetBackBufferHeight();
    DebugText::GetInstance()->ConsolePrintf(std::format("width:{}, height:{}\n", w, h).c_str());

    // DirextxCommonクラスが管理しているコマンドリストの取得
    ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

    /// RootSignatureの設定 --------------------
    RootSignature rs;
    rs.Create();

    // VSshader　--------------------
    Shader vs;
    vs.LoadDxc(L"resources/shaders/TestVS.hlsl", L"vs_6_0");
    assert(vs.GetDxcBlob() != nullptr);

    // PSshader ---------------------
    Shader ps;
    ps.LoadDxc(L"resources/shaders/TestPS.hlsl", L"ps_6_0");
    assert(ps.GetDxcBlob() != nullptr);

    /// PSOの生成 --------------------
    PipelineState pipelineState;
    SetupPipelineState(pipelineState, rs, vs, ps);

    /// VertexResourceの生成 --------------------
    VertexBuffer vb;
    vb.Create(sizeof(Vector4) * 3, sizeof(Vector4));

    /// 頂点リソースにデータを書き込む --------------------
    Vector4* vertexData = nullptr;
    vb.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
    vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
    vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };

    // ===============
    // Mainループ
    // ===============
    while (true) {

        // エンジンの更新
        if (KamataEngine::Update()) {
            break;
        }

        // 描画処理開始       
        dxCommon->PreDraw();

        // コマンドを積む
        commandList->SetGraphicsRootSignature(rs.Get());                           // ルートシグネチャの設定
        commandList->SetPipelineState(pipelineState.Get());                        // PSOの設定
        commandList->IASetVertexBuffers(0, 1, vb.GetView());                       // 頂点バッファの設定
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // トポロジの設定

        commandList->DrawInstanced(3, 1, 0, 0);  // 描画（頂点数、インスタンス数、開始頂点インデックス、インデックスのオフセット）

        //描画処理終了       
        dxCommon->PostDraw();
    }

    // KamataEngineの終了 
    KamataEngine::Finalize();

    return 0;
}

void SetupPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps)
{
    // InputLayoutの設定 --------------------
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
    inputElementDescs[0].SemanticName = "POSITION";
    inputElementDescs[0].SemanticIndex = 0;
    inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
    inputLayoutDesc.pInputElementDescs = inputElementDescs;
    inputLayoutDesc.NumElements = _countof(inputElementDescs);

    // BlendState ---------------------
    D3D12_BLEND_DESC blendDesc{};
    // すべての色要素を書き込む
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // RasterizerState ---------------------
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    // 裏面（反時計回り）をカリングする
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    // 塗りつぶしモードをソリッドにする（ワイヤーフレームならD3D12_FILL_MODE_WIRERRAME)
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

    // PSOの設定 --------------------
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rs.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
    graphicsPipelineStateDesc.VS = { vs.GetDxcBlob()->GetBufferPointer(), vs.GetDxcBlob()->GetBufferSize() };
    graphicsPipelineStateDesc.PS = { ps.GetDxcBlob()->GetBufferPointer(), ps.GetDxcBlob()->GetBufferSize() };
    graphicsPipelineStateDesc.BlendState = blendDesc;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

    // RTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    // トポロジの設定
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // どのように画面に色を打ち込むのかの設定
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    // PSOの生成
    pipelineState.Create(graphicsPipelineStateDesc);
}
