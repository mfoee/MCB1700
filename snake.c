#include "stdio.h"
#include "stdlib.h"
#include "LPC17xx.h"
#include "KBD.h"
#include "GLCD.h"
#include "LED.h"


#define DELAY_2N 20

int xpos; //horizontal position
int ypos; //vertical position
int size; //size of the body
int direct = 0; //current direction
int prev_direct = 0; //previous direction
int joystick_val = 0; //current joystick val
int joystick_prev_val = KBD_RIGHT; //previous joystick val
char str[20],str1[20],str2[20],str3[20];
int snake[100][2]; //snake coordinates.
int delx, dely; //used to figure out where to turn
int speed; //how fast the snake will move
int xfood, yfood; //food coordinates
int tempx, tempy; // for the point at which the body turns
int collision = 0;
int border = 0;
int score = 0;

void food(){
	int i;
	xfood = rand()%9;
	yfood = rand()%20;
	for(i=0;i<size;i++){
		if(xfood == snake[i][0])
			if(yfood == snake[i][1])
				food();
	}
	GLCD_DisplayChar(xfood,yfood,1,0x81);
}

void delay (int count){
	count <<= DELAY_2N;
	while(count--);
}

void setbody(){
	int i; //counting
	
	for(i=0;i<size;i++){
		switch(direct){
			case 0://right
				snake[i][0] = xpos;
				snake[i][1] = ypos-i;
				break;
			case 1://left
				snake[i][0] = xpos;
				snake[i][1] = ypos+i;
				break;
			case 2://down
				snake[i][0] = xpos+i;
				snake[i][1] = ypos;
				break;
			case 3://up
				snake[i][0] = xpos-1;
				snake[i][1] = ypos;
				break;
		}
	}
}

void addbody(){
	int n=1;
	size++;
	score = score + 2*n;
	if(speed != 0)
		speed--;
	n++;
}

void check(){
	int i;
	
	//food check
	if(xfood == snake[0][0])
		if(yfood == snake[0][1]){
			addbody();
			food();
		}
		
	//tail collision check
	for(i=1;i<size;i++){
		if(snake[0][0] == snake[i][0])
			if(snake[0][1] == snake[i][1])
				collision = 1;
	}
	
	//collision to wall
	if(border == 1){
		//check right wall
		if(snake[0][1] == 19 && snake[1][1] == 18)
			collision = 1;
		//check left wall
		if(snake[0][1] == 0 && snake[1][1] == 1)
			collision = 1;
		//check bottom wall
		if(snake[0][0] == 9 && snake[1][0] == 8)
			collision = 1;
		//check top wall
		if(snake[0][0] == 0 && snake[1][0] == 1)
			collision = 1;
	}
}

void updatebody(){
	int i;
	if(direct == 0){//move right
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = ypos;
				snake[0][0] = xpos;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],1,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}		
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],1,0x8B);
			GLCD_DisplayChar(snake[i][0],snake[i][1],1,0x82);
		}
		delay(speed);
	}else if(direct == 1){//move left
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = ypos;
				snake[0][0] = xpos;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],1,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],1,0x89);
			GLCD_DisplayChar(snake[i][0],snake[i][1],1,0x82);
		}
		delay(speed);
	}else if(direct == 2){//move down
		for(i=size;i>0;i--){
			if(i -1 == 0){
				snake[0][1] = ypos;
				snake[0][0] = xpos;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],1,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],1,0x87);
			GLCD_DisplayChar(snake[i][0],snake[i][1],1,0x82);
		}
		delay(speed);
	}else if(direct == 3){//move up
		for(i=size;i>0;i--){
			if(i-1 == 0){
				snake[0][1] = ypos;
				snake[0][0] = xpos;				
			}else{
				GLCD_DisplayChar(snake[i-1][0],snake[i-1][1],1,' ');
				snake[i-1][1] = snake[i-2][1];
				snake[i-1][0] = snake[i-2][0];
			}
		}
		for(i=1;i<size;i++){
			GLCD_DisplayChar(snake[0][0],snake[0][1],1,0x85);
			GLCD_DisplayChar(snake[i][0],snake[i][1],1,0x82);
		}
		delay(speed);
	}
	check();
}

void direction(int joyval){

	switch(joyval){
		case KBD_UP:
			if (joystick_prev_val == KBD_LEFT || joystick_prev_val == KBD_RIGHT){
				xpos--;
				if (xpos < 0){
					xpos = 9;
				}
				direct = 3;
				prev_direct = direct;
				joystick_prev_val = joystick_val;
				updatebody();
			}
			break;
		case KBD_DOWN:
			if (joystick_prev_val == KBD_LEFT || joystick_prev_val == KBD_RIGHT){
				xpos++;
				if (xpos > 9){
					xpos = 0;
				}
				direct = 2;
				prev_direct = direct;
				joystick_prev_val = joystick_val;
				updatebody();
			}
			break;
		case KBD_LEFT:
			if (joystick_prev_val == KBD_UP || joystick_prev_val == KBD_DOWN){
				ypos--;
				if (ypos < 0){
					ypos = 20;
				}
				direct = 1;
				prev_direct = direct;
				joystick_prev_val = joystick_val;
				updatebody();
			}
			break;
		case KBD_RIGHT:
			if (joystick_prev_val == KBD_UP || joystick_prev_val == KBD_DOWN){
				ypos++;
				if (ypos > 20 ){
					ypos = 0;
				}
				direct = 0;
				prev_direct = direct;
				joystick_prev_val = joystick_val;
				updatebody();
			}
			break;
		default:
			switch(direct){
				case 0://right
					ypos++;
					if (ypos > 20){
						ypos = 0;
					}
					updatebody();
					check();
					break;
				case 1://left
					ypos--;
					if (ypos < 0){
						ypos = 20;
					}
					updatebody();
					check();
					break;
				case 2://down
					xpos++;
					if (xpos > 9){
						xpos = 0;
					}
					updatebody();
					check();
					break;
				case 3://up
					xpos--;
					if (xpos < 0){
						xpos = 9;
					}
					updatebody();
					check();
					break;
			}
			break;
	}	
}

void clearsnake(){
	int i;
	for(i=0;i<size;i++){
		snake[i][0]=1;
		snake[i][1]=1;
	}
}

int snakegame(){
	int joy_difficulty, joy_try;
	int mode, modesel;
	int highscore=0;
	int gameover, tryagain;
	char scores[54];
// 	char fd[20];
	int done=0;
	
	GLCD_Init();
	KBD_Init();
	LED_Init();

	while(!done){
		modesel = 1;
		mode = 1;
		tryagain = 1;
		direct = 0;
		prev_direct = 0;
		joystick_val = 0;
		joystick_prev_val = KBD_RIGHT;
		speed = 15;
		size = 2;
		xpos = 5;
		ypos = 10;
		GLCD_Clear(Black);
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Green);
		GLCD_DisplayString(2,0,1,"Select Border mode: ");
		GLCD_DisplayString(4,0,1,"-->  ON             ");
		GLCD_DisplayString(5,0,1,"     OFF            ");
		GLCD_DisplayString(29,0,0,"          press joystick to select the mode           ");
		while(modesel == 1){
			joy_difficulty = get_button();
			switch(joy_difficulty){
				case KBD_DOWN:
					GLCD_DisplayString(4,0,1,"     ON             ");
					GLCD_DisplayString(5,0,1,"-->  OFF            ");
					mode = 2;
					break;
				case KBD_UP:
					GLCD_DisplayString(4,0,1,"-->  ON             ");
					GLCD_DisplayString(5,0,1,"     OFF            ");
					mode = 1;
					break;
				case KBD_SELECT:
					if(mode == 1)
						border = 1;
					if(mode == 2)
						border = 0;
					modesel = 0;
					GLCD_Clear(White);
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					break;
			}
		}
		setbody();
		food();	
		while(collision == 0){
			joystick_val = get_button();
			direction(joystick_val);
			sprintf(str,"     score:[%d]",score);
			GLCD_DisplayString(0,0,0,(unsigned char *)str);	
		}
		if(collision == 1){
			GLCD_Clear(Red);
			GLCD_SetBackColor(Red);
			GLCD_SetTextColor(White);
			if(score>=highscore)
					highscore = score;
			sprintf(scores,"     [SCORE: %d]           [HIGH SCORE: %d]",score,highscore);
			GLCD_DisplayString(0,0,0,(unsigned char *)scores);
			GLCD_DisplayString(2,0,1,"      GAME OVER     ");
			GLCD_DisplayString(7,0,1,"Wanna try again? :P ");
			GLCD_DisplayString(8,0,1," --> YES");
			GLCD_DisplayString(9,0,1,"     NO ");
			gameover = 0;
			while(gameover == 0){
				joy_try = get_button();
				switch(joy_try){
					case KBD_DOWN:
						GLCD_DisplayString(8,0,1,"     YES");
						GLCD_DisplayString(9,0,1," --> NO ");
						tryagain = 0;
						break;
					case KBD_UP:
						GLCD_DisplayString(8,0,1," --> YES");
						GLCD_DisplayString(9,0,1,"     NO ");
						tryagain = 1;
						break;
					case KBD_SELECT:
						if(tryagain == 0){
							GLCD_Clear(White);
							done =1;
							return 0;
						}
						if(tryagain == 1){
							gameover = 1;
							collision = 0;
							border = 0;
							clearsnake();
						}
						break;
				}
			}
		}
		delay(5);
	}
	return 0;
}