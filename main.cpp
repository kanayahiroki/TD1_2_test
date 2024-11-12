#include <Novice.h>
#include <corecrt_math.h>

const char kWindowTitle[] = "LC1D_09_カナヤ_ヒロキ_タイトル";

struct Vector2 {
	float x;
	float y;
};

struct Enemy {
	Vector2 pos;
	float width;
	float height;
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

struct PlayerBullet
{
	Vector2 pos;
	float radius;
	float speed;

	int isShot;
	

};
struct Player
{
	Vector2 pos;
	Vector2 dir;
	float radius;
	float speed;
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

	// --- enemyBullet初期化 ---
	// 左辺
	Bullet leftSideEnemyBullet[4]{};
	for (int i = 0; i < 4; ++i) {
		leftSideEnemyBullet[i].width = 10.0f;
		leftSideEnemyBullet[i].height = 10.0f;

		leftSideEnemyBullet[i].start.x = enemy.pos.x;
		leftSideEnemyBullet[i].start.y = enemy.pos.y;

		leftSideEnemyBullet[i].end.x = (enemy.pos.x - (leftSideEnemyBullet[i].width * 2.0f));
		leftSideEnemyBullet[i].end.y = (enemy.pos.y - leftSideEnemyBullet[i].height);

		leftSideEnemyBullet[i].pos.x = leftSideEnemyBullet[i].start.x;
		leftSideEnemyBullet[i].pos.y = leftSideEnemyBullet[i].start.y;

		leftSideEnemyBullet[i].columns = 4;
		leftSideEnemyBullet[i].space = 20;
		leftSideEnemyBullet[i].speed = 5.0f;
		leftSideEnemyBullet[i].length = 0.0f;
		leftSideEnemyBullet[i].t = 0.0f;
		leftSideEnemyBullet[i].easedT = 0.0f;

		leftSideEnemyBullet[i].interpolation = false;
		leftSideEnemyBullet[i].isBulletShot = false;
	}

	// 右辺
	Bullet rightSideEnemyBullet[4]{};
	for (int i = 0; i < 4; ++i) {
		rightSideEnemyBullet[i].width = 10.0f;
		rightSideEnemyBullet[i].height = 10.0f;

		rightSideEnemyBullet[i].start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet[i].width;
		rightSideEnemyBullet[i].start.y = enemy.pos.y;

	
	

		rightSideEnemyBullet[i].end.x = (enemy.pos.x + enemy.width + rightSideEnemyBullet[i].width);
	
		rightSideEnemyBullet[i].end.y = (enemy.pos.y - rightSideEnemyBullet[i].height);

		rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
		rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;

		rightSideEnemyBullet[i].columns = 4;
		rightSideEnemyBullet[i].space = 20;
		rightSideEnemyBullet[i].speed = 5.0f;
		rightSideEnemyBullet[i].length = 0.0f;
		rightSideEnemyBullet[i].t = 0.0f;
		rightSideEnemyBullet[i].easedT = 0.0f;

		rightSideEnemyBullet[i].interpolation = false;
		rightSideEnemyBullet[i].isBulletShot = false;
	}
	
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
				// --- 敵の左辺の弾 ---
		for (int i = 0; i < leftSideEnemyBullet[0].columns; ++i) {




			if (!leftSideEnemyBullet[i].isBulletShot) {
				leftSideEnemyBullet[i].isBulletShot = true;
				leftSideEnemyBullet[i].length = 0.0f;

				leftSideEnemyBullet[i].t = 0.0f;
				leftSideEnemyBullet[i].easedT = 0.0f;
			}

			if (leftSideEnemyBullet[i].isBulletShot) {

				if (!leftSideEnemyBullet[i].interpolation) {

					leftSideEnemyBullet[i].t += 0.008f;

					if (leftSideEnemyBullet[i].t >= 1.0f) {
						leftSideEnemyBullet[i].t = 1.0f;
						leftSideEnemyBullet[i].interpolation = true;
					}

					leftSideEnemyBullet[i].easedT = 1.0f - powf(1.0f - leftSideEnemyBullet[i].t, 3.0f);


					leftSideEnemyBullet[i].pos.x = (1.0f - leftSideEnemyBullet[i].easedT) * leftSideEnemyBullet[i].start.x + leftSideEnemyBullet[i].easedT * leftSideEnemyBullet[i].end.x;
					leftSideEnemyBullet[i].pos.y = (1.0f - leftSideEnemyBullet[i].easedT) * leftSideEnemyBullet[i].start.y + leftSideEnemyBullet[i].easedT * leftSideEnemyBullet[i].end.y;

					leftSideEnemyBullet[i].space = static_cast<int>((1.0f - leftSideEnemyBullet[i].easedT) * 20 + leftSideEnemyBullet[i].easedT * 27 + 0.5f);

				} else {

					player.dir.x = player.pos.x - enemy.pos.x;
					player.dir.y = player.pos.y - enemy.pos.y;

					leftSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

					if (leftSideEnemyBullet[i].length > 0.0f) {
						leftSideEnemyBullet[i].velocity.x = (player.dir.x / leftSideEnemyBullet[i].length) * leftSideEnemyBullet[i].speed;
						leftSideEnemyBullet[i].velocity.y = (player.dir.y / leftSideEnemyBullet[i].length) * leftSideEnemyBullet[i].speed;
					}

					leftSideEnemyBullet[i].pos.x += leftSideEnemyBullet[i].velocity.x;
					leftSideEnemyBullet[i].pos.y += leftSideEnemyBullet[i].velocity.y;

					if (leftSideEnemyBullet[i].pos.y <= 0.0f ||
						leftSideEnemyBullet[i].pos.x <= 0.0f ||
						leftSideEnemyBullet[i].pos.x >= 1280.0f ||
						leftSideEnemyBullet[i].pos.y + leftSideEnemyBullet[i].height >= 720.0f) {

						leftSideEnemyBullet[i].pos.x = leftSideEnemyBullet[i].start.x;
						leftSideEnemyBullet[i].pos.y = leftSideEnemyBullet[i].start.y;
						leftSideEnemyBullet[i].isBulletShot = false;
						leftSideEnemyBullet[i].interpolation = false;
					}
				}
			}
		}

				// --- 敵の右辺の弾 ---
				for (int i = 0; i < rightSideEnemyBullet[0].columns; ++i) {

					if (!rightSideEnemyBullet[i].isBulletShot) {
						rightSideEnemyBullet[i].isBulletShot = true;
						rightSideEnemyBullet[i].length = 0.0f;

						rightSideEnemyBullet[i].t = 0.0f;
						rightSideEnemyBullet[i].easedT = 0.0f;
					}

					if (rightSideEnemyBullet[i].isBulletShot) {

						if (!rightSideEnemyBullet[i].interpolation) {

							rightSideEnemyBullet[i].t += 0.008f;

							if (rightSideEnemyBullet[i].t >= 1.0f) {
								rightSideEnemyBullet[i].t = 1.0f;
								rightSideEnemyBullet[i].interpolation = true;
							}

							rightSideEnemyBullet[i].easedT = 1.0f - powf(1.0f - rightSideEnemyBullet[i].t, 3.0f);

							rightSideEnemyBullet[i].pos.x = (1.0f - rightSideEnemyBullet[i].easedT) * rightSideEnemyBullet[i].start.x + rightSideEnemyBullet[i].easedT * rightSideEnemyBullet[i].end.x;
							rightSideEnemyBullet[i].pos.y = (1.0f - rightSideEnemyBullet[i].easedT) * rightSideEnemyBullet[i].start.y + rightSideEnemyBullet[i].easedT * rightSideEnemyBullet[i].end.y;

							rightSideEnemyBullet[i].space = static_cast<int>((1.0f - rightSideEnemyBullet[i].easedT) * 20 + rightSideEnemyBullet[i].easedT * 27 + 0.5f);

						} else {

							player.dir.x = player.pos.x - enemy.pos.x;
							player.dir.y = player.pos.y - enemy.pos.y;

							rightSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

							if (rightSideEnemyBullet[i].length > 0.0f) {
								rightSideEnemyBullet[i].velocity.x = (player.dir.x / rightSideEnemyBullet[i].length) * rightSideEnemyBullet[i].speed;
								rightSideEnemyBullet[i].velocity.y = (player.dir.y / rightSideEnemyBullet[i].length) * rightSideEnemyBullet[i].speed;
							}

							rightSideEnemyBullet[i].pos.x += rightSideEnemyBullet[i].velocity.x;
							rightSideEnemyBullet[i].pos.y += rightSideEnemyBullet[i].velocity.y;

							if (rightSideEnemyBullet[i].pos.y <= 0.0f ||
								rightSideEnemyBullet[i].pos.x <= 0.0f ||
								rightSideEnemyBullet[i].pos.x >= 1280.0f ||
								rightSideEnemyBullet[i].pos.y + rightSideEnemyBullet[i].height >= 720.0f) {

								rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
								rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;
								rightSideEnemyBullet[i].isBulletShot = false;
								rightSideEnemyBullet[i].interpolation = false;
							}
						}
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
					static_cast<int>(player.pos.x),
					static_cast<int>(player.pos.y),
					static_cast<int>(player.radius),
					static_cast<int>(player.radius),

					0.0f, WHITE, kFillModeSolid);
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
				Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
				Novice::ScreenPrintf(20, 40, "%d", type);




				///
				/// ↑更新処理ここまで
				///

				///
				/// ↓描画処理ここから
				///



				// 敵の左辺の弾
				for (int j = 0; j < leftSideEnemyBullet[0].columns; ++j) {
					if (leftSideEnemyBullet[j].isBulletShot) {
						Novice::DrawBox(
							static_cast<int>(leftSideEnemyBullet[j].pos.x),
							static_cast<int>(leftSideEnemyBullet[j].pos.y) + j * (static_cast<int>(leftSideEnemyBullet[j].width) + leftSideEnemyBullet[j].space),
							static_cast<int>(leftSideEnemyBullet[j].width),
							static_cast<int>(leftSideEnemyBullet[j].height),
							0.0f, WHITE,
							kFillModeSolid
						);
					}
				}


				// 敵の右辺の弾
				for (int j = 0; j < rightSideEnemyBullet[0].columns; ++j) {
					if (rightSideEnemyBullet[j].isBulletShot) {
						Novice::DrawBox(
							static_cast<int>(rightSideEnemyBullet[j].pos.x),
							static_cast<int>(rightSideEnemyBullet[j].pos.y) + j * (static_cast<int>(rightSideEnemyBullet[j].width) + rightSideEnemyBullet[j].space),
							static_cast<int>(rightSideEnemyBullet[j].width),
							static_cast<int>(rightSideEnemyBullet[j].height),
							0.0f, WHITE,
							kFillModeSolid
						);
					}
				}

				// 敵描画
				Novice::DrawBox(
					static_cast<int>(enemy.pos.x),
					static_cast<int>(enemy.pos.y),
					static_cast<int>(enemy.width),
					static_cast<int>(enemy.height),
					0.0f, WHITE,
					kFillModeSolid
				);

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
	
