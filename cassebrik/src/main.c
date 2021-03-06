/*
 * main.c
 *
 *  Created on: 2 avr. 2021
 *      Author: blegrin
 */

#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH       800
#define HEIGHT      800
#define NBR_BRIK_W    8
#define NBR_BRIK_H   10
#define TAB_WIDTH   100
#define TAB_HEIGHT   20
#define VITESSE     2.0

typedef enum BOOL {TRUE = 0, FALSE = 1} BOOL;

struct Item {
	Vector2 position;
	Vector2 size;
	Color   color;
	Vector2 haut_droite;
	Vector2 haut_gauche;
	Vector2 bas_droite;
	Vector2 bas_gauche;
	BOOL    hit;
};

typedef struct BALL {
	Vector2 center;
	float   radius;
	Vector2 north;
	Vector2 east;
	Vector2 south;
	Vector2 west;
	Vector2 dir;
	Color   color;
} Ball;

typedef struct Item Paddle;
typedef struct Item Brik;

void affSepBrik ();
void affTabBrik(Brik tab[][NBR_BRIK_H]);

int main (int argc, char** argv) {


	Brik tabBrick[NBR_BRIK_W][NBR_BRIK_H];
	Paddle paddle;
	Ball ball;
	BOOL start = FALSE;
	int left;
	int failled;

	paddle.size        = (Vector2) {200,5};
	paddle.color       = (Color)   {0,0,1};
	paddle.position    = (Vector2) {WIDTH/2-paddle.size.x/2,HEIGHT - 50};
	paddle.haut_droite = (Vector2) {paddle.position.x,paddle.position.y};
	paddle.haut_gauche = (Vector2) {paddle.position.x+paddle.size.x,paddle.position.y};
	paddle.bas_droite  = (Vector2) {paddle.position.x,paddle.position.y-paddle.size.y};
	paddle.bas_gauche  = (Vector2) {paddle.position.x+paddle.size.x,paddle.position.y-paddle.size.y};
	paddle.hit         = FALSE;

	ball.center.x = 0;
	ball.center.y = 0;
	ball.radius   = 5;
	ball.north    = (Vector2){ball.radius,ball.radius};
	ball.east     = (Vector2){ball.radius,0};
	ball.south    = (Vector2){-ball.radius,-ball.radius};
	ball.west     = (Vector2){-ball.radius,0};
	ball.dir      = (Vector2){1,-1};
	ball.color    = RED;


	Brik tmp;
	for (int x=0; x<NBR_BRIK_W; x++) {
		for (int y=0; y<NBR_BRIK_H; y++) {
			tmp.size.x = TAB_WIDTH;
			tmp.size.y = TAB_HEIGHT;
			tmp.position.x = x * (tmp.size.x * WIDTH / WIDTH);
			tmp.position.y = y * (tmp.size.y * HEIGHT / HEIGHT);
			tmp.haut_droite = (Vector2) {tmp.position.x,tmp.position.y};
			tmp.haut_gauche = (Vector2) {tmp.position.x+tmp.size.x,tmp.position.y};
			tmp.bas_droite  = (Vector2) {tmp.position.x,tmp.position.y-tmp.size.y};
			tmp.bas_gauche  = (Vector2) {tmp.position.x+tmp.size.x,tmp.position.y-tmp.size.y};
			tmp.color = SKYBLUE;
			tmp.hit = FALSE;
			tabBrick[x][y] = tmp;
		}
	}

	InitWindow(800, 800, "Casse-briques");
	SetTargetFPS(60);

	left = NBR_BRIK_H * NBR_BRIK_W;
	failled = 0;
	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(BLACK);

		if (IsKeyDown(KEY_ESCAPE)) {
			start = FALSE;
		}

		if (IsKeyDown(KEY_SPACE)){
			start = TRUE;
		}

		if (IsKeyDown(KEY_Q) && paddle.position.x > 0.0f ) {
			paddle.position.x -= 5.0f;
		}

		if (IsKeyDown(KEY_P) && paddle.position.x < WIDTH - paddle.size.x){
			paddle.position.x += 5.0f;
		}


		if (left == 0){
			start = FALSE;
		}

		if (start == FALSE) {
			affTabBrik(tabBrick);
			affSepBrik();
			ball.center.x = paddle.position.x + paddle.size.x / 2;
			ball.center.y = paddle.position.y - ball.radius;
			ball.north = (Vector2){ball.center.x+ball.radius,ball.center.y+ball.radius};
			ball.east  = (Vector2){ball.center.x+ball.radius,ball.center.y};
			ball.south = (Vector2){ball.center.x+ball.radius,ball.center.y};
			ball.west  = (Vector2){ball.center.x-ball.radius,ball.center.y-ball.radius};
		} else {

			// rebond sur le bord haut.
			if (ball.center.x + ball.radius <= 0 ) {
				ball.center.x = -ball.radius;
				ball.dir.x = -ball.dir.x;
			}

			// rebond sur le bord gauche de l'?cran.
			if (ball.center.x + ball.radius >= WIDTH) {
				ball.center.x = WIDTH - ball.radius;
				ball.dir.x = -ball.dir.x;
			}

			// rebond sur le bord droit de l'?cran.
			if (ball.center.y + ball.radius <= 0 ) {
				ball.center.y = ball.radius;
				ball.dir.y = -ball.dir.y;
			}

			// rebond sur la raquette.
			if (   ball.center.x + ball.radius >= paddle.position.x
			    && ball.center.x - ball.radius <= paddle.position.x + paddle.size.x
				&& ball.center.y + ball.radius >= paddle.position.y
				&& ball.center.y + ball.radius <= paddle.position.y + paddle.size.y) {
					ball.dir.y = -1;
			}

			// echec: perte de la balle
			if (ball.center.y > HEIGHT) {
				start = FALSE;
				failled++;
			}


			Brik b;
		    for (int x=0; x<NBR_BRIK_W+1; x++){
		    	for (int y=0; y<NBR_BRIK_H+1; y++) {
					b = tabBrick[x][y];

					// frapper par en-dessous.
					if (   b.hit == FALSE
						&& ball.center.x + ball.radius >= b.position.x
						&& ball.center.x + ball.radius <= b.position.x + b.size.x
						&& ball.center.y + ball.radius <= b.position.y + b.size.y) {
						tabBrick[x][y].hit   = TRUE;
						ball.dir.y = 1;
						left--;
					}

					// frapper par en-dessus.
                    if (    b.hit == FALSE
                         && ball.center.x + ball.radius >= b.position.x
						 && ball.center.x + ball.radius <= b.position.x + b.size.x
						 && ball.center.y + ball.radius <= b.position.y
                    ) {
                    	tabBrick[x][y].hit = TRUE;
                        ball.dir.y = -1;
                        left--;
                    }

					// frapper par la gauche.
                    /*if (  b.hit == FALSE
                    	 && ball.center.x + ball.radius <= b.position.x + b.position.x
						 && ball.center.y + ball.radius >= b.position.y
						 && ball.center.y + ball.radius <= b.position.y + b.position.y) {
                    	tabBrick[x][y].hit = TRUE;
                    	ball.dir.x = 1;
                    	left--;
                    }*/

					// frapper par la droite.
                    /*if (  b.hit == FALSE
                       && ball.center.x + ball.radius >= b.position.x
					   && ball.center.y + ball.radius >= b.position.y
					   && ball.center.y + ball.radius <= b.position.y + b.position.y
                    ) {
                       tabBrick[x][y].hit   = TRUE;
                       ball.dir.x = -1;
                       left--;
                    }*/

					affTabBrik(tabBrick);
				    affSepBrik();
				    DrawText(TextFormat("%02i",left), WIDTH/2, HEIGHT/2, 12, WHITE);
		    	}
		    }



			ball.center.x = ball.center.x + VITESSE * ball.dir.x;
			ball.center.y = ball.center.y + VITESSE * ball.dir.y;
			ball.north = (Vector2){ball.center.x+ball.radius,ball.center.y+ball.radius};
			ball.east  = (Vector2){ball.center.x+ball.radius,ball.center.y};
			ball.south = (Vector2){ball.center.x+ball.radius,ball.center.y};
			ball.west  = (Vector2){ball.center.x-ball.radius,ball.center.y-ball.radius};
		}

		DrawCircleV(ball.center,ball.radius,ball.color);
		DrawRectangleV(paddle.position,paddle.size,WHITE);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}

/**
 *
 */
void affSepBrik () {
	for (int x=0; x<NBR_BRIK_W; x++) {
		DrawLine(TAB_WIDTH*(x+1),0,TAB_WIDTH*(x+1),NBR_BRIK_H*TAB_HEIGHT,BLACK);
	}
	for (int y=0; y<NBR_BRIK_H; y++) {
		DrawLine(0,TAB_HEIGHT*(y+1),NBR_BRIK_W*TAB_WIDTH,TAB_HEIGHT*(y+1),BLACK);
	}
}

/**
 *
 */
void affTabBrik(Brik tab[][NBR_BRIK_H]) {
	Brik bb;
	for (int x=0; x<NBR_BRIK_W; x++){
		for (int y=0; y<NBR_BRIK_H; y++){
			bb = (Brik)tab[x][y];
			if (bb.hit == FALSE) {
				DrawRectangleV((Vector2){bb.position.x,bb.position.y},(Vector2){bb.size.x,bb.size.y},SKYBLUE);
			} else {
				DrawRectangleV((Vector2){bb.position.x,bb.position.y},(Vector2){bb.size.x,bb.size.y},BLACK);
			}
		}
	}
}
