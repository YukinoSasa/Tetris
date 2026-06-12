#include <DxLib.h>
#include "Board.h"
#include "Block.h"

namespace
{
	// マスの大きさ
	constexpr int kCellSize = 32;
	// 入力クールタイム(フレーム数)
	constexpr int kCoolTime = 10;
	// 入力タイマー
	int inputTimer = 0;
	// 1マス落下フレーム数(レベル1)
	constexpr int kFallFrameEasy = 45;
	// 1マス落下フレーム数(レベル2)
	constexpr int kFallFrameNormal = 30;
	// 1マス落下フレーム数(レベル3)
	constexpr int kFallFrameHard = 15;
	// 落下判定タイマー
	int fallTimer = 0;
	// ライン消去のスコア
	constexpr int kScore = 100;
}

Board::Board():
	m_board(), m_blockX(0), m_blockY(0), m_FallFrame(kFallFrameEasy),
	m_CurrentBlock(BlockType::I), m_NextBlock(BlockType::O), m_RotateIndex(0),
	m_LineFullY(-1), m_isGameOver(false), m_score(0)
{

}

Board::~Board()
{

}

void Board::Init()
{

}

void Board::Update()
{
	// ゲームオーバーのとき、以降の更新処理をしない
	checkGameOver();
	if (m_isGameOver)
	{
		return;
	}

	// スコアに応じてレベルを変更
	if (m_score >= 1000)
	{
		SetFallFrame(kFallFrameNormal);
	}
	if (m_score >= 2000)
	{
		SetFallFrame(kFallFrameHard);
	}

	inputTimer++;
	fallTimer++;

	// 入力クールタイム中は入力を受け付けない
	if (inputTimer <= kCoolTime)
		return;

	// 左移動
	if (CanMoveLeft() && CheckHitKey(KEY_INPUT_A))
	{
		m_blockX--;

		if (m_blockX < 0)
		{
			m_blockX = 0;
		}

		inputTimer = 0;
	}
	// 右移動
	else if (CanMoveRight() && CheckHitKey(KEY_INPUT_D))
	{
		m_blockX++;

		if (m_blockX > kWidth - 1)
		{
			m_blockX = kWidth - 1;
		}

		inputTimer = 0;
	}
	// 下移動
	else if (CanFall() && CheckHitKey(KEY_INPUT_S))
	{
		m_blockY++;
	}
	// 右回転
	else if (CheckHitKey(KEY_INPUT_E))
	{
		m_RotateIndex++;

		// ブロック一回転で値をリセット
		if (m_RotateIndex > 3)
		{
			m_RotateIndex = 0;
		}

		if (CanRotate(m_RotateIndex))
		{
			m_CurrentBlock.Rotate(m_RotateIndex);
			inputTimer = 0;
		}
	}
	// 左回転
	else if (CheckHitKey(KEY_INPUT_Q))
	{
		m_RotateIndex--;

		// ブロック一回転で値をリセット
		if (m_RotateIndex < 0)
		{
			m_RotateIndex = 3;
		}

		if (CanRotate(m_RotateIndex))
		{
			m_CurrentBlock.Rotate(m_RotateIndex);
			inputTimer = 0;
		}
	}


	// 落下フレーム以下の場合落下処理を行わない
	if (fallTimer <= m_FallFrame)
	{
		return;
	}

	// 落下可能の場合落下、それ以外は盤面に固定
	if (CanFall())
	{
		m_blockY++;
		fallTimer = 0;
	}
	else
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				if (m_CurrentBlock.m_blockData[y][x] == 1)
				{
					m_board[y + m_blockY][x + m_blockX] = 1;
				}
			}
		}
		
		// 新しいブロックを生成
		m_blockX = 4;
		m_blockY = 0;
		m_RotateIndex = 0;
		fallTimer = 0;

		// 現在のブロックにコピー
		m_CurrentBlock = m_NextBlock;
		// 次のブロックを生成
		m_NextBlock.SetNextIndex(rand() % static_cast<int>(BlockType::Count));
		m_NextBlock = Block(static_cast<BlockType>(m_NextBlock.GetNextIndex()));
	}

	// 横一列埋まっている場合ブロック削除、加点
	if (isLineFull())
	{
		m_score += kScore;
		for (int x = 0; x < kWidth; x++)
		{
			m_board[m_LineFullY][x] = 0;
		}
		 // ブロックを下げる
		for (int y = m_LineFullY; y > 0; y--)
		{
			for (int x = 0; x < kWidth; x++)
			{
				m_board[y][x] = m_board[y - 1][x];
			}
		}
	}
}

void Board::Draw()
{
	// 盤面を描画
	for (int y = 0; y < kHeight; y++)
	{
		for (int x = 0; x < kWidth; x++)
		{
			// 落下済みのブロックを描画
			if (m_board[y][x] == 1)
			{
				DrawBox(x * kCellSize, y * kCellSize,
					x * kCellSize + kCellSize, y * kCellSize + kCellSize,
					GetColor(255, 255, 255), true);
			}
			// ブロックなしのマスを描画
			else
			{
				DrawBox(x * kCellSize, y * kCellSize,
					x * kCellSize + kCellSize, y * kCellSize + kCellSize,
					GetColor(255, 255, 255), false);
			}
		}
	}

	// 落下中のブロックを描画
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_CurrentBlock.m_blockData[y][x] == 1)
			{
				DrawBox((x + m_blockX) * kCellSize, (y + m_blockY) * kCellSize,
					(x + m_blockX) * kCellSize + kCellSize, (y + m_blockY) * kCellSize + kCellSize,
					GetColor(255, 255, 255), true);
			}
		}
	}
	
	// 次のブロックを描画
	DrawFormatString(500, 100, GetColor(255, 255, 255), "Next Block");
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_NextBlock.m_blockData[y][x] == 1)
			{
				DrawBox(536 + (x * kCellSize), 161 + (y * kCellSize),
					536 + kCellSize + (x * kCellSize), 161 + kCellSize + (y * kCellSize),
					GetColor(255, 255, 255), true);
			}
			else
			{
				DrawBox(536 + (x * kCellSize), 161 + (y * kCellSize),
					536 + kCellSize + (x * kCellSize), 161 + kCellSize + (y * kCellSize),
					GetColor(255, 255, 255), false);
			}
		}
	}
	// 枠
	DrawBox(500, 125, 700, 325, GetColor(255, 255, 255), false);

	// 現在のスコアを表示
	DrawFormatString(500, 50, GetColor(255, 255, 255), "Score：%d", m_score);

	// ゲームオーバー描画
	if (m_isGameOver)
	{
		DrawString(400, 360, "GameOver...", GetColor(255, 255, 255));
	}
}

bool Board::isLineFull()
{
	for (int y = 0; y < kHeight; y++)
	{
		int fullCount = 0;

		for (int x = 0; x < kWidth; x++)
		{
			if (m_board[y][x] == 1)
			{
				fullCount++;
			}
		}
		// 横一列埋まっている場合、その行を記録
		if (fullCount == kWidth)
		{
			m_LineFullY = y;
			return true;
		}
	}

	return false;
}

void Board::checkGameOver()
{
	// 上の段が1マスでも埋まっていたらゲームオーバー
	for (int x = 0; x < kWidth; x++)
	{
		if (m_board[0][x] == 1)
		{
			m_isGameOver = true;
			return;
		}
		else
		{
			m_isGameOver = false;
		}
	}
}

bool Board::CanFall()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_CurrentBlock.m_blockData[y][x] == 0)
			{
				continue;
			}

			// 盤面上の位置を計算
			int boardX = x + m_blockX;
			int boardY = y + m_blockY;

			// 床にぶつかる場合、落下不可能
			if (boardY >= kHeight - 1)
			{
				return false;
			}

			// 下にブロックがある場合、落下不可能
			if (boardY < kHeight - 1 && boardX < kWidth && m_board[boardY + 1][boardX] == 1)
			{
				return false;
			}
		}

	}
	
	return true;
}

bool Board::CanMoveLeft()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_CurrentBlock.m_blockData[y][x] == 0)
			{
				continue;
			}

			// ブロックの盤面上のx位置、y位置
			int boardX = x + m_blockX;
			int boardY = y + m_blockY;

			if (boardX <= 0 || m_board[boardY][boardX - 1] == 1)
			{
				return false;
			}
		}
	}

	return true;
}

bool Board::CanMoveRight()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_CurrentBlock.m_blockData[y][x] == 0)
			{
				continue;
			}

			// ブロックの盤面上のx位置、y位置
			int boardX = x + m_blockX;
			int boardY = y + m_blockY;

			if (boardX >= kWidth - 1 || m_board[boardY][boardX + 1] == 1)
			{
				return false;
			}
		}
	}

	return true;
}

bool Board::CanRotate(int rotateIndex)
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{

			if (!m_CurrentBlock.GetBlockCell(rotateIndex, x, y))
			{
				continue;
			}

			// 盤面の座標を計算
			int boardX = x + m_blockX;
			int boardY = y + m_blockY;

			// 壁にぶつかる場合
			if (boardX < 0 || boardX >= kWidth)
			{
				return false;
			}
			// 下の床やブロックにぶつかる場合
			else if (boardY >= kHeight || m_board[boardY][boardX] == 1)
			{
				return false;
			}
		}
	}

	return true;
}