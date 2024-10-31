#include <Novice.h>
#include <corecrt_math_defines.h>
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
	float width;
	float height;
	float speed;
	int columns;
	int space;
	int isBulletShot;
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
  
		enemy.pos.x = 640.0f;
		enemy.pos.y = 100.0f;
		enemy.height = 100.0f;
		enemy.width = 200.0f;
		//敵のHP
		//int enemyCount = 50;

		//ふり幅
		float amplitude = 540.0f;//ふり幅、波の高さ（大きさ）
		float theta = 0.0f;

		//ワープするカウント
		float enemyTimer = 0.0f;
		//ウェーブの動き
		float enemyWave = false;




	// --- enemyBullet初期化 ---
	Bullet enemyBullet[4]{};

	for (int i = 0; i < 4; ++i) {
		enemyBullet[i].width = 20.0f;
		enemyBullet[i].height = 20.0f;
		enemyBullet[i].pos.x = enemy.pos.x + 8.0f;
		enemyBullet[i].pos.y = enemy.pos.y + enemy.height;
		enemyBullet[i].columns = 4;
		enemyBullet[i].space = 34;
		enemyBullet[i].speed = 1.0f;
		enemyBullet[i].isBulletShot = false;
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
		
		
		//敵のワープ処理
		enemyTimer++;
		if (enemyWave) 
		{
			//波打つ動き
			enemy.pos.x = static_cast<float>(sin(theta)) * amplitude + 540;
			theta += float(M_PI) / 100.0f;
		}
		if(enemyTimer == 200)
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


		
		




		// --- 敵の弾 ---
		for (int i = 0; i < enemyBullet[0].columns; ++i) {
			if (!enemyBullet[i].isBulletShot) {
				enemyBullet[i].isBulletShot = true;


				enemyBullet[i].pos.y = enemy.pos.y + enemy.height;
			}

			if (enemyBullet[i].isBulletShot) {
				enemyBullet[i].pos.y += enemyBullet[i].speed;
				if (enemyBullet[i].pos.y + enemyBullet[i].height >= 720.0f) {
					enemyBullet[i].isBulletShot = false;
				}
			}
		}
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(0, 0, "%f",enemyTimer);
		
		
		for (int j = 0; j < enemyBullet[0].columns; ++j) {
			if (enemyBullet[j].isBulletShot) {
				Novice::DrawBox(
					static_cast<int>(enemyBullet[0].pos.x) + j * (static_cast<int>(enemyBullet[j].width) + enemyBullet[j].space),
					static_cast<int>(enemyBullet[j].pos.y),
					static_cast<int>(enemyBullet[j].width),
					static_cast<int>(enemyBullet[j].height),
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
