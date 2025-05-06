#include <Windows.h>
#include <KamataEngine.h>
#include <d3dcompiler.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    // DirectXCommonのインスタンスの取得
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // DirectXCommonクラスが管理している、ウィンドウの幅と高さの値を取得
    int32_t w = dxCommon->GetBackBufferWidth();
    int32_t h = dxCommon->GetBackBufferHeight();
    DebugText::GetInstance()->ConsolePrintf(std::format("width:{}, height:{}\n", w, h).c_str());

    // DirextxCommonクラスが管理しているコマンドリストの取得
    ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();
	
    // KamataEngineの初期化
    KamataEngine::Initialize(L"LE3C_14_タカキ_ケンゴ");

    // Mainループ
    while (true) {

        // エンジンの更新
        if (KamataEngine::Update()) {
            break;
        }
                
        // 描画処理開始       
        dxCommon->PreDraw();

        //=======================
        // ここに描画処理を書く
        //=======================

        /// RootSignatureの設定 --------------------
        // 構造体にデータを用意する
        D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature{};
        descriptorRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        ID3DBlob* signatureBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        HRESULT hr = D3D12SerializeRootSignature(&descriptorRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

        if (FAILED(hr)) {
            DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            assert(false);
        }

        // バイナリをもとに生成
        ID3D12RootSignature* rootSignature = nullptr;
        hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
        assert(SUCCEEDED(hr));

        /// InputLayout --------------------
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
        inputElementDescs[0].SemanticName = "POSITION";
        inputElementDescs[0].SemanticIndex = 0;
        inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
        inputLayoutDesc.pInputElementDescs = inputElementDescs;
        inputLayoutDesc.NumElements = _countof(inputElementDescs);

        /// BlendState --------------------
        D3D12_BLEND_DESC blendDesc{};
        // すべての色要素を書き込む
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        /// RasterizerState --------------------
        D3D12_RASTERIZER_DESC rasterizerDesc{};
        // 裏面（反時計回り）をカリングする
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        // 塗りつぶしモードをソリッドにする（ワイヤーフレームならD3D12_FILL_MODE_WIRERRAME)
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

        // コンパイル済みのShader、エラー時の情報の格納場所の用意
        ID3DBlob* vsBlob = nullptr;
        ID3DBlob* psBlob = nullptr;
        ID3DBlob* erroeBlob = nullptr;

        // VSshader 
        std::wstring vsFile = L"Resources/shaders/TestVS.hlsl";
        hr = D3DCompileFromFile(vsFile.c_str(), nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "main", "vs_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0, &vsBlob, &erroeBlob);

        if (FAILED(hr)) {
            DebugText::GetInstance()->ConsolePrintf(std::system_category().message(hr).c_str());

            if (errorBlob) {
                DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            }
        }
        assert(false);

        // PSshader 
        std::wstring psFile = L"Resources/shaders/TestPS.hlsl";
        hr = D3DCompileFromFile(psFile.c_str(), nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "main", "ps_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0, &psBlob, &errorBlob);

        if (FAILED(hr)) {
            DebugText::GetInstance()->ConsolePrintf(std::system_category().message(hr).c_str());

            if (errorBlob) {
                DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            }
        }
        assert(false);

        /// PSOの生成 --------------------



        //描画処理終了       
        dxCommon->PostDraw();
    }

    // KamataEngineの終了 
    KamataEngine::Finalize();
   
	return 0;
}
