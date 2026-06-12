#pragma once

/// <summary>
/// ブロック種類の列挙
/// </summary>
enum class BlockType
{
	I,
	O,
	T,
	L,
	Z,
	J,
	S,

	Count
};
/// <summary>
/// ブロックの回転状態の列挙
/// </summary>
enum class RotateState
{
	Up,
	Right,
	Down,
	Left,
};

/// <summary>
/// ブロックのクラス
/// </summary>
class Block
{
public:
	// コンストラクタ
	Block(BlockType type);

	// 次のブロックのインデックスを返す
	int GetNextIndex() { return m_BlockIndex; }

	// 次のブロックのインデックスをセット
	void SetNextIndex(int index) { m_BlockIndex = index; }

	// 次のブロックの形を返す
	bool GetBlockCell(int rotateIndex, int x, int y) const;

	// 回転
	void Rotate(int rotateIndex);

	// ブロックのデータ
	int m_blockData[4][4];

private:
	// ブロックのインデックス
	int m_BlockIndex;
};