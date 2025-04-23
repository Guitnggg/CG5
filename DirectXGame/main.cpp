#include <Windows.h>
#include <KamataEngine.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    // DirectXCommonのインスタンスの取得
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	
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

        //描画処理終了       
        dxCommon->PostDraw();
    }

    // KamataEngineの終了 
    KamataEngine::Finalize();
   
	return 0;
}
