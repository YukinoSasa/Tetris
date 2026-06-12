#include <Dxlib.h>
#include "MainScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
#ifndef _DEBUG
	SetOutApplicationLogValidFlag(false);
#endif
	// 画面サイズの変更
	SetGraphMode(800, 720, 32);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
		return -1;

	MainScene* pMainScene = new MainScene;
	pMainScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面クリア
		ClearDrawScreen();

		pMainScene->Update();
		pMainScene->Draw();

		// 裏画面と表画面を入れ替え
		ScreenFlip();

		// FPSを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}

		// ESCAPEキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	delete pMainScene;

	DxLib_End();
	return 0;
}