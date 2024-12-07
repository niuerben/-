#include<iostream>
#include <easyx.h>
#include<sysinfoapi.h>
#include<time.h>

//1、打开窗口、加载图片、链表结构的实现、变量的定义
//2、生成我方飞机、我方飞机的移动
//3、子弹的生成、发射、释放

IMAGE img[3];//图片数组//存放图片的数组
IMAGE image_enemy[2][2];//敌机图片

enum MyEenum {
	
	BGWIDTH = 400,				BGHEIGHT = 600,
	myAirWIDTH = 80,			myAirHEIGHT = 120,
	enemyPlaneWIDTH = 25,		enemyPlaneHEIGHT = 25,
	bulletWIDTH = 8,			bulletHEIGHT = 8,
	bullet_Num = 15,			//子弹的数量
	BIG,						SMALL,
	ENEMY_NUM = 10				//敌机数量
};

//飞机属性表
struct Plane {
	int x;
	int y;
	bool live;	//是否存活
	int width;
	int height;
	int hp;		//血量
	int type;	//类型
}player,bull[bullet_Num], enemy[ENEMY_NUM];

//图片载入
void loadImg() {
	loadimage(&img[0], "res\\背景.jpg", (int)BGWIDTH, (int)BGHEIGHT);
	loadimage(&img[1], "res\\飞机.jpg",myAirWIDTH,myAirHEIGHT);//我方飞机

	loadimage(&img[2], "res\\子弹.jpg", bulletWIDTH, bulletHEIGHT);
	loadimage(&image_enemy[0][0], "res\\敌机.jpg", 60, 75);
	loadimage(&image_enemy[1][0], "res\\敌机.jpg", 40, 50);
}

void enemyHP(int i) {
	//大飞机和小飞机
	if (rand() % 8) {
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
	else  {
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 100;
		enemy[i].height = 144;
	}
	
}
//飞机初始化
void gameInit() {
	player.x = BGWIDTH / 2;
	player.y = BGHEIGHT - myAirHEIGHT;
	player.live = true;
	//初始化子弹
	for (int i = 0; i < bullet_Num; i++) {
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].live = false;
		//生产敌机
		enemyHP(i);
	}
	/*enemy[0].live = true;
	enemy[0].y = 10;
	enemy[0].x = 10;*/
}

void gameDraw() {
	//初始化图像
	loadImg();
	putimage(0, 0, &img[0]);
	//NOTSCERASE 二进制
	if (player.live) {
		putimage(player.x, player.y, &img[1]);//我方飞机
	}
	
	//绘制子弹
	for (int i = 0; i < bullet_Num; i++)
		if (bull[i].live) {
			putimage(bull[i].x, bull[i].y, &img[2]);
		}
	//绘制敌机
	for (int i = 0; i < ENEMY_NUM; i++) 
		if (enemy[i].live) {
			if (enemy[i].type == BIG)
				putimage(enemy[i].x, enemy[i].y, &image_enemy[0][0]);
			else if (enemy[i].type == SMALL) {
				putimage(enemy[i].x, enemy[i].y, &image_enemy[1][0]);
			}
			
		}
}

void createBullet() {
	for (int i = 0; i < bullet_Num; i++) {
		if (!bull[i].live) {
			bull[i].x = player.x + myAirWIDTH/2;
			bull[i].y = player.y;
			bull[i].live = true;
			break;
		}
	}
}

void bulletMove() {
	for (int i = 0; i < bullet_Num; i++) {
		if (bull[i].live) {
			bull[i].y -= 1;
			if (bull[i].y < 0) {
				bull[i].live = false;
			}
		}
			
	} 
}

void createEnemy() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			enemy[i].live = true;

			//x是随机的
			enemy[i].x = rand() % (BGWIDTH - 60);
			enemy[i].y = 0;
			break;
		}
	}
}

void playerMove(int speed){
	//键盘事件
	if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) {
		if (player.y > 0)
			player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) {
		if (player.y < BGHEIGHT - myAirHEIGHT)
			player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
		if(player.x > 0)
			player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) {
		if(player.x < BGWIDTH - myAirWIDTH)
			player.x += speed;
	}

	static DWORD t1 = 0, t2 = 0;
	if (GetAsyncKeyState(VK_SPACE) && t2-t1 > 100) {
		
		createBullet();
		t1 = t2;
	}
	t2 = GetTickCount();

}

void enemyMove(int speed) {
	for (int i = 0; i < ENEMY_NUM;i++) {
		if (enemy[i].live) {
			enemy[i].y += speed;
			if (enemy[i].y > BGHEIGHT) {
				enemy[i].live = false;
			}
		}
	}
}

void hit() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			continue;
		}
		if (enemy[i].x > player.x
			&& enemy[i].x + enemy[i].width <= player.x + myAirWIDTH
			&& enemy[i].y > player.y
			&& enemy[i].y + enemy[i].height <= player.y + myAirHEIGHT) {
			player.live = false;
			
		}



		for (int k = 0; k < ENEMY_NUM; k++) {
			if (bull[k].x > enemy[i].x 
				&& bull[k].x < enemy[i].x + enemy[i].width
				&& bull[k].y > enemy[i].y 
				&& bull[k].y < enemy[i].y + enemy[i].height) {
				bull[k].live = false;
				enemy[i].hp--;
				if (enemy[i].hp <= 0) {
					enemy[i].live = false;
				}
				
			}
			
		}
		
	}
}




//我们所有的功能，都从这个函数开始
void start() {
	initgraph(BGWIDTH,BGHEIGHT,1);
	gameInit();
	//初始化

	BeginBatchDraw();
	while (player.live) {
		gameDraw();
		FlushBatchDraw();
		playerMove(3);
		//子弹移动
		bulletMove();


		static DWORD t1, t2;
		if (t2 - t1 > 500) {
			//创建飞机
			createEnemy();
			t1 = t2;
		}
		t2 = clock();
		//飞机移动
		enemyMove(1);
		hit();

	}
	if (player.live == false) {
		MessageBox(NULL, "游戏结束（悲）", "114514曼波の提醒", MB_OK);
	}

	std::cout << "愉快的游戏结束（悲）!\n";
	closegraph();
}