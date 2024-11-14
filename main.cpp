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

PlayerBullet playerbullet1
{
	640,620,
	10,
	5,
	false,

};

PlayerBullet playerbullet2
{
	680,620,
	10,
	5,
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
	int type = 0;

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
			player.pos.y -= player.speed;
		}

		if (keys[DIK_S])
		{
			player.pos.y += player.speed;
		}

		if (keys[DIK_A])
		{
			player.pos.x -= player.speed;
		}

		if (keys[DIK_D])
		{
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

		if (playerbullet1.isShot == false || playerbullet2.isShot == false) {
			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
			{

				//スペースを押したときのplayerbullet1の動き
				if (type == 1)
				{

					playerbullet1.pos.x = player.pos.x;
					playerbullet1.pos.y = player.pos.y;

					playerbullet1.isShot = true;
				}
				////スペースを押したときのplayerbullet2の動き
				if (type == 2)
				{


					playerbullet2.isShot = true;
				}
				if (playerbullet1.isShot == 0 || playerbullet2.isShot == 0)

					playerbullet2.pos.x = player.pos.x + player.radius;
				playerbullet2.pos.y = player.pos.y;
				playerbullet2.isShot = true;
			}
			if (type == 0)

			{
				playerbullet1.pos.x = player.pos.x;
				playerbullet1.pos.y = player.pos.y;
				playerbullet1.isShot = true;
				playerbullet2.pos.x = player.pos.x + player.radius;
				playerbullet2.pos.y = player.pos.y;
				playerbullet2.isShot = true;
			}
		}

		if (playerbullet1.isShot == 1)
		{
			playerbullet1.pos.y -= playerbullet1.speed;

			if (playerbullet1.pos.y < 0)
			{
				playerbullet1.isShot = false;
				playerbullet1.pos.y = player.pos.x;

			}
		}

		if (playerbullet2.isShot == 1)
		{
			playerbullet2.pos.y -= playerbullet2.speed;
			if (playerbullet2.pos.y < 0)
			{
				playerbullet2.isShot = false;
				playerbullet2.pos.y = player.pos.x;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//platerの描画
		Novice::DrawBox(
			int(player.pos.x),
			int(player.pos.y),
			int(player.radius),
			int(player.radius),
			0.0f, WHITE, kFillModeSolid
		);

		if (playerbullet1.isShot) {
			Novice::DrawEllipse(
				static_cast<int>(playerbullet1.pos.x),
				static_cast<int>(playerbullet1.pos.y),
				static_cast<int>(playerbullet1.radius),
				static_cast<int>(playerbullet1.radius),
				0.0f, WHITE, kFillModeSolid);
		}

		if (playerbullet2.isShot) {
			Novice::DrawEllipse(
				static_cast<int>(playerbullet2.pos.x),
				static_cast<int>(playerbullet2.pos.y),
				static_cast<int>(playerbullet2.radius),
				static_cast<int>(playerbullet2.radius),
				0.0f, WHITE, kFillModeSolid);
		}
		
		if (playerbullet1.isShot)
		{
			Novice::DrawEllipse(
				static_cast<int>(playerbullet1.pos.x),
				static_cast<int>(playerbullet1.pos.y),
				static_cast<int>(playerbullet1.radius),
				static_cast<int>(playerbullet1.radius),
				0.0f, WHITE, kFillModeSolid);
		}

		if (playerbullet2.isShot)
		{
			Novice::DrawEllipse(
				static_cast<int>(playerbullet2.pos.x),
				static_cast<int>(playerbullet2.pos.y),
				static_cast<int>(playerbullet2.radius),
				static_cast<int>(playerbullet2.radius),
				0.0f, WHITE, kFillModeSolid);
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

		Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
		Novice::ScreenPrintf(20, 40, "%d", playerbullet1.isShot);
		Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
		Novice::ScreenPrintf(20, 40, "%d", type);

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