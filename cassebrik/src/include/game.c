/*
 * game.c
 *
 *  Created on: 1 mai 2021
 *      Author: blegrin
 */

#include "game.h";
#include <raylib.h>
#include <stdio.h>

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

/**
 *
 */
Brik* init(Brik * tab) {

	Brik tmp;
	Brik * ligne;

	if (sizeof(tab) > 0)
		free(tab);

	tab = (Brik *) malloc(sizeof(Brik) * NBR_BRIK_W - 1);
	for (int x=0; x<NBR_BRIK_W; x++) {
		ligne = (Brik *) malloc(sizeof(Brik) * NBR_BRIK_H-1);
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
			*(ligne+y) = tmp;
		}
		*(tab+x) = ligne;
	}
}

