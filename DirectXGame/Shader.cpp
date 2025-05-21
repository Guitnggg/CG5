#include "Shader.h"

#include <d3dcompiler.h>
#include <cassert>

Shader::Shader()
{
    // 何もなし
}

Shader::~Shader()
{
    if (blob_ != nullptr) {
        blob_->Release();
        blob_ = nullptr;
    }

    if (dxcBlob_ != nullptr) {
        dxcBlob_->Release();
        dxcBlob_ = nullptr;
    }
}

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel)
{
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    // wstingqをstringに変換
    std::string mbShaderModel = std::string(shaderModel.begin(), shaderModel.end());

    // shaderコンパイル
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),                                // シェーダーのファイル名
        nullptr,                                         // マクロ定義
        D3D_COMPILE_STANDARD_FILE_INCLUDE,               // インクルードファイルの指定
        "main",                                          // エントリーポイント
        mbShaderModel.c_str(),                           // シェーダーモデル
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // コンパイルオプション
        0, &shaderBlob, &errorBlob
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            errorBlob->Release();
        }
        assert(false);
    }

    // コンパイル済みデータを保存
    blob_ = shaderBlob;
}

void Shader::LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel)
{
    static IDxcUtils* dxcUtils = nullptr;
    static IDxcCompiler3* dxcCompiler = nullptr;
    static IDxcIncludeHandler* includeHandler = nullptr;

    HRESULT hr;
    
    if (dxcUtils == nullptr) {
        hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
        assert(SUCCEEDED(hr));
    }
    
    if (dxcCompiler == nullptr) {
        hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
        assert(SUCCEEDED(hr));
    }

    if (includeHandler == nullptr) {
        hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
        assert(SUCCEEDED(hr));
    }

    /// 1. hlalfファイルを読み込む
    IDxcBlobEncoding* shaderSource = nullptr;
    hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
    assert(SUCCEEDED(hr));

    // 読み込んだファイルの内容をDxcBufferに設定
    DxcBuffer shaderSourceBuffer{};
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8;

    /// 2.コンパイルする
    // コンパイルに必要なオプションの準備
    LPCWSTR aragments[] = {
        filePath.c_str(),            // シェーダーファイル名
        L"-E", L"main",              // エントリーポイント
        L"-T", shaderModel.c_str(),  // シェーダーモデル
        L"-Zi",L"-Qembed_degug",     // デバッグ情報を埋め込む
        L"-Od",                      // 最適化を無効にする
        L"-Zpr"                      // プリミティブレイアウト
    };

    // コンパイル実行
    IDxcResult* shaderResult = nullptr;
    hr = dxcCompiler->Compile(
        &shaderSourceBuffer,         // シェーダーソース
        aragments,                   // コンパイルオプション
        _countof(aragments),         // オプションの数
        includeHandler,              // インクルードハンドラ
        IID_PPV_ARGS(&shaderResult)  // コンパイル結果
    );
    assert(SUCCEEDED(hr));

    /// 3.エラー処理
    IDxcBlobUtf8* shaderError = nullptr;
    IDxcBlobWide* nameBlob = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), &nameBlob);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
        OutputDebugStringA(shaderError->GetStringPointer());
        assert(false);
    }

    /// 4.コンパイル結果を取得
    IDxcBlob* shaderBlob = nullptr;
    hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &nameBlob);
    assert(SUCCEEDED(hr));

    // リソースの解放
    shaderSource->Release();
    shaderResult->Release();

    // 実行用のBlobを保存
    dxcBlob_ = shaderBlob;
}

ID3DBlob* Shader::GetBlob()
{
    return blob_;
}

IDxcBlob* Shader::GetDxcBlob()
{
    return dxcBlob_;
}
