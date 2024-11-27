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
	int frameCount;
	int frameCounter;
};

struct Player {
	Vertex vertex;
	Vector2 pos;
	Vector2 dir;
	float radius;
	float width;
	float height;
	float speed;

	int hp;
	int direction;
	int back;
	int front;
	int leftBulletCount;
	int rightBulletCount;
	int type;
	int leftBullet[2];
	int rightBullet[2];
};

struct PlayerBullet
{
	Vertex vertex;
	Vector2 pos;
	float radius;
	float speed;
	float width;
	float height;
	int isShot;
	int isHitX;
	int isHitY;
	int isHit;
	int damage;
};

enum Scene
{
	TITLE,
	OPENE1,
	OPENE2,
	GAME,
	CLEAR,
	OVER,
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

#pragma region Initialization

#pragma region --- enemyの初期化 ---

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

	int enemyBulletTexture = Novice::LoadTexture("./Resources/images/tekitama01.png");

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

	leftSideEnemyBullet1.frameCount = 0;
	leftSideEnemyBullet1.frameCounter = 0;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	leftSideEnemyBullet2.width = 20.0f;  // 弾の幅
	leftSideEnemyBullet2.height = 20.0f; // 弾の高さ

	// 弾の開始位置（敵の左辺）
	leftSideEnemyBullet2.start.x = enemy.pos.x;
	leftSideEnemyBullet2.start.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

	// 弾の終了位置（左方向への移動を設定）
	leftSideEnemyBullet2.end.x = enemy.pos.x - (leftSideEnemyBullet2.width * 2.0f);
	leftSideEnemyBullet2.end.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

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

	leftSideEnemyBullet2.frameCount = 0;
	leftSideEnemyBullet2.frameCounter = 0;

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

	rightSideEnemyBullet1.frameCount = 0;
	rightSideEnemyBullet1.frameCounter = 0;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	rightSideEnemyBullet2.width = 20.0f;
	rightSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の右端から発射)
	rightSideEnemyBullet2.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet2.width;
	rightSideEnemyBullet2.start.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

	// 弾の終了座標 (右方向へ進む)
	rightSideEnemyBullet2.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet2.width);
	rightSideEnemyBullet2.end.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

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

	rightSideEnemyBullet2.frameCount = 0;
	rightSideEnemyBullet2.frameCounter = 0;

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

	topSideEnemyBullet1.frameCount = 0;
	topSideEnemyBullet1.frameCounter = 0;

#pragma endregion

#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	topSideEnemyBullet2.width = 20.0f;
	topSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の上端から発射)
	topSideEnemyBullet2.start.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
	topSideEnemyBullet2.start.y = enemy.pos.y;

	// 弾の終了座標 (上方向へ進む)
	topSideEnemyBullet2.end.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
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

	topSideEnemyBullet2.frameCount = 0;
	topSideEnemyBullet2.frameCounter = 0;

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

	bottomSideEnemyBullet1.frameCount = 0;
	bottomSideEnemyBullet1.frameCounter = 0;

#pragma endregion
	Bullet bottomSideEnemyBullet2{};
#pragma region --- 弾2の基本情報 ---

	// 弾の幅と高さ
	bottomSideEnemyBullet2.width = 20.0f;
	bottomSideEnemyBullet2.height = 20.0f;

	// 弾の開始座標 (敵の下端から発射)
	bottomSideEnemyBullet2.start.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
	bottomSideEnemyBullet2.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet2.height;

	// 弾の終了座標 (下方向へ進む)
	bottomSideEnemyBullet2.end.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
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

	bottomSideEnemyBullet2.frameCount = 0;
	bottomSideEnemyBullet2.frameCounter = 0;

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

	leftTopEnemyBullet.frameCount = 0;
	leftTopEnemyBullet.frameCounter = 0;

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

	rightTopEnemyBullet.frameCount = 0;
	rightTopEnemyBullet.frameCounter = 0;

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

	leftBottomEnemyBullet.frameCount = 0;
	leftBottomEnemyBullet.frameCounter = 0;

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

	rightBottomEnemyBullet.frameCount = 0;
	rightBottomEnemyBullet.frameCounter = 0;

#pragma endregion

#pragma endregion


	//====================================

#pragma endregion

#pragma region --- playerの初期化 ---

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


	player.type = 0;//玉の属性
	player.direction = 0;//プレイヤーの向き
	player.hp = 200;
	player.back = true;
	player.front = false;
	player.leftBulletCount = 0;
	player.rightBulletCount = 0;

	PlayerBullet strightBullet1{};
	strightBullet1.pos.x = 640.0f;
	strightBullet1.pos.y = 620.0f;
	strightBullet1.radius = 10.0f;
	strightBullet1.speed = 5.0f;
	strightBullet1.width = 32.0f;
	strightBullet1.height = 32.0f;
	strightBullet1.isShot = false;
	strightBullet1.isHitX = false;
	strightBullet1.isHitY = false;
	strightBullet1.isHit = false;

	strightBullet1.vertex.leftTop.x = strightBullet1.pos.x;
	strightBullet1.vertex.leftTop.y = strightBullet1.pos.y;

	strightBullet1.vertex.rightTop.x = strightBullet1.pos.x + strightBullet1.width;
	strightBullet1.vertex.rightTop.y = strightBullet1.pos.y;

	strightBullet1.vertex.leftBottom.x = strightBullet1.pos.x;
	strightBullet1.vertex.leftBottom.y = strightBullet1.pos.y + strightBullet1.height;

	strightBullet1.vertex.rightBottom.x = strightBullet1.pos.x + strightBullet1.width;
	strightBullet1.vertex.rightBottom.y = strightBullet1.pos.y + strightBullet1.height;

	strightBullet1.damage = 10;




	PlayerBullet strightBullet2{};
	strightBullet2.pos.x = 640.0f;
	strightBullet2.pos.y = 620.0f;
	strightBullet2.radius = 10.0f;
	strightBullet2.speed = 5.0f;
	strightBullet2.width = 32.0f;
	strightBullet2.height = 32.0f;
	strightBullet2.isShot = false;
	strightBullet2.isHitX = false;
	strightBullet2.isHitY = false;
	strightBullet2.isHit = false;

	strightBullet2.vertex.leftTop.x = strightBullet2.pos.x;
	strightBullet2.vertex.leftTop.y = strightBullet2.pos.y;

	strightBullet2.vertex.rightTop.x = strightBullet2.pos.x + strightBullet2.width;
	strightBullet2.vertex.rightTop.y = strightBullet2.pos.y;

	strightBullet2.vertex.leftBottom.x = strightBullet2.pos.x;
	strightBullet2.vertex.leftBottom.y = strightBullet2.pos.y + strightBullet2.height;

	strightBullet2.vertex.rightBottom.x = strightBullet2.pos.x + strightBullet2.width;
	strightBullet2.vertex.rightBottom.y = strightBullet2.pos.y + strightBullet2.height;




	PlayerBullet backBullet1{};
	backBullet1.pos.x = 640.0f;
	backBullet1.pos.y = 620.0f;
	backBullet1.radius = 10.0f;
	backBullet1.speed = 5.0f;
	backBullet1.width = 32.0f;
	backBullet1.height = 32.0f;
	backBullet1.isShot = false;
	backBullet1.isHitX = false;
	backBullet1.isHitY = false;
	backBullet1.isHit = false;

	backBullet1.vertex.leftTop.x = backBullet1.pos.x;
	backBullet1.vertex.leftTop.y = backBullet1.pos.y;

	backBullet1.vertex.rightTop.x = backBullet1.pos.x + backBullet1.width;
	backBullet1.vertex.rightTop.y = backBullet1.pos.y;

	backBullet1.vertex.leftBottom.x = backBullet1.pos.x;
	backBullet1.vertex.leftBottom.y = backBullet1.pos.y + backBullet1.height;

	backBullet1.vertex.rightBottom.x = backBullet1.pos.x + backBullet1.width;
	backBullet1.vertex.rightBottom.y = backBullet1.pos.y + backBullet1.height;

	backBullet1.damage = 10;




	PlayerBullet backBullet2{};
	backBullet2.pos.x = 640.0f;
	backBullet2.pos.y = 620.0f;
	backBullet2.radius = 10.0f;
	backBullet2.speed = 5.0f;
	backBullet2.width = 32.0f;
	backBullet2.height = 32.0f;
	backBullet2.isShot = false;
	backBullet2.isHitX = false;
	backBullet2.isHitY = false;
	backBullet2.isHit = false;

	backBullet2.vertex.leftTop.x = backBullet2.pos.x;
	backBullet2.vertex.leftTop.y = backBullet2.pos.y;

	backBullet2.vertex.rightTop.x = backBullet2.pos.x + backBullet2.width;
	backBullet2.vertex.rightTop.y = backBullet2.pos.y;

	backBullet2.vertex.leftBottom.x = backBullet2.pos.x;
	backBullet2.vertex.leftBottom.y = backBullet2.pos.y + backBullet2.height;

	backBullet2.vertex.rightBottom.x = backBullet2.pos.x + backBullet2.width;
	backBullet2.vertex.rightBottom.y = backBullet2.pos.y + backBullet2.height;

	backBullet2.damage = 10;




	PlayerBullet leftBullet1{};
	leftBullet1.pos.x = 640.0f;
	leftBullet1.pos.y = 620.0f;
	leftBullet1.radius = 10.0f;
	leftBullet1.speed = 5.0f;
	leftBullet1.width = 32.0f;
	leftBullet1.height = 32.0f;
	leftBullet1.isShot = false;
	leftBullet1.isHitX = false;
	leftBullet1.isHitY = false;
	leftBullet1.isHit = false;

	leftBullet1.vertex.leftTop.x = leftBullet1.pos.x;
	leftBullet1.vertex.leftTop.y = leftBullet1.pos.y;

	leftBullet1.vertex.rightTop.x = leftBullet1.pos.x + leftBullet1.width;
	leftBullet1.vertex.rightTop.y = leftBullet1.pos.y;

	leftBullet1.vertex.leftBottom.x = leftBullet1.pos.x;
	leftBullet1.vertex.leftBottom.y = leftBullet1.pos.y + leftBullet1.height;

	leftBullet1.vertex.rightBottom.x = leftBullet1.pos.x + leftBullet1.width;
	leftBullet1.vertex.rightBottom.y = leftBullet1.pos.y + leftBullet1.height;

	leftBullet1.damage = 10;






	PlayerBullet leftBullet2{};
	leftBullet2.pos.x = 640.0f;
	leftBullet2.pos.y = 620.0f;
	leftBullet2.radius = 10.0f;
	leftBullet2.speed = 5.0f;
	leftBullet2.width = 32.0f;
	leftBullet2.height = 32.0f;
	leftBullet2.isShot = false;
	leftBullet2.isHitX = false;
	leftBullet2.isHitY = false;
	leftBullet2.isHit = false;

	leftBullet2.vertex.leftTop.x = leftBullet2.pos.x;
	leftBullet2.vertex.leftTop.y = leftBullet2.pos.y;

	leftBullet2.vertex.rightTop.x = leftBullet2.pos.x + leftBullet2.width;
	leftBullet2.vertex.rightTop.y = leftBullet2.pos.y;

	leftBullet2.vertex.leftBottom.x = leftBullet2.pos.x;
	leftBullet2.vertex.leftBottom.y = leftBullet2.pos.y + leftBullet2.height;

	leftBullet2.vertex.rightBottom.x = leftBullet2.pos.x + leftBullet2.width;
	leftBullet2.vertex.rightBottom.y = leftBullet2.pos.y + leftBullet2.height;

	leftBullet2.damage = 10;







	PlayerBullet rightBullet1{};
	rightBullet1.pos.x = 640.0f;
	rightBullet1.pos.y = 620.0f;
	rightBullet1.radius = 10.0f;
	rightBullet1.speed = 5.0f;
	rightBullet1.width = 32.0f;
	rightBullet1.height = 32.0f;
	rightBullet1.isShot = false;
	rightBullet1.isHitX = false;
	rightBullet1.isHitY = false;
	rightBullet1.isHit = false;

	rightBullet1.vertex.leftTop.x = rightBullet1.pos.x;
	rightBullet1.vertex.leftTop.y = rightBullet1.pos.y;

	rightBullet1.vertex.rightTop.x = rightBullet1.pos.x + rightBullet1.width;
	rightBullet1.vertex.rightTop.y = rightBullet1.pos.y;

	rightBullet1.vertex.leftBottom.x = rightBullet1.pos.x;
	rightBullet1.vertex.leftBottom.y = rightBullet1.pos.y + rightBullet1.height;

	rightBullet1.vertex.rightBottom.x = rightBullet1.pos.x + rightBullet1.width;
	rightBullet1.vertex.rightBottom.y = rightBullet1.pos.y + rightBullet1.height;

	rightBullet1.damage = 10;





	PlayerBullet rightBullet2{};
	rightBullet2.pos.x = 640.0f;
	rightBullet2.pos.y = 620.0f;
	rightBullet2.radius = 10.0f;
	rightBullet2.speed = 5.0f;
	rightBullet2.width = 32.0f;
	rightBullet2.height = 32.0f;
	rightBullet2.isShot = false;
	rightBullet2.isHitX = false;
	rightBullet2.isHitY = false;
	rightBullet2.isHit = false;

	rightBullet2.vertex.leftTop.x = rightBullet2.pos.x;
	rightBullet2.vertex.leftTop.y = rightBullet2.pos.y;

	rightBullet2.vertex.rightTop.x = rightBullet2.pos.x + rightBullet2.width;
	rightBullet2.vertex.rightTop.y = rightBullet2.pos.y;

	rightBullet2.vertex.leftBottom.x = rightBullet2.pos.x;
	rightBullet2.vertex.leftBottom.y = rightBullet2.pos.y + rightBullet2.height;

	rightBullet2.vertex.rightBottom.x = rightBullet2.pos.x + rightBullet2.width;
	rightBullet2.vertex.rightBottom.y = rightBullet2.pos.y + rightBullet2.height;

	rightBullet2.damage = 10;

	int rightHandTexture = Novice::LoadTexture("./Resources/images/righthand.png");
	int leftHandTexture = Novice::LoadTexture("./Resources/images/lefthand.png");
	int playerBackTexture = Novice::LoadTexture("./Resources/images/psychic.png");
	int playerFrontTexture = Novice::LoadTexture("./Resources/images/front.png");

#pragma endregion

#pragma region --- 敵の挙動の初期化 ---

#pragma endregion

#pragma region --- 画像の初期化 ---

	//タイトル//
	int TitleHandle = Novice::LoadTexture("./Resources/images/saihand.png");

	//操作説明1
	int Open1Handle = Novice::LoadTexture("./Resources/images/sousa1.png");

	//操作説明2
	int Open2Handle = Novice::LoadTexture("./Resources/images/sousa2.png");

	//プレイ
	int GameHandle = Novice::LoadTexture("./Resources/images/handplay.png");

	//ゲームクリア
	int KuriaHandle = Novice::LoadTexture("./Resources/images/kuria.png");

	//ゲームオーバ
	int OverHandle = Novice::LoadTexture("./Resources/images/gameover.png");

	int numbers[10]{};
	numbers[0] = Novice::LoadTexture("./Resources/images/0.png");
	numbers[1] = Novice::LoadTexture("./Resources/images/1.png");
	numbers[2] = Novice::LoadTexture("./Resources/images/2.png");
	numbers[3] = Novice::LoadTexture("./Resources/images/3.png");
	numbers[4] = Novice::LoadTexture("./Resources/images/4.png");
	numbers[5] = Novice::LoadTexture("./Resources/images/5.png");
	numbers[6] = Novice::LoadTexture("./Resources/images/6.png");
	numbers[7] = Novice::LoadTexture("./Resources/images/7.png");
	numbers[8] = Novice::LoadTexture("./Resources/images/8.png");
	numbers[9] = Novice::LoadTexture("./Resources/images/9.png");

#pragma endregion

	int scene = TITLE;

#pragma endregion

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene) {

		case TITLE:

			///
			/// ↓更新処理ここから
			///

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				scene = OPENE1;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, TitleHandle, 1.0f, 1.0f, 0.0f, WHITE);

			///
			/// ↑描画処理ここまで
			///

			break;

		case OPENE1:

			///
			/// ↓更新処理ここから
			///

			if (!keys[DIK_D] && preKeys[DIK_D]) {
				scene = OPENE2;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, Open1Handle, 1.0f, 1.0f, 0.0f, WHITE);

			///
			/// ↑描画処理ここまで
			///

			break;

		case OPENE2:

			///
			/// ↓更新処理ここから
			///

			if (!keys[DIK_RETURN] && preKeys[DIK_RETURN]) {
				scene = GAME;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, Open2Handle, 1.0f, 1.0f, 0.0f, WHITE);

			///
			/// ↑描画処理ここまで
			///

			break;

		case GAME:

#pragma region UpdateProcessing

			///
			/// ↓更新処理ここから
			///

#pragma region --- 敵の左辺の弾 ---

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
						leftSideEnemyBullet2.start.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

						// 弾の終了位置（左方向への移動を設定）
						leftSideEnemyBullet2.end.x = enemy.pos.x - (leftSideEnemyBullet2.width * 2.0f);
						leftSideEnemyBullet2.end.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

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

#pragma region --- 敵の右辺の弾 ---

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

			// --- 敵2の右辺の弾 ---
			if (!rightSideEnemyBullet2.isBulletShot) {

				rightSideEnemyBullet2.length = 0.0f;
				rightSideEnemyBullet2.t = 0.0f;
				rightSideEnemyBullet2.easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				rightSideEnemyBullet2.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (rightSideEnemyBullet2.length > 0.0f) {
					rightSideEnemyBullet2.velocity.x = (player.dir.x / rightSideEnemyBullet2.length) * rightSideEnemyBullet2.speed;
					rightSideEnemyBullet2.velocity.y = (player.dir.y / rightSideEnemyBullet2.length) * rightSideEnemyBullet2.speed;
				}

				rightSideEnemyBullet2.isBulletShot = true;
			}

			if (rightSideEnemyBullet2.isBulletShot) {

				if (!rightSideEnemyBullet2.interpolation) {

					rightSideEnemyBullet2.t += 0.008f;

					if (rightSideEnemyBullet2.t >= 1.0f) {
						rightSideEnemyBullet2.t = 1.0f;
						rightSideEnemyBullet2.interpolation = true;
					}

					rightSideEnemyBullet2.easedT = 1.0f - powf(1.0f - rightSideEnemyBullet2.t, 3.0f);
					rightSideEnemyBullet2.pos.x = (1.0f - rightSideEnemyBullet2.easedT) * rightSideEnemyBullet2.start.x + rightSideEnemyBullet2.easedT * rightSideEnemyBullet2.end.x;
					rightSideEnemyBullet2.pos.y = (1.0f - rightSideEnemyBullet2.easedT) * rightSideEnemyBullet2.start.y + rightSideEnemyBullet2.easedT * rightSideEnemyBullet2.end.y;

				} else {

					rightSideEnemyBullet2.pos.x += rightSideEnemyBullet2.velocity.x;
					rightSideEnemyBullet2.pos.y += rightSideEnemyBullet2.velocity.y;

					if (rightSideEnemyBullet2.pos.y <= 0.0f ||
						rightSideEnemyBullet2.pos.x <= 0.0f ||
						rightSideEnemyBullet2.pos.x >= 1280.0f ||
						rightSideEnemyBullet2.pos.y + rightSideEnemyBullet2.height >= 720.0f) {

						// 弾の開始座標 (敵の右端から発射)
						rightSideEnemyBullet2.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet2.width;
						rightSideEnemyBullet2.start.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

						// 弾の終了座標 (右方向へ進む)
						rightSideEnemyBullet2.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet2.width);
						rightSideEnemyBullet2.end.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

						// 弾の初期位置を設定
						rightSideEnemyBullet2.pos.x = rightSideEnemyBullet2.start.x;
						rightSideEnemyBullet2.pos.y = rightSideEnemyBullet2.start.y;

						// 弾の現在座標
						rightSideEnemyBullet2.pos.x = rightSideEnemyBullet2.start.x;
						rightSideEnemyBullet2.pos.y = rightSideEnemyBullet2.start.y;
						rightSideEnemyBullet2.isBulletShot = false;
						rightSideEnemyBullet2.interpolation = false;
					}
				}
			}

#pragma endregion

#pragma region --- 敵の上辺の弾 ---

			// 1
			if (!topSideEnemyBullet1.isBulletShot) {

				topSideEnemyBullet1.length = 0.0f;
				topSideEnemyBullet1.t = 0.0f;
				topSideEnemyBullet1.easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				topSideEnemyBullet1.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (topSideEnemyBullet1.length > 0.0f) {
					topSideEnemyBullet1.velocity.x = (player.dir.x / topSideEnemyBullet1.length) * topSideEnemyBullet1.speed;
					topSideEnemyBullet1.velocity.y = (player.dir.y / topSideEnemyBullet1.length) * topSideEnemyBullet1.speed;
				}

				topSideEnemyBullet1.isBulletShot = true;
			}

			if (topSideEnemyBullet1.isBulletShot) {

				if (!topSideEnemyBullet1.interpolation) {

					topSideEnemyBullet1.t += 0.008f;

					if (topSideEnemyBullet1.t >= 1.0f) {
						topSideEnemyBullet1.t = 1.0f;
						topSideEnemyBullet1.interpolation = true;
					}

					topSideEnemyBullet1.easedT = 1.0f - powf(1.0f - topSideEnemyBullet1.t, 3.0f);
					topSideEnemyBullet1.pos.x = (1.0f - topSideEnemyBullet1.easedT) * topSideEnemyBullet1.start.x + topSideEnemyBullet1.easedT * topSideEnemyBullet1.end.x;
					topSideEnemyBullet1.pos.y = (1.0f - topSideEnemyBullet1.easedT) * topSideEnemyBullet1.start.y + topSideEnemyBullet1.easedT * topSideEnemyBullet1.end.y;

				} else {

					topSideEnemyBullet1.pos.x += topSideEnemyBullet1.velocity.x;
					topSideEnemyBullet1.pos.y += topSideEnemyBullet1.velocity.y;

					if (topSideEnemyBullet1.pos.y <= 0.0f ||
						topSideEnemyBullet1.pos.x <= 0.0f ||
						topSideEnemyBullet1.pos.x >= 1280.0f ||
						topSideEnemyBullet1.pos.y + topSideEnemyBullet1.height >= 720.0f) {

						// 弾の開始座標 (敵の上端から発射)
						topSideEnemyBullet1.start.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
						topSideEnemyBullet1.start.y = enemy.pos.y;

						// 弾の終了座標 (上方向へ進む)
						topSideEnemyBullet1.end.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
						topSideEnemyBullet1.end.y = enemy.pos.y - (topSideEnemyBullet1.height * 2.0f);

						// 弾の現在座標
						topSideEnemyBullet1.pos.x = topSideEnemyBullet1.start.x;
						topSideEnemyBullet1.pos.y = topSideEnemyBullet1.start.y;

						topSideEnemyBullet1.isBulletShot = false;
						topSideEnemyBullet1.interpolation = false;
					}
				}
			}

			// 2
			if (!topSideEnemyBullet2.isBulletShot) {

				topSideEnemyBullet2.length = 0.0f;
				topSideEnemyBullet2.t = 0.0f;
				topSideEnemyBullet2.easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				topSideEnemyBullet2.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (topSideEnemyBullet2.length > 0.0f) {
					topSideEnemyBullet2.velocity.x = (player.dir.x / topSideEnemyBullet2.length) * topSideEnemyBullet2.speed;
					topSideEnemyBullet2.velocity.y = (player.dir.y / topSideEnemyBullet2.length) * topSideEnemyBullet2.speed;
				}

				topSideEnemyBullet2.isBulletShot = true;
			}

			if (topSideEnemyBullet2.isBulletShot) {

				if (!topSideEnemyBullet2.interpolation) {

					topSideEnemyBullet2.t += 0.008f;

					if (topSideEnemyBullet2.t >= 1.0f) {
						topSideEnemyBullet2.t = 1.0f;
						topSideEnemyBullet2.interpolation = true;
					}

					topSideEnemyBullet2.easedT = 1.0f - powf(1.0f - topSideEnemyBullet2.t, 3.0f);
					topSideEnemyBullet2.pos.x = (1.0f - topSideEnemyBullet2.easedT) * topSideEnemyBullet2.start.x + topSideEnemyBullet2.easedT * topSideEnemyBullet2.end.x;
					topSideEnemyBullet2.pos.y = (1.0f - topSideEnemyBullet2.easedT) * topSideEnemyBullet2.start.y + topSideEnemyBullet2.easedT * topSideEnemyBullet2.end.y;

				} else {

					topSideEnemyBullet2.pos.x += topSideEnemyBullet2.velocity.x;
					topSideEnemyBullet2.pos.y += topSideEnemyBullet2.velocity.y;

					if (topSideEnemyBullet2.pos.y <= 0.0f ||
						topSideEnemyBullet2.pos.x <= 0.0f ||
						topSideEnemyBullet2.pos.x >= 1280.0f ||
						topSideEnemyBullet2.pos.y + topSideEnemyBullet2.height >= 720.0f) {

						// 弾の開始座標 (敵の上端から発射)
						topSideEnemyBullet2.start.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
						topSideEnemyBullet2.start.y = enemy.pos.y;

						// 弾の終了座標 (上方向へ進む)
						topSideEnemyBullet2.end.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
						topSideEnemyBullet2.end.y = enemy.pos.y - (topSideEnemyBullet2.height * 2.0f);

						// 弾の初期位置を設定
						topSideEnemyBullet2.pos.x = topSideEnemyBullet2.start.x;
						topSideEnemyBullet2.pos.y = topSideEnemyBullet2.start.y;

						topSideEnemyBullet2.isBulletShot = false;
						topSideEnemyBullet2.interpolation = false;
					}
				}
			}

#pragma endregion

#pragma region --- 敵の下辺の弾 ---

			// 1
			if (!bottomSideEnemyBullet1.isBulletShot) {

				bottomSideEnemyBullet1.length = 0.0f;
				bottomSideEnemyBullet1.t = 0.0f;
				bottomSideEnemyBullet1.easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				bottomSideEnemyBullet1.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (bottomSideEnemyBullet1.length > 0.0f) {
					bottomSideEnemyBullet1.velocity.x = (player.dir.x / bottomSideEnemyBullet1.length) * bottomSideEnemyBullet1.speed;
					bottomSideEnemyBullet1.velocity.y = (player.dir.y / bottomSideEnemyBullet1.length) * bottomSideEnemyBullet1.speed;
				}

				bottomSideEnemyBullet1.isBulletShot = true;
			}

			if (bottomSideEnemyBullet1.isBulletShot) {

				if (!bottomSideEnemyBullet1.interpolation) {

					bottomSideEnemyBullet1.t += 0.008f;

					if (bottomSideEnemyBullet1.t >= 1.0f) {
						bottomSideEnemyBullet1.t = 1.0f;
						bottomSideEnemyBullet1.interpolation = true;
					}

					bottomSideEnemyBullet1.easedT = 1.0f - powf(1.0f - bottomSideEnemyBullet1.t, 3.0f);
					bottomSideEnemyBullet1.pos.x = (1.0f - bottomSideEnemyBullet1.easedT) * bottomSideEnemyBullet1.start.x + bottomSideEnemyBullet1.easedT * bottomSideEnemyBullet1.end.x;
					bottomSideEnemyBullet1.pos.y = (1.0f - bottomSideEnemyBullet1.easedT) * bottomSideEnemyBullet1.start.y + bottomSideEnemyBullet1.easedT * bottomSideEnemyBullet1.end.y;

				} else {

					bottomSideEnemyBullet1.pos.x += bottomSideEnemyBullet1.velocity.x;
					bottomSideEnemyBullet1.pos.y += bottomSideEnemyBullet1.velocity.y;

					if (bottomSideEnemyBullet1.pos.y <= 0.0f ||
						bottomSideEnemyBullet1.pos.x <= 0.0f ||
						bottomSideEnemyBullet1.pos.x >= 1280.0f ||
						bottomSideEnemyBullet1.pos.y + bottomSideEnemyBullet1.height >= 720.0f) {

						// 弾の開始座標 (敵の下端から発射)
						bottomSideEnemyBullet1.start.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
						bottomSideEnemyBullet1.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet1.height;

						// 弾の終了座標 (下方向へ進む)
						bottomSideEnemyBullet1.end.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
						bottomSideEnemyBullet1.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet1.height;

						// 弾の現在座標
						bottomSideEnemyBullet1.pos.x = bottomSideEnemyBullet1.start.x;
						bottomSideEnemyBullet1.pos.y = bottomSideEnemyBullet1.start.y;

						bottomSideEnemyBullet1.isBulletShot = false;
						bottomSideEnemyBullet1.interpolation = false;
					}
				}
			}

			// 2
			if (!bottomSideEnemyBullet2.isBulletShot) {

				bottomSideEnemyBullet2.length = 0.0f;
				bottomSideEnemyBullet2.t = 0.0f;
				bottomSideEnemyBullet2.easedT = 0.0f;

				player.dir.x = player.pos.x - enemy.pos.x;
				player.dir.y = player.pos.y - enemy.pos.y;

				bottomSideEnemyBullet2.length = sqrtf(player.dir.x * player.dir.x + player.dir.y * player.dir.y);
				if (bottomSideEnemyBullet2.length > 0.0f) {
					bottomSideEnemyBullet2.velocity.x = (player.dir.x / bottomSideEnemyBullet2.length) * bottomSideEnemyBullet2.speed;
					bottomSideEnemyBullet2.velocity.y = (player.dir.y / bottomSideEnemyBullet2.length) * bottomSideEnemyBullet2.speed;
				}

				bottomSideEnemyBullet2.isBulletShot = true;
			}

			if (bottomSideEnemyBullet2.isBulletShot) {

				if (!bottomSideEnemyBullet2.interpolation) {

					bottomSideEnemyBullet2.t += 0.008f;

					if (bottomSideEnemyBullet2.t >= 1.0f) {
						bottomSideEnemyBullet2.t = 1.0f;
						bottomSideEnemyBullet2.interpolation = true;
					}

					bottomSideEnemyBullet2.easedT = 1.0f - powf(1.0f - bottomSideEnemyBullet2.t, 3.0f);
					bottomSideEnemyBullet2.pos.x = (1.0f - bottomSideEnemyBullet2.easedT) * bottomSideEnemyBullet2.start.x + bottomSideEnemyBullet2.easedT * bottomSideEnemyBullet2.end.x;
					bottomSideEnemyBullet2.pos.y = (1.0f - bottomSideEnemyBullet2.easedT) * bottomSideEnemyBullet2.start.y + bottomSideEnemyBullet2.easedT * bottomSideEnemyBullet2.end.y;

				} else {

					bottomSideEnemyBullet2.pos.x += bottomSideEnemyBullet2.velocity.x;
					bottomSideEnemyBullet2.pos.y += bottomSideEnemyBullet2.velocity.y;

					if (bottomSideEnemyBullet2.pos.y <= 0.0f ||
						bottomSideEnemyBullet2.pos.x <= 0.0f ||
						bottomSideEnemyBullet2.pos.x >= 1280.0f ||
						bottomSideEnemyBullet2.pos.y + bottomSideEnemyBullet2.height >= 720.0f) {

						// 弾の開始座標 (敵の下端から発射)
						bottomSideEnemyBullet2.start.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
						bottomSideEnemyBullet2.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet2.height;

						// 弾の終了座標 (下方向へ進む)
						bottomSideEnemyBullet2.end.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
						bottomSideEnemyBullet2.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet2.height;

						// 弾の初期位置を設定
						bottomSideEnemyBullet2.pos.x = bottomSideEnemyBullet2.start.x;
						bottomSideEnemyBullet2.pos.y = bottomSideEnemyBullet2.start.y;

						bottomSideEnemyBullet2.isBulletShot = false;
						bottomSideEnemyBullet2.interpolation = false;
					}
				}
			}

#pragma endregion

#pragma region --- 敵の左上の弾 ---


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

#pragma region --- 敵の左下の弾 ---

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

#pragma endregion

#pragma region --- 敵の右下の弾 ---

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

#pragma endregion

#pragma region --- 敵の挙動 ---

#pragma endregion

#pragma region --- 値更新 ---

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

			// 左　敵弾1
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

			// 左　敵弾2
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



			// 右辺 弾1
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


			// 右辺 弾2
			// 左上
			rightSideEnemyBullet2.vertex.leftTop.x = rightSideEnemyBullet2.pos.x;
			rightSideEnemyBullet2.vertex.leftTop.y = rightSideEnemyBullet2.pos.y;

			// 右上
			rightSideEnemyBullet2.vertex.rightTop.x = rightSideEnemyBullet2.pos.x + rightSideEnemyBullet2.width;
			rightSideEnemyBullet2.vertex.rightTop.y = rightSideEnemyBullet2.pos.y;

			// 左下
			rightSideEnemyBullet2.vertex.leftBottom.x = rightSideEnemyBullet2.pos.x;
			rightSideEnemyBullet2.vertex.leftBottom.y = rightSideEnemyBullet2.pos.y + rightSideEnemyBullet2.height;

			// 右下
			rightSideEnemyBullet2.vertex.rightBottom.x = rightSideEnemyBullet2.pos.x + rightSideEnemyBullet2.width;
			rightSideEnemyBullet2.vertex.rightBottom.y = rightSideEnemyBullet2.pos.y + rightSideEnemyBullet2.height;



			// 上辺 弾1
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

			// 上辺 弾2
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



			// 下辺 弾1
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

			// 下辺 弾2
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

			// strightBullet1
			// 左上
			strightBullet1.vertex.leftTop.x = strightBullet1.pos.x;
			strightBullet1.vertex.leftTop.y = strightBullet1.pos.y;

			// 右上
			strightBullet1.vertex.rightTop.x = strightBullet1.pos.x + strightBullet1.width;
			strightBullet1.vertex.rightTop.y = strightBullet1.pos.y;

			// 左下
			strightBullet1.vertex.leftBottom.x = strightBullet1.pos.x;
			strightBullet1.vertex.leftBottom.y = strightBullet1.pos.y + strightBullet1.height;

			// 右下
			strightBullet1.vertex.rightBottom.x = strightBullet1.pos.x + strightBullet1.width;
			strightBullet1.vertex.rightBottom.y = strightBullet1.pos.y + strightBullet1.height;

			// strightBullet2
			// 左上
			strightBullet2.vertex.leftTop.x = strightBullet2.pos.x;
			strightBullet2.vertex.leftTop.y = strightBullet2.pos.y;

			// 右上
			strightBullet2.vertex.rightTop.x = strightBullet2.pos.x + strightBullet2.width;
			strightBullet2.vertex.rightTop.y = strightBullet2.pos.y;

			// 左下
			strightBullet2.vertex.leftBottom.x = strightBullet2.pos.x;
			strightBullet2.vertex.leftBottom.y = strightBullet2.pos.y + strightBullet2.height;

			// 右下
			strightBullet2.vertex.rightBottom.x = strightBullet2.pos.x + strightBullet2.width;
			strightBullet2.vertex.rightBottom.y = strightBullet2.pos.y + strightBullet2.height;



			// backBullet1
			// 左上
			backBullet1.vertex.leftTop.x = backBullet1.pos.x;
			backBullet1.vertex.leftTop.y = backBullet1.pos.y;

			// 右上
			backBullet1.vertex.rightTop.x = backBullet1.pos.x + backBullet1.width;
			backBullet1.vertex.rightTop.y = backBullet1.pos.y;

			// 左下
			backBullet1.vertex.leftBottom.x = backBullet1.pos.x;
			backBullet1.vertex.leftBottom.y = backBullet1.pos.y + backBullet1.height;

			// 右下
			backBullet1.vertex.rightBottom.x = backBullet1.pos.x + backBullet1.width;
			backBullet1.vertex.rightBottom.y = backBullet1.pos.y + backBullet1.height;

			// backBullet2
			// 左上
			backBullet2.vertex.leftTop.x = backBullet2.pos.x;
			backBullet2.vertex.leftTop.y = backBullet2.pos.y;

			// 右上
			backBullet2.vertex.rightTop.x = backBullet2.pos.x + backBullet2.width;
			backBullet2.vertex.rightTop.y = backBullet2.pos.y;

			// 左下
			backBullet2.vertex.leftBottom.x = backBullet2.pos.x;
			backBullet2.vertex.leftBottom.y = backBullet2.pos.y + backBullet2.height;

			// 右下
			backBullet2.vertex.rightBottom.x = backBullet2.pos.x + backBullet2.width;
			backBullet2.vertex.rightBottom.y = backBullet2.pos.y + backBullet2.height;



			// leftBullet1
			// 左上
			leftBullet1.vertex.leftTop.x = leftBullet1.pos.x;
			leftBullet1.vertex.leftTop.y = leftBullet1.pos.y;

			// 右上
			leftBullet1.vertex.rightTop.x = leftBullet1.pos.x + leftBullet1.width;
			leftBullet1.vertex.rightTop.y = leftBullet1.pos.y;

			// 左下
			leftBullet1.vertex.leftBottom.x = leftBullet1.pos.x;
			leftBullet1.vertex.leftBottom.y = leftBullet1.pos.y + leftBullet1.height;

			// 右下
			leftBullet1.vertex.rightBottom.x = leftBullet1.pos.x + leftBullet1.width;
			leftBullet1.vertex.rightBottom.y = leftBullet1.pos.y + leftBullet1.height;

			// leftBullet2
			// 左上
			leftBullet2.vertex.leftTop.x = leftBullet2.pos.x;
			leftBullet2.vertex.leftTop.y = leftBullet2.pos.y;

			// 右上
			leftBullet2.vertex.rightTop.x = leftBullet2.pos.x + leftBullet2.width;
			leftBullet2.vertex.rightTop.y = leftBullet2.pos.y;

			// 左下
			leftBullet2.vertex.leftBottom.x = leftBullet2.pos.x;
			leftBullet2.vertex.leftBottom.y = leftBullet2.pos.y + leftBullet2.height;

			// 右下
			leftBullet2.vertex.rightBottom.x = leftBullet2.pos.x + leftBullet2.width;
			leftBullet2.vertex.rightBottom.y = leftBullet2.pos.y + leftBullet2.height;


			// rightBullet1
			// 左上
			rightBullet1.vertex.leftTop.x = rightBullet1.pos.x;
			rightBullet1.vertex.leftTop.y = rightBullet1.pos.y;

			// 右上
			rightBullet1.vertex.rightTop.x = rightBullet1.pos.x + rightBullet1.width;
			rightBullet1.vertex.rightTop.y = rightBullet1.pos.y;

			// 左下
			rightBullet1.vertex.leftBottom.x = rightBullet1.pos.x;
			rightBullet1.vertex.leftBottom.y = rightBullet1.pos.y + rightBullet1.height;

			// 右下
			rightBullet1.vertex.rightBottom.x = rightBullet1.pos.x + rightBullet1.width;
			rightBullet1.vertex.rightBottom.y = rightBullet1.pos.y + rightBullet1.height;

			// rightBullet2
			// 左上
			rightBullet2.vertex.leftTop.x = rightBullet2.pos.x;
			rightBullet2.vertex.leftTop.y = rightBullet2.pos.y;

			// 右上
			rightBullet2.vertex.rightTop.x = rightBullet2.pos.x + rightBullet2.width;
			rightBullet2.vertex.rightTop.y = rightBullet2.pos.y;

			// 左下
			rightBullet2.vertex.leftBottom.x = rightBullet2.pos.x;
			rightBullet2.vertex.leftBottom.y = rightBullet2.pos.y + rightBullet2.height;

			// 右下
			rightBullet2.vertex.rightBottom.x = rightBullet2.pos.x + rightBullet2.width;
			rightBullet2.vertex.rightBottom.y = rightBullet2.pos.y + rightBullet2.height;

#pragma endregion

#pragma region --- 敵弾と自機の当たり判定 ---

			// 敵弾と自機の当たり判定
			// 左 弾1
			if ((player.vertex.leftTop.x) < (leftSideEnemyBullet1.vertex.rightBottom.x) && (leftSideEnemyBullet1.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				leftSideEnemyBullet1.isHitX = true;
			} else {
				leftSideEnemyBullet1.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (leftSideEnemyBullet1.vertex.rightBottom.y) && (leftSideEnemyBullet1.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				leftSideEnemyBullet1.isHitY = true;
			} else {
				leftSideEnemyBullet1.isHitY = false;
			}

			if (leftSideEnemyBullet1.isHitX && leftSideEnemyBullet1.isHitY) {

				leftSideEnemyBullet1.isHit = true;

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

			} else {
				leftSideEnemyBullet1.isHit = false;
			}


			// 左 弾2
			if ((player.vertex.leftTop.x) < (leftSideEnemyBullet2.vertex.rightBottom.x) && (leftSideEnemyBullet2.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				leftSideEnemyBullet2.isHitX = true;
			} else {
				leftSideEnemyBullet2.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (leftSideEnemyBullet2.vertex.rightBottom.y) && (leftSideEnemyBullet2.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				leftSideEnemyBullet2.isHitY = true;
			} else {
				leftSideEnemyBullet2.isHitY = false;
			}

			if (leftSideEnemyBullet2.isHitX && leftSideEnemyBullet2.isHitY) {

				leftSideEnemyBullet2.isHit = true;

				// 弾の開始位置（敵の左辺）
				leftSideEnemyBullet2.start.x = enemy.pos.x;
				leftSideEnemyBullet2.start.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

				// 弾の終了位置（左方向への移動を設定）
				leftSideEnemyBullet2.end.x = enemy.pos.x - (leftSideEnemyBullet2.width * 2.0f);
				leftSideEnemyBullet2.end.y = enemy.pos.y + (leftSideEnemyBullet2.height * 2.8f);

				// 弾の初期位置を設定
				leftSideEnemyBullet2.pos.x = leftSideEnemyBullet2.start.x;
				leftSideEnemyBullet2.pos.y = leftSideEnemyBullet2.start.y;



				leftSideEnemyBullet2.isBulletShot = false;
				leftSideEnemyBullet2.interpolation = false;

			} else {
				leftSideEnemyBullet2.isHit = false;
			}

			// 右 弾1
			if ((player.vertex.leftTop.x) < (rightSideEnemyBullet1.vertex.rightBottom.x) && (rightSideEnemyBullet1.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				rightSideEnemyBullet1.isHitX = true;
			} else {
				rightSideEnemyBullet1.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (rightSideEnemyBullet1.vertex.rightBottom.y) && (rightSideEnemyBullet1.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				rightSideEnemyBullet1.isHitY = true;
			} else {
				rightSideEnemyBullet1.isHitY = false;
			}

			if (rightSideEnemyBullet1.isHitX && rightSideEnemyBullet1.isHitY) {

				rightSideEnemyBullet1.isHit = true;

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
			} else {
				rightSideEnemyBullet1.isHit = false;
			}

			// 右 弾2
			if ((player.vertex.leftTop.x) < (rightSideEnemyBullet2.vertex.rightBottom.x) && (rightSideEnemyBullet2.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				rightSideEnemyBullet2.isHitX = true;
			} else {
				rightSideEnemyBullet2.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (rightSideEnemyBullet2.vertex.rightBottom.y) && (rightSideEnemyBullet2.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				rightSideEnemyBullet2.isHitY = true;
			} else {
				rightSideEnemyBullet2.isHitY = false;
			}

			if (rightSideEnemyBullet2.isHitX && rightSideEnemyBullet2.isHitY) {

				rightSideEnemyBullet2.isHit = true;

				// 弾の開始座標 (敵の右端から発射)
				rightSideEnemyBullet2.start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet2.width;
				rightSideEnemyBullet2.start.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

				// 弾の終了座標 (右方向へ進む)
				rightSideEnemyBullet2.end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet2.width);
				rightSideEnemyBullet2.end.y = enemy.pos.y + (rightSideEnemyBullet2.height * 2.8f);

				// 弾の初期位置を設定
				rightSideEnemyBullet2.pos.x = rightSideEnemyBullet2.start.x;
				rightSideEnemyBullet2.pos.y = rightSideEnemyBullet2.start.y;



				rightSideEnemyBullet2.isBulletShot = false;
				rightSideEnemyBullet2.interpolation = false;
			} else {
				rightSideEnemyBullet2.isHit = false;
			}

			// 上 弾1
			if ((player.vertex.leftTop.x) < (topSideEnemyBullet1.vertex.rightBottom.x) && (topSideEnemyBullet1.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				topSideEnemyBullet1.isHitX = true;
			} else {
				topSideEnemyBullet1.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (topSideEnemyBullet1.vertex.rightBottom.y) && (topSideEnemyBullet1.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				topSideEnemyBullet1.isHitY = true;
			} else {
				topSideEnemyBullet1.isHitY = false;
			}

			if (topSideEnemyBullet1.isHitX && topSideEnemyBullet1.isHitY) {

				topSideEnemyBullet1.isHit = true;

				// 弾の開始座標 (敵の上端から発射)
				topSideEnemyBullet1.start.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
				topSideEnemyBullet1.start.y = enemy.pos.y;

				// 弾の終了座標 (上方向へ進む)
				topSideEnemyBullet1.end.x = enemy.pos.x + (topSideEnemyBullet1.width * 2.6f);
				topSideEnemyBullet1.end.y = enemy.pos.y - (topSideEnemyBullet1.height * 2.0f);

				// 弾の現在座標
				topSideEnemyBullet1.pos.x = topSideEnemyBullet1.start.x;
				topSideEnemyBullet1.pos.y = topSideEnemyBullet1.start.y;



				topSideEnemyBullet1.isBulletShot = false;
				topSideEnemyBullet1.interpolation = false;

			} else {
				topSideEnemyBullet1.isHit = false;
			}

			// 上 弾2
			if ((player.vertex.leftTop.x) < (topSideEnemyBullet2.vertex.rightBottom.x) && (topSideEnemyBullet2.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				topSideEnemyBullet2.isHitX = true;
			} else {
				topSideEnemyBullet2.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (topSideEnemyBullet2.vertex.rightBottom.y) && (topSideEnemyBullet2.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				topSideEnemyBullet2.isHitY = true;
			} else {
				topSideEnemyBullet2.isHitY = false;
			}

			if (topSideEnemyBullet2.isHitX && topSideEnemyBullet2.isHitY) {

				topSideEnemyBullet2.isHit = true;

				// 弾の開始座標 (敵の上端から発射)
				topSideEnemyBullet2.start.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
				topSideEnemyBullet2.start.y = enemy.pos.y;

				// 弾の終了座標 (上方向へ進む)
				topSideEnemyBullet2.end.x = enemy.pos.x + (topSideEnemyBullet2.width * 6.4f);
				topSideEnemyBullet2.end.y = enemy.pos.y - (topSideEnemyBullet2.height * 2.0f);

				// 弾の初期位置を設定
				topSideEnemyBullet2.pos.x = topSideEnemyBullet2.start.x;
				topSideEnemyBullet2.pos.y = topSideEnemyBullet2.start.y;



				topSideEnemyBullet2.isBulletShot = false;
				topSideEnemyBullet2.interpolation = false;

			} else {
				topSideEnemyBullet2.isHit = false;
			}

			// 下 弾1
			if ((player.vertex.leftTop.x) < (bottomSideEnemyBullet1.vertex.rightBottom.x) && (bottomSideEnemyBullet1.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				bottomSideEnemyBullet1.isHitX = true;
			} else {
				bottomSideEnemyBullet1.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (bottomSideEnemyBullet1.vertex.rightBottom.y) && (bottomSideEnemyBullet1.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				bottomSideEnemyBullet1.isHitY = true;
			} else {
				bottomSideEnemyBullet1.isHitY = false;
			}

			if (bottomSideEnemyBullet1.isHitX && bottomSideEnemyBullet1.isHitY) {

				bottomSideEnemyBullet1.isHit = true;

				// 弾の開始座標 (敵の下端から発射)
				bottomSideEnemyBullet1.start.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
				bottomSideEnemyBullet1.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet1.height;

				// 弾の終了座標 (下方向へ進む)
				bottomSideEnemyBullet1.end.x = enemy.pos.x + (bottomSideEnemyBullet1.width * 2.6f);
				bottomSideEnemyBullet1.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet1.height;

				// 弾の現在座標
				bottomSideEnemyBullet1.pos.x = bottomSideEnemyBullet1.start.x;
				bottomSideEnemyBullet1.pos.y = bottomSideEnemyBullet1.start.y;



				bottomSideEnemyBullet1.isBulletShot = false;
				bottomSideEnemyBullet1.interpolation = false;
			} else {
				bottomSideEnemyBullet1.isHit = false;
			}

			// 下 弾2
			if ((player.vertex.leftTop.x) < (bottomSideEnemyBullet2.vertex.rightBottom.x) && (bottomSideEnemyBullet2.vertex.leftTop.x) < (player.vertex.rightBottom.x)) {
				bottomSideEnemyBullet2.isHitX = true;
			} else {
				bottomSideEnemyBullet2.isHitX = false;
			}

			if ((player.vertex.leftTop.y) < (bottomSideEnemyBullet2.vertex.rightBottom.y) && (bottomSideEnemyBullet2.vertex.leftTop.y) < (player.vertex.rightBottom.y)) {
				bottomSideEnemyBullet2.isHitY = true;
			} else {
				bottomSideEnemyBullet2.isHitY = false;
			}

			if (bottomSideEnemyBullet2.isHitX && bottomSideEnemyBullet2.isHitY) {

				bottomSideEnemyBullet2.isHit = true;

				// 弾の開始座標 (敵の下端から発射)
				bottomSideEnemyBullet2.start.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
				bottomSideEnemyBullet2.start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet2.height;

				// 弾の終了座標 (下方向へ進む)
				bottomSideEnemyBullet2.end.x = enemy.pos.x + (bottomSideEnemyBullet2.width * 6.4f);
				bottomSideEnemyBullet2.end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet2.height;

				// 弾の初期位置を設定
				bottomSideEnemyBullet2.pos.x = bottomSideEnemyBullet2.start.x;
				bottomSideEnemyBullet2.pos.y = bottomSideEnemyBullet2.start.y;



				bottomSideEnemyBullet2.isBulletShot = false;
				bottomSideEnemyBullet2.interpolation = false;
			} else {
				bottomSideEnemyBullet2.isHit = false;
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

#pragma endregion

#pragma region --- 自機弾と敵の当たり判定 ---

			// strightBullet1
			if ((enemy.vertex.leftTop.x) < (strightBullet1.vertex.rightBottom.x) && (strightBullet1.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				strightBullet1.isHitX = true;
			} else {
				strightBullet1.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (strightBullet1.vertex.rightBottom.y) && (strightBullet1.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				strightBullet1.isHitY = true;
			} else {
				strightBullet1.isHitY = false;
			}

			if (strightBullet1.isHitX && strightBullet1.isHitY) {

				strightBullet1.isHit = true;

				player.leftBulletCount += 1;

				strightBullet1.isShot = false;

			} else {
				strightBullet1.isHit = false;
			}

			// strightBullet2
			if ((enemy.vertex.leftTop.x) < (strightBullet2.vertex.rightBottom.x) && (strightBullet2.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				strightBullet2.isHitX = true;
			} else {
				strightBullet2.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (strightBullet2.vertex.rightBottom.y) && (strightBullet2.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				strightBullet2.isHitY = true;
			} else {
				strightBullet2.isHitY = false;
			}

			if (strightBullet2.isHitX && strightBullet2.isHitY) {

				strightBullet2.isHit = true;

				player.rightBulletCount += 1;

				strightBullet2.isShot = false;

			} else {
				strightBullet2.isHit = false;
			}

			// backBullet1
			if ((enemy.vertex.leftTop.x) < (backBullet1.vertex.rightBottom.x) && (backBullet1.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				backBullet1.isHitX = true;
			} else {
				backBullet1.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (backBullet1.vertex.rightBottom.y) && (backBullet1.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				backBullet1.isHitY = true;
			} else {
				backBullet1.isHitY = false;
			}

			if (backBullet1.isHitX && backBullet1.isHitY) {

				backBullet1.isHit = true;

				player.leftBulletCount += 1;

				backBullet1.isShot = false;

			} else {
				backBullet1.isHit = false;
			}

			// backBullet2
			if ((enemy.vertex.leftTop.x) < (backBullet2.vertex.rightBottom.x) && (backBullet2.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				backBullet2.isHitX = true;
			} else {
				backBullet2.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (backBullet2.vertex.rightBottom.y) && (backBullet2.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				backBullet2.isHitY = true;
			} else {
				backBullet2.isHitY = false;
			}

			if (backBullet2.isHitX && backBullet2.isHitY) {

				backBullet2.isHit = true;

				player.rightBulletCount += 1;

				backBullet2.isShot = false;

			} else {
				backBullet2.isHit = false;
			}

			// leftBullet1
			if ((enemy.vertex.leftTop.x) < (leftBullet1.vertex.rightBottom.x) && (leftBullet1.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				leftBullet1.isHitX = true;
			} else {
				leftBullet1.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (leftBullet1.vertex.rightBottom.y) && (leftBullet1.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				leftBullet1.isHitY = true;
			} else {
				leftBullet1.isHitY = false;
			}

			if (leftBullet1.isHitX && leftBullet1.isHitY) {

				leftBullet1.isHit = true;

				player.leftBulletCount += 1;

				leftBullet1.isShot = false;

			} else {
				leftBullet1.isHit = false;
			}

			// leftBullet2
			if ((enemy.vertex.leftTop.x) < (leftBullet2.vertex.rightBottom.x) && (leftBullet2.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				leftBullet2.isHitX = true;
			} else {
				leftBullet2.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (leftBullet2.vertex.rightBottom.y) && (leftBullet2.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				leftBullet2.isHitY = true;
			} else {
				leftBullet2.isHitY = false;
			}

			if (leftBullet2.isHitX && leftBullet2.isHitY) {

				leftBullet2.isHit = true;

				player.rightBulletCount += 1;

				leftBullet2.isShot = false;

			} else {
				leftBullet2.isHit = false;
			}

			// rightBullet1
			if ((enemy.vertex.leftTop.x) < (rightBullet1.vertex.rightBottom.x) && (rightBullet1.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				rightBullet1.isHitX = true;
			} else {
				rightBullet1.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (rightBullet1.vertex.rightBottom.y) && (rightBullet1.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				rightBullet1.isHitY = true;
			} else {
				rightBullet1.isHitY = false;
			}

			if (rightBullet1.isHitX && rightBullet1.isHitY) {

				rightBullet1.isHit = true;

				player.leftBulletCount += 1;

				rightBullet1.isShot = false;

			} else {
				rightBullet1.isHit = false;
			}

			// rightBullet2
			if ((enemy.vertex.leftTop.x) < (rightBullet2.vertex.rightBottom.x) && (rightBullet2.vertex.leftTop.x) < (enemy.vertex.rightBottom.x)) {
				rightBullet2.isHitX = true;
			} else {
				rightBullet2.isHitX = false;
			}

			if ((enemy.vertex.leftTop.y) < (rightBullet2.vertex.rightBottom.y) && (rightBullet2.vertex.leftTop.y) < (enemy.vertex.rightBottom.y)) {
				rightBullet2.isHitY = true;
			} else {
				rightBullet2.isHitY = false;
			}

			if (rightBullet2.isHitX && rightBullet2.isHitY) {

				rightBullet2.isHit = true;

				player.rightBulletCount += 1;

				rightBullet2.isShot = false;

			} else {
				rightBullet2.isHit = false;
			}

#pragma endregion

#pragma region --- プレイヤーの移動と弾 ---

			if (keys[DIK_W])
			{
				player.direction = 0;
				player.pos.y -= player.speed;
				player.front = false;
				player.back = true;
			}

			if (keys[DIK_S])
			{
				player.direction = 1;
				player.pos.y += player.speed;
				player.front = true;
				player.back = false;
			}

			if (keys[DIK_A])
			{
				player.direction = 2;
				player.pos.x -= player.speed;
			}

			if (keys[DIK_D])
			{
				player.direction = 3;
				player.pos.x += player.speed;
			}


			//玉の属性の切り替え
			if (keys[DIK_Q])
			{
				player.type = 1;
			}
			//玉の属性の切り替え
			if (keys[DIK_E])
			{
				player.type = 2;
			}
			if (keys[DIK_X])
			{
				player.type = 0;
			}


			if (strightBullet1.isShot == false)
			{
				if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
				{

					//スペースを押したときのplayerbullet1の動き
					if (player.type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (player.direction == 0)
							{
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
				if (player.type == 2)
				{
					strightBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (player.direction == 0)
						{
							strightBullet2.pos.x = player.pos.x + player.radius;
							strightBullet2.pos.y = player.pos.y;
							strightBullet2.isShot = true;

						}
					}
				}

			}

			if (player.leftBulletCount >= 10 && player.rightBulletCount >= 10)
			{
				if (strightBullet1.isShot == false || strightBullet2.isShot == false)
				{
					if (player.type == 0)
					{
						strightBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (player.direction == 0)
							{
								player.hp -= 10;
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
					if (player.type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (player.direction == 1)
							{
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
				if (player.type == 2)
				{
					backBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (player.direction == 1)
						{
							backBullet2.pos.x = player.pos.x + player.radius;
							backBullet2.pos.y = player.pos.y;
							backBullet2.isShot = true;
						}
					}
				}

			}

			if (player.leftBulletCount >= 10 && player.rightBulletCount >= 10)
			{
				if (backBullet1.isShot == false || backBullet2.isShot == false)
				{
					if (player.type == 0)
					{
						backBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (player.direction == 1)
							{
								player.hp -= 10;
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
					if (player.type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (player.direction == 2)
							{
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
				if (player.type == 2)
				{
					leftBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (player.direction == 2)
						{
							leftBullet2.pos.x = player.pos.x + player.radius;
							leftBullet2.pos.y = player.pos.y;
							leftBullet2.isShot = true;
						}
					}
				}

			}

			if (player.leftBulletCount >= 10 && player.rightBulletCount >= 10)
			{
				if (leftBullet1.isShot == false || leftBullet2.isShot == false)
				{
					if (player.type == 0)
					{
						leftBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (player.direction == 2)
							{
								player.hp -= 10;
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

				if (leftBullet1.pos.x < 0)
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
					if (player.type == 1)
					{
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{
							if (player.direction == 3)
							{
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
				if (player.type == 2)
				{
					rightBullet2.speed = 3;
					if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
					{
						if (player.direction == 3)
						{
							rightBullet2.pos.x = player.pos.x + player.radius;
							rightBullet2.pos.y = player.pos.y;
							rightBullet2.isShot = true;
						}
					}
				}

			}

			if (player.leftBulletCount >= 10 && player.rightBulletCount >= 10)
			{
				if (rightBullet1.isShot == false || rightBullet2.isShot == false)
				{
					if (player.type == 0)
					{
						rightBullet2.speed = 10;
						if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
						{

							if (player.direction == 3)
							{
								player.hp -= 10;
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

				if (rightBullet1.pos.x > 1280)
				{
					rightBullet1.isShot = false;
					rightBullet1.pos.y = player.pos.x;
				}
			}
			if (rightBullet2.isShot == 1)
			{
				rightBullet2.pos.x += rightBullet2.speed;

				if (rightBullet2.pos.x > 1280)
				{
					rightBullet2.isShot = false;
					rightBullet2.pos.y = player.pos.x;

				}
			}

#pragma endregion
						
			player.leftBullet[1] = player.leftBulletCount / 10;
			player.leftBulletCount %= 10;

			player.leftBullet[0] = player.leftBulletCount / 1;
			player.leftBulletCount %= 1;


			player.rightBullet[1] = player.rightBulletCount / 10;
			player.rightBulletCount %= 10;

			player.rightBullet[0] = player.rightBulletCount / 1;
			player.rightBulletCount %= 1;

#pragma region --- アニメーション ---

			// teki
			enemy.frameCount++;
			if (enemy.frameCount >= 2) {
				enemy.frameCount = 0;
				enemy.frameCounter = (enemy.frameCounter + 9) % 8;
			}

			// アニメーション
			enemy.src.x = enemy.frameCounter * enemy.width;

			// tekitama
			// 左辺1
			leftSideEnemyBullet1.frameCount++;
			if (leftSideEnemyBullet1.frameCount >= 3) {
				leftSideEnemyBullet1.frameCount = 0;
				leftSideEnemyBullet1.frameCounter = (leftSideEnemyBullet1.frameCounter + 1) % 3;
			}

			// アニメーション
			leftSideEnemyBullet1.src.x = leftSideEnemyBullet1.frameCounter * leftSideEnemyBullet1.width;

			// 左辺2
			leftSideEnemyBullet2.frameCount++;
			if (leftSideEnemyBullet2.frameCount >= 3) {
				leftSideEnemyBullet2.frameCount = 0;
				leftSideEnemyBullet2.frameCounter = (leftSideEnemyBullet2.frameCounter + 1) % 3;
			}

			// アニメーション
			leftSideEnemyBullet2.src.x = leftSideEnemyBullet2.frameCounter * leftSideEnemyBullet2.width;



			// 右辺1
			rightSideEnemyBullet1.frameCount++;
			if (rightSideEnemyBullet1.frameCount >= 3) {
				rightSideEnemyBullet1.frameCount = 0;
				rightSideEnemyBullet1.frameCounter = (rightSideEnemyBullet1.frameCounter + 1) % 3;
			}

			// アニメーション
			rightSideEnemyBullet1.src.x = rightSideEnemyBullet1.frameCounter * rightSideEnemyBullet1.width;

			// 右辺2
			rightSideEnemyBullet2.frameCount++;
			if (rightSideEnemyBullet2.frameCount >= 3) {
				rightSideEnemyBullet2.frameCount = 0;
				rightSideEnemyBullet2.frameCounter = (rightSideEnemyBullet2.frameCounter + 1) % 3;
			}

			// アニメーション
			rightSideEnemyBullet2.src.x = rightSideEnemyBullet2.frameCounter * rightSideEnemyBullet2.width;



			// 上辺1
			topSideEnemyBullet1.frameCount++;
			if (topSideEnemyBullet1.frameCount >= 3) {
				topSideEnemyBullet1.frameCount = 0;
				topSideEnemyBullet1.frameCounter = (topSideEnemyBullet1.frameCounter + 1) % 3;
			}

			// アニメーション
			topSideEnemyBullet1.src.x = topSideEnemyBullet1.frameCounter * topSideEnemyBullet1.width;

			// 上辺2
			topSideEnemyBullet2.frameCount++;
			if (topSideEnemyBullet2.frameCount >= 3) {
				topSideEnemyBullet2.frameCount = 0;
				topSideEnemyBullet2.frameCounter = (topSideEnemyBullet2.frameCounter + 1) % 3;
			}

			// アニメーション
			topSideEnemyBullet2.src.x = topSideEnemyBullet2.frameCounter * topSideEnemyBullet2.width;




			// 下辺1
			bottomSideEnemyBullet1.frameCount++;
			if (bottomSideEnemyBullet1.frameCount >= 3) {
				bottomSideEnemyBullet1.frameCount = 0;
				bottomSideEnemyBullet1.frameCounter = (bottomSideEnemyBullet1.frameCounter + 1) % 3;
			}

			// アニメーション
			bottomSideEnemyBullet1.src.x = bottomSideEnemyBullet1.frameCounter * bottomSideEnemyBullet1.width;

			// 下辺2
			bottomSideEnemyBullet2.frameCount++;
			if (bottomSideEnemyBullet2.frameCount >= 3) {
				bottomSideEnemyBullet2.frameCount = 0;
				bottomSideEnemyBullet2.frameCounter = (bottomSideEnemyBullet2.frameCounter + 1) % 3;
			}

			// アニメーション
			bottomSideEnemyBullet2.src.x = bottomSideEnemyBullet2.frameCounter * bottomSideEnemyBullet2.width;



			// 左上
			leftTopEnemyBullet.frameCount++;
			if (leftTopEnemyBullet.frameCount >= 3) {
				leftTopEnemyBullet.frameCount = 0;
				leftTopEnemyBullet.frameCounter = (leftTopEnemyBullet.frameCounter + 1) % 3;
			}

			// アニメーション
			leftTopEnemyBullet.src.x = leftTopEnemyBullet.frameCounter * leftTopEnemyBullet.width;

			// 右上
			rightTopEnemyBullet.frameCount++;
			if (rightTopEnemyBullet.frameCount >= 3) {
				rightTopEnemyBullet.frameCount = 0;
				rightTopEnemyBullet.frameCounter = (rightTopEnemyBullet.frameCounter + 1) % 3;
			}

			// アニメーション
			rightTopEnemyBullet.src.x = rightTopEnemyBullet.frameCounter * rightTopEnemyBullet.width;

			// 左下
			leftBottomEnemyBullet.frameCount++;
			if (leftBottomEnemyBullet.frameCount >= 3) {
				leftBottomEnemyBullet.frameCount = 0;
				leftBottomEnemyBullet.frameCounter = (leftBottomEnemyBullet.frameCounter + 1) % 3;
			}

			// アニメーション
			leftBottomEnemyBullet.src.x = leftBottomEnemyBullet.frameCounter * leftBottomEnemyBullet.width;

			// 右下
			rightBottomEnemyBullet.frameCount++;
			if (rightBottomEnemyBullet.frameCount >= 3) {
				rightBottomEnemyBullet.frameCount = 0;
				rightBottomEnemyBullet.frameCounter = (rightBottomEnemyBullet.frameCounter + 1) % 3;
			}

			// アニメーション
			rightBottomEnemyBullet.src.x = rightBottomEnemyBullet.frameCounter * rightBottomEnemyBullet.width;

#pragma endregion

			///
			/// ↑更新処理ここまで
			///

#pragma endregion

#pragma region DrawingProcessing

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, GameHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(0, 0, leftHandTexture, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(160, 0, rightHandTexture, 1.0f, 1.0f, 0.0f, WHITE);

			for (int k = 0; k < 2; ++k) {
				Novice::DrawSprite(32 + (64 + k), 0, numbers[player.leftBullet[k]], 1, 1, 0.0f, WHITE);
			}

			for (int r = 0; r < 2; ++r) {
				Novice::DrawSprite(192 + (64 + r), 0, numbers[player.rightBullet[r]], 1, 1, 0.0f, WHITE);
			}

			//playerの描画
			if (player.back)
			{
				Novice::DrawSprite(
					int(player.pos.x),
					int(player.pos.y),
					playerBackTexture,
					1, 1, 0.0f,
					WHITE);
			}
			if (player.front)
			{
				Novice::DrawSprite(
					int(player.pos.x),
					int(player.pos.y),
					playerFrontTexture,
					1, 1, 0.0f,
					WHITE);
			}

			if (strightBullet1.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(strightBullet1.pos.x),
					static_cast<int>(strightBullet1.pos.y),
					leftHandTexture, 1, 1,
					0.0f, WHITE);
			}

			if (strightBullet2.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(strightBullet2.pos.x),
					static_cast<int>(strightBullet2.pos.y),
					rightHandTexture, 1, 1,
					0.0f, WHITE);
			}
			if (backBullet1.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(backBullet1.pos.x),
					static_cast<int>(backBullet1.pos.y),
					leftHandTexture, 1, 1,
					0.0f, WHITE);
			}

			if (backBullet2.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(backBullet2.pos.x),
					static_cast<int>(backBullet2.pos.y),
					rightHandTexture, 1, 1,
					0.0f, WHITE);
			}
			if (leftBullet1.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(leftBullet1.pos.x),
					static_cast<int>(leftBullet1.pos.y),
					leftHandTexture, 1, 1,
					0.0f, WHITE);
			}

			if (leftBullet2.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(leftBullet2.pos.x),
					static_cast<int>(leftBullet2.pos.y),
					rightHandTexture, 1, 1,
					0.0f, WHITE);
			}
			if (rightBullet1.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(rightBullet1.pos.x),
					static_cast<int>(rightBullet1.pos.y),
					leftHandTexture, 1, 1,
					0.0f, WHITE);
			}
			if (rightBullet2.isShot)
			{
				Novice::DrawSprite(
					static_cast<int>(rightBullet2.pos.x),
					static_cast<int>(rightBullet2.pos.y),
					rightHandTexture, 1, 1,
					0.0f, WHITE);
			}

			// 敵の左辺の弾1
			if (leftSideEnemyBullet1.isBulletShot) {
				Novice::DrawQuad(
					int(leftSideEnemyBullet1.vertex.leftTop.x),
					int(leftSideEnemyBullet1.vertex.leftTop.y),

					int(leftSideEnemyBullet1.vertex.rightTop.x),
					int(leftSideEnemyBullet1.vertex.rightTop.y),

					int(leftSideEnemyBullet1.vertex.leftBottom.x),
					int(leftSideEnemyBullet1.vertex.leftBottom.y),

					int(leftSideEnemyBullet1.vertex.rightBottom.x),
					int(leftSideEnemyBullet1.vertex.rightBottom.y),

					int(leftSideEnemyBullet1.src.x), int(leftSideEnemyBullet1.src.y),
					int(leftSideEnemyBullet1.width), int(leftSideEnemyBullet1.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の左辺の弾2
			if (leftSideEnemyBullet2.isBulletShot) {
				Novice::DrawQuad(
					int(leftSideEnemyBullet2.vertex.leftTop.x),
					int(leftSideEnemyBullet2.vertex.leftTop.y),

					int(leftSideEnemyBullet2.vertex.rightTop.x),
					int(leftSideEnemyBullet2.vertex.rightTop.y),

					int(leftSideEnemyBullet2.vertex.leftBottom.x),
					int(leftSideEnemyBullet2.vertex.leftBottom.y),

					int(leftSideEnemyBullet2.vertex.rightBottom.x),
					int(leftSideEnemyBullet2.vertex.rightBottom.y),

					int(leftSideEnemyBullet2.src.x), int(leftSideEnemyBullet2.src.y),
					int(leftSideEnemyBullet2.width), int(leftSideEnemyBullet2.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の右辺の弾1
			if (rightSideEnemyBullet1.isBulletShot) {
				Novice::DrawQuad(
					int(rightSideEnemyBullet1.vertex.leftTop.x),
					int(rightSideEnemyBullet1.vertex.leftTop.y),

					int(rightSideEnemyBullet1.vertex.rightTop.x),
					int(rightSideEnemyBullet1.vertex.rightTop.y),

					int(rightSideEnemyBullet1.vertex.leftBottom.x),
					int(rightSideEnemyBullet1.vertex.leftBottom.y),

					int(rightSideEnemyBullet1.vertex.rightBottom.x),
					int(rightSideEnemyBullet1.vertex.rightBottom.y),

					int(rightSideEnemyBullet1.src.x), int(rightSideEnemyBullet1.src.y),
					int(rightSideEnemyBullet1.width), int(rightSideEnemyBullet1.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の右辺の弾2
			if (rightSideEnemyBullet2.isBulletShot) {
				Novice::DrawQuad(
					int(rightSideEnemyBullet2.vertex.leftTop.x),
					int(rightSideEnemyBullet2.vertex.leftTop.y),

					int(rightSideEnemyBullet2.vertex.rightTop.x),
					int(rightSideEnemyBullet2.vertex.rightTop.y),

					int(rightSideEnemyBullet2.vertex.leftBottom.x),
					int(rightSideEnemyBullet2.vertex.leftBottom.y),

					int(rightSideEnemyBullet2.vertex.rightBottom.x),
					int(rightSideEnemyBullet2.vertex.rightBottom.y),

					int(rightSideEnemyBullet2.src.x), int(rightSideEnemyBullet2.src.y),
					int(rightSideEnemyBullet2.width), int(rightSideEnemyBullet2.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の上辺の弾1
			if (topSideEnemyBullet1.isBulletShot) {
				Novice::DrawQuad(
					int(topSideEnemyBullet1.vertex.leftTop.x),
					int(topSideEnemyBullet1.vertex.leftTop.y),

					int(topSideEnemyBullet1.vertex.rightTop.x),
					int(topSideEnemyBullet1.vertex.rightTop.y),

					int(topSideEnemyBullet1.vertex.leftBottom.x),
					int(topSideEnemyBullet1.vertex.leftBottom.y),

					int(topSideEnemyBullet1.vertex.rightBottom.x),
					int(topSideEnemyBullet1.vertex.rightBottom.y),

					int(topSideEnemyBullet1.src.x), int(topSideEnemyBullet1.src.y),
					int(topSideEnemyBullet1.width), int(topSideEnemyBullet1.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の上辺の弾1
			if (topSideEnemyBullet2.isBulletShot) {
				Novice::DrawQuad(
					int(topSideEnemyBullet2.vertex.leftTop.x),
					int(topSideEnemyBullet2.vertex.leftTop.y),

					int(topSideEnemyBullet2.vertex.rightTop.x),
					int(topSideEnemyBullet2.vertex.rightTop.y),

					int(topSideEnemyBullet2.vertex.leftBottom.x),
					int(topSideEnemyBullet2.vertex.leftBottom.y),

					int(topSideEnemyBullet2.vertex.rightBottom.x),
					int(topSideEnemyBullet2.vertex.rightBottom.y),

					int(topSideEnemyBullet2.src.x), int(topSideEnemyBullet2.src.y),
					int(topSideEnemyBullet2.width), int(topSideEnemyBullet2.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の下辺の弾1
			if (bottomSideEnemyBullet1.isBulletShot) {
				Novice::DrawQuad(
					int(bottomSideEnemyBullet1.vertex.leftTop.x),
					int(bottomSideEnemyBullet1.vertex.leftTop.y),

					int(bottomSideEnemyBullet1.vertex.rightTop.x),
					int(bottomSideEnemyBullet1.vertex.rightTop.y),

					int(bottomSideEnemyBullet1.vertex.leftBottom.x),
					int(bottomSideEnemyBullet1.vertex.leftBottom.y),

					int(bottomSideEnemyBullet1.vertex.rightBottom.x),
					int(bottomSideEnemyBullet1.vertex.rightBottom.y),

					int(bottomSideEnemyBullet1.src.x), int(bottomSideEnemyBullet1.src.y),
					int(bottomSideEnemyBullet1.width), int(bottomSideEnemyBullet1.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の下辺の弾2
			if (bottomSideEnemyBullet2.isBulletShot) {
				Novice::DrawQuad(
					int(bottomSideEnemyBullet2.vertex.leftTop.x),
					int(bottomSideEnemyBullet2.vertex.leftTop.y),

					int(bottomSideEnemyBullet2.vertex.rightTop.x),
					int(bottomSideEnemyBullet2.vertex.rightTop.y),

					int(bottomSideEnemyBullet2.vertex.leftBottom.x),
					int(bottomSideEnemyBullet2.vertex.leftBottom.y),

					int(bottomSideEnemyBullet2.vertex.rightBottom.x),
					int(bottomSideEnemyBullet2.vertex.rightBottom.y),

					int(bottomSideEnemyBullet2.src.x), int(bottomSideEnemyBullet2.src.y),
					int(bottomSideEnemyBullet2.width), int(bottomSideEnemyBullet2.height),

					enemyBulletTexture, 0xFFFFFFFF
				);
			}

			// 敵の左上の弾
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

			break;

		case CLEAR:

			///
			/// ↓更新処理ここから
			///

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				scene = TITLE;

			}

			///
			/// ↑更新処理ここまで
			///

			Novice::DrawSprite(0, 0, KuriaHandle, 1.0f, 1.0f, 0.0f, WHITE);

			///
			/// ↓描画処理ここから
			///

			///
			/// ↑描画処理ここまで
			///

			break;

		case OVER:

			///
			/// ↓更新処理ここから
			///

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				scene = TITLE;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, OverHandle, 1.0f, 1.0f, 0.0f, WHITE);

			///
			/// ↑描画処理ここまで
			///

			break;

		}

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