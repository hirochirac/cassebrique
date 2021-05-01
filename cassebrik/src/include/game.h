/*
 * game.h
 *
 *  Created on: 1 mai 2021
 *      Author: blegrin
 */

#include <raylib.h>
#include <stdio.h>

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

#define WIDTH       800
#define HEIGHT      800
#define NBR_BRIK_W    8
#define NBR_BRIK_H   10
#define TAB_WIDTH   100
#define TAB_HEIGHT   20
#define VITESSE     4.0

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

int isLeft(Brik tab[][NBR_BRIK_H]);
void affSepBrik ();
void affTabBrik(Brik tab[][NBR_BRIK_H]);
Brik* init(Brik * tab);


#endif /* INCLUDE_GAME_H_ */
