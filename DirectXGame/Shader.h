#pragma once

#include <string>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

class Shader
{
public:
    // コンストラクタ
    Shader();

    // デストラクタ
    ~Shader();

    // Shaderファイルを読み込み、コンパイル済みデータを生成する
    void Load(const std::wstring& filePath, const std::wstring& shaderModel);
    void LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel);

    // コンパイル済みデータを取得する
    ID3DBlob* GetBlob();
    IDxcBlob* GetDxcBlob();

private:
    ID3DBlob* blob_ = nullptr;
    IDxcBlob* dxcBlob_ = nullptr;

};

