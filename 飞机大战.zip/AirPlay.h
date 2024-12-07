#include<iostream>
#include <easyx.h>
#include<sysinfoapi.h>
#include<time.h>

//1���򿪴��ڡ�����ͼƬ������ṹ��ʵ�֡������Ķ���
//2�������ҷ��ɻ����ҷ��ɻ����ƶ�
//3���ӵ������ɡ����䡢�ͷ�

IMAGE img[3];//ͼƬ����//���ͼƬ������
IMAGE image_enemy[2][2];//�л�ͼƬ

enum MyEenum {
	
	BGWIDTH = 400,				BGHEIGHT = 600,
	myAirWIDTH = 80,			myAirHEIGHT = 120,
	enemyPlaneWIDTH = 25,		enemyPlaneHEIGHT = 25,
	bulletWIDTH = 8,			bulletHEIGHT = 8,
	bullet_Num = 15,			//�ӵ�������
	BIG,						SMALL,
	ENEMY_NUM = 10				//�л�����
};

//�ɻ����Ա�
struct Plane {
	int x;
	int y;
	bool live;	//�Ƿ���
	int width;
	int height;
	int hp;		//Ѫ��
	int type;	//����
}player,bull[bullet_Num], enemy[ENEMY_NUM];

//ͼƬ����
void loadImg() {
	loadimage(&img[0], "res\\����.jpg", (int)BGWIDTH, (int)BGHEIGHT);
	loadimage(&img[1], "res\\�ɻ�.jpg",myAirWIDTH,myAirHEIGHT);//�ҷ��ɻ�

	loadimage(&img[2], "res\\�ӵ�.jpg", bulletWIDTH, bulletHEIGHT);
	loadimage(&image_enemy[0][0], "res\\�л�.jpg", 60, 75);
	loadimage(&image_enemy[1][0], "res\\�л�.jpg", 40, 50);
}

void enemyHP(int i) {
	//��ɻ���С�ɻ�
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
//�ɻ���ʼ��
void gameInit() {
	player.x = BGWIDTH / 2;
	player.y = BGHEIGHT - myAirHEIGHT;
	player.live = true;
	//��ʼ���ӵ�
	for (int i = 0; i < bullet_Num; i++) {
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].live = false;
		//�����л�
		enemyHP(i);
	}
	/*enemy[0].live = true;
	enemy[0].y = 10;
	enemy[0].x = 10;*/
}

void gameDraw() {
	//��ʼ��ͼ��
	loadImg();
	putimage(0, 0, &img[0]);
	//NOTSCERASE ������
	if (player.live) {
		putimage(player.x, player.y, &img[1]);//�ҷ��ɻ�
	}
	
	//�����ӵ�
	for (int i = 0; i < bullet_Num; i++)
		if (bull[i].live) {
			putimage(bull[i].x, bull[i].y, &img[2]);
		}
	//���Ƶл�
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

			//x�������
			enemy[i].x = rand() % (BGWIDTH - 60);
			enemy[i].y = 0;
			break;
		}
	}
}

void playerMove(int speed){
	//�����¼�
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




//�������еĹ��ܣ��������������ʼ
void start() {
	initgraph(BGWIDTH,BGHEIGHT,1);
	gameInit();
	//��ʼ��

	BeginBatchDraw();
	while (player.live) {
		gameDraw();
		FlushBatchDraw();
		playerMove(3);
		//�ӵ��ƶ�
		bulletMove();


		static DWORD t1, t2;
		if (t2 - t1 > 500) {
			//�����ɻ�
			createEnemy();
			t1 = t2;
		}
		t2 = clock();
		//�ɻ��ƶ�
		enemyMove(1);
		hit();

	}
	if (player.live == false) {
		MessageBox(NULL, "��Ϸ����������", "114514����������", MB_OK);
	}

	std::cout << "������Ϸ����������!\n";
	closegraph();
}