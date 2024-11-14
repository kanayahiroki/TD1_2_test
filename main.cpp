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

	//ボスのHP
	int enemyCount = 40;

	///ふり幅///
	float amplitude = 540.0f;//ふり幅、波の高さ（大きさ）
	float theta = 0.0f;
	///////////

	//最初の動き
	float enemyMove = false;

	//第二の動き

	//最後の動き//
	//ウェーブの動き
	float enemyWave = false;
	//ワープするカウント
	float enemyTimer = 0.0f;

	Player player{};
	player.pos.x = 640.0f;
	player.pos.y = 620.0f;
	player.dir.x = player.pos.x - enemy.pos.y;
	player.dir.y = player.pos.y - enemy.pos.y;
	player.radius = 40.0f;
	player.speed = 5.0f;
	int type = 0;

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

	// 上辺
	Bullet topSideEnemyBullet[4]{};
	for (int i = 0; i < 4; ++i) {
		topSideEnemyBullet[i].width = 10.0f;
		topSideEnemyBullet[i].height = 10.0f;

		topSideEnemyBullet[i].start.x = enemy.pos.x;
		topSideEnemyBullet[i].start.y = enemy.pos.y;

		topSideEnemyBullet[i].end.x = enemy.pos.x;
		topSideEnemyBullet[i].end.y = enemy.pos.y - (topSideEnemyBullet[i].height * 2);

		topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
		topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;

		topSideEnemyBullet[i].columns = 4;
		topSideEnemyBullet[i].space = 55;
		topSideEnemyBullet[i].speed = 5.0f;
		topSideEnemyBullet[i].length = 0.0f;
		topSideEnemyBullet[i].t = 0.0f;
		topSideEnemyBullet[i].easedT = 0.0f;

		topSideEnemyBullet[i].interpolation = false;
		topSideEnemyBullet[i].isBulletShot = false;
	}

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

		//ワープ
		if (enemyCount == 20)
		{
			enemyMove = false;
			enemyTimer++;
			if (enemyWave)
			{
				//波打つ動き
				enemy.pos.x = static_cast<float>(sin(theta)) * amplitude + 540;
				theta += float(M_PI) / 100.0f;
			}
			if (enemyTimer == 200)
			{
				enemyWave = false;
				enemy.pos.x = 100.0f;
				enemy.pos.y = 500.0f;
			}
			if (enemyTimer == 400)
			{
				enemyWave = false;
				enemy.pos.x = 800.0f;
				enemy.pos.y = 500.0f;
			}
			if (enemyTimer == 600)
			{
				enemyWave = false;
				enemy.pos.x = 800.0f;
				enemy.pos.y = 100.0f;
				enemyTimer = 0.0f;
			}
		}

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
			static_cast<int>(player.pos.x),
			static_cast<int>(player.pos.y),
			static_cast<int>(player.radius),
			static_cast<int>(player.radius),
			0.0f, WHITE, kFillModeSolid);

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
		
		//platerの描画
		Novice::DrawBox(
			int(player.pos.x),
			int(player.pos.y),
			int(player.radius),
			int(player.radius),
			0.0f, WHITE, kFillModeSolid
		);

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

	    // 敵の左辺の弾
		for (int j = 0; j < leftSideEnemyBullet[0].columns; ++j) {
			if (leftSideEnemyBullet[j].isBulletShot) {

				Novice::DrawBox(
					int(leftSideEnemyBullet[j].pos.x),
					int(leftSideEnemyBullet[j].pos.y) + j * (int(leftSideEnemyBullet[j].height) + leftSideEnemyBullet[j].space),
					int(leftSideEnemyBullet[j].width),
					int(leftSideEnemyBullet[j].height),
					0.0f, WHITE,
					kFillModeSolid
				);
			}
		}

		// 敵の右辺の弾
		for (int j = 0; j < rightSideEnemyBullet[0].columns; ++j) {
			if (rightSideEnemyBullet[j].isBulletShot) {
				Novice::DrawBox(
					int(rightSideEnemyBullet[j].pos.x),
					int(rightSideEnemyBullet[j].pos.y) + j * (int(rightSideEnemyBullet[j].height) + rightSideEnemyBullet[j].space),
					int(rightSideEnemyBullet[j].width),
					int(rightSideEnemyBullet[j].height),
					0.0f, WHITE,
					kFillModeSolid
				);
			}
		}

		// 敵の上辺の弾
		for (int j = 0; j < topSideEnemyBullet[0].columns; ++j) {
			if (topSideEnemyBullet[j].isBulletShot) {
				Novice::DrawBox(
					int(topSideEnemyBullet[j].pos.x) + j * (int(topSideEnemyBullet[j].width) + topSideEnemyBullet[j].space),
					int(topSideEnemyBullet[j].pos.y),
					int(topSideEnemyBullet[j].width),
					int(topSideEnemyBullet[j].height),
					0.0f, WHITE,
					kFillModeSolid
				);
			}
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
		Novice::ScreenPrintf(0, 0, "%f", enemyTimer);

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