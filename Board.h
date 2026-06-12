#pragma once
#include "Block.h"


/// <summary>
/// 盤面のクラス
/// </summary>
class Board
{
public:
	// コンストラクタ
	Board();

	// デストラクタ
	~Board();

	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 落下フレームをセット
	void SetFallFrame(float fallFrame) { m_FallFrame = fallFrame; }

	// 盤面の横の長さ
	static constexpr int kWidth = 10;

	// 盤面の縦の長さ
	static constexpr int kHeight = 20;

private:
	// 横一列埋まっているかどうか
	bool isLineFull();

	// ゲームオーバー判定
	void checkGameOver();

	// 盤面の二次元配列
	int m_board[kHeight][kWidth];

	// ブロック配列(0,0)のxマス
	int m_blockX;

	// ブロック配列(0,0)のyマス
	int m_blockY;

	// 落下フレーム数
	float m_FallFrame;

	// 現在のブロック情報
	Block m_CurrentBlock;

	// 次のブロック情報
	Block m_NextBlock;

	// 回転の向きを管理する変数
	int m_RotateIndex;

	// 落下可能かどうか判定
	bool CanFall();

	// 左に移動可能かどうか判定
	bool CanMoveLeft();

	// 右に移動可能かどうか判定
	bool CanMoveRight();

	// 回転可能かどうか判定
	bool CanRotate(int rotateIndex);

	// 横一列埋まっているラインのy座標
	int m_LineFullY;

	// ゲームオーバーかどうか
	bool m_isGameOver;

	// スコア用の変数
	int m_score;
};
