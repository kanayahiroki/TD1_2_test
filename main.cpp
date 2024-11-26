#include <Novice.h>
#include <corecrt_math_defines.h>
#include <corecrt_math.h>

const char kWindowTitle[] = "LC1D_09_カナヤ_ヒロキ_タイトル";


struct Vector2
{
	float x;
	float y;
};
struct Player
{
	Vector2 pos;
	float radius;
	float speed;
};
struct PlayerBullet
{
	Vector2 pos;
	float radius;
	float speed;
	int isShot;
	int type;
};



struct Enemy {
	Vector2 pos;
	float width;
	float height;
	float speed;
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

enum Scene
{
	TITLE,
	OPENE,
	GAME,
	CLEAR,
	OVER,
};

int scene = TITLE;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	Player player
	{
		640,620,
		40,
		5
	};
	PlayerBullet playerbullet1
	{
		640,620,
		10,
		5,
		false,
		0,
	};
	PlayerBullet playerbullet2
	{
		680,620,
		10,
		5,
		false,
		0,
	};

	// --- enemy初期化 ---
	Enemy enemy = {};
	enemy.pos.x = 640.0f;
	enemy.pos.y = 100.0f;
	enemy.height = 100.0f;
	enemy.width = 200.0f;
	enemy.speed = 8.0f;
	//ボスのHP
	int enemyCount = 40;
	///ふり幅///
	float amplitude = 540.0f;//ふり幅、波の高さ（大きさ）
	float theta = 0.0f;
	//// 敵の動き////////

	//最初の動き
	int enemyMove = false;


	//第二の動き
	//動いてるかイージング
	int enemyMoveFlag = 0;

	//最後の動き//
	//ウェーブの動き
	float enemyWave = false;
	//ワープするカウント
	float enemyTimer = 0.0f;

	////タイトルBGM/////
	int TitleBGM = Novice::LoadAudio("./Resources/Sounds/title.mp3");


	//////操作説明////////
	int SousaBGM = Novice::LoadAudio("./Resources/Sounds/sousa.mp3");



	////バトルBGM/////
	int BoseBGM = Novice::LoadAudio("./Resources/Sounds/bose.mp3");

	//BGMの変数
	int playHandle = -1;




	////////////////////////

	//敵の移動のためのブロック
	Enemy enemyBox = 
	{
	1000.0f,
	600.0f,
	200.0f,
	100.0f,
	};


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
		/// 
		
		switch (scene)
		{
		case TITLE:

			if (!Novice::IsPlayingAudio(playHandle)) //サウンドが再生されているか
			{
				playHandle = Novice::PlayAudio(TitleBGM, false, 1.0f);//サウンドを再生する
			}





			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0)
			{
				scene = OPENE;
				enemyCount = 40;
				Novice::StopAudio(playHandle);//サウンドが停止される
			}
			Novice::ScreenPrintf(500, 500, "title");



			break;

		case OPENE:
			if (!Novice::IsPlayingAudio(playHandle))//サウンドが再生されているか
			{
				playHandle = Novice::PlayAudio(SousaBGM, false, 1.0f);//サウンドを再生する

			}

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0)
			{
				scene = GAME;
				Novice::StopAudio(playHandle);//サウンドが停止される
			}
			Novice::ScreenPrintf(500, 500, "opene");

			break;

		case GAME:
			if (!Novice::IsPlayingAudio(playHandle)) //サウンドが再生されているか
			{
				playHandle = Novice::PlayAudio(BoseBGM, false, 1.0f);//サウンドを再生する
			}
			Novice::ScreenPrintf(500, 500, "game");

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
				playerbullet1.type = 1;
			}
			//玉の属性の切り替え
			if (keys[DIK_E])
			{
				playerbullet2.type = 2;
			}
			if (playerbullet1.isShot == false || playerbullet2.isShot == false) {
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{
					playerbullet1.pos.y = player.pos.x;
					playerbullet2.pos.y = player.pos.x;
					//スペースを押したときのplayerbullet1の動き
					if (playerbullet1.type == 1)
					{

						playerbullet1.isShot = true;
					}


					////スペースを押したときのplayerbullet2の動き
					if (playerbullet2.type == 2)
					{

						playerbullet2.isShot = true;
					}
					if (playerbullet1.isShot == 0 || playerbullet2.isShot == 0)
					{
						playerbullet1.pos.y = player.pos.x;
						playerbullet1.isShot = true;
						playerbullet2.pos.y = player.pos.x;
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
			Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
			Novice::ScreenPrintf(20, 40, "%d", playerbullet1.isShot);

			////////////////////////////
			//敵の動き、ワープの処理///
			///////////////////////////

			//敵の動き//
			///////////

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
			{
				enemyCount--;
			}

			if (enemyCount >= 40)
			{
				enemyMove = true;
			}

			//最初の動き//
			if (enemyMove)
			{
				enemy.pos.x += enemy.speed;
				if (enemy.pos.x > 1280 - enemy.width)
				{
					enemy.speed = -enemy.speed;
				}
				if (enemy.pos.x < 0)
				{
					enemy.speed = -enemy.speed;
				}
			}

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
			{
				enemyCount--;
			}
			//第二の動き
			/*if(enemyCount <20)
			{
				enemyMove = false;
				enemyMoveFlag = true;
			}
			if (enemyMoveFlag)
			{
				t += 1.0f / 120.0f;
				if (1.0f < t)
				{
					t = 1.0f;
				}
				enemy.pos.x = (1.0f - t) * enemy.pos.x + 80.0f;
				enemy.pos.y = (1.0f - t) * enemy.pos.y + 80.0f;
			}*/
			//if( enemy.pos.x = 80. && enemy.pos.y = 80.0f )












			//ワープ

			if (enemyCount < 0)
			{
				enemyMoveFlag = false;
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
					//左下
					enemy.pos.x = 80.0f;
					enemy.pos.y = 600.0f;
				}
				if (enemyTimer == 500)
				{
					enemyWave = false;
					//右下
					enemy.pos.x = 1000.0f;
					enemy.pos.y = 600.0f;
				}
				if (enemyTimer == 800)
				{
					enemyWave = false;
					//右上
					enemy.pos.x = 1000.0f;
					enemy.pos.y = 80.0f;
				}
				if (enemyTimer == 1100)
				{
					enemyWave = false;
					//左上
					enemy.pos.x = 80.0f;
					enemy.pos.y = 80.0f;
					enemyTimer = 0.0f;
				}
			}



			////////////////////////////////////
		   ////////////////////////////////////


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

			if(enemyCount < 0)
			{
				scene = CLEAR;
			}

			if (!keys[DIK_H] && preKeys[DIK_H])
			{
				scene = OVER;
			}


			break;

			case CLEAR:
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{
					scene = TITLE;
				}
				Novice::ScreenPrintf(500, 500, "clear");
				break;
			case OVER:
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{
					scene = TITLE;
				}
				Novice::ScreenPrintf(500, 500, "over");
				break;





		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		switch (scene)
		{
		case TITLE:

			
			Novice::ScreenPrintf(0, 0, "%f", enemyTimer);
			

			break;


		case OPENE:


			break;

		case GAME:

			Novice::ScreenPrintf(0, 100, "%d", enemyCount);
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
			//敵移動ブロック
			Novice::DrawBox(
				static_cast<int>(enemyBox.pos.x),
				static_cast<int>(enemyBox.pos.y),
				static_cast<int>(enemyBox.width),
				static_cast<int>(enemyBox.height),
				0.0f, WHITE,
				kFillModeSolid
			);

			break;

		case CLEAR:
			Novice::ScreenPrintf(500, 500, "clear");


			break;

		case OVER:
			Novice::ScreenPrintf(500, 500, "over");

			break;





		}

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