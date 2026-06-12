#include "MainScene.h"
#include "Board.h"

MainScene::MainScene()
{
	m_pBoard = new Board;
}

MainScene::~MainScene()
{
	delete m_pBoard;
}

void MainScene::Init()
{
	m_pBoard->Init();
}

void MainScene::Update()
{
	m_pBoard->Update();
}

void MainScene::Draw()
{
	m_pBoard->Draw();
}