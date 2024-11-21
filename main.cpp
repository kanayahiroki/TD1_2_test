#include <Novice.h>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

const char kWindowTitle[] = "LC1D_09_カナヤ_ヒロキ_タイトル";

struct Vector2 {
	float x;
	float y;
};

struct Vertex {
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
};

struct Enemy {
	Vector2 pos;
	Vector2 src;
	Vertex vertex;
	int frameCount;
	int frameCounter;
	float width;
	float height;
	float speed;
};

struct Bullet {
	Vertex vertex;
	Vector2 pos;
	Vector2 src;
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

#pragma region Initialization

	// --- enemy初期化 ---
    // 敵キャラクターの基本情報を設定
	int enemyTexture = Novice::LoadTexture("./Resources/images/teki.png");

	Enemy enemy = {};
	enemy.pos.x = 200.0f;    // 初期位置のX座標
	enemy.pos.y = 100.0f;    // 初期位置のY座標

	enemy.src.x = 0.0f;      // テクスチャの描画開始位置X
	enemy.src.y = 0.0f;      // テクスチャの描画開始位置Y

	enemy.frameCount = 0;    // アニメーションフレーム数
	enemy.frameCounter = 0;  // フレーム更新用のカウンター

	// サイズ設定
	enemy.width = 200.0f;
	enemy.height = 100.0f;
	// 頂点位置の初期化（敵の四隅）
	enemy.vertex.leftTop.x = enemy.pos.x;
	enemy.vertex.leftTop.y = enemy.pos.y;

	enemy.vertex.rightTop.x = enemy.pos.x + enemy.width;
	enemy.vertex.rightTop.y = enemy.pos.y;

	enemy.vertex.leftBottom.x = enemy.pos.x;
	enemy.vertex.leftBottom.y = enemy.pos.y + enemy.height;

	enemy.vertex.rightBottom.x = enemy.pos.x + enemy.width;
	enemy.vertex.rightBottom.y = enemy.pos.y + enemy.height;

	// --- enemyBullet初期化 ---
	
	int enemyBulletTexture = Novice::LoadTexture("./Resources/images/tekitama.png");

	// 左辺の弾の初期化
	Bullet leftSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		// 弾の基本情報
		leftSideEnemyBullet[i].width = 20.0f;  // 弾の幅
		leftSideEnemyBullet[i].height = 20.0f; // 弾の高さ

		// 弾の開始位置（敵の左辺）
		leftSideEnemyBullet[i].start.x = enemy.pos.x;
		leftSideEnemyBullet[i].start.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

		// 弾の終了位置（左方向への移動を設定）
		leftSideEnemyBullet[i].end.x = enemy.pos.x - (leftSideEnemyBullet[i].width * 2.0f);
		leftSideEnemyBullet[i].end.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

		// 弾の初期位置を設定
		leftSideEnemyBullet[i].pos.x = leftSideEnemyBullet[i].start.x;
		leftSideEnemyBullet[i].pos.y = leftSideEnemyBullet[i].start.y;

		leftSideEnemyBullet[i].columns = 4;
		leftSideEnemyBullet[i].space = 20;
		leftSideEnemyBullet[i].speed = 5.0f;
		leftSideEnemyBullet[i].length = 0.0f;
		leftSideEnemyBullet[i].t = 0.0f;
		leftSideEnemyBullet[i].easedT = 0.0f;


		// 左上
		leftSideEnemyBullet[i].vertex.leftTop.x = leftSideEnemyBullet[i].pos.x;
		leftSideEnemyBullet[i].vertex.leftTop.y = leftSideEnemyBullet[i].pos.y;

		// 右上
		leftSideEnemyBullet[i].vertex.rightTop.x = leftSideEnemyBullet[i].pos.x + leftSideEnemyBullet[i].width;
		leftSideEnemyBullet[i].vertex.rightTop.y = leftSideEnemyBullet[i].pos.y;

		// 左下
		leftSideEnemyBullet[i].vertex.leftBottom.x = leftSideEnemyBullet[i].pos.x;
		leftSideEnemyBullet[i].vertex.leftBottom.y = leftSideEnemyBullet[i].pos.y + leftSideEnemyBullet[i].height;

		// 右下
		leftSideEnemyBullet[i].vertex.rightBottom.x = leftSideEnemyBullet[i].pos.x + leftSideEnemyBullet[i].width;
		leftSideEnemyBullet[i].vertex.rightBottom.y = leftSideEnemyBullet[i].pos.y + leftSideEnemyBullet[i].height;



		leftSideEnemyBullet[i].src.x = 0.0f;
		leftSideEnemyBullet[i].src.y = 0.0f;

		// 弾のその他プロパティ
		leftSideEnemyBullet[i].columns = 2;   // 列数（アニメーション用）
		leftSideEnemyBullet[i].space = 12;    // 弾同士の間隔
		leftSideEnemyBullet[i].speed = 5.0f;  // 弾の速度
		leftSideEnemyBullet[i].length = 0.0f; // 移動距離
		leftSideEnemyBullet[i].t = 0.0f;      // 時間補間
		leftSideEnemyBullet[i].easedT = 0.0f; // 補間値

		leftSideEnemyBullet[i].interpolation = false;  // 補間処理のフラグ
		leftSideEnemyBullet[i].isBulletShot = false;   // 発射中フラグ
	}

	// 右辺の弾の初期化
	Bullet rightSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		// サイズ設定
		rightSideEnemyBullet[i].width = 20.0f;
		rightSideEnemyBullet[i].height = 20.0f;

		// 弾の開始座標 (敵の右端から発射)
		rightSideEnemyBullet[i].start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet[i].width;
		rightSideEnemyBullet[i].start.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

		rightSideEnemyBullet[i].end.x = (enemy.pos.x + enemy.width + rightSideEnemyBullet[i].width);
		rightSideEnemyBullet[i].end.y = (enemy.pos.y - rightSideEnemyBullet[i].height);
		// 弾の終了座標 (右方向へ進む)
		rightSideEnemyBullet[i].end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet[i].width);
		rightSideEnemyBullet[i].end.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

		// 弾の現在座標
		rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
		rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;



		// 左上
		rightSideEnemyBullet[i].vertex.leftTop.x = rightSideEnemyBullet[i].pos.x;
		rightSideEnemyBullet[i].vertex.leftTop.y = rightSideEnemyBullet[i].pos.y;

		// 右上
		rightSideEnemyBullet[i].vertex.rightTop.x = rightSideEnemyBullet[i].pos.x + rightSideEnemyBullet[i].width;
		rightSideEnemyBullet[i].vertex.rightTop.y = rightSideEnemyBullet[i].pos.y;

		// 左下
		rightSideEnemyBullet[i].vertex.leftBottom.x = rightSideEnemyBullet[i].pos.x;
		rightSideEnemyBullet[i].vertex.leftBottom.y = rightSideEnemyBullet[i].pos.y + rightSideEnemyBullet[i].height;

		// 右下
		rightSideEnemyBullet[i].vertex.rightBottom.x = rightSideEnemyBullet[i].pos.x + rightSideEnemyBullet[i].width;
		rightSideEnemyBullet[i].vertex.rightBottom.y = rightSideEnemyBullet[i].pos.y + rightSideEnemyBullet[i].height;


		rightSideEnemyBullet[i].src.x = 0.0f;
		rightSideEnemyBullet[i].src.y = 0.0f;

		rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
		rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;

		// 弾の動作パラメータ
		rightSideEnemyBullet[i].columns = 2;
		rightSideEnemyBullet[i].space = 12;
		rightSideEnemyBullet[i].speed = 5.0f;
		rightSideEnemyBullet[i].length = 0.0f;
		rightSideEnemyBullet[i].t = 0.0f;
		rightSideEnemyBullet[i].easedT = 0.0f;

		// フラグの初期化
		rightSideEnemyBullet[i].interpolation = false;
		rightSideEnemyBullet[i].isBulletShot = false;
	}

	// 上辺の弾の初期化
	Bullet topSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		// サイズ設定
		topSideEnemyBullet[i].width = 20.0f;
		topSideEnemyBullet[i].height = 20.0f;

		// 弾の開始座標 (敵の上端から発射)
		topSideEnemyBullet[i].start.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
		topSideEnemyBullet[i].start.y = enemy.pos.y;

		// 弾の終了座標 (上方向へ進む)
		topSideEnemyBullet[i].end.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
		topSideEnemyBullet[i].end.y = enemy.pos.y - (topSideEnemyBullet[i].height * 2.0f);

		// 弾の現在座標
		topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
		topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;


		// 左上
		topSideEnemyBullet[i].vertex.leftTop.x = topSideEnemyBullet[i].pos.x;
		topSideEnemyBullet[i].vertex.leftTop.y = topSideEnemyBullet[i].pos.y;

		// 右上
		topSideEnemyBullet[i].vertex.rightTop.x = topSideEnemyBullet[i].pos.x + topSideEnemyBullet[i].width;
		topSideEnemyBullet[i].vertex.rightTop.y = topSideEnemyBullet[i].pos.y;

		// 左下
		topSideEnemyBullet[i].vertex.leftBottom.x = topSideEnemyBullet[i].pos.x;
		topSideEnemyBullet[i].vertex.leftBottom.y = topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height;

		// 右下
		topSideEnemyBullet[i].vertex.rightBottom.x = topSideEnemyBullet[i].pos.x + topSideEnemyBullet[i].width;
		topSideEnemyBullet[i].vertex.rightBottom.y = topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height;


		topSideEnemyBullet[i].src.x = 0.0f;
		topSideEnemyBullet[i].src.y = 0.0f;


		// 弾の動作パラメータ
		topSideEnemyBullet[i].columns = 2;
		topSideEnemyBullet[i].space = 55;
		topSideEnemyBullet[i].speed = 5.0f;
		topSideEnemyBullet[i].length = 0.0f;
		topSideEnemyBullet[i].t = 0.0f;
		topSideEnemyBullet[i].easedT = 0.0f;

		// フラグの初期化
		topSideEnemyBullet[i].interpolation = false;
		topSideEnemyBullet[i].isBulletShot = false;
	}

	// 下辺の弾の初期化
	Bullet bottomSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		// サイズ設定
		bottomSideEnemyBullet[i].width = 20.0f;
		bottomSideEnemyBullet[i].height = 20.0f;

		// 弾の開始座標 (敵の下端から発射)
		bottomSideEnemyBullet[i].start.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
		bottomSideEnemyBullet[i].start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet[i].height;

		// 弾の終了座標 (下方向へ進む)
		bottomSideEnemyBullet[i].end.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
		bottomSideEnemyBullet[i].end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet[i].height;

		// 弾の現在座標
		bottomSideEnemyBullet[i].pos.x = bottomSideEnemyBullet[i].start.x;
		bottomSideEnemyBullet[i].pos.y = bottomSideEnemyBullet[i].start.y;


		// 左上
		bottomSideEnemyBullet[i].vertex.leftTop.x = bottomSideEnemyBullet[i].pos.x;
		bottomSideEnemyBullet[i].vertex.leftTop.y = bottomSideEnemyBullet[i].pos.y;

		// 右上
		bottomSideEnemyBullet[i].vertex.rightTop.x = bottomSideEnemyBullet[i].pos.x + bottomSideEnemyBullet[i].width;
		bottomSideEnemyBullet[i].vertex.rightTop.y = bottomSideEnemyBullet[i].pos.y;

		// 左下
		bottomSideEnemyBullet[i].vertex.leftBottom.x = bottomSideEnemyBullet[i].pos.x;
		bottomSideEnemyBullet[i].vertex.leftBottom.y = bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height;

		// 右下
		bottomSideEnemyBullet[i].vertex.rightBottom.x = bottomSideEnemyBullet[i].pos.x + bottomSideEnemyBullet[i].width;
		bottomSideEnemyBullet[i].vertex.rightBottom.y = bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height;

		bottomSideEnemyBullet[i].src.x = 0.0f;
		bottomSideEnemyBullet[i].src.y = 0.0f;

		// 弾の動作パラメータ
		bottomSideEnemyBullet[i].columns = 2;
		bottomSideEnemyBullet[i].space = 55;
		bottomSideEnemyBullet[i].speed = 5.0f;
		bottomSideEnemyBullet[i].length = 0.0f;
		bottomSideEnemyBullet[i].t = 0.0f;
		bottomSideEnemyBullet[i].easedT = 0.0f;

		// フラグの初期化
		bottomSideEnemyBullet[i].interpolation = false;
		bottomSideEnemyBullet[i].isBulletShot = false;
	}

	// =======================
    // 左上の弾丸初期化
    // =======================
	Bullet leftTopEnemyBullet{};

	// サイズ設定
	leftTopEnemyBullet.width = 20.0f;
	leftTopEnemyBullet.height = 20.0f;

	// 開始座標 (敵の左上)
	leftTopEnemyBullet.start.x = enemy.pos.x;
	leftTopEnemyBullet.start.y = enemy.pos.y;

	// 終了座標 (左上方向へ進む)
	leftTopEnemyBullet.end.x = enemy.pos.x - (leftTopEnemyBullet.width * 2.0f);
	leftTopEnemyBullet.end.y = enemy.pos.y - (leftTopEnemyBullet.height * 2.0f);

	// 現在座標の初期化
	leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
	leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;

	// 左上
	leftTopEnemyBullet.vertex.leftTop.x = leftTopEnemyBullet.pos.x;
	leftTopEnemyBullet.vertex.leftTop.y = leftTopEnemyBullet.pos.y;

	// 右上
	leftTopEnemyBullet.vertex.rightTop.x = leftTopEnemyBullet.pos.x + leftTopEnemyBullet.width;
	leftTopEnemyBullet.vertex.rightTop.y = leftTopEnemyBullet.pos.y;

	// 左下
	leftTopEnemyBullet.vertex.leftBottom.x = leftTopEnemyBullet.pos.x;
	leftTopEnemyBullet.vertex.leftBottom.y = leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height;

	// 右下
	leftTopEnemyBullet.vertex.rightBottom.x = leftTopEnemyBullet.pos.x + leftTopEnemyBullet.width;
	leftTopEnemyBullet.vertex.rightBottom.y = leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height;

	leftTopEnemyBullet.src.x = 0.0f;
	leftTopEnemyBullet.src.y = 0.0f;

	// 弾の動作パラメータ
	leftTopEnemyBullet.speed = 5.0f;
	leftTopEnemyBullet.length = 0.0f;
	leftTopEnemyBullet.t = 0.0f;
	leftTopEnemyBullet.easedT = 0.0f;

	// フラグの初期化
	leftTopEnemyBullet.interpolation = false;
	leftTopEnemyBullet.isBulletShot = false;

	// =======================
    // 右上の弾丸初期化
    // =======================
	Bullet rightTopEnemyBullet{};

	// サイズ設定
	rightTopEnemyBullet.width = 20.0f;
	rightTopEnemyBullet.height = 20.0f;

	// 開始座標 (敵の右上)
	rightTopEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightTopEnemyBullet.width;
	rightTopEnemyBullet.start.y = enemy.pos.y;

	// 終了座標 (右上方向へ進む)
	rightTopEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightTopEnemyBullet.width) + (rightTopEnemyBullet.width * 2.0f);
	rightTopEnemyBullet.end.y = enemy.pos.y - (rightTopEnemyBullet.height * 2.0f);

	// 現在座標の初期化
	rightTopEnemyBullet.pos.x = rightTopEnemyBullet.start.x;
	rightTopEnemyBullet.pos.y = rightTopEnemyBullet.start.y;



	// 左上
	rightTopEnemyBullet.vertex.leftTop.x = rightTopEnemyBullet.pos.x;
	rightTopEnemyBullet.vertex.leftTop.y = rightTopEnemyBullet.pos.y;

	// 右上
	rightTopEnemyBullet.vertex.rightTop.x = rightTopEnemyBullet.pos.x + rightTopEnemyBullet.width;
	rightTopEnemyBullet.vertex.rightTop.y = rightTopEnemyBullet.pos.y;

	// 左下
	rightTopEnemyBullet.vertex.leftBottom.x = rightTopEnemyBullet.pos.x;
	rightTopEnemyBullet.vertex.leftBottom.y = rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height;

	// 右下
	rightTopEnemyBullet.vertex.rightBottom.x = rightTopEnemyBullet.pos.x + rightTopEnemyBullet.width;
	rightTopEnemyBullet.vertex.rightBottom.y = rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height;

	rightTopEnemyBullet.src.x = 0.0f;
	rightTopEnemyBullet.src.y = 0.0f;

	// 弾の動作パラメータ
	rightTopEnemyBullet.speed = 5.0f;
	rightTopEnemyBullet.length = 0.0f;
	rightTopEnemyBullet.t = 0.0f;
	rightTopEnemyBullet.easedT = 0.0f;

	// フラグの初期化
	rightTopEnemyBullet.interpolation = false;
	rightTopEnemyBullet.isBulletShot = false;

	// =======================
    // 左下の弾丸初期化
    // =======================
	Bullet leftBottomEnemyBullet{};

	// サイズ設定
	leftBottomEnemyBullet.width = 20.0f;
	leftBottomEnemyBullet.height = 20.0f;

	// 開始座標 (敵の左下)
	leftBottomEnemyBullet.start.x = enemy.pos.x;
	leftBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height;

	// 終了座標 (左下方向へ進む)
	leftBottomEnemyBullet.end.x = enemy.pos.x - (leftBottomEnemyBullet.width * 2.0f);
	leftBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height) + (leftBottomEnemyBullet.height * 2.0f);

	// 現在座標の初期化
	leftBottomEnemyBullet.pos.x = leftBottomEnemyBullet.start.x;
	leftBottomEnemyBullet.pos.y = leftBottomEnemyBullet.start.y;



	// 左上
	leftBottomEnemyBullet.vertex.leftTop.x = leftBottomEnemyBullet.pos.x;
	leftBottomEnemyBullet.vertex.leftTop.y = leftBottomEnemyBullet.pos.y;

	// 右上
	leftBottomEnemyBullet.vertex.rightTop.x = leftBottomEnemyBullet.pos.x + leftBottomEnemyBullet.width;
	leftBottomEnemyBullet.vertex.rightTop.y = leftBottomEnemyBullet.pos.y;

	// 左下
	leftBottomEnemyBullet.vertex.leftBottom.x = leftBottomEnemyBullet.pos.x;
	leftBottomEnemyBullet.vertex.leftBottom.y = leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height;

	// 右下
	leftBottomEnemyBullet.vertex.rightBottom.x = leftBottomEnemyBullet.pos.x + leftBottomEnemyBullet.width;
	leftBottomEnemyBullet.vertex.rightBottom.y = leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height;

	leftBottomEnemyBullet.src.x = 0.0f;
	leftBottomEnemyBullet.src.y = 0.0f;

	// 弾の動作パラメータ
	leftBottomEnemyBullet.speed = 5.0f;
	leftBottomEnemyBullet.length = 0.0f;
	leftBottomEnemyBullet.t = 0.0f;
	leftBottomEnemyBullet.easedT = 0.0f;

	// フラグの初期化
	leftBottomEnemyBullet.interpolation = false;
	leftBottomEnemyBullet.isBulletShot = false;

	// =======================
    // 右下の弾丸初期化
    // =======================
	Bullet rightBottomEnemyBullet{};

	// サイズ設定
	rightBottomEnemyBullet.width = 20.0f;
	rightBottomEnemyBullet.height = 20.0f;

	// 開始座標 (敵の右下)
	rightBottomEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width;
	rightBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height;

	// 終了座標 (右下方向へ進む)
	rightBottomEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width) + (rightBottomEnemyBullet.width * 2.0f);
	rightBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height) + (rightBottomEnemyBullet.height * 2.0f);

	// 現在座標の初期化
	rightBottomEnemyBullet.pos.x = rightBottomEnemyBullet.start.x;
	rightBottomEnemyBullet.pos.y = rightBottomEnemyBullet.start.y;



	// 左上
	rightBottomEnemyBullet.vertex.leftTop.x = rightBottomEnemyBullet.pos.x;
	rightBottomEnemyBullet.vertex.leftTop.y = rightBottomEnemyBullet.pos.y;

	// 右上
	rightBottomEnemyBullet.vertex.rightTop.x = rightBottomEnemyBullet.pos.x + rightBottomEnemyBullet.width;
	rightBottomEnemyBullet.vertex.rightTop.y = rightBottomEnemyBullet.pos.y;

	// 左下
	rightBottomEnemyBullet.vertex.leftBottom.x = rightBottomEnemyBullet.pos.x;
	rightBottomEnemyBullet.vertex.leftBottom.y = rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height;

	// 右下
	rightBottomEnemyBullet.vertex.rightBottom.x = rightBottomEnemyBullet.pos.x + rightBottomEnemyBullet.width;
	rightBottomEnemyBullet.vertex.rightBottom.y = rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height;




	rightBottomEnemyBullet.src.x = 0.0f;
	rightBottomEnemyBullet.src.y = 0.0f;

	// 弾の動作パラメータ
	rightBottomEnemyBullet.speed = 5.0f;
	rightBottomEnemyBullet.length = 0.0f;
	rightBottomEnemyBullet.t = 0.0f;
	rightBottomEnemyBullet.easedT = 0.0f;

	// フラグの初期化
	rightBottomEnemyBullet.interpolation = false;
	rightBottomEnemyBullet.isBulletShot = false;

	Player player{};
	player.pos.x = 640.0f;
	player.pos.y = 620.0f;
	player.dir.x = player.pos.x - enemy.pos.y;
	player.dir.y = player.pos.y - enemy.pos.y;
	player.radius = 40.0f;
	player.speed = 5.0f;

#pragma endregion

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

#pragma region UpdateProcessing

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

		// --- 敵の左辺の弾 ---
		for (int i = 0; i < leftSideEnemyBullet[0].columns; ++i) {

			if (!leftSideEnemyBullet[i].isBulletShot) {
				leftSideEnemyBullet[i].isBulletShot = true;
				leftSideEnemyBullet[i].length = 0.0f;

				leftSideEnemyBullet[i].t = 0.0f;
				leftSideEnemyBullet[i].easedT = 0.0f;
			}

		if (playerbullet1.isShot == 1)
		{
			playerbullet1.pos.y -= playerbullet1.speed;

			if (playerbullet1.pos.y < 0)
			{
				playerbullet1.isShot = false;
				playerbullet1.pos.y = player.pos.x;

					leftSideEnemyBullet[i].t += 0.008f;

					if (leftSideEnemyBullet[i].t >= 1.0f) {
						leftSideEnemyBullet[i].t = 1.0f;
						leftSideEnemyBullet[i].interpolation = true;
					}

					leftSideEnemyBullet[i].easedT = 1.0f - powf(1.0f - leftSideEnemyBullet[i].t, 3.0f);

					leftSideEnemyBullet[i].pos.x = (1.0f - leftSideEnemyBullet[i].easedT) * leftSideEnemyBullet[i].start.x + leftSideEnemyBullet[i].easedT * leftSideEnemyBullet[i].end.x;
					leftSideEnemyBullet[i].pos.y = (1.0f - leftSideEnemyBullet[i].easedT) * leftSideEnemyBullet[i].start.y + leftSideEnemyBullet[i].easedT * leftSideEnemyBullet[i].end.y;

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

		// --- 敵の上辺の弾 ---
		for (int i = 0; i < topSideEnemyBullet[0].columns; ++i) {

			if (!topSideEnemyBullet[i].isBulletShot) {
				topSideEnemyBullet[i].isBulletShot = true;
				topSideEnemyBullet[i].length = 0.0f;

				topSideEnemyBullet[i].t = 0.0f;
				topSideEnemyBullet[i].easedT = 0.0f;
			}

			if (topSideEnemyBullet[i].isBulletShot) {

				if (!topSideEnemyBullet[i].interpolation) {

					topSideEnemyBullet[i].t += 0.008f;

					if (topSideEnemyBullet[i].t >= 1.0f) {
						topSideEnemyBullet[i].t = 1.0f;
						topSideEnemyBullet[i].interpolation = true;
					}

					topSideEnemyBullet[i].easedT = 1.0f - powf(1.0f - topSideEnemyBullet[i].t, 3.0f);

					topSideEnemyBullet[i].pos.x = (1.0f - topSideEnemyBullet[i].easedT) * topSideEnemyBullet[i].start.x + topSideEnemyBullet[i].easedT * topSideEnemyBullet[i].end.x;
					topSideEnemyBullet[i].pos.y = (1.0f - topSideEnemyBullet[i].easedT) * topSideEnemyBullet[i].start.y + topSideEnemyBullet[i].easedT * topSideEnemyBullet[i].end.y;

				} else {

					player.dir.x = player.pos.x - enemy.pos.x;
					player.dir.y = player.pos.y - enemy.pos.y;

					topSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

					if (topSideEnemyBullet[i].length > 0.0f) {
						topSideEnemyBullet[i].velocity.x = (player.dir.x / topSideEnemyBullet[i].length) * topSideEnemyBullet[i].speed;
						topSideEnemyBullet[i].velocity.y = (player.dir.y / topSideEnemyBullet[i].length) * topSideEnemyBullet[i].speed;
					}

					topSideEnemyBullet[i].pos.x += topSideEnemyBullet[i].velocity.x;
					topSideEnemyBullet[i].pos.y += topSideEnemyBullet[i].velocity.y;

					if (topSideEnemyBullet[i].pos.y <= 0.0f ||
						topSideEnemyBullet[i].pos.x <= 0.0f ||
						topSideEnemyBullet[i].pos.x >= 1280.0f ||
						topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height >= 720.0f) {

						topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
						topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;
						topSideEnemyBullet[i].isBulletShot = false;
						topSideEnemyBullet[i].interpolation = false;
					}
				}
			}
		}

		// --- 敵の下辺の弾 ---
		for (int i = 0; i < bottomSideEnemyBullet[0].columns; ++i) {

			if (!bottomSideEnemyBullet[i].isBulletShot) {
				bottomSideEnemyBullet[i].isBulletShot = true;
				bottomSideEnemyBullet[i].length = 0.0f;

				bottomSideEnemyBullet[i].t = 0.0f;
				bottomSideEnemyBullet[i].easedT = 0.0f;
			}

			if (bottomSideEnemyBullet[i].isBulletShot) {

				if (!bottomSideEnemyBullet[i].interpolation) {

					bottomSideEnemyBullet[i].t += 0.008f;

					if (bottomSideEnemyBullet[i].t >= 1.0f) {
						bottomSideEnemyBullet[i].t = 1.0f;
						bottomSideEnemyBullet[i].interpolation = true;
					}

					bottomSideEnemyBullet[i].easedT = 1.0f - powf(1.0f - bottomSideEnemyBullet[i].t, 3.0f);

					bottomSideEnemyBullet[i].pos.x = (1.0f - bottomSideEnemyBullet[i].easedT) * bottomSideEnemyBullet[i].start.x + bottomSideEnemyBullet[i].easedT * bottomSideEnemyBullet[i].end.x;
					bottomSideEnemyBullet[i].pos.y = (1.0f - bottomSideEnemyBullet[i].easedT) * bottomSideEnemyBullet[i].start.y + bottomSideEnemyBullet[i].easedT * bottomSideEnemyBullet[i].end.y;

				} else {

					player.dir.x = player.pos.x - enemy.pos.x;
					player.dir.y = player.pos.y - enemy.pos.y;

					bottomSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

					if (bottomSideEnemyBullet[i].length > 0.0f) {
						bottomSideEnemyBullet[i].velocity.x = (player.dir.x / bottomSideEnemyBullet[i].length) * bottomSideEnemyBullet[i].speed;
						bottomSideEnemyBullet[i].velocity.y = (player.dir.y / bottomSideEnemyBullet[i].length) * bottomSideEnemyBullet[i].speed;
					}

					bottomSideEnemyBullet[i].pos.x += bottomSideEnemyBullet[i].velocity.x;
					bottomSideEnemyBullet[i].pos.y += bottomSideEnemyBullet[i].velocity.y;

					if (bottomSideEnemyBullet[i].pos.y <= 0.0f ||
						bottomSideEnemyBullet[i].pos.x <= 0.0f ||
						bottomSideEnemyBullet[i].pos.x >= 1280.0f ||
						bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height >= 720.0f) {

						bottomSideEnemyBullet[i].pos.x = bottomSideEnemyBullet[i].start.x;
						bottomSideEnemyBullet[i].pos.y = bottomSideEnemyBullet[i].start.y;
						bottomSideEnemyBullet[i].isBulletShot = false;
						bottomSideEnemyBullet[i].interpolation = false;
					}
				}
			}
		}

		// --- 敵の左上の弾 ---
		if (!leftTopEnemyBullet.isBulletShot) {
			leftTopEnemyBullet.isBulletShot = true;
			leftTopEnemyBullet.length = 0.0f;

			leftTopEnemyBullet.t = 0.0f;
			leftTopEnemyBullet.easedT = 0.0f;
		}

		if (leftTopEnemyBullet.isBulletShot) {

			if (!leftTopEnemyBullet.interpolation) {

				leftTopEnemyBullet.t += 0.008f;

				if (leftTopEnemyBullet.t >= 1.0f) {
					leftTopEnemyBullet.t = 1.0f;
					leftTopEnemyBullet.interpolation = true;
				}

				leftTopEnemyBullet.easedT = 1.0f - powf(1.0f - leftTopEnemyBullet.t, 3.0f);

				leftTopEnemyBullet.pos.x = (1.0f - leftTopEnemyBullet.easedT) * leftTopEnemyBullet.start.x + leftTopEnemyBullet.easedT * leftTopEnemyBullet.end.x;
				leftTopEnemyBullet.pos.y = (1.0f - leftTopEnemyBullet.easedT) * leftTopEnemyBullet.start.y + leftTopEnemyBullet.easedT * leftTopEnemyBullet.end.y;

			} else {

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				leftTopEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

				if (leftTopEnemyBullet.length > 0.0f) {
					leftTopEnemyBullet.velocity.x = (player.dir.x / leftTopEnemyBullet.length) * leftTopEnemyBullet.speed;
					leftTopEnemyBullet.velocity.y = (player.dir.y / leftTopEnemyBullet.length) * leftTopEnemyBullet.speed;
				}

				leftTopEnemyBullet.pos.x += leftTopEnemyBullet.velocity.x;
				leftTopEnemyBullet.pos.y += leftTopEnemyBullet.velocity.y;

				if (leftTopEnemyBullet.pos.y <= 0.0f ||
					leftTopEnemyBullet.pos.x <= 0.0f ||
					leftTopEnemyBullet.pos.x >= 1280.0f ||
					leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height >= 720.0f) {

					leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
					leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;
					leftTopEnemyBullet.isBulletShot = false;
					leftTopEnemyBullet.interpolation = false;
				}
			}
		}

		// --- 敵の右上の弾 ---
		if (!rightTopEnemyBullet.isBulletShot) {
			rightTopEnemyBullet.isBulletShot = true;
			rightTopEnemyBullet.length = 0.0f;

			rightTopEnemyBullet.t = 0.0f;
			rightTopEnemyBullet.easedT = 0.0f;
		}

		if (rightTopEnemyBullet.isBulletShot) {

			if (!rightTopEnemyBullet.interpolation) {

				rightTopEnemyBullet.t += 0.008f;

				if (rightTopEnemyBullet.t >= 1.0f) {
					rightTopEnemyBullet.t = 1.0f;
					rightTopEnemyBullet.interpolation = true;
				}

				rightTopEnemyBullet.easedT = 1.0f - powf(1.0f - rightTopEnemyBullet.t, 3.0f);

				rightTopEnemyBullet.pos.x = (1.0f - rightTopEnemyBullet.easedT) * rightTopEnemyBullet.start.x + rightTopEnemyBullet.easedT * rightTopEnemyBullet.end.x;
				rightTopEnemyBullet.pos.y = (1.0f - rightTopEnemyBullet.easedT) * rightTopEnemyBullet.start.y + rightTopEnemyBullet.easedT * rightTopEnemyBullet.end.y;

			} else {

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				rightTopEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

				if (rightTopEnemyBullet.length > 0.0f) {
					rightTopEnemyBullet.velocity.x = (player.dir.x / rightTopEnemyBullet.length) * rightTopEnemyBullet.speed;
					rightTopEnemyBullet.velocity.y = (player.dir.y / rightTopEnemyBullet.length) * rightTopEnemyBullet.speed;
				}

				rightTopEnemyBullet.pos.x += rightTopEnemyBullet.velocity.x;
				rightTopEnemyBullet.pos.y += rightTopEnemyBullet.velocity.y;

				if (rightTopEnemyBullet.pos.y <= 0.0f ||
					rightTopEnemyBullet.pos.x <= 0.0f ||
					rightTopEnemyBullet.pos.x >= 1280.0f ||
					rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height >= 720.0f) {

					rightTopEnemyBullet.pos.x = rightTopEnemyBullet.start.x;
					rightTopEnemyBullet.pos.y = rightTopEnemyBullet.start.y;
					rightTopEnemyBullet.isBulletShot = false;
					rightTopEnemyBullet.interpolation = false;
				}
			}
		}

		// --- 敵の左下の弾 ---
		if (!leftBottomEnemyBullet.isBulletShot) {
			leftBottomEnemyBullet.isBulletShot = true;
			leftBottomEnemyBullet.length = 0.0f;

			leftBottomEnemyBullet.t = 0.0f;
			leftBottomEnemyBullet.easedT = 0.0f;
		}

		if (leftBottomEnemyBullet.isBulletShot) {

			if (!leftBottomEnemyBullet.interpolation) {

				leftBottomEnemyBullet.t += 0.008f;

				if (leftBottomEnemyBullet.t >= 1.0f) {
					leftBottomEnemyBullet.t = 1.0f;
					leftBottomEnemyBullet.interpolation = true;
				}

				leftBottomEnemyBullet.easedT = 1.0f - powf(1.0f - leftBottomEnemyBullet.t, 3.0f);

				leftBottomEnemyBullet.pos.x = (1.0f - leftBottomEnemyBullet.easedT) * leftBottomEnemyBullet.start.x + leftBottomEnemyBullet.easedT * leftBottomEnemyBullet.end.x;
				leftBottomEnemyBullet.pos.y = (1.0f - leftBottomEnemyBullet.easedT) * leftBottomEnemyBullet.start.y + leftBottomEnemyBullet.easedT * leftBottomEnemyBullet.end.y;

			} else {

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				leftBottomEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

				if (leftBottomEnemyBullet.length > 0.0f) {
					leftBottomEnemyBullet.velocity.x = (player.dir.x / leftBottomEnemyBullet.length) * leftBottomEnemyBullet.speed;
					leftBottomEnemyBullet.velocity.y = (player.dir.y / leftBottomEnemyBullet.length) * leftBottomEnemyBullet.speed;
				}

				leftBottomEnemyBullet.pos.x += leftBottomEnemyBullet.velocity.x;
				leftBottomEnemyBullet.pos.y += leftBottomEnemyBullet.velocity.y;

				if (leftBottomEnemyBullet.pos.y <= 0.0f ||
					leftBottomEnemyBullet.pos.x <= 0.0f ||
					leftBottomEnemyBullet.pos.x >= 1280.0f ||
					leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height >= 720.0f) {

					leftBottomEnemyBullet.pos.x = leftBottomEnemyBullet.start.x;
					leftBottomEnemyBullet.pos.y = leftBottomEnemyBullet.start.y;
					leftBottomEnemyBullet.isBulletShot = false;
					leftBottomEnemyBullet.interpolation = false;
				}
			}
		}

		// --- 敵の右下の弾 ---
		if (!rightBottomEnemyBullet.isBulletShot) {
			rightBottomEnemyBullet.isBulletShot = true;
			rightBottomEnemyBullet.length = 0.0f;

			rightBottomEnemyBullet.t = 0.0f;
			rightBottomEnemyBullet.easedT = 0.0f;
		}

		if (rightBottomEnemyBullet.isBulletShot) {

			if (!rightBottomEnemyBullet.interpolation) {

				rightBottomEnemyBullet.t += 0.008f;

				if (rightBottomEnemyBullet.t >= 1.0f) {
					rightBottomEnemyBullet.t = 1.0f;
					rightBottomEnemyBullet.interpolation = true;
				}

				rightBottomEnemyBullet.easedT = 1.0f - powf(1.0f - rightBottomEnemyBullet.t, 3.0f);

				rightBottomEnemyBullet.pos.x = (1.0f - rightBottomEnemyBullet.easedT) * rightBottomEnemyBullet.start.x + rightBottomEnemyBullet.easedT * rightBottomEnemyBullet.end.x;
				rightBottomEnemyBullet.pos.y = (1.0f - rightBottomEnemyBullet.easedT) * rightBottomEnemyBullet.start.y + rightBottomEnemyBullet.easedT * rightBottomEnemyBullet.end.y;

			} else {

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				rightBottomEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

				if (rightBottomEnemyBullet.length > 0.0f) {
					rightBottomEnemyBullet.velocity.x = (player.dir.x / rightBottomEnemyBullet.length) * rightBottomEnemyBullet.speed;
					rightBottomEnemyBullet.velocity.y = (player.dir.y / rightBottomEnemyBullet.length) * rightBottomEnemyBullet.speed;
				}

				rightBottomEnemyBullet.pos.x += rightBottomEnemyBullet.velocity.x;
				rightBottomEnemyBullet.pos.y += rightBottomEnemyBullet.velocity.y;

				if (rightBottomEnemyBullet.pos.y <= 0.0f ||
					rightBottomEnemyBullet.pos.x <= 0.0f ||
					rightBottomEnemyBullet.pos.x >= 1280.0f ||
					rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height >= 720.0f) {

					rightBottomEnemyBullet.pos.x = rightBottomEnemyBullet.start.x;
					rightBottomEnemyBullet.pos.y = rightBottomEnemyBullet.start.y;
					rightBottomEnemyBullet.isBulletShot = false;
					rightBottomEnemyBullet.interpolation = false;
				}
			}
		}

		// 敵
		// 左上
		enemy.vertex.leftTop.x = enemy.pos.x;
		enemy.vertex.leftTop.y = enemy.pos.y;

		// 右上
		enemy.vertex.rightTop.x = enemy.pos.x + enemy.width;
		enemy.vertex.rightTop.y = enemy.pos.y;

		// 左下
		enemy.vertex.leftBottom.x = enemy.pos.x;
		enemy.vertex.leftBottom.y = enemy.pos.y + enemy.height;

		// 右下
		enemy.vertex.rightBottom.x = enemy.pos.x + enemy.width;
		enemy.vertex.rightBottom.y = enemy.pos.y + enemy.height;

		// 敵弾
		// 左辺
		for (int i = 0; i < leftSideEnemyBullet[0].columns; ++i) {

			// 左上
			leftSideEnemyBullet[i].vertex.leftTop.x = leftSideEnemyBullet[i].pos.x;
			leftSideEnemyBullet[i].vertex.leftTop.y = leftSideEnemyBullet[i].pos.y;

			// 右上
			leftSideEnemyBullet[i].vertex.rightTop.x = leftSideEnemyBullet[i].pos.x + leftSideEnemyBullet[i].width;
			leftSideEnemyBullet[i].vertex.rightTop.y = leftSideEnemyBullet[i].pos.y;

			// 左下
			leftSideEnemyBullet[i].vertex.leftBottom.x = leftSideEnemyBullet[i].pos.x;
			leftSideEnemyBullet[i].vertex.leftBottom.y = leftSideEnemyBullet[i].pos.y + leftSideEnemyBullet[i].height;

			// 右下
			leftSideEnemyBullet[i].vertex.rightBottom.x = leftSideEnemyBullet[i].pos.x + leftSideEnemyBullet[i].width;
			leftSideEnemyBullet[i].vertex.rightBottom.y = leftSideEnemyBullet[i].pos.y + leftSideEnemyBullet[i].height;
		}

		// 右辺
		for (int i = 0; i < rightSideEnemyBullet[0].columns; ++i) {

			// 左上
			rightSideEnemyBullet[i].vertex.leftTop.x = rightSideEnemyBullet[i].pos.x;
			rightSideEnemyBullet[i].vertex.leftTop.y = rightSideEnemyBullet[i].pos.y;

			// 右上
			rightSideEnemyBullet[i].vertex.rightTop.x = rightSideEnemyBullet[i].pos.x + rightSideEnemyBullet[i].width;
			rightSideEnemyBullet[i].vertex.rightTop.y = rightSideEnemyBullet[i].pos.y;

			// 左下
			rightSideEnemyBullet[i].vertex.leftBottom.x = rightSideEnemyBullet[i].pos.x;
			rightSideEnemyBullet[i].vertex.leftBottom.y = rightSideEnemyBullet[i].pos.y + rightSideEnemyBullet[i].height;

			// 右下
			rightSideEnemyBullet[i].vertex.rightBottom.x = rightSideEnemyBullet[i].pos.x + rightSideEnemyBullet[i].width;
			rightSideEnemyBullet[i].vertex.rightBottom.y = rightSideEnemyBullet[i].pos.y + rightSideEnemyBullet[i].height;
		}

		// 上辺
		for (int i = 0; i < topSideEnemyBullet[0].columns; ++i) {

			// 左上
			topSideEnemyBullet[i].vertex.leftTop.x = topSideEnemyBullet[i].pos.x;
			topSideEnemyBullet[i].vertex.leftTop.y = topSideEnemyBullet[i].pos.y;

			// 右上
			topSideEnemyBullet[i].vertex.rightTop.x = topSideEnemyBullet[i].pos.x + topSideEnemyBullet[i].width;
			topSideEnemyBullet[i].vertex.rightTop.y = topSideEnemyBullet[i].pos.y;

			// 左下
			topSideEnemyBullet[i].vertex.leftBottom.x = topSideEnemyBullet[i].pos.x;
			topSideEnemyBullet[i].vertex.leftBottom.y = topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height;

			// 右下
			topSideEnemyBullet[i].vertex.rightBottom.x = topSideEnemyBullet[i].pos.x + topSideEnemyBullet[i].width;
			topSideEnemyBullet[i].vertex.rightBottom.y = topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height;
		}

		// 下辺
		for (int i = 0; i < bottomSideEnemyBullet[0].columns; ++i) {

			// 左上
			bottomSideEnemyBullet[i].vertex.leftTop.x = bottomSideEnemyBullet[i].pos.x;
			bottomSideEnemyBullet[i].vertex.leftTop.y = bottomSideEnemyBullet[i].pos.y;

			// 右上
			bottomSideEnemyBullet[i].vertex.rightTop.x = bottomSideEnemyBullet[i].pos.x + bottomSideEnemyBullet[i].width;
			bottomSideEnemyBullet[i].vertex.rightTop.y = bottomSideEnemyBullet[i].pos.y;

			// 左下
			bottomSideEnemyBullet[i].vertex.leftBottom.x = bottomSideEnemyBullet[i].pos.x;
			bottomSideEnemyBullet[i].vertex.leftBottom.y = bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height;

			// 右下
			bottomSideEnemyBullet[i].vertex.rightBottom.x = bottomSideEnemyBullet[i].pos.x + bottomSideEnemyBullet[i].width;
			bottomSideEnemyBullet[i].vertex.rightBottom.y = bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height;
		}

		//// 左上
		// 左上
		leftTopEnemyBullet.vertex.leftTop.x = leftTopEnemyBullet.pos.x;
		leftTopEnemyBullet.vertex.leftTop.y = leftTopEnemyBullet.pos.y;

		// 右上
		leftTopEnemyBullet.vertex.rightTop.x = leftTopEnemyBullet.pos.x + leftTopEnemyBullet.width;
		leftTopEnemyBullet.vertex.rightTop.y = leftTopEnemyBullet.pos.y;

		// 左下
		leftTopEnemyBullet.vertex.leftBottom.x = leftTopEnemyBullet.pos.x;
		leftTopEnemyBullet.vertex.leftBottom.y = leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height;

		// 右下
		leftTopEnemyBullet.vertex.rightBottom.x = leftTopEnemyBullet.pos.x + leftTopEnemyBullet.width;
		leftTopEnemyBullet.vertex.rightBottom.y = leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height;



		//// 右上
		// 左上
		rightTopEnemyBullet.vertex.leftTop.x = rightTopEnemyBullet.pos.x;
		rightTopEnemyBullet.vertex.leftTop.y = rightTopEnemyBullet.pos.y;

		// 右上
		rightTopEnemyBullet.vertex.rightTop.x = rightTopEnemyBullet.pos.x + rightTopEnemyBullet.width;
		rightTopEnemyBullet.vertex.rightTop.y = rightTopEnemyBullet.pos.y;

		// 左下
		rightTopEnemyBullet.vertex.leftBottom.x = rightTopEnemyBullet.pos.x;
		rightTopEnemyBullet.vertex.leftBottom.y = rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height;

		// 右下
		rightTopEnemyBullet.vertex.rightBottom.x = rightTopEnemyBullet.pos.x + rightTopEnemyBullet.width;
		rightTopEnemyBullet.vertex.rightBottom.y = rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height;



		//// 左下
		// 左上
		leftBottomEnemyBullet.vertex.leftTop.x = leftBottomEnemyBullet.pos.x;
		leftBottomEnemyBullet.vertex.leftTop.y = leftBottomEnemyBullet.pos.y;

		// 右上
		leftBottomEnemyBullet.vertex.rightTop.x = leftBottomEnemyBullet.pos.x + leftBottomEnemyBullet.width;
		leftBottomEnemyBullet.vertex.rightTop.y = leftBottomEnemyBullet.pos.y;

		// 左下
		leftBottomEnemyBullet.vertex.leftBottom.x = leftBottomEnemyBullet.pos.x;
		leftBottomEnemyBullet.vertex.leftBottom.y = leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height;

		// 右下
		leftBottomEnemyBullet.vertex.rightBottom.x = leftBottomEnemyBullet.pos.x + leftBottomEnemyBullet.width;
		leftBottomEnemyBullet.vertex.rightBottom.y = leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height;



		//// 右下
		// 左上
		rightBottomEnemyBullet.vertex.leftTop.x = rightBottomEnemyBullet.pos.x;
		rightBottomEnemyBullet.vertex.leftTop.y = rightBottomEnemyBullet.pos.y;

		// 右上
		rightBottomEnemyBullet.vertex.rightTop.x = rightBottomEnemyBullet.pos.x + rightBottomEnemyBullet.width;
		rightBottomEnemyBullet.vertex.rightTop.y = rightBottomEnemyBullet.pos.y;

		// 左下
		rightBottomEnemyBullet.vertex.leftBottom.x = rightBottomEnemyBullet.pos.x;
		rightBottomEnemyBullet.vertex.leftBottom.y = rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height;

		// 右下
		rightBottomEnemyBullet.vertex.rightBottom.x = rightBottomEnemyBullet.pos.x + rightBottomEnemyBullet.width;
		rightBottomEnemyBullet.vertex.rightBottom.y = rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height;

		// アニメーション
		enemy.frameCount++;
		if (enemy.frameCount >= 2) {
			enemy.frameCount = 0;
			enemy.frameCounter = (enemy.frameCounter + 9) % 8;
		}

		// アニメーション
		enemy.src.x = enemy.frameCounter * enemy.width;

		///
		/// ↑更新処理ここまで
		///

#pragma endregion

#pragma region DrawingProcessing

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

	    // 敵の左辺の弾
		for (int j = 0; j < leftSideEnemyBullet[0].columns; ++j) {
			if (leftSideEnemyBullet[j].isBulletShot) {
				Novice::DrawQuad(
					int(leftSideEnemyBullet[j].vertex.leftTop.x),
					int(leftSideEnemyBullet[j].vertex.leftTop.y) + j * (int(leftSideEnemyBullet[j].height) + leftSideEnemyBullet[j].space),

					int(leftSideEnemyBullet[j].vertex.rightTop.x),
					int(leftSideEnemyBullet[j].vertex.rightTop.y) + j * (int(leftSideEnemyBullet[j].height) + leftSideEnemyBullet[j].space),

					int(leftSideEnemyBullet[j].vertex.leftBottom.x),
					int(leftSideEnemyBullet[j].vertex.leftBottom.y) + j * (int(leftSideEnemyBullet[j].height) + leftSideEnemyBullet[j].space),

					int(leftSideEnemyBullet[j].vertex.rightBottom.x),
					int(leftSideEnemyBullet[j].vertex.rightBottom.y) + j * (int(leftSideEnemyBullet[j].height) + leftSideEnemyBullet[j].space),

					int(leftSideEnemyBullet[j].src.x), int(leftSideEnemyBullet[j].src.y),
					int(leftSideEnemyBullet[j].width), int(leftSideEnemyBullet[j].height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}
		}

		

		// 敵の右辺の弾
		for (int j = 0; j < rightSideEnemyBullet[0].columns; ++j) {
			if (rightSideEnemyBullet[j].isBulletShot) {
				Novice::DrawQuad(
					int(rightSideEnemyBullet[j].vertex.leftTop.x),
					int(rightSideEnemyBullet[j].vertex.leftTop.y) + j * (int(rightSideEnemyBullet[j].height) + rightSideEnemyBullet[j].space),

					int(rightSideEnemyBullet[j].vertex.rightTop.x),
					int(rightSideEnemyBullet[j].vertex.rightTop.y) + j * (int(rightSideEnemyBullet[j].height) + rightSideEnemyBullet[j].space),

					int(rightSideEnemyBullet[j].vertex.leftBottom.x),
					int(rightSideEnemyBullet[j].vertex.leftBottom.y) + j * (int(rightSideEnemyBullet[j].height) + rightSideEnemyBullet[j].space),

					int(rightSideEnemyBullet[j].vertex.rightBottom.x),
					int(rightSideEnemyBullet[j].vertex.rightBottom.y) + j * (int(rightSideEnemyBullet[j].height) + rightSideEnemyBullet[j].space),

					int(rightSideEnemyBullet[j].src.x), int(rightSideEnemyBullet[j].src.y),
					int(rightSideEnemyBullet[j].width), int(rightSideEnemyBullet[j].height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}
		}

		// 敵の上辺の弾
		for (int j = 0; j < topSideEnemyBullet[0].columns; ++j) {
			if (topSideEnemyBullet[j].isBulletShot) {
				Novice::DrawQuad(
					int(topSideEnemyBullet[j].vertex.leftTop.x) + j * (int(topSideEnemyBullet[j].width) + topSideEnemyBullet[j].space),
					int(topSideEnemyBullet[j].vertex.leftTop.y),

					int(topSideEnemyBullet[j].vertex.rightTop.x) + j * (int(topSideEnemyBullet[j].width) + topSideEnemyBullet[j].space),
					int(topSideEnemyBullet[j].vertex.rightTop.y),

					int(topSideEnemyBullet[j].vertex.leftBottom.x) + j * (int(topSideEnemyBullet[j].width) + topSideEnemyBullet[j].space),
					int(topSideEnemyBullet[j].vertex.leftBottom.y),

					int(topSideEnemyBullet[j].vertex.rightBottom.x) + j * (int(topSideEnemyBullet[j].width) + topSideEnemyBullet[j].space),
					int(topSideEnemyBullet[j].vertex.rightBottom.y),

					int(topSideEnemyBullet[j].src.x), int(topSideEnemyBullet[j].src.y),
					int(topSideEnemyBullet[j].width), int(topSideEnemyBullet[j].height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}
		}
		
		// 敵の下辺の弾
		for (int j = 0; j < bottomSideEnemyBullet[0].columns; ++j) {
			if (bottomSideEnemyBullet[j].isBulletShot) {
				Novice::DrawQuad(
					int(bottomSideEnemyBullet[j].vertex.leftTop.x) + j * (int(bottomSideEnemyBullet[j].width) + bottomSideEnemyBullet[j].space),
					int(bottomSideEnemyBullet[j].vertex.leftTop.y),

					int(bottomSideEnemyBullet[j].vertex.rightTop.x) + j * (int(bottomSideEnemyBullet[j].width) + bottomSideEnemyBullet[j].space),
					int(bottomSideEnemyBullet[j].vertex.rightTop.y),

					int(bottomSideEnemyBullet[j].vertex.leftBottom.x) + j * (int(bottomSideEnemyBullet[j].width) + bottomSideEnemyBullet[j].space),
					int(bottomSideEnemyBullet[j].vertex.leftBottom.y),

					int(bottomSideEnemyBullet[j].vertex.rightBottom.x) + j * (int(bottomSideEnemyBullet[j].width) + bottomSideEnemyBullet[j].space),
					int(bottomSideEnemyBullet[j].vertex.rightBottom.y),

					int(bottomSideEnemyBullet[j].src.x), int(bottomSideEnemyBullet[j].src.y),
					int(bottomSideEnemyBullet[j].width), int(bottomSideEnemyBullet[j].height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}
		}
		
		// 左上
		if (leftTopEnemyBullet.isBulletShot) {
			Novice::DrawQuad(
				int(leftTopEnemyBullet.vertex.leftTop.x),
				int(leftTopEnemyBullet.vertex.leftTop.y),

				int(leftTopEnemyBullet.vertex.rightTop.x),
				int(leftTopEnemyBullet.vertex.rightTop.y),

				int(leftTopEnemyBullet.vertex.leftBottom.x),
				int(leftTopEnemyBullet.vertex.leftBottom.y),

				int(leftTopEnemyBullet.vertex.rightBottom.x),
				int(leftTopEnemyBullet.vertex.rightBottom.y),

				int(leftTopEnemyBullet.src.x), int(leftTopEnemyBullet.src.y),
				int(leftTopEnemyBullet.width), int(leftTopEnemyBullet.height),

				enemyBulletTexture, 0xFFFFFFFF
			);
		}

		// 右上
		if (rightTopEnemyBullet.isBulletShot) {
			Novice::DrawQuad(
				int(rightTopEnemyBullet.vertex.leftTop.x),
				int(rightTopEnemyBullet.vertex.leftTop.y),

				int(rightTopEnemyBullet.vertex.rightTop.x),
				int(rightTopEnemyBullet.vertex.rightTop.y),

				int(rightTopEnemyBullet.vertex.leftBottom.x),
				int(rightTopEnemyBullet.vertex.leftBottom.y),

				int(rightTopEnemyBullet.vertex.rightBottom.x),
				int(rightTopEnemyBullet.vertex.rightBottom.y),

				int(rightTopEnemyBullet.src.x), int(rightTopEnemyBullet.src.y),
				int(rightTopEnemyBullet.width), int(rightTopEnemyBullet.height),

				enemyBulletTexture, 0xFFFFFFFF
			);
		}

		// 左下
		if (leftBottomEnemyBullet.isBulletShot) {
			Novice::DrawQuad(
				int(leftBottomEnemyBullet.vertex.leftTop.x),
				int(leftBottomEnemyBullet.vertex.leftTop.y),

				int(leftBottomEnemyBullet.vertex.rightTop.x),
				int(leftBottomEnemyBullet.vertex.rightTop.y),

				int(leftBottomEnemyBullet.vertex.leftBottom.x),
				int(leftBottomEnemyBullet.vertex.leftBottom.y),

				int(leftBottomEnemyBullet.vertex.rightBottom.x),
				int(leftBottomEnemyBullet.vertex.rightBottom.y),

				int(leftBottomEnemyBullet.src.x), int(leftBottomEnemyBullet.src.y),
				int(leftBottomEnemyBullet.width), int(leftBottomEnemyBullet.height),

				enemyBulletTexture, 0xFFFFFFFF
			);
		}

		// 右下
		if (rightBottomEnemyBullet.isBulletShot) {
			Novice::DrawQuad(
				int(rightBottomEnemyBullet.vertex.leftTop.x),
				int(rightBottomEnemyBullet.vertex.leftTop.y),

				int(rightBottomEnemyBullet.vertex.rightTop.x),
				int(rightBottomEnemyBullet.vertex.rightTop.y),

				int(rightBottomEnemyBullet.vertex.leftBottom.x),
				int(rightBottomEnemyBullet.vertex.leftBottom.y),

				int(rightBottomEnemyBullet.vertex.rightBottom.x),
				int(rightBottomEnemyBullet.vertex.rightBottom.y),

				int(rightBottomEnemyBullet.src.x), int(rightBottomEnemyBullet.src.y),
				int(rightBottomEnemyBullet.width), int(rightBottomEnemyBullet.height),

				enemyBulletTexture, 0xFFFFFFFF
			);
		}

		// 敵描画
		Novice::DrawQuad(
			int(enemy.vertex.leftTop.x),
			int(enemy.vertex.leftTop.y),

			int(enemy.vertex.rightTop.x),
			int(enemy.vertex.rightTop.y),

			int(enemy.vertex.leftBottom.x),
			int(enemy.vertex.leftBottom.y),

			int(enemy.vertex.rightBottom.x),
			int(enemy.vertex.rightBottom.y),

			int(enemy.src.x), int(enemy.src.y),
			int(enemy.width), int(enemy.height),

			enemyTexture, 0xFFFFFFFF
		);

		Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
		Novice::ScreenPrintf(20, 40, "%d", playerbullet1.isShot);
		Novice::ScreenPrintf(20, 20, "%f", playerbullet1.pos.y);
		Novice::ScreenPrintf(20, 40, "%d", type);
		Novice::ScreenPrintf(0, 0, "%f", enemyTimer);

		///
		/// ↑描画処理ここまで
		///

#pragma endregion

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