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
	float width;
	float height;
	float speed;
	int frameCount;
	int frameCounter;
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
	int isHitX;
	int isHitY;
	int isHit;
	int damage;
};

struct Player
{
	Vertex vertex;
	Vector2 pos;
	Vector2 dir;
	float radius;
	float width;
	float height;
	float speed;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

#pragma region Initialization

#pragma region --- enemy初期化 ---

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

#pragma endregion

#pragma region === enemyBulletの初期化 ===

	int enemyBulletTexture = Novice::LoadTexture("./Resources/images/tekitama.png");

	//====================================
#pragma region --- 左辺の弾の初期化 ---

	Bullet leftSideEnemyBullet1{};
	Bullet leftSideEnemyBullet2{};

#pragma region --- 弾1の基本情報 ---

	// 弾の幅と高さ
	leftSideEnemyBullet1.width = 20.0f;  // 弾の幅
	leftSideEnemyBullet1.height = 20.0f; // 弾の高さ

	// 弾の開始位置（敵の左辺）
	leftSideEnemyBullet1.start.x = enemy.pos.x;
	leftSideEnemyBullet1.start.y = enemy.pos.y + (leftSideEnemyBullet1.height * 1.2f);

	// 弾の終了位置（左方向への移動を設定）
	leftSideEnemyBullet1.end.x = enemy.pos.x - (leftSideEnemyBullet1.width * 2.0f);
	leftSideEnemyBullet1.end.y = enemy.pos.y + (leftSideEnemyBullet1.height * 1.2f);

	// 弾の初期位置を設定
	leftSideEnemyBullet1.pos.x = leftSideEnemyBullet1.start.x;
	leftSideEnemyBullet1.pos.y = leftSideEnemyBullet1.start.y;

	// 弾の速度や補間処理フラグなど
	leftSideEnemyBullet1.speed = 5.0f;
	leftSideEnemyBullet1.length = 0.0f;
	leftSideEnemyBullet1.t = 0.0f;
	leftSideEnemyBullet1.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	leftSideEnemyBullet1.vertex.leftTop.x = leftSideEnemyBullet1.pos.x;
	leftSideEnemyBullet1.vertex.leftTop.y = leftSideEnemyBullet1.pos.y;

	// 右上
	leftSideEnemyBullet1.vertex.rightTop.x = leftSideEnemyBullet1.pos.x + leftSideEnemyBullet1.width;
	leftSideEnemyBullet1.vertex.rightTop.y = leftSideEnemyBullet1.pos.y;

	// 左下
	leftSideEnemyBullet1.vertex.leftBottom.x = leftSideEnemyBullet1.pos.x;
	leftSideEnemyBullet1.vertex.leftBottom.y = leftSideEnemyBullet1.pos.y + leftSideEnemyBullet1.height;

	// 右下
	leftSideEnemyBullet1.vertex.rightBottom.x = leftSideEnemyBullet1.pos.x + leftSideEnemyBullet1.width;
	leftSideEnemyBullet1.vertex.rightBottom.y = leftSideEnemyBullet1.pos.y + leftSideEnemyBullet1.height;

	// テクスチャ情報とダメージ
	leftSideEnemyBullet1.src.x = 0.0f;
	leftSideEnemyBullet1.src.y = 0.0f;
	leftSideEnemyBullet1.damage = 5;

	// --- 状態フラグの初期化 ---
	leftSideEnemyBullet1.interpolation = false;  // 補間処理のフラグ
	leftSideEnemyBullet1.isBulletShot = false;   // 発射中フラグ
	leftSideEnemyBullet1.isHitX = false;
	leftSideEnemyBullet1.isHitY = false;
	leftSideEnemyBullet1.isHit = false;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	leftSideEnemyBullet2.width = 20.0f;  // 弾の幅
	leftSideEnemyBullet2.height = 20.0f; // 弾の高さ

	// 弾の開始位置（敵の左辺）
	leftSideEnemyBullet2.start.x = enemy.pos.x;
	leftSideEnemyBullet2.start.y = enemy.pos.y + (leftSideEnemyBullet2.height * 1.2f);

	// 弾の終了位置（左方向への移動を設定）
	leftSideEnemyBullet2.end.x = enemy.pos.x - (leftSideEnemyBullet2.width * 2.0f);
	leftSideEnemyBullet2.end.y = enemy.pos.y + (leftSideEnemyBullet2.height * 1.2f);

	// 弾の初期位置を設定
	leftSideEnemyBullet2.pos.x = leftSideEnemyBullet2.start.x;
	leftSideEnemyBullet2.pos.y = leftSideEnemyBullet2.start.y;

	// 弾の速度や補間処理フラグなど
	leftSideEnemyBullet2.speed = 5.0f;
	leftSideEnemyBullet2.length = 0.0f;
	leftSideEnemyBullet2.t = 0.0f;
	leftSideEnemyBullet2.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	leftSideEnemyBullet2.vertex.leftTop.x = leftSideEnemyBullet2.pos.x;
	leftSideEnemyBullet2.vertex.leftTop.y = leftSideEnemyBullet2.pos.y;

	// 右上
	leftSideEnemyBullet2.vertex.rightTop.x = leftSideEnemyBullet2.pos.x + leftSideEnemyBullet2.width;
	leftSideEnemyBullet2.vertex.rightTop.y = leftSideEnemyBullet2.pos.y;

	// 左下
	leftSideEnemyBullet2.vertex.leftBottom.x = leftSideEnemyBullet2.pos.x;
	leftSideEnemyBullet2.vertex.leftBottom.y = leftSideEnemyBullet2.pos.y + leftSideEnemyBullet2.height;

	// 右下
	leftSideEnemyBullet2.vertex.rightBottom.x = leftSideEnemyBullet2.pos.x + leftSideEnemyBullet2.width;
	leftSideEnemyBullet2.vertex.rightBottom.y = leftSideEnemyBullet2.pos.y + leftSideEnemyBullet2.height;

	// テクスチャ情報とダメージ
	leftSideEnemyBullet2.src.x = 0.0f;
	leftSideEnemyBullet2.src.y = 0.0f;
	leftSideEnemyBullet2.damage = 5;

	// --- 状態フラグの初期化 ---
	leftSideEnemyBullet2.interpolation = false;  // 補間処理のフラグ
	leftSideEnemyBullet2.isBulletShot = false;   // 発射中フラグ
	leftSideEnemyBullet2.isHitX = false;
	leftSideEnemyBullet2.isHitY = false;
	leftSideEnemyBullet2.isHit = false;

#pragma endregion

#pragma endregion
	//------------------
#pragma region --- 右辺の弾の初期化 ---

	Bullet rightSideEnemyBullet1{};
	Bullet rightSideEnemyBullet2{};

#pragma region --- 弾1の基本情報 ---

	// 弾の幅と高さ
	rightSideEnemyBullet1.width = 20.0f;
	rightSideEnemyBullet1.height = 20.0f;

	// 弾の開始座標 (敵の右端から発射)
	rightSideEnemyBullet1.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet1.width;
	rightSideEnemyBullet1.start.y = enemy.pos.y + (rightSideEnemyBullet1.height * 1.2f);

	// 弾の終了座標 (右方向へ進む)
	rightSideEnemyBullet1.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet1.width);
	rightSideEnemyBullet1.end.y = enemy.pos.y + (rightSideEnemyBullet1.height * 1.2f);

	// 弾の初期位置を設定
	rightSideEnemyBullet1.pos.x = rightSideEnemyBullet1.start.x;
	rightSideEnemyBullet1.pos.y = rightSideEnemyBullet1.start.y;

	// 弾の速度や補間処理フラグなど
	rightSideEnemyBullet1.speed = 5.0f;
	rightSideEnemyBullet1.length = 0.0f;
	rightSideEnemyBullet1.t = 0.0f;
	rightSideEnemyBullet1.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	rightSideEnemyBullet1.vertex.leftTop.x = rightSideEnemyBullet1.pos.x;
	rightSideEnemyBullet1.vertex.leftTop.y = rightSideEnemyBullet1.pos.y;

	// 右上
	rightSideEnemyBullet1.vertex.rightTop.x = rightSideEnemyBullet1.pos.x + rightSideEnemyBullet1.width;
	rightSideEnemyBullet1.vertex.rightTop.y = rightSideEnemyBullet1.pos.y;

	// 左下
	rightSideEnemyBullet1.vertex.leftBottom.x = rightSideEnemyBullet1.pos.x;
	rightSideEnemyBullet1.vertex.leftBottom.y = rightSideEnemyBullet1.pos.y + rightSideEnemyBullet1.height;

	// 右下
	rightSideEnemyBullet1.vertex.rightBottom.x = rightSideEnemyBullet1.pos.x + rightSideEnemyBullet1.width;
	rightSideEnemyBullet1.vertex.rightBottom.y = rightSideEnemyBullet1.pos.y + rightSideEnemyBullet1.height;

	// テクスチャ情報とダメージ
	rightSideEnemyBullet1.src.x = 0.0f;
	rightSideEnemyBullet1.src.y = 0.0f;
	rightSideEnemyBullet1.damage = 5;

	// フラグの初期化
	rightSideEnemyBullet1.interpolation = false;
	rightSideEnemyBullet1.isBulletShot = false;
	rightSideEnemyBullet1.isHitX = false;
	rightSideEnemyBullet1.isHitY = false;
	rightSideEnemyBullet1.isHit = false;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	rightSideEnemyBullet2.width = 20.0f;
	rightSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の右端から発射)
	rightSideEnemyBullet2.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet2.width;
	rightSideEnemyBullet2.start.y = enemy.pos.y + (rightSideEnemyBullet2.height * 1.2f);

	// 弾の終了座標 (右方向へ進む)
	rightSideEnemyBullet2.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet2.width);
	rightSideEnemyBullet2.end.y = enemy.pos.y + (rightSideEnemyBullet2.height * 1.2f);

	// 弾の初期位置を設定
	rightSideEnemyBullet2.pos.x = rightSideEnemyBullet2.start.x;
	rightSideEnemyBullet2.pos.y = rightSideEnemyBullet2.start.y;

	// 弾の速度や補間処理フラグなど
	rightSideEnemyBullet2.speed = 5.0f;
	rightSideEnemyBullet2.length = 0.0f;
	rightSideEnemyBullet2.t = 0.0f;
	rightSideEnemyBullet2.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	rightSideEnemyBullet2.vertex.leftTop.x = rightSideEnemyBullet2.pos.x;
	rightSideEnemyBullet2.vertex.leftTop.y = rightSideEnemyBullet2.pos.y;

	// 右上
	rightSideEnemyBullet2.vertex.rightTop.x = rightSideEnemyBullet2.pos.x + rightSideEnemyBullet2.width;
	rightSideEnemyBullet2.vertex.rightTop.y = rightSideEnemyBullet2.pos.y;

	// 左下
	rightSideEnemyBullet2.vertex.leftBottom.x = rightSideEnemyBullet2.pos.x;
	rightSideEnemyBullet2.vertex.leftBottom.y = rightSideEnemyBullet2.pos.y + rightSideEnemyBullet1.height;

	// 右下
	rightSideEnemyBullet2.vertex.rightBottom.x = rightSideEnemyBullet2.pos.x + rightSideEnemyBullet2.width;
	rightSideEnemyBullet2.vertex.rightBottom.y = rightSideEnemyBullet2.pos.y + rightSideEnemyBullet2.height;

	// テクスチャ情報とダメージ
	rightSideEnemyBullet2.src.x = 0.0f;
	rightSideEnemyBullet2.src.y = 0.0f;
	rightSideEnemyBullet2.damage = 5;

	// フラグの初期化
	rightSideEnemyBullet2.interpolation = false;
	rightSideEnemyBullet2.isBulletShot = false;
	rightSideEnemyBullet2.isHitX = false;
	rightSideEnemyBullet2.isHitY = false;
	rightSideEnemyBullet2.isHit = false;

#pragma endregion

#pragma endregion
	//------------------
#pragma region --- 上辺の弾の初期化 ---

	Bullet topSideEnemyBullet1{};
	Bullet topSideEnemyBullet2{};

#pragma region --- 弾1の基本情報 ---

	// 弾の幅と高さ
	topSideEnemyBullet1.width = 20.0f;
	topSideEnemyBullet1.height = 20.0f;

	// 弾の開始座標 (敵の上端から発射)
	topSideEnemyBullet1.start.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
	topSideEnemyBullet1.start.y = enemy.pos.y;

	// 弾の終了座標 (上方向へ進む)
	topSideEnemyBullet1.end.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
	topSideEnemyBullet1.end.y = enemy.pos.y - (topSideEnemyBullet1.height * 2.0f);

	// 弾の初期位置を設定
	topSideEnemyBullet1.pos.x = topSideEnemyBullet1.start.x;
	topSideEnemyBullet1.pos.y = topSideEnemyBullet1.start.y;

	// 弾の速度や補間処理フラグなど
	topSideEnemyBullet1.speed = 5.0f;
	topSideEnemyBullet1.length = 0.0f;
	topSideEnemyBullet1.t = 0.0f;
	topSideEnemyBullet1.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	topSideEnemyBullet1.vertex.leftTop.x = topSideEnemyBullet1.pos.x;
	topSideEnemyBullet1.vertex.leftTop.y = topSideEnemyBullet1.pos.y;

	// 右上
	topSideEnemyBullet1.vertex.rightTop.x = topSideEnemyBullet1.pos.x + topSideEnemyBullet1.width;
	topSideEnemyBullet1.vertex.rightTop.y = topSideEnemyBullet1.pos.y;

	// 左下
	topSideEnemyBullet1.vertex.leftBottom.x = topSideEnemyBullet1.pos.x;
	topSideEnemyBullet1.vertex.leftBottom.y = topSideEnemyBullet1.pos.y + topSideEnemyBullet1.height;

	// 右下
	topSideEnemyBullet1.vertex.rightBottom.x = topSideEnemyBullet1.pos.x + topSideEnemyBullet1.width;
	topSideEnemyBullet1.vertex.rightBottom.y = topSideEnemyBullet1.pos.y + topSideEnemyBullet1.height;

	// テクスチャ情報とダメージ
	topSideEnemyBullet1.src.x = 0.0f;
	topSideEnemyBullet1.src.y = 0.0f;
	topSideEnemyBullet1.damage = 5;

	// フラグの初期化
	topSideEnemyBullet1.interpolation = false;
	topSideEnemyBullet1.isBulletShot = false;
	topSideEnemyBullet1.isHitX = false;
	topSideEnemyBullet1.isHitY = false;
	topSideEnemyBullet1.isHit = false;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	topSideEnemyBullet2.width = 20.0f;
	topSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の上端から発射)
	topSideEnemyBullet2.start.x = enemy.pos.x + (topSideEnemyBullet2.width * 2.6f);
	topSideEnemyBullet2.start.y = enemy.pos.y;

	// 弾の終了座標 (上方向へ進む)
	topSideEnemyBullet2.end.x = enemy.pos.x + (topSideEnemyBullet2.width * 2.6f);
	topSideEnemyBullet2.end.y = enemy.pos.y - (topSideEnemyBullet2.height * 2.0f);

	// 弾の初期位置を設定
	topSideEnemyBullet2.pos.x = topSideEnemyBullet2.start.x;
	topSideEnemyBullet2.pos.y = topSideEnemyBullet2.start.y;

	// 弾の速度や補間処理フラグなど
	topSideEnemyBullet2.speed = 5.0f;
	topSideEnemyBullet2.length = 0.0f;
	topSideEnemyBullet2.t = 0.0f;
	topSideEnemyBullet2.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	topSideEnemyBullet2.vertex.leftTop.x = topSideEnemyBullet2.pos.x;
	topSideEnemyBullet2.vertex.leftTop.y = topSideEnemyBullet2.pos.y;

	// 右上
	topSideEnemyBullet2.vertex.rightTop.x = topSideEnemyBullet2.pos.x + topSideEnemyBullet2.width;
	topSideEnemyBullet2.vertex.rightTop.y = topSideEnemyBullet2.pos.y;

	// 左下
	topSideEnemyBullet2.vertex.leftBottom.x = topSideEnemyBullet2.pos.x;
	topSideEnemyBullet2.vertex.leftBottom.y = topSideEnemyBullet2.pos.y + topSideEnemyBullet2.height;

	// 右下
	topSideEnemyBullet2.vertex.rightBottom.x = topSideEnemyBullet2.pos.x + topSideEnemyBullet2.width;
	topSideEnemyBullet2.vertex.rightBottom.y = topSideEnemyBullet2.pos.y + topSideEnemyBullet2.height;

	// テクスチャ情報とダメージ
	topSideEnemyBullet2.src.x = 0.0f;
	topSideEnemyBullet2.src.y = 0.0f;
	topSideEnemyBullet2.damage = 5;

	// フラグの初期化
	topSideEnemyBullet2.interpolation = false;
	topSideEnemyBullet2.isBulletShot = false;
	topSideEnemyBullet2.isHitX = false;
	topSideEnemyBullet2.isHitY = false;
	topSideEnemyBullet2.isHit = false;

#pragma endregion

#pragma endregion
	//------------------
#pragma region --- 下辺の弾の初期化 ---

	Bullet bottomSideEnemyBullet1{};
#pragma region --- 弾1の基本情報 ---

	// 弾の幅と高さ
	bottomSideEnemyBullet1.width = 20.0f;
	bottomSideEnemyBullet1.height = 20.0f;

	// 弾の開始座標 (敵の下端から発射)
	bottomSideEnemyBullet1.start.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
	bottomSideEnemyBullet1.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet1.height;

	// 弾の終了座標 (下方向へ進む)
	bottomSideEnemyBullet1.end.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
	bottomSideEnemyBullet1.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet1.height;

	// 弾の初期位置を設定
	bottomSideEnemyBullet1.pos.x = bottomSideEnemyBullet1.start.x;
	bottomSideEnemyBullet1.pos.y = bottomSideEnemyBullet1.start.y;

	// 弾の速度や補間処理フラグなど
	bottomSideEnemyBullet1.speed = 5.0f;
	bottomSideEnemyBullet1.length = 0.0f;
	bottomSideEnemyBullet1.t = 0.0f;
	bottomSideEnemyBullet1.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	bottomSideEnemyBullet1.vertex.leftTop.x = bottomSideEnemyBullet1.pos.x;
	bottomSideEnemyBullet1.vertex.leftTop.y = bottomSideEnemyBullet1.pos.y;

	// 右上
	bottomSideEnemyBullet1.vertex.rightTop.x = bottomSideEnemyBullet1.pos.x + bottomSideEnemyBullet1.width;
	bottomSideEnemyBullet1.vertex.rightTop.y = bottomSideEnemyBullet1.pos.y;

	// 左下
	bottomSideEnemyBullet1.vertex.leftBottom.x = bottomSideEnemyBullet1.pos.x;
	bottomSideEnemyBullet1.vertex.leftBottom.y = bottomSideEnemyBullet1.pos.y + bottomSideEnemyBullet1.height;

	// 右下
	bottomSideEnemyBullet1.vertex.rightBottom.x = bottomSideEnemyBullet1.pos.x + bottomSideEnemyBullet1.width;
	bottomSideEnemyBullet1.vertex.rightBottom.y = bottomSideEnemyBullet1.pos.y + bottomSideEnemyBullet1.height;

	// テクスチャ情報とダメージ
	bottomSideEnemyBullet1.src.x = 0.0f;
	bottomSideEnemyBullet1.src.y = 0.0f;
	bottomSideEnemyBullet1.damage = 5;

	// フラグの初期化
	bottomSideEnemyBullet1.interpolation = false;
	bottomSideEnemyBullet1.isBulletShot = false;
	bottomSideEnemyBullet1.isHitX = false;
	bottomSideEnemyBullet1.isHitY = false;
	bottomSideEnemyBullet1.isHit = false;

#pragma endregion
	Bullet bottomSideEnemyBullet2{};
#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	bottomSideEnemyBullet2.width = 20.0f;
	bottomSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の下端から発射)
	bottomSideEnemyBullet2.start.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 2.6f);
	bottomSideEnemyBullet2.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet2.height;

	// 弾の終了座標 (下方向へ進む)
	bottomSideEnemyBullet2.end.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 2.6f);
	bottomSideEnemyBullet2.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet2.height;

	// 弾の初期位置を設定
	bottomSideEnemyBullet2.pos.x = bottomSideEnemyBullet2.start.x;
	bottomSideEnemyBullet2.pos.y = bottomSideEnemyBullet2.start.y;

	// 弾の速度や補間処理フラグなど
	bottomSideEnemyBullet2.speed = 5.0f;
	bottomSideEnemyBullet2.length = 0.0f;
	bottomSideEnemyBullet2.t = 0.0f;
	bottomSideEnemyBullet2.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
	// 左上
	bottomSideEnemyBullet2.vertex.leftTop.x = bottomSideEnemyBullet2.pos.x;
	bottomSideEnemyBullet2.vertex.leftTop.y = bottomSideEnemyBullet2.pos.y;

	// 右上
	bottomSideEnemyBullet2.vertex.rightTop.x = bottomSideEnemyBullet2.pos.x + bottomSideEnemyBullet2.width;
	bottomSideEnemyBullet2.vertex.rightTop.y = bottomSideEnemyBullet2.pos.y;

	// 左下
	bottomSideEnemyBullet2.vertex.leftBottom.x = bottomSideEnemyBullet2.pos.x;
	bottomSideEnemyBullet2.vertex.leftBottom.y = bottomSideEnemyBullet2.pos.y + bottomSideEnemyBullet2.height;

	// 右下
	bottomSideEnemyBullet2.vertex.rightBottom.x = bottomSideEnemyBullet2.pos.x + bottomSideEnemyBullet2.width;
	bottomSideEnemyBullet2.vertex.rightBottom.y = bottomSideEnemyBullet2.pos.y + bottomSideEnemyBullet2.height;

	// テクスチャ情報とダメージ
	bottomSideEnemyBullet2.src.x = 0.0f;
	bottomSideEnemyBullet2.src.y = 0.0f;
	bottomSideEnemyBullet2.damage = 5;

	// フラグの初期化
	bottomSideEnemyBullet2.interpolation = false;
	bottomSideEnemyBullet2.isBulletShot = false;
	bottomSideEnemyBullet2.isHitX = false;
	bottomSideEnemyBullet2.isHitY = false;
	bottomSideEnemyBullet2.isHit = false;

#pragma endregion

#pragma endregion
	//====================================



	//====================================
#pragma region --- 左上の弾の初期化 ---
	//====================================

#pragma region --- 弾の基本情報 ---

	Bullet leftTopEnemyBullet{};

	// 弾の幅と高さ
	leftTopEnemyBullet.width = 20.0f;
	leftTopEnemyBullet.height = 20.0f;

	// 開始座標 (敵の左上)
	leftTopEnemyBullet.start.x = enemy.pos.x;
	leftTopEnemyBullet.start.y = enemy.pos.y;

	// 終了座標 (左上方向へ進む)
	leftTopEnemyBullet.end.x = enemy.pos.x - (leftTopEnemyBullet.width * 2.0f);
	leftTopEnemyBullet.end.y = enemy.pos.y - (leftTopEnemyBullet.height * 2.0f);

	// 弾の初期位置を設定
	leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
	leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;

	// 弾の速度や補間処理フラグなど
	leftTopEnemyBullet.speed = 5.0f;
	leftTopEnemyBullet.length = 0.0f;
	leftTopEnemyBullet.t = 0.0f;
	leftTopEnemyBullet.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
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

	// テクスチャ情報とダメージ
	leftTopEnemyBullet.src.x = 0.0f;
	leftTopEnemyBullet.src.y = 0.0f;
	leftTopEnemyBullet.damage = 5;

	// フラグの初期化
	leftTopEnemyBullet.interpolation = false;
	leftTopEnemyBullet.isBulletShot = false;
	leftTopEnemyBullet.isHitX = false;
	leftTopEnemyBullet.isHitY = false;
	leftTopEnemyBullet.isHit = false;

#pragma endregion

#pragma endregion


	//------------------
#pragma region --- 右上の弾の初期化 ---
	//====================================

#pragma region --- 弾の基本情報 ---

	Bullet rightTopEnemyBullet{};

	// 弾の幅と高さ
	rightTopEnemyBullet.width = 20.0f;
	rightTopEnemyBullet.height = 20.0f;

	// 開始座標 (敵の右上)
	rightTopEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightTopEnemyBullet.width;
	rightTopEnemyBullet.start.y = enemy.pos.y;

	// 終了座標 (右上方向へ進む)
	rightTopEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightTopEnemyBullet.width) + (rightTopEnemyBullet.width * 2.0f);
	rightTopEnemyBullet.end.y = enemy.pos.y - (rightTopEnemyBullet.height * 2.0f);

	// 弾の初期位置を設定
	rightTopEnemyBullet.pos.x = rightTopEnemyBullet.start.x;
	rightTopEnemyBullet.pos.y = rightTopEnemyBullet.start.y;

	// 弾の速度や補間処理フラグなど
	rightTopEnemyBullet.speed = 5.0f;
	rightTopEnemyBullet.length = 0.0f;
	rightTopEnemyBullet.t = 0.0f;
	rightTopEnemyBullet.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
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

	// テクスチャ情報とダメージ
	rightTopEnemyBullet.src.x = 0.0f;
	rightTopEnemyBullet.src.y = 0.0f;
	rightTopEnemyBullet.damage = 5;

	// フラグの初期化
	rightTopEnemyBullet.interpolation = false;
	rightTopEnemyBullet.isBulletShot = false;
	rightTopEnemyBullet.isHitX = false;
	rightTopEnemyBullet.isHitY = false;
	rightTopEnemyBullet.isHit = false;

#pragma endregion

#pragma endregion


	//------------------
#pragma region --- 左下の弾の初期化 ---
	//====================================

#pragma region --- 弾の基本情報 ---

	Bullet leftBottomEnemyBullet{};

	// 弾の幅と高さ
	leftBottomEnemyBullet.width = 20.0f;
	leftBottomEnemyBullet.height = 20.0f;

	// 開始座標 (敵の左下)
	leftBottomEnemyBullet.start.x = enemy.pos.x;
	leftBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height;

	// 終了座標 (左下方向へ進む)
	leftBottomEnemyBullet.end.x = enemy.pos.x - (leftBottomEnemyBullet.width * 2.0f);
	leftBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height) + (leftBottomEnemyBullet.height * 2.0f);

	// 弾の初期位置を設定
	leftBottomEnemyBullet.pos.x = leftBottomEnemyBullet.start.x;
	leftBottomEnemyBullet.pos.y = leftBottomEnemyBullet.start.y;

	// 弾の速度や補間処理フラグなど
	leftBottomEnemyBullet.speed = 5.0f;
	leftBottomEnemyBullet.length = 0.0f;
	leftBottomEnemyBullet.t = 0.0f;
	leftBottomEnemyBullet.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
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

	// テクスチャ情報とダメージ
	leftBottomEnemyBullet.src.x = 0.0f;
	leftBottomEnemyBullet.src.y = 0.0f;
	leftBottomEnemyBullet.damage = 5;

	// フラグの初期化
	leftBottomEnemyBullet.interpolation = false;
	leftBottomEnemyBullet.isBulletShot = false;
	leftBottomEnemyBullet.isHitX = false;
	leftBottomEnemyBullet.isHitY = false;
	leftBottomEnemyBullet.isHit = false;

#pragma endregion

#pragma endregion


	//------------------
#pragma region --- 右下の弾の初期化 ---
	//====================================

#pragma region --- 弾の基本情報 ---

	Bullet rightBottomEnemyBullet{};

	// 弾の幅と高さ
	rightBottomEnemyBullet.width = 20.0f;
	rightBottomEnemyBullet.height = 20.0f;

	// 開始座標 (敵の右下)
	rightBottomEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width;
	rightBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height;

	// 終了座標 (右下方向へ進む)
	rightBottomEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width) + (rightBottomEnemyBullet.width * 2.0f);
	rightBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height) + (rightBottomEnemyBullet.height * 2.0f);

	// 弾の初期位置を設定
	rightBottomEnemyBullet.pos.x = rightBottomEnemyBullet.start.x;
	rightBottomEnemyBullet.pos.y = rightBottomEnemyBullet.start.y;

	// 弾の速度や補間処理フラグなど
	rightBottomEnemyBullet.speed = 5.0f;
	rightBottomEnemyBullet.length = 0.0f;
	rightBottomEnemyBullet.t = 0.0f;
	rightBottomEnemyBullet.easedT = 0.0f;

	// --- 頂点座標の初期化 ---
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

	// テクスチャ情報とダメージ
	rightBottomEnemyBullet.src.x = 0.0f;
	rightBottomEnemyBullet.src.y = 0.0f;
	rightBottomEnemyBullet.damage = 5;

	// フラグの初期化
	rightBottomEnemyBullet.interpolation = false;
	rightBottomEnemyBullet.isBulletShot = false;
	rightBottomEnemyBullet.isHitX = false;
	rightBottomEnemyBullet.isHitY = false;
	rightBottomEnemyBullet.isHit = false;

#pragma endregion

#pragma endregion


	//====================================

#pragma endregion

#pragma region player

	Player player{};
	player.pos.x = 640.0f;
	player.pos.y = 620.0f;
	player.width = 64.0f;
	player.height = 64.0f;

	// 左上
	player.vertex.leftTop.x = player.pos.x;
	player.vertex.leftTop.y = player.pos.y;

	// 右上
	player.vertex.rightTop.x = player.pos.x + player.width;
	player.vertex.rightTop.y = player.pos.y;

	// 左下
	player.vertex.leftBottom.x = player.pos.x;
	player.vertex.leftBottom.y = player.pos.y + player.height;

	// 右下
	player.vertex.rightBottom.x = player.pos.x + player.width;
	player.vertex.rightBottom.y = player.pos.y + player.width;

	player.dir.x = player.pos.x - enemy.pos.y;
	player.dir.y = player.pos.y - enemy.pos.y;
	player.radius = 40.0f;
	player.speed = 5.0f;

	int direction = 0;//プレイヤーの向き

	int psychicTexture = Novice::LoadTexture("./Resources/images/psychic.png");

#pragma endregion

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
			direction = 0;
			player.pos.y -= player.speed;
		}

		if (keys[DIK_S])
		{
			direction = 1;
			player.pos.y += player.speed;
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

#pragma region --- 左辺の弾の更新処理 ---

#pragma region --- 弾1の左辺の弾 ---
		
		if (!leftSideEnemyBullet1.isBulletShot) {

			leftSideEnemyBullet1.length = 0.0f;
			leftSideEnemyBullet1.t = 0.0f;
			leftSideEnemyBullet1.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			leftSideEnemyBullet1.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (leftSideEnemyBullet1.length > 0.0f) {
				leftSideEnemyBullet1.velocity.x = (player.dir.x / leftSideEnemyBullet1.length) * leftSideEnemyBullet1.speed;
				leftSideEnemyBullet1.velocity.y = (player.dir.y / leftSideEnemyBullet1.length) * leftSideEnemyBullet1.speed;
			}

			leftSideEnemyBullet1.isBulletShot = true;
		}

		if (leftSideEnemyBullet1.isBulletShot) {

			if (!leftSideEnemyBullet1.interpolation) {

				leftSideEnemyBullet1.t += 0.008f;

				if (leftSideEnemyBullet1.t >= 1.0f) {
					leftSideEnemyBullet1.t = 1.0f;
					leftSideEnemyBullet1.interpolation = true;
				}

				leftSideEnemyBullet1.easedT = 1.0f - powf(1.0f - leftSideEnemyBullet1.t, 3.0f);
				leftSideEnemyBullet1.pos.x = (1.0f - leftSideEnemyBullet1.easedT) * leftSideEnemyBullet1.start.x + leftSideEnemyBullet1.easedT * leftSideEnemyBullet1.end.x;
				leftSideEnemyBullet1.pos.y = (1.0f - leftSideEnemyBullet1.easedT) * leftSideEnemyBullet1.start.y + leftSideEnemyBullet1.easedT * leftSideEnemyBullet1.end.y;

			} else {

				leftSideEnemyBullet1.pos.x += leftSideEnemyBullet1.velocity.x;
				leftSideEnemyBullet1.pos.y += leftSideEnemyBullet1.velocity.y;


				if (leftSideEnemyBullet1.pos.y <= 0.0f ||
					leftSideEnemyBullet1.pos.x <= 0.0f ||
					leftSideEnemyBullet1.pos.x >= 1280.0f ||
					leftSideEnemyBullet1.pos.y + leftSideEnemyBullet1.height >= 720.0f) {

					// 弾の開始位置（敵の左辺）
					leftSideEnemyBullet1.start.x = enemy.pos.x;
					leftSideEnemyBullet1.start.y = enemy.pos.y + (leftSideEnemyBullet1.height * 1.2f);

					// 弾の終了位置（左方向への移動を設定）
					leftSideEnemyBullet1.end.x = enemy.pos.x - (leftSideEnemyBullet1.width * 2.0f);
					leftSideEnemyBullet1.end.y = enemy.pos.y + (leftSideEnemyBullet1.height * 1.2f);

					// 弾の初期位置を設定
					leftSideEnemyBullet1.pos.x = leftSideEnemyBullet1.start.x;
					leftSideEnemyBullet1.pos.y = leftSideEnemyBullet1.start.y;

					leftSideEnemyBullet1.isBulletShot = false;
					leftSideEnemyBullet1.interpolation = false;
				}
			}
		}

#pragma endregion

#pragma region --- 弾2の左辺の弾 ---

		if (!leftSideEnemyBullet2.isBulletShot) {

			leftSideEnemyBullet2.length = 0.0f;
			leftSideEnemyBullet2.t = 0.0f;
			leftSideEnemyBullet2.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			leftSideEnemyBullet2.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (leftSideEnemyBullet2.length > 0.0f) {
				leftSideEnemyBullet2.velocity.x = (player.dir.x / leftSideEnemyBullet2.length) * leftSideEnemyBullet2.speed;
				leftSideEnemyBullet2.velocity.y = (player.dir.y / leftSideEnemyBullet2.length) * leftSideEnemyBullet2.speed;
			}

			leftSideEnemyBullet2.isBulletShot = true;
		}

		if (leftSideEnemyBullet2.isBulletShot) {

			if (!leftSideEnemyBullet2.interpolation) {

				leftSideEnemyBullet2.t += 0.008f;

				if (leftSideEnemyBullet2.t >= 1.0f) {
					leftSideEnemyBullet2.t = 1.0f;
					leftSideEnemyBullet2.interpolation = true;
				}

				leftSideEnemyBullet2.easedT = 1.0f - powf(1.0f - leftSideEnemyBullet2.t, 3.0f);
				leftSideEnemyBullet2.pos.x = (1.0f - leftSideEnemyBullet2.easedT) * leftSideEnemyBullet2.start.x + leftSideEnemyBullet2.easedT * leftSideEnemyBullet2.end.x;
				leftSideEnemyBullet2.pos.y = (1.0f - leftSideEnemyBullet2.easedT) * leftSideEnemyBullet2.start.y + leftSideEnemyBullet2.easedT * leftSideEnemyBullet2.end.y;

			} else {

				leftSideEnemyBullet2.pos.x += leftSideEnemyBullet2.velocity.x;
				leftSideEnemyBullet2.pos.y += leftSideEnemyBullet2.velocity.y;


				if (leftSideEnemyBullet2.pos.y <= 0.0f ||
					leftSideEnemyBullet2.pos.x <= 0.0f ||
					leftSideEnemyBullet2.pos.x >= 1280.0f ||
					leftSideEnemyBullet2.pos.y + leftSideEnemyBullet2.height >= 720.0f) {

					// 弾の開始位置（敵の左辺）
					leftSideEnemyBullet2.start.x = enemy.pos.x;
					leftSideEnemyBullet2.start.y = enemy.pos.y + (leftSideEnemyBullet2.height * 1.2f);

					// 弾の終了位置（左方向への移動を設定）
					leftSideEnemyBullet2.end.x = enemy.pos.x - (leftSideEnemyBullet2.width * 2.0f);
					leftSideEnemyBullet2.end.y = enemy.pos.y + (leftSideEnemyBullet2.height * 1.2f);

					// 弾の初期位置を設定
					leftSideEnemyBullet2.pos.x = leftSideEnemyBullet2.start.x;
					leftSideEnemyBullet2.pos.y = leftSideEnemyBullet2.start.y;

					leftSideEnemyBullet2.isBulletShot = false;
					leftSideEnemyBullet2.interpolation = false;
				}
			}
		}

#pragma endregion

#pragma endregion

#pragma region --- 右辺の弾の更新処理 ---

		// --- 敵1の右辺の弾 ---
		if (!rightSideEnemyBullet1.isBulletShot) {

			rightSideEnemyBullet1.length = 0.0f;
			rightSideEnemyBullet1.t = 0.0f;
			rightSideEnemyBullet1.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			rightSideEnemyBullet1.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (rightSideEnemyBullet1.length > 0.0f) {
				rightSideEnemyBullet1.velocity.x = (player.dir.x / rightSideEnemyBullet1.length) * rightSideEnemyBullet1.speed;
				rightSideEnemyBullet1.velocity.y = (player.dir.y / rightSideEnemyBullet1.length) * rightSideEnemyBullet1.speed;
			}

			rightSideEnemyBullet1.isBulletShot = true;
		}

		if (rightSideEnemyBullet1.isBulletShot) {

			if (!rightSideEnemyBullet1.interpolation) {

				rightSideEnemyBullet1.t += 0.008f;

				if (rightSideEnemyBullet1.t >= 1.0f) {
					rightSideEnemyBullet1.t = 1.0f;
					rightSideEnemyBullet1.interpolation = true;
				}

				rightSideEnemyBullet1.easedT = 1.0f - powf(1.0f - rightSideEnemyBullet1.t, 3.0f);
				rightSideEnemyBullet1.pos.x = (1.0f - rightSideEnemyBullet1.easedT) * rightSideEnemyBullet1.start.x + rightSideEnemyBullet1.easedT * rightSideEnemyBullet1.end.x;
				rightSideEnemyBullet1.pos.y = (1.0f - rightSideEnemyBullet1.easedT) * rightSideEnemyBullet1.start.y + rightSideEnemyBullet1.easedT * rightSideEnemyBullet1.end.y;

			} else {

				rightSideEnemyBullet1.pos.x += rightSideEnemyBullet1.velocity.x;
				rightSideEnemyBullet1.pos.y += rightSideEnemyBullet1.velocity.y;

				if (rightSideEnemyBullet1.pos.y <= 0.0f ||
					rightSideEnemyBullet1.pos.x <= 0.0f ||
					rightSideEnemyBullet1.pos.x >= 1280.0f ||
					rightSideEnemyBullet1.pos.y + rightSideEnemyBullet1.height >= 720.0f) {

					// 弾の開始座標 (敵の右端から発射)
					rightSideEnemyBullet1.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet1.width;
					rightSideEnemyBullet1.start.y = enemy.pos.y + (rightSideEnemyBullet1.height * 1.2f);

					// 弾の終了座標 (右方向へ進む)
					rightSideEnemyBullet1.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet1.width);
					rightSideEnemyBullet1.end.y = enemy.pos.y + (rightSideEnemyBullet1.height * 1.2f);

					// 弾の現在座標
					rightSideEnemyBullet1.pos.x = rightSideEnemyBullet1.start.x;
					rightSideEnemyBullet1.pos.y = rightSideEnemyBullet1.start.y;
					rightSideEnemyBullet1.isBulletShot = false;
					rightSideEnemyBullet1.interpolation = false;
				}
			}
		}

#pragma endregion

#pragma region --- 上辺の弾の更新処理 ---

		// --- 敵の上辺の弾 ---
		for (int i = 0; i < topSideEnemyBullet[0].columns; ++i) {

			if (!topSideEnemyBullet[i].isBulletShot) {

				topSideEnemyBullet[i].length = 0.0f;
				topSideEnemyBullet[i].t = 0.0f;
				topSideEnemyBullet[i].easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				topSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (topSideEnemyBullet[i].length > 0.0f) {
					topSideEnemyBullet[i].velocity.x = (player.dir.x / topSideEnemyBullet[i].length) * topSideEnemyBullet[i].speed;
					topSideEnemyBullet[i].velocity.y = (player.dir.y / topSideEnemyBullet[i].length) * topSideEnemyBullet[i].speed;
				}

				topSideEnemyBullet[i].isBulletShot = true;
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

					topSideEnemyBullet[i].pos.x += topSideEnemyBullet[i].velocity.x;
					topSideEnemyBullet[i].pos.y += topSideEnemyBullet[i].velocity.y;

					if (topSideEnemyBullet[i].pos.y <= 0.0f ||
						topSideEnemyBullet[i].pos.x <= 0.0f ||
						topSideEnemyBullet[i].pos.x >= 1280.0f ||
						topSideEnemyBullet[i].pos.y + topSideEnemyBullet[i].height >= 720.0f) {

						// 弾の開始座標 (敵の上端から発射)
						topSideEnemyBullet[i].start.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
						topSideEnemyBullet[i].start.y = enemy.pos.y;

						// 弾の終了座標 (上方向へ進む)
						topSideEnemyBullet[i].end.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
						topSideEnemyBullet[i].end.y = enemy.pos.y - (topSideEnemyBullet[i].height * 2.0f);

						// 弾の現在座標
						topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
						topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;

						topSideEnemyBullet[i].isBulletShot = false;
						topSideEnemyBullet[i].interpolation = false;
					}
				}
			}
		}

#pragma endregion

#pragma region --- 上辺の弾の更新処理 ---

		// --- 敵の下辺の弾 ---
		for (int i = 0; i < bottomSideEnemyBullet[0].columns; ++i) {

			if (!bottomSideEnemyBullet[i].isBulletShot) {

				bottomSideEnemyBullet[i].length = 0.0f;
				bottomSideEnemyBullet[i].t = 0.0f;
				bottomSideEnemyBullet[i].easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				bottomSideEnemyBullet[i].length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (bottomSideEnemyBullet[i].length > 0.0f) {
					bottomSideEnemyBullet[i].velocity.x = (player.dir.x / bottomSideEnemyBullet[i].length) * bottomSideEnemyBullet[i].speed;
					bottomSideEnemyBullet[i].velocity.y = (player.dir.y / bottomSideEnemyBullet[i].length) * bottomSideEnemyBullet[i].speed;
				}

				bottomSideEnemyBullet[i].isBulletShot = true;
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

					bottomSideEnemyBullet[i].pos.x += bottomSideEnemyBullet[i].velocity.x;
					bottomSideEnemyBullet[i].pos.y += bottomSideEnemyBullet[i].velocity.y;

					if (bottomSideEnemyBullet[i].pos.y <= 0.0f ||
						bottomSideEnemyBullet[i].pos.x <= 0.0f ||
						bottomSideEnemyBullet[i].pos.x >= 1280.0f ||
						bottomSideEnemyBullet[i].pos.y + bottomSideEnemyBullet[i].height >= 720.0f) {

						// 弾の開始座標 (敵の下端から発射)
						bottomSideEnemyBullet[i].start.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
						bottomSideEnemyBullet[i].start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet[i].height;

						// 弾の終了座標 (下方向へ進む)
						bottomSideEnemyBullet[i].end.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
						bottomSideEnemyBullet[i].end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet[i].height;

						// 弾の現在座標
						bottomSideEnemyBullet[i].pos.x = bottomSideEnemyBullet[i].start.x;
						bottomSideEnemyBullet[i].pos.y = bottomSideEnemyBullet[i].start.y;

						bottomSideEnemyBullet[i].isBulletShot = false;
						bottomSideEnemyBullet[i].interpolation = false;
					}
				}
			}
		}

#pragma endregion

#pragma region --- 敵の左上の弾 ---

		// --- 敵の左上の弾 ---
		if (!leftTopEnemyBullet.isBulletShot) {

			leftTopEnemyBullet.length = 0.0f;
			leftTopEnemyBullet.t = 0.0f;
			leftTopEnemyBullet.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			leftTopEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (leftTopEnemyBullet.length > 0.0f) {
				leftTopEnemyBullet.velocity.x = (player.dir.x / leftTopEnemyBullet.length) * leftTopEnemyBullet.speed;
				leftTopEnemyBullet.velocity.y = (player.dir.y / leftTopEnemyBullet.length) * leftTopEnemyBullet.speed;
			}

			leftTopEnemyBullet.isBulletShot = true;
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

				leftTopEnemyBullet.pos.x += leftTopEnemyBullet.velocity.x;
				leftTopEnemyBullet.pos.y += leftTopEnemyBullet.velocity.y;

				if (leftTopEnemyBullet.pos.y <= 0.0f ||
					leftTopEnemyBullet.pos.x <= 0.0f ||
					leftTopEnemyBullet.pos.x >= 1280.0f ||
					leftTopEnemyBullet.pos.y + leftTopEnemyBullet.height >= 720.0f) {

					// 開始座標 (敵の左上)
					leftTopEnemyBullet.start.x = enemy.pos.x;
					leftTopEnemyBullet.start.y = enemy.pos.y;

					// 終了座標 (左上方向へ進む)
					leftTopEnemyBullet.end.x = enemy.pos.x - (leftTopEnemyBullet.width * 2.0f);
					leftTopEnemyBullet.end.y = enemy.pos.y - (leftTopEnemyBullet.height * 2.0f);

					// 現在座標の初期化
					leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
					leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;

					leftTopEnemyBullet.isBulletShot = false;
					leftTopEnemyBullet.interpolation = false;
				}
			}
		}

#pragma endregion

#pragma region --- 敵の右上の弾 ---

		// --- 敵の右上の弾 ---
		if (!rightTopEnemyBullet.isBulletShot) {

			rightTopEnemyBullet.length = 0.0f;
			rightTopEnemyBullet.t = 0.0f;
			rightTopEnemyBullet.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			rightTopEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (rightTopEnemyBullet.length > 0.0f) {
				rightTopEnemyBullet.velocity.x = (player.dir.x / rightTopEnemyBullet.length) * rightTopEnemyBullet.speed;
				rightTopEnemyBullet.velocity.y = (player.dir.y / rightTopEnemyBullet.length) * rightTopEnemyBullet.speed;
			}

			rightTopEnemyBullet.isBulletShot = true;
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

				rightTopEnemyBullet.pos.x += rightTopEnemyBullet.velocity.x;
				rightTopEnemyBullet.pos.y += rightTopEnemyBullet.velocity.y;

				if (rightTopEnemyBullet.pos.y <= 0.0f ||
					rightTopEnemyBullet.pos.x <= 0.0f ||
					rightTopEnemyBullet.pos.x >= 1280.0f ||
					rightTopEnemyBullet.pos.y + rightTopEnemyBullet.height >= 720.0f) {

					// 開始座標 (敵の右上)
					rightTopEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightTopEnemyBullet.width;
					rightTopEnemyBullet.start.y = enemy.pos.y;

					// 終了座標 (右上方向へ進む)
					rightTopEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightTopEnemyBullet.width) + (rightTopEnemyBullet.width * 2.0f);
					rightTopEnemyBullet.end.y = enemy.pos.y - (rightTopEnemyBullet.height * 2.0f);

					// 現在座標の初期化
					rightTopEnemyBullet.pos.x = rightTopEnemyBullet.start.x;
					rightTopEnemyBullet.pos.y = rightTopEnemyBullet.start.y;

					rightTopEnemyBullet.isBulletShot = false;
					rightTopEnemyBullet.interpolation = false;
				}
			}
		}

#pragma endregion

		// --- 敵の左下の弾 ---
		if (!leftBottomEnemyBullet.isBulletShot) {

			leftBottomEnemyBullet.length = 0.0f;
			leftBottomEnemyBullet.t = 0.0f;
			leftBottomEnemyBullet.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			leftBottomEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);

			if (leftBottomEnemyBullet.length > 0.0f) {
				leftBottomEnemyBullet.velocity.x = (player.dir.x / leftBottomEnemyBullet.length) * leftBottomEnemyBullet.speed;
				leftBottomEnemyBullet.velocity.y = (player.dir.y / leftBottomEnemyBullet.length) * leftBottomEnemyBullet.speed;
			}

			leftBottomEnemyBullet.isBulletShot = true;
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

				leftBottomEnemyBullet.pos.x += leftBottomEnemyBullet.velocity.x;
				leftBottomEnemyBullet.pos.y += leftBottomEnemyBullet.velocity.y;

				if (leftBottomEnemyBullet.pos.y <= 0.0f ||
					leftBottomEnemyBullet.pos.x <= 0.0f ||
					leftBottomEnemyBullet.pos.x >= 1280.0f ||
					leftBottomEnemyBullet.pos.y + leftBottomEnemyBullet.height >= 720.0f) {

					// 開始座標 (敵の左下)
					leftBottomEnemyBullet.start.x = enemy.pos.x;
					leftBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height;

					// 終了座標 (左下方向へ進む)
					leftBottomEnemyBullet.end.x = enemy.pos.x - (leftBottomEnemyBullet.width * 2.0f);
					leftBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height) + (leftBottomEnemyBullet.height * 2.0f);

					// 現在座標の初期化
					leftBottomEnemyBullet.pos.x = leftBottomEnemyBullet.start.x;
					leftBottomEnemyBullet.pos.y = leftBottomEnemyBullet.start.y;

					leftBottomEnemyBullet.isBulletShot = false;
					leftBottomEnemyBullet.interpolation = false;
				}
			}
		}

		// --- 敵の右下の弾 ---
		if (!rightBottomEnemyBullet.isBulletShot) {

			rightBottomEnemyBullet.length = 0.0f;
			rightBottomEnemyBullet.t = 0.0f;
			rightBottomEnemyBullet.easedT = 0.0f;

			player.dir.x = player.pos.x - enemy.pos.x;
			player.dir.y = player.pos.y - enemy.pos.y;

			rightBottomEnemyBullet.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
			if (rightBottomEnemyBullet.length > 0.0f) {
				rightBottomEnemyBullet.velocity.x = (player.dir.x / rightBottomEnemyBullet.length) * rightBottomEnemyBullet.speed;
				rightBottomEnemyBullet.velocity.y = (player.dir.y / rightBottomEnemyBullet.length) * rightBottomEnemyBullet.speed;
			}

			rightBottomEnemyBullet.isBulletShot = true;
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

				rightBottomEnemyBullet.pos.x += rightBottomEnemyBullet.velocity.x;
				rightBottomEnemyBullet.pos.y += rightBottomEnemyBullet.velocity.y;

				if (rightBottomEnemyBullet.pos.y <= 0.0f ||
					rightBottomEnemyBullet.pos.x <= 0.0f ||
					rightBottomEnemyBullet.pos.x >= 1280.0f ||
					rightBottomEnemyBullet.pos.y + rightBottomEnemyBullet.height >= 720.0f) {

					// 開始座標 (敵の右下)
					rightBottomEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width;
					rightBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height;

					// 終了座標 (右下方向へ進む)
					rightBottomEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width) + (rightBottomEnemyBullet.width * 2.0f);
					rightBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height) + (rightBottomEnemyBullet.height * 2.0f);

					// 現在座標の初期化
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


		// player
		// 左上
		player.vertex.leftTop.x = player.pos.x;
		player.vertex.leftTop.y = player.pos.y;

		// 右上
		player.vertex.rightTop.x = player.pos.x + player.width;
		player.vertex.rightTop.y = player.pos.y;

		// 左下
		player.vertex.leftBottom.x = player.pos.x;
		player.vertex.leftBottom.y = player.pos.y + player.height;

		// 右下
		player.vertex.rightBottom.x = player.pos.x + player.width;
		player.vertex.rightBottom.y = player.pos.y + player.width;

		// 敵弾と自機の当たり判定
		// 左
		for (int i = 0; i < leftSideEnemyBullet[0].columns; ++i) {

			if ((player.vertex.leftTop.x) < (leftSideEnemyBullet[i].vertex.rightBottom.x) && (leftSideEnemyBullet[i].vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				leftSideEnemyBullet[i].isHitX = true;
			} else {
				leftSideEnemyBullet[i].isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (leftSideEnemyBullet[i].vertex.rightBottom.y) && (leftSideEnemyBullet[i].vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				leftSideEnemyBullet[i].isHitY = true;
			} else {
				leftSideEnemyBullet[i].isHitY = false;
			}

			if (leftSideEnemyBullet[i].isHitX && leftSideEnemyBullet[i].isHitY) {

				leftSideEnemyBullet[i].isHit = true;

				// 弾の開始位置（敵の左辺）
				leftSideEnemyBullet[i].start.x = enemy.pos.x;
				leftSideEnemyBullet[i].start.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

				// 弾の終了位置（左方向への移動を設定）
				leftSideEnemyBullet[i].end.x = enemy.pos.x - (leftSideEnemyBullet[i].width * 2.0f);
				leftSideEnemyBullet[i].end.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

				// 弾の初期位置を設定
				leftSideEnemyBullet[i].pos.x = leftSideEnemyBullet[i].start.x;
				leftSideEnemyBullet[i].pos.y = leftSideEnemyBullet[i].start.y;



				leftSideEnemyBullet[i].isBulletShot = false;
				leftSideEnemyBullet[i].interpolation = false;

			} else {
				leftSideEnemyBullet[i].isHit = false;
			}
		}

		// 右
		for (int i = 0; i < rightSideEnemyBullet[0].columns; ++i) {

			if ((player.vertex.leftTop.x) < (rightSideEnemyBullet[i].vertex.rightBottom.x) && (rightSideEnemyBullet[i].vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				rightSideEnemyBullet[i].isHitX = true;
			} else {
				rightSideEnemyBullet[i].isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (rightSideEnemyBullet[i].vertex.rightBottom.y) && (rightSideEnemyBullet[i].vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				rightSideEnemyBullet[i].isHitY = true;
			} else {
				rightSideEnemyBullet[i].isHitY = false;
			}

			if (rightSideEnemyBullet[i].isHitX && rightSideEnemyBullet[i].isHitY) {

				rightSideEnemyBullet[i].isHit = true;

				// 弾の開始座標 (敵の右端から発射)
				rightSideEnemyBullet[i].start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet[i].width;
				rightSideEnemyBullet[i].start.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

				// 弾の終了座標 (右方向へ進む)
				rightSideEnemyBullet[i].end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet[i].width);
				rightSideEnemyBullet[i].end.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

				// 弾の現在座標
				rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
				rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;



				rightSideEnemyBullet[i].isBulletShot = false;
				rightSideEnemyBullet[i].interpolation = false;
			} else {
				rightSideEnemyBullet[i].isHit = false;
			}
		}

		// 上
		for (int i = 0; i < topSideEnemyBullet[0].columns; ++i) {

			if ((player.vertex.leftTop.x) < (topSideEnemyBullet[i].vertex.rightBottom.x) && (topSideEnemyBullet[i].vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				topSideEnemyBullet[i].isHitX = true;
			} else {
				topSideEnemyBullet[i].isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (topSideEnemyBullet[i].vertex.rightBottom.y) && (topSideEnemyBullet[i].vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				topSideEnemyBullet[i].isHitY = true;
			} else {
				topSideEnemyBullet[i].isHitY = false;
			}

			if (topSideEnemyBullet[i].isHitX && topSideEnemyBullet[i].isHitY) {

				topSideEnemyBullet[i].isHit = true;

				// 弾の開始座標 (敵の上端から発射)
				topSideEnemyBullet[i].start.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
				topSideEnemyBullet[i].start.y = enemy.pos.y;

				// 弾の終了座標 (上方向へ進む)
				topSideEnemyBullet[i].end.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
				topSideEnemyBullet[i].end.y = enemy.pos.y - (topSideEnemyBullet[i].height * 2.0f);

				// 弾の現在座標
				topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
				topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;



				topSideEnemyBullet[i].isBulletShot = false;
				topSideEnemyBullet[i].interpolation = false;

			} else {
				topSideEnemyBullet[i].isHit = false;
			}
		}

		// 下
		for (int i = 0; i < bottomSideEnemyBullet[0].columns; ++i) {

			if ((player.vertex.leftTop.x) < (bottomSideEnemyBullet[i].vertex.rightBottom.x) && (bottomSideEnemyBullet[i].vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				bottomSideEnemyBullet[i].isHitX = true;
			} else {
				bottomSideEnemyBullet[i].isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (bottomSideEnemyBullet[i].vertex.rightBottom.y) && (bottomSideEnemyBullet[i].vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				bottomSideEnemyBullet[i].isHitY = true;
			} else {
				bottomSideEnemyBullet[i].isHitY = false;
			}

			if (bottomSideEnemyBullet[i].isHitX && bottomSideEnemyBullet[i].isHitY) {

				bottomSideEnemyBullet[i].isHit = true;

				// 弾の開始座標 (敵の下端から発射)
				bottomSideEnemyBullet[i].start.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
				bottomSideEnemyBullet[i].start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet[i].height;

				// 弾の終了座標 (下方向へ進む)
				bottomSideEnemyBullet[i].end.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
				bottomSideEnemyBullet[i].end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet[i].height;

				// 弾の現在座標
				bottomSideEnemyBullet[i].pos.x = bottomSideEnemyBullet[i].start.x;
				bottomSideEnemyBullet[i].pos.y = bottomSideEnemyBullet[i].start.y;



				bottomSideEnemyBullet[i].isBulletShot = false;
				bottomSideEnemyBullet[i].interpolation = false;
			} else {
				bottomSideEnemyBullet[i].isHit = false;
			}
		}

		// 左上
		if ((player.vertex.leftTop.x) < (leftTopEnemyBullet.vertex.rightBottom.x) && (leftTopEnemyBullet.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
			leftTopEnemyBullet.isHitX = true;
		} else {
			leftTopEnemyBullet.isHitX = false;
		}

		if ((player.vertex.leftTop.y) < (leftTopEnemyBullet.vertex.rightBottom.y) && (leftTopEnemyBullet.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
			leftTopEnemyBullet.isHitY = true;
		} else {
			leftTopEnemyBullet.isHitY = false;
		}

		if (leftTopEnemyBullet.isHitX && leftTopEnemyBullet.isHitY) {

			leftTopEnemyBullet.isHit = true;

			// 開始座標 (敵の左上)
			leftTopEnemyBullet.start.x = enemy.pos.x;
			leftTopEnemyBullet.start.y = enemy.pos.y;

			// 終了座標 (左上方向へ進む)
			leftTopEnemyBullet.end.x = enemy.pos.x - (leftTopEnemyBullet.width * 2.0f);
			leftTopEnemyBullet.end.y = enemy.pos.y - (leftTopEnemyBullet.height * 2.0f);

			// 現在座標の初期化
			leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
			leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;



			leftTopEnemyBullet.isBulletShot = false;
			leftTopEnemyBullet.interpolation = false;
		} else {
			leftTopEnemyBullet.isHit = false;
		}

		// 右上
		if ((player.vertex.leftTop.x) < (rightTopEnemyBullet.vertex.rightBottom.x) && (rightTopEnemyBullet.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
			rightTopEnemyBullet.isHitX = true;
		} else {
			rightTopEnemyBullet.isHitX = false;
		}

		if ((player.vertex.leftTop.y) < (rightTopEnemyBullet.vertex.rightBottom.y) && (rightTopEnemyBullet.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
			rightTopEnemyBullet.isHitY = true;
		} else {
			rightTopEnemyBullet.isHitY = false;
		}

		if (rightTopEnemyBullet.isHitX && rightTopEnemyBullet.isHitY) {

			rightTopEnemyBullet.isHit = true;

			// 開始座標 (敵の右上)
			rightTopEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightTopEnemyBullet.width;
			rightTopEnemyBullet.start.y = enemy.pos.y;

			// 終了座標 (右上方向へ進む)
			rightTopEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightTopEnemyBullet.width) + (rightTopEnemyBullet.width * 2.0f);
			rightTopEnemyBullet.end.y = enemy.pos.y - (rightTopEnemyBullet.height * 2.0f);

			// 現在座標の初期化
			rightTopEnemyBullet.pos.x = rightTopEnemyBullet.start.x;
			rightTopEnemyBullet.pos.y = rightTopEnemyBullet.start.y;



			rightTopEnemyBullet.isBulletShot = false;
			rightTopEnemyBullet.interpolation = false;
		} else {
			rightTopEnemyBullet.isHit = false;
		}

		// 左下
		if ((player.vertex.leftTop.x) < (leftBottomEnemyBullet.vertex.rightBottom.x) && (leftBottomEnemyBullet.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
			leftBottomEnemyBullet.isHitX = true;
		} else {
			leftBottomEnemyBullet.isHitX = false;
		}

		if ((player.vertex.leftTop.y) < (leftBottomEnemyBullet.vertex.rightBottom.y) && (leftBottomEnemyBullet.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
			leftBottomEnemyBullet.isHitY = true;
		} else {
			leftBottomEnemyBullet.isHitY = false;
		}

		if (leftBottomEnemyBullet.isHitX && leftBottomEnemyBullet.isHitY) {

			leftBottomEnemyBullet.isHit = true;

			// 開始座標 (敵の左下)
			leftBottomEnemyBullet.start.x = enemy.pos.x;
			leftBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height;

			// 終了座標 (左下方向へ進む)
			leftBottomEnemyBullet.end.x = enemy.pos.x - (leftBottomEnemyBullet.width * 2.0f);
			leftBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - leftBottomEnemyBullet.height) + (leftBottomEnemyBullet.height * 2.0f);

			// 現在座標の初期化
			leftBottomEnemyBullet.pos.x = leftBottomEnemyBullet.start.x;
			leftBottomEnemyBullet.pos.y = leftBottomEnemyBullet.start.y;



			leftBottomEnemyBullet.isBulletShot = false;
			leftBottomEnemyBullet.interpolation = false;

		} else {
			leftBottomEnemyBullet.isHit = false;
		}

		// 右下
		if ((player.vertex.leftTop.x) < (rightBottomEnemyBullet.vertex.rightBottom.x) && (rightBottomEnemyBullet.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
			rightBottomEnemyBullet.isHitX = true;
		} else {
			rightBottomEnemyBullet.isHitX = false;
		}

		if ((player.vertex.leftTop.y) < (rightBottomEnemyBullet.vertex.rightBottom.y) && (rightBottomEnemyBullet.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
			rightBottomEnemyBullet.isHitY = true;
		} else {
			rightBottomEnemyBullet.isHitY = false;
		}

		if (rightBottomEnemyBullet.isHitX && rightBottomEnemyBullet.isHitY) {

			rightBottomEnemyBullet.isHit = true;

			// 開始座標 (敵の右下)
			rightBottomEnemyBullet.start.x = (enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width;
			rightBottomEnemyBullet.start.y = (enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height;

			// 終了座標 (右下方向へ進む)
			rightBottomEnemyBullet.end.x = ((enemy.pos.x + enemy.width) - rightBottomEnemyBullet.width) + (rightBottomEnemyBullet.width * 2.0f);
			rightBottomEnemyBullet.end.y = ((enemy.pos.y + enemy.height) - rightBottomEnemyBullet.height) + (rightBottomEnemyBullet.height * 2.0f);

			// 現在座標の初期化
			rightBottomEnemyBullet.pos.x = rightBottomEnemyBullet.start.x;
			rightBottomEnemyBullet.pos.y = rightBottomEnemyBullet.start.y;



			rightBottomEnemyBullet.isBulletShot = false;
			rightBottomEnemyBullet.interpolation = false;

		} else {
			rightBottomEnemyBullet.isHit = false;
		}

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
		Novice::DrawSprite(
			int(player.pos.x),
			int(player.pos.y),
			psychicTexture,
			1, 1,
			0.0f, 0xFFFFFFFF
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