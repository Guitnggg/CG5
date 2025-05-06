#include <Windows.h>
#include <KamataEngine.h>

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

        // RootSignatureの設定
        // 構造体にデータを用意する
        D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature{};



        //描画処理終了       
        dxCommon->PostDraw();
    }

    // KamataEngineの終了 
    KamataEngine::Finalize();
   
	return 0;
}
