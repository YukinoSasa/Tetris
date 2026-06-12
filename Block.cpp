#include <Dxlib.h>
#include "Block.h"

namespace
{
	// ブロックの形を定義
	const int kBlockShapes[7][4][4] =
	{
		// I
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0}
		},

		// O
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// T
		{
			{1, 1, 1, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},

		// L
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0}
		},

		// Z
		{
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// J
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		// S
		{
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	};

	// 各ブロックの右に90°の形を定義
	const int kRightShapes[7][4][4] =
	{
		// I
		{
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// O
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// T
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0}
		},

		// L
		{
			{1, 1, 1, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// Z
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// J
		{
			{0, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0}
		},

		// S
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		}
	};

	// 各ブロックの右に180°の形を定義
	const int kDownShapes[7][4][4] =
	{
		// I
		{
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0}
		},

		// O
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// T
		{
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{1, 1, 1, 0}
		},

		// L
		{
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},

		// Z
		{
			{1, 1, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// J
		{
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// S
		{
			{0, 1, 1, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		}
	};

	// 各ブロックの右に270°の形を定義
	const int kLeftShapes[7][4][4] =
	{
		// I
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{1, 1, 1, 1}
		},

		// O
		{
			{1, 1, 0, 0},
			{1, 1, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// T
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0}
		},

		// L
		{
			{0, 0, 0, 0},
			{0, 0, 1, 0},
			{1, 1, 1, 0},
			{0, 0, 0, 0}
		},

		// Z
		{
			{0, 1, 0, 0},
			{1, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// J
		{
			{1, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		// S
		{
			{1, 0, 0, 0},
			{1, 1, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		}
	};

}


Block::Block(BlockType type)
{
	// ブロックのタイプを数値に変換
	m_BlockIndex = static_cast<int>(type);

	// ブロックの初期化
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			m_blockData[y][x] = kBlockShapes[m_BlockIndex][y][x];
		}
	}

}

bool Block::GetBlockCell(int rotateIndex, int x, int y) const
{
	switch (rotateIndex)
	{
	case 0:
		return kBlockShapes[m_BlockIndex][y][x];
	case 1:
		return kRightShapes[m_BlockIndex][y][x];
	case 2:
		return kDownShapes[m_BlockIndex][y][x];
	case 3:
		return kLeftShapes[m_BlockIndex][y][x];
	}

	return false;
}

void Block::Rotate(int rotateIndex)
{
	switch (rotateIndex)
	{
	case 0:
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_blockData[y][x] = kBlockShapes[m_BlockIndex][y][x];
			}
		}
		break;
	case 1:
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_blockData[y][x] = kRightShapes[m_BlockIndex][y][x];
			}
		}
		break;
	case 2:
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_blockData[y][x] = kDownShapes[m_BlockIndex][y][x];
			}
		}
		break;
	case 3:
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m_blockData[y][x] = kLeftShapes[m_BlockIndex][y][x];
			}
		}
		break;
	default:
		break;
	}
}
