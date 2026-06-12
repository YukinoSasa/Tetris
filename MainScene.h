#pragma once

class Board;

/// <summary>
/// メインシーンクラス
/// </summary>
class MainScene
{
public:
	// コンストラクタ
	MainScene();

	// デストラクタ
	~MainScene();

	//初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 盤面のポインタ変数
	Board* m_pBoard;
};