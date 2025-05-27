#include "Shader.h"

#include "MiscUtility.h"

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

    if(dxcBlob_ != nullptr) {
        dxcBlob_->Release();
        dxcBlob_ = nullptr;
    }
}

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel)
{
    std::string mbShaderModel = ConvertString(shaderModel);

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",mbShaderModel.c_str(),
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &shaderBlob, &errorBlob);

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

    if(dxcUtils == nullptr) {
        hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
        assert(SUCCEEDED(hr));
    }

    if(dxcCompiler == nullptr) {
        hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
        assert(SUCCEEDED(hr));
    }

    if(includeHandler == nullptr) {
        hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
        assert(SUCCEEDED(hr));
    }

    /// 1. hlslファイルを読み込む
    IDxcBlobEncoding* shaderSource = nullptr;
    hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
    assert(SUCCEEDED(hr));

    // 読み込んだファイルの内容をDxcBufferに設定する
    DxcBuffer shaderSourceBuffer{};
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8;

    /// 2. シェーダーのコンパイルを行う
    LPCWSTR argments[] = {
        filePath.c_str(),            // コンパイル対象のhlslファイル名
        L"-E", L"main",              // エントリーポイントの関数名
        L"-T", shaderModel.c_str(),  // ShaderProfileの指定
        L"-Zi",L"-Qembed_debug",     // デバッグ情報を生成
        L"-Od",                      // 最適化を外しておく
        L"-Zpr",                     // メモリレイアウトは行優先
    };

    // コンパイルを実行
    IDxcResult* shaderResult = nullptr;
    hr= dxcCompiler->Compile(
        &shaderSourceBuffer,         // 読み込んだファイル
        argments,                    // コンパイルオプション
        _countof(argments),          // オプションの数
        includeHandler,              // includeが含まれたいろいろ
        IID_PPV_ARGS(&shaderResult)  // コンパイル結果
    ); 
    assert(SUCCEEDED(hr));

    /// 3.警告やエラーのチェック
    IDxcBlobUtf8* shaderError = nullptr;
    IDxcBlobWide* nameBlob = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), &nameBlob);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
        OutputDebugStringA(shaderError->GetStringPointer());
        assert(false);
    }

    /// 4. コンパイル結果の取得
    IDxcBlob* shaderBlob = nullptr;
    hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &nameBlob);
    assert(SUCCEEDED(hr));

    // 不要リソースの解放
    shaderSource->Release();
    shaderResult->Release();

    // 実行用のバイナリを取っておく
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
