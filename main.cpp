#include <Novice.h>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

const char kWindowTitle[] = "LC1D_09_カナヤ_ヒロキ_タイトル";

struct Vector2 {
	float x;
	float y;
};

struct Enemy {
	Vector2 pos;
	float width;
	float height;
	float speed;
};

struct Bullet {
	Vector2 pos;
	Vector2 velocity;
	Vector2 start;
	Vector2 end;
	float width;
	float height;
	float speed;
	float length;
	float t;
	float easedT;
	int columns;
	int space;
	int isBulletShot;
	int interpolation;
};

struct Player
{
	Vector2 pos;
	Vector2 dir;
	float radius;
	float speed;
};

struct PlayerBullet
{
	Vector2 pos;
	float radius;
	float speed;

	int isShot;


};

PlayerBullet strightBullet1
{
	640,620,
	10,
	10,
	false,

};

PlayerBullet strightBullet2
{
	680,620,
	10,
	3,
	false,
};

PlayerBullet backBullet1
{
	640,620,
	10,
	10,
	false,

};

PlayerBullet backBullet2
{
	680,620,
	10,
	10,
	false,
};

PlayerBullet leftBullet1
{
	640,620,
	10,
	10,
	false,

};

PlayerBullet leftBullet2
{
	680,620,
	10,
	10,
	false,
};

PlayerBullet rightBullet1
{
	640,620,
	10,
	10,
	false,

};

PlayerBullet rightBullet2
{
	680,620,
	10,
	10,
	false,
};
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// --- enemy初期化 ---
	Enemy enemy = {};
	enemy.pos.x = 200.0f;
	enemy.pos.y = 100.0f;
	enemy.height = 100.0f;
	enemy.width = 200.0f;

	Player player{};
	player.pos.x = 640.0f;
	player.pos.y = 620.0f;
	player.dir.x = player.pos.x - enemy.pos.y;
	player.dir.y = player.pos.y - enemy.pos.y;
	player.radius = 40.0f;
	player.speed = 5.0f;
	int type = 0;//玉の属性
	int direction = 0;//プレイヤーの向き
	int playerHP = 200;

	int playerBack = true;
	int playerFront = false;
	int leftBulletCount = 0;
	int rightBulletCount = 0;

	int playerBackHandle=Novice::LoadTexture("./Resources/images/psychic.png");
	int playerFrontHandle = Novice::LoadTexture("./Resources/images/front.png");
	int leftHandHandle = Novice::LoadTexture("./Resources/images/lefthand.png");
	int rightHandHandle = Novice::LoadTexture("./Resources/images/righthand.png");
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_W])
		{
			direction = 0;
			player.pos.y -= player.speed;
			playerFront = false;
			playerBack = true;
		}

		if (keys[DIK_S])
		{
			direction = 1;
			player.pos.y += player.speed;
			playerFront = true;
			playerBack = false;
		}

		if (keys[DIK_A])
		{
			direction = 2;
			player.pos.x -= player.speed;
		}

		if (keys[DIK_D])
		{
			direction = 3;
			player.pos.x += player.speed;
		}


		//玉の属性の切り替え
		if (keys[DIK_Q])
		{
			type = 1;
		}
		//玉の属性の切り替え
		if (keys[DIK_E])
		{
			type = 2;
		}
		if (keys[DIK_X])
		{
			type = 0;
		}

		
			if (strightBullet1.isShot == false)
			{
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{

					//スペースを押したときのplayerbullet1の動き
					if (type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (direction==0)
							{
							if (strightBullet1.isShot == 0)
							{
								leftBulletCount += 1;
							}
								strightBullet1.pos.x = player.pos.x;
								strightBullet1.pos.y = player.pos.y;
								strightBullet1.isShot = true;
							}
						}
					}
				}
			}
			if (strightBullet2.isShot == false)
			{
				////スペースを押したときのplayerbullet2の動き
				if (type == 2)
				{
					strightBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (direction == 0)
						{
						if (strightBullet2.isShot == 0)
						{
							rightBulletCount += 1;
						}
							strightBullet2.pos.x = player.pos.x + player.radius;
							strightBullet2.pos.y = player.pos.y;
							strightBullet2.isShot = true;
							
						}
					}
				}

			}

			if (leftBulletCount >= 10 && rightBulletCount >= 10)
			{
				if (strightBullet1.isShot == false || strightBullet2.isShot == false)
				{
					if (type == 0)
					{
						strightBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (direction == 0)
							{
								playerHP -= 10;
								strightBullet2.pos.x = player.pos.x + player.radius;
								strightBullet2.pos.y = player.pos.y;
								strightBullet2.isShot = true;
								strightBullet1.pos.x = player.pos.x;
								strightBullet1.pos.y = player.pos.y;
								strightBullet1.isShot = true;
							}

						}
					}
				}
			}
			
			if (strightBullet1.isShot == 1)
			{
				strightBullet1.pos.y -= strightBullet1.speed;

				if (strightBullet1.pos.y < 0)
				{
				
					strightBullet1.isShot = false;
					strightBullet1.pos.y = player.pos.x;

				}
			}
			if (strightBullet2.isShot == 1)
			{
				
				strightBullet2.pos.y -= strightBullet2.speed;

				if (strightBullet2.pos.y < 0)
				{
					strightBullet2.isShot = false;
					strightBullet2.pos.y = player.pos.x;

				}
			}
		
		
			if (backBullet1.isShot == false)
			{
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{

					//スペースを押したときのplayerbullet1の動き
					if (type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (direction == 1)
							{
							if (backBullet1.isShot == 0)
							{
								leftBulletCount += 1;
							}
								backBullet1.pos.x = player.pos.x;
								backBullet1.pos.y = player.pos.y;
								backBullet1.isShot = true;
							}
						}
					}
				}
			}
			if (backBullet2.isShot == false)
			{
				////スペースを押したときのplayerbullet2の動き
				if (type == 2)
				{
					backBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (direction==1)
						{
						if (backBullet2.isShot == 0)
						{
							rightBulletCount += 1;
						}
							backBullet2.pos.x = player.pos.x + player.radius;
							backBullet2.pos.y = player.pos.y;
							backBullet2.isShot = true;
						}
					}
				}

			}

			if (leftBulletCount >= 10 && rightBulletCount >= 10)
			{
				if (backBullet1.isShot == false || backBullet2.isShot == false)
				{
					if (type == 0)
					{
						backBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (direction == 1)
							{
								playerHP -= 10;
								backBullet2.pos.x = player.pos.x + player.radius;
								backBullet2.pos.y = player.pos.y;
								backBullet2.isShot = true;
								backBullet1.pos.x = player.pos.x;
								backBullet1.pos.y = player.pos.y;
								backBullet1.isShot = true;
							}
						}
					}
				}
			}

			if (backBullet1.isShot == 1)
			{
				backBullet1.pos.y += backBullet1.speed;

				if (backBullet1.pos.y > 720)
				{
					backBullet1.isShot = false;
					backBullet1.pos.y = player.pos.x;
				}
			}
			if (backBullet2.isShot == 1)
			{
				backBullet2.pos.y += backBullet2.speed;

				if (backBullet2.pos.y > 720)
				{
					backBullet2.isShot = false;
					backBullet2.pos.y = player.pos.x;

				}
			}
		
			if (leftBullet1.isShot == false)
			{
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{

					//スペースを押したときのplayerbullet1の動き
					if (type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (direction==2)
							{
							if (leftBullet1.isShot == 0)
							{
								leftBulletCount += 1;
							}
								leftBullet1.pos.x = player.pos.x;
								leftBullet1.pos.y = player.pos.y;
								leftBullet1.isShot = true;
							}
						}
					}
				}
			}
			if (leftBullet2.isShot == false)
			{
				////スペースを押したときのplayerbullet2の動き
				if (type == 2)
				{
					leftBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (direction == 2)
						{
						if (leftBullet2.isShot == 0)
						{
							rightBulletCount += 1;
						}
							leftBullet2.pos.x = player.pos.x + player.radius;
							leftBullet2.pos.y = player.pos.y;
							leftBullet2.isShot = true;
						}
					}
				}

			}

			if (leftBulletCount >= 10 && rightBulletCount >= 10)
			{
				if (leftBullet1.isShot == false || leftBullet2.isShot == false)
				{
					if (type == 0)
					{
						leftBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (direction == 2)
							{
								playerHP -= 10;
								leftBullet2.pos.x = player.pos.x + player.radius;
								leftBullet2.pos.y = player.pos.y;
								leftBullet2.isShot = true;
								leftBullet1.pos.x = player.pos.x;
								leftBullet1.pos.y = player.pos.y;
								leftBullet1.isShot = true;
							}
						}
					}
				}
			}

			if (leftBullet1.isShot == 1)
			{
				leftBullet1.pos.x -= leftBullet1.speed;

				if (leftBullet1.pos.x <0)
				{
					leftBullet1.isShot = false;
					leftBullet1.pos.y = player.pos.x;
				}
			}
			if (leftBullet2.isShot == 1)
			{
				leftBullet2.pos.x -= leftBullet2.speed;

				if (leftBullet2.pos.x < 0)
				{
					leftBullet2.isShot = false;
					leftBullet2.pos.y = player.pos.x;

				}
			}
			if (rightBullet1.isShot == false)
			{
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{

					//スペースを押したときのplayerbullet1の動き
					if (type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (direction == 3)
							{
							if (rightBullet1.isShot == 0)
							{
								leftBulletCount += 1;
							}
								rightBullet1.pos.x = player.pos.x;
								rightBullet1.pos.y = player.pos.y;
								rightBullet1.isShot = true;
							}
						}
					}
				}
			}
			if (rightBullet2.isShot == false)
			{
				////スペースを押したときのplayerbullet2の動き
				if (type == 2)
				{
					rightBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (direction == 3)
						{
						if (rightBullet2.isShot == 0)
						{
							rightBulletCount += 1;
						}
							rightBullet2.pos.x = player.pos.x + player.radius;
							rightBullet2.pos.y = player.pos.y;
							rightBullet2.isShot = true;
						}
					}
				}

			}

			if (leftBulletCount >= 10 && rightBulletCount >= 10)
			{
				if (rightBullet1.isShot == false || rightBullet2.isShot == false)
				{
					if (type == 0)
					{
						rightBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (direction == 3)
							{
								playerHP -= 10;
								rightBullet2.pos.x = player.pos.x + player.radius;
								rightBullet2.pos.y = player.pos.y;
								rightBullet2.isShot = true;
								rightBullet1.pos.x = player.pos.x;
								rightBullet1.pos.y = player.pos.y;
								rightBullet1.isShot = true;
							}
						}
					}
				}
			}

			if (rightBullet1.isShot == 1)
			{
				rightBullet1.pos.x += rightBullet1.speed;

				if (rightBullet1.pos.x >1280)
				{
					rightBullet1.isShot = false;
					rightBullet1.pos.y = player.pos.x;
				}
			}
			if (rightBullet2.isShot == 1)
			{
				rightBullet2.pos.x += rightBullet2.speed;

				if (rightBullet2.pos.x >1280)
				{
					rightBullet2.isShot = false;
					rightBullet2.pos.y = player.pos.x;

				}
			}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//platerの描画
			if (playerBack)
			{
				Novice::DrawSprite(
					int(player.pos.x),
					int(player.pos.y),
					playerBackHandle,
					1, 1, 0.0f,
					WHITE);
			}
			if (playerFront)
			{
				Novice::DrawSprite(
					int(player.pos.x),
					int(player.pos.y),
					playerFrontHandle,
					1, 1, 0.0f,
					WHITE);
			}

		if (strightBullet1.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(strightBullet1.pos.x),
				static_cast<int>(strightBullet1.pos.y),
				leftHandHandle,1,1,
				0.0f, WHITE);
		}

		if (strightBullet2.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(strightBullet2.pos.x),
				static_cast<int>(strightBullet2.pos.y),
				rightHandHandle,1,1,
				0.0f, WHITE);
		}
		if (backBullet1.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(backBullet1.pos.x),
				static_cast<int>(backBullet1.pos.y),
				leftHandHandle, 1, 1,
				0.0f, WHITE);
		}

		if (backBullet2.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(backBullet2.pos.x),
				static_cast<int>(backBullet2.pos.y),
				rightHandHandle, 1, 1,
				0.0f, WHITE);
		}
		if (leftBullet1.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(leftBullet1.pos.x),
				static_cast<int>(leftBullet1.pos.y),
				leftHandHandle, 1, 1,
				0.0f, WHITE);
		}

		if (leftBullet2.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(leftBullet2.pos.x),
				static_cast<int>(leftBullet2.pos.y),
				rightHandHandle, 1, 1,
				0.0f, WHITE);
		}
		if (rightBullet1.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(rightBullet1.pos.x),
				static_cast<int>(rightBullet1.pos.y),
				leftHandHandle, 1, 1,
				0.0f, WHITE);
		}
		if (rightBullet2.isShot)
		{
			Novice::DrawSprite(
				static_cast<int>(rightBullet2.pos.x),
				static_cast<int>(rightBullet2.pos.y),
				rightHandHandle, 1, 1,
				0.0f, WHITE);
		}
		// 敵描画
		Novice::DrawBox(
			int(enemy.pos.x),
			int(enemy.pos.y),
			int(enemy.width),
			int(enemy.height),
			0.0f, WHITE,
			kFillModeSolid
		);


		Novice::ScreenPrintf(20, 20, "%f", strightBullet1.pos.y);
		Novice::ScreenPrintf(20, 40, "%d", type);
		Novice::ScreenPrintf(20, 60, "%d", backBullet1.isShot);
		Novice::ScreenPrintf(20, 80, "%d", direction);
		Novice::ScreenPrintf(20, 100, "%d", leftBulletCount);
		Novice::ScreenPrintf(20, 120, "%d", rightBulletCount);
		Novice::ScreenPrintf(20, 140, "%d", playerHP);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}