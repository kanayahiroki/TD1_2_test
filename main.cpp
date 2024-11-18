#include <Novice.h>
#include <corecrt_math.h>

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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// --- enemy初期化 ---
	int enemyTexture = Novice::LoadTexture("./Resources/images/teki.png");

	Enemy enemy = {};
	enemy.pos.x = 200.0f;
	enemy.pos.y = 100.0f;

	enemy.src.x = 0.0f;
	enemy.src.y = 0.0f;

	enemy.frameCount = 0;
	enemy.frameCounter = 0;

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

	// サイズ
	enemy.width = 200.0f;
	enemy.height = 100.0f;

	// --- enemyBullet初期化 ---
	// 左辺
	Bullet leftSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		leftSideEnemyBullet[i].width = 20.0f;
		leftSideEnemyBullet[i].height = 20.0f;

		leftSideEnemyBullet[i].start.x = enemy.pos.x;
		leftSideEnemyBullet[i].start.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

		leftSideEnemyBullet[i].end.x = (enemy.pos.x - (leftSideEnemyBullet[i].width * 2.0f));
		leftSideEnemyBullet[i].end.y = enemy.pos.y + (leftSideEnemyBullet[i].height * 1.2f);

		leftSideEnemyBullet[i].pos.x = leftSideEnemyBullet[i].start.x;
		leftSideEnemyBullet[i].pos.y = leftSideEnemyBullet[i].start.y;

		leftSideEnemyBullet[i].columns = 2;
		leftSideEnemyBullet[i].space = 12;
		leftSideEnemyBullet[i].speed = 5.0f;
		leftSideEnemyBullet[i].length = 0.0f;
		leftSideEnemyBullet[i].t = 0.0f;
		leftSideEnemyBullet[i].easedT = 0.0f;

		leftSideEnemyBullet[i].interpolation = false;
		leftSideEnemyBullet[i].isBulletShot = false;
	}

	// 右辺
	Bullet rightSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		rightSideEnemyBullet[i].width = 20.0f;
		rightSideEnemyBullet[i].height = 20.0f;

		rightSideEnemyBullet[i].start.x = (enemy.pos.x + enemy.width) - rightSideEnemyBullet[i].width;
		rightSideEnemyBullet[i].start.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

		rightSideEnemyBullet[i].end.x = ((enemy.pos.x + enemy.width) + rightSideEnemyBullet[i].width);
		rightSideEnemyBullet[i].end.y = enemy.pos.y + (rightSideEnemyBullet[i].height * 1.2f);

		rightSideEnemyBullet[i].pos.x = rightSideEnemyBullet[i].start.x;
		rightSideEnemyBullet[i].pos.y = rightSideEnemyBullet[i].start.y;

		rightSideEnemyBullet[i].columns = 2;
		rightSideEnemyBullet[i].space = 12;
		rightSideEnemyBullet[i].speed = 5.0f;
		rightSideEnemyBullet[i].length = 0.0f;
		rightSideEnemyBullet[i].t = 0.0f;
		rightSideEnemyBullet[i].easedT = 0.0f;

		rightSideEnemyBullet[i].interpolation = false;
		rightSideEnemyBullet[i].isBulletShot = false;
	}

	// 上辺
	Bullet topSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		topSideEnemyBullet[i].width = 20.0f;
		topSideEnemyBullet[i].height = 20.0f;

		topSideEnemyBullet[i].start.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
		topSideEnemyBullet[i].start.y = enemy.pos.y;

		topSideEnemyBullet[i].end.x = enemy.pos.x + (topSideEnemyBullet[i].width * 2.6f);
		topSideEnemyBullet[i].end.y = enemy.pos.y - (topSideEnemyBullet[i].height * 2.0f);

		topSideEnemyBullet[i].pos.x = topSideEnemyBullet[i].start.x;
		topSideEnemyBullet[i].pos.y = topSideEnemyBullet[i].start.y;

		topSideEnemyBullet[i].columns = 2;
		topSideEnemyBullet[i].space = 55;
		topSideEnemyBullet[i].speed = 5.0f;
		topSideEnemyBullet[i].length = 0.0f;
		topSideEnemyBullet[i].t = 0.0f;
		topSideEnemyBullet[i].easedT = 0.0f;

		topSideEnemyBullet[i].interpolation = false;
		topSideEnemyBullet[i].isBulletShot = false;
	}

	// 下辺
	Bullet bottomSideEnemyBullet[2]{};
	for (int i = 0; i < 2; ++i) {
		bottomSideEnemyBullet[i].width = 20.0f;
		bottomSideEnemyBullet[i].height = 20.0f;

		bottomSideEnemyBullet[i].start.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
		bottomSideEnemyBullet[i].start.y = (enemy.pos.y + enemy.height) - bottomSideEnemyBullet[i].height;

		bottomSideEnemyBullet[i].end.x = enemy.pos.x + (bottomSideEnemyBullet[i].width * 2.6f);
		bottomSideEnemyBullet[i].end.y = (enemy.pos.y + enemy.height) + bottomSideEnemyBullet[i].height;

		bottomSideEnemyBullet[i].pos.x = bottomSideEnemyBullet[i].start.x;
		bottomSideEnemyBullet[i].pos.y = bottomSideEnemyBullet[i].start.y;

		bottomSideEnemyBullet[i].columns = 2;
		bottomSideEnemyBullet[i].space = 55;
		bottomSideEnemyBullet[i].speed = 5.0f;
		bottomSideEnemyBullet[i].length = 0.0f;
		bottomSideEnemyBullet[i].t = 0.0f;
		bottomSideEnemyBullet[i].easedT = 0.0f;

		bottomSideEnemyBullet[i].interpolation = false;
		bottomSideEnemyBullet[i].isBulletShot = false;
	}

	// 左上
	Bullet leftTopEnemyBullet{};

	leftTopEnemyBullet.width = 20.0f;
	leftTopEnemyBullet.height = 20.0f;

	leftTopEnemyBullet.start.x = enemy.pos.x;
	leftTopEnemyBullet.start.y = enemy.pos.y;

	leftTopEnemyBullet.end.x = enemy.pos.x - (leftTopEnemyBullet.width * 2.0f);
	leftTopEnemyBullet.end.y = enemy.pos.y - (leftTopEnemyBullet.height * 2.0f);

	leftTopEnemyBullet.pos.x = leftTopEnemyBullet.start.x;
	leftTopEnemyBullet.pos.y = leftTopEnemyBullet.start.y;

	leftTopEnemyBullet.speed = 5.0f;
	leftTopEnemyBullet.length = 0.0f;
	leftTopEnemyBullet.t = 0.0f;
	leftTopEnemyBullet.easedT = 0.0f;

	leftTopEnemyBullet.interpolation = false;
	leftTopEnemyBullet.isBulletShot = false;

	Player player{};
	player.pos.x = 640.0f;
	player.pos.y = 620.0f;
	player.dir.x = player.pos.x - enemy.pos.y;
	player.dir.y = player.pos.y - enemy.pos.y;
	player.radius = 40.0f;
	player.speed = 5.0f;

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

		// アニメーション
		enemy.frameCount++;
		if (enemy.frameCount >= 5) {
			enemy.frameCount = 0;
			enemy.frameCounter = (enemy.frameCounter + 9) % 2;
		}

		// アニメーション
		enemy.src.x = enemy.frameCounter * enemy.width;

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

		// 敵の下辺の弾
		for (int j = 0; j < bottomSideEnemyBullet[0].columns; ++j) {
			if (bottomSideEnemyBullet[j].isBulletShot) {
				Novice::DrawBox(
					int(bottomSideEnemyBullet[j].pos.x) + j * (int(bottomSideEnemyBullet[j].width) + bottomSideEnemyBullet[j].space),
					int(bottomSideEnemyBullet[j].pos.y),
					int(bottomSideEnemyBullet[j].width),
					int(bottomSideEnemyBullet[j].height),
					0.0f, WHITE,
					kFillModeSolid
				);
			}
		}

		// 左上
		if (leftTopEnemyBullet.isBulletShot) {
			Novice::DrawBox(
				int(leftTopEnemyBullet.pos.x),
				int(leftTopEnemyBullet.pos.y),
				int(leftTopEnemyBullet.width),
				int(leftTopEnemyBullet.height),
				0.0f, WHITE,
				kFillModeSolid
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