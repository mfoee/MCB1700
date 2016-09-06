/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher and Graphic Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "usbdmain.h"
#include <math.h>

extern unsigned char all[];		//this array contains all the bitmap images for the game (30 bitmap images in total)
															//the first bitmap image has an id = 0; 2nd bitmap image has an id = 1, etc
															//the starting address o the 11th bitmap image is (all+ (11*length)) where length
															//represents the size of each of the image
															
extern unsigned char facedown[];	//this is the bitmap image for the facedown card

#define height 25
#define width 25
#define length 1250
#define x_space 1
#define y_space 5
#define y_pitch 30
#define x_pitch 26

#define x_origin 5	//this is ((320 - (10*width))/2 we want to start displaying at the middle of the row 
#define y_origin 40		//we are gonna dispalay the timer on the top of LCD so make space for it

unsigned int shuffle[60];	//this array contains the 'id' of each of the cards

int facedown_cards(int row)	//row is the number of rows to display
{
	int x=0;		//the x coordinate where the bitmap will be displayed (starting)
	int y=0;	//the y coordinate where the bitamp will be displayed
	GLCD_Clear  (White);
	
	//display 10 items for the first row
	
	for (y=0; y<=(row-1); y++)
	{
		for (x=1; x<=10; x++)	//we are displaying 10 items per row
			{
				GLCD_Bitmap( (x_origin + (x*x_pitch)), (y_origin+(y * y_pitch)), height, width, facedown);
			}
			
	}
	return 0;
}

int create_level(int level)
{
	int max;
	int x=0;	//x,y and i are used for initializing and shuffling the shuffle[60];
	int y=0;	
	int i=0;
	int max2=0;
	double delay;	//this is used for delay
	if (level == 1)
				max=19;	//we can only have 20 cards
	else if(level == 2)
				max=39;	//we can only have 40 cards
	else if (level== 3)
				max=59;	//we can only have 40 cards
	
	max2 = ((max+1)/2)-1;
	 
	for (i=0; i<=max2; i++)	//this routine initialized the value of the array
	{
			shuffle[x] = i; //shuffle[0]= shuffle[1] = 0; shuffle[2] = shuffle[3] =1, shuffle[4]=shuffle[5]=2.... etc
			shuffle[x+1] = i;
		  x+=2;
	}
	while (x<=59)		//the rest of the array will be given on an illegal value
	{
		shuffle[x]=-1;
		x++;
	}
	
	//at this point the 'id' of each card will be on the shuffle[0] exactly twice
	//however, these ids are right beside each other, therefore we need to shuffle it

	for (i=0; i<=max; i=i++)	//the following routine shuffle the array 
	{
		x=0;	//just refresh these temporary values
		y=0;
		x = abs((rand()%max2)); //this will generate a number from 0 to 29
		y = shuffle[i]; //y temporarily holds the value of shuffle[i]
		shuffle[i] = shuffle[x];
		shuffle[x] = y;		//shuffle [i] and shuffle[x] switches contents
	}
	
	
	/*each card is represented by an 'id', its location on the shuffle[60] is related to its location in the pool of cards*/
		
		/*the following routine will display the cards on the LCD*/
 
	GLCD_Clear  (White);
	i=0;
	
	x = x_origin;
	y = y_origin;
	for (y=0; y<=(level*2 - 1); y++)	//if we choose level 1, then we display 2 rows, row 0 and row 1
	{
		for (x=1; x<=10; x++)	//we are displaying 10 items per row
			{
				GLCD_Bitmap( (x_origin + (x*x_pitch)), (y_origin+(y * y_pitch)), height, width, (all+(shuffle[i]*length)));
				i++;
			}
			
	}
	for (delay=0xFFFFF;delay>=0;delay--){}

	facedown_cards(level*2);	//after a brief period has expired, turn the cards down
	return 0;
				
}


/*the following commands are for user interface*/
void draw_line(int x, int y)	//this command will clear a line on location x and y, 																		//z = 0 if we wanna clear it, z = 3 if we wanna write
{
	int a=0;
	GLCD_SetTextColor(Red);
	for (a=0; a<=25; a++)
		{
			GLCD_PutPixel((x+a), y+25-2);
			GLCD_PutPixel((x+a), y+1+25-2);
		}
}


void clear_line(int x, int y)	//this command will clear a line on location x and y, 
																		//z = 0 if we wanna clear it, z = 3 if we wanna write
{
	int a=0;
	GLCD_SetTextColor(White);
	for (a=0; a<=25; a++)
		{
			GLCD_PutPixel((x+a), y+25-2);
			GLCD_PutPixel((x+a), y+1+25-2);
		}
}

/*this routine will makle the pool of cards to face down*/

int start_game(int lvl)
{
	char text[20];		//the text will be used to display the current score*/ 
										/*a score is awarded each time the player succesfully pair two cards*/
	int done = 0;
	int joystick_val = get_button();   //track the current joystick value
	int joystick_prev = get_button();  //track the previous value for the joystick
	
	int chosen = 0;		//this variable will track the 'id' of the card that the player has currently selected
	int chosen_previous = 0;	//this variable will track the 'id' of the card that the player has previously selected
		
	int x_chosen = 0;	//this will track the x and y coordinates of the card that the player has selelcted
	int y_chosen = 0;
	int x_chosen_prev = 0;	//this will track the x and y coordinates of the card that the player has previously selelcted
	int y_chosen_prev = 0;
	
	int level = 2*lvl*10;
	int turn = 1;	//this will track the number of turn the player has performed
	
	int score = 0;	/*a score is awarded each time the player succesfully pair two cards*/
	
  int selector = 0;	//this will track where the cursor is, if the cursor is on the 10th card, the selector = 10
	int selector_previous = -1;	//the player picked the 10th card, the selector previous =10
	
	int x=x_origin+x_pitch;	//these hold the coordinates of where to put the cursor
	int y=y_origin;				//initialize to point at the beggining of the pool of cards
	while(done ==0)	//while we are not done
		{
			//we are polling the joystick with its current value
			draw_line(x,y);	//cursor for player to see where he is
			joystick_val = get_button();	
			if (joystick_val !=joystick_prev)	//this means that the user used the joystick
			  {
						clear_line(x,y);	//we erase the line that is previously drawn
						if (joystick_val == KBD_DOWN)
							{
								selector +=10;
								if( shuffle[selector]==-1)
									{
										selector = selector % 10;
										y=y_origin - y_pitch;
									}
								y=y+y_pitch;	//we increment the value of y since the cursor should move down
							}
					
						else if (joystick_val == KBD_UP)
						{
								selector -=10;
								if( selector <=-1 || shuffle[selector]==-1)
									{
										selector = selector + level;
										y=y_origin + ((level/10)*y_pitch);
									}
								y=y-y_pitch;	//we decrement the value of y since the cursor should move up
						}
					
						else if(joystick_val == KBD_RIGHT)
						{
								selector +=1;
								if(shuffle[selector] == -1)	//if the selector has been chosen to be more than 60
										{
											selector = 0;	//we map it back to the beginning of the array
											x = x_origin;	//also the cursor needs to be maped back to teh beginning of the array
											y = y_origin;			
										}
								x=x+x_pitch;		//we update the location x coordinate of the cursor
									
								if (x > (x_origin + (10*x_pitch)))	//if we covered the entire row
									{
										x = x_origin+x_pitch;
										y = y+y_pitch;	//then we move to the first item of the next row	
									}			
						}
					
						else if(joystick_val == KBD_LEFT)
						{
							selector = selector-1;
						
								if(shuffle[selector] == -1)	//if we selector has been chosen to be more than 60
									{
										selector = (level-1);
										x = x_origin + (11*x_pitch);
										y = y_origin + (5*y_pitch);			
									}	
								x=x-x_pitch;			
								if (x < (x_origin+x_pitch))	//if we covered the entire row
									{
										x = x_origin + (10*x_pitch);
										y = y-y_pitch;	//then we move to the first item of the previous row	
									}
						}
					
						else if(joystick_val == KBD_SELECT)	//this mean that we choose a card
						{
							while (selector != selector_previous )	//if he did not choose the same card he chose previosly then the moved is 'valid'
							{																																//selection will also be invalid if the player chose a card that is already paired up
								chosen = shuffle[selector];	//we take the id that the selector is currently pointing
								x_chosen = x;							//we take the x and y coordinate of the cursor
								y_chosen =y;
								GLCD_Bitmap(x_chosen,y_chosen,width,height,all+(chosen * length));		//we display what was chosen
						
								if (turn ==1)		//if it is just the first turn
								{
									chosen_previous = chosen;		//we just display it and save that as previosly chosen
									x_chosen_prev =x_chosen;
									y_chosen_prev = y_chosen;
									turn = 2;					//then the next card that he will choose will be the second card
								}
							
								else if (turn == 2)	//if the player chose a second card, (one card was already facing up)
								{
									if(chosen_previous == chosen)	//then we check if the two cards are a pair
										{
			
											turn = 1;		//if it was a match,then the player can choose the first card again, 
											score++;		//and we increase the score
										}
									else		//if it wasn a match
										{
											turn = 2;	//he will have to chose another card
											GLCD_Bitmap(x_chosen_prev,y_chosen_prev,width,height,facedown);	//then we turn down the card the he chose previously
											chosen_previous = chosen;		//we save it as previosly chosen
											x_chosen_prev =x_chosen;
											y_chosen_prev = y_chosen;
										}
									
								}	
							
								selector_previous=selector;	//we update the value of the previous_selector
							}
							selector_previous=selector;
						}	//end of else if (KBD_SELECT)
					
					
						joystick_prev = joystick_val;	//we just update the previous value of the joystick
						sprintf(text ,"score:%d",score);	//and display teh score
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Red);
						GLCD_DisplayString(0,6,1, (unsigned char*) text);				
					
					} //end of (JOYSTICK_CURERENT != JOYSTICK_PREVIOUS)
				if (score >= lvl*10)
						done =1;
				}	//end of while(done==0)
		return done;
}	//end of for loop



/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int game_on (void) 
{  	/* Main Program                       */
	int joystick_val = get_button();   //track the current joystick value
	int joystick_prev = get_button();  //track the previous value for the joystick
	int selector = 0;
	int level = 0;
	int done=0;
	int delay=0;

 // KBD_Init();	//initialize the joystick
 // GLCD_Init();	//initialze the GLCD
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	GLCD_DisplayString (0,6, 1, "MEME-MORY");
	GLCD_DisplayString (2,4,1,"Select level");
	
	GLCD_DisplayString (4,7,1,"Level 1");
	GLCD_DisplayString (5,7,1,"Level 2");
	GLCD_DisplayString (6,7,1,"Level 3");

	while (!done)
	{
		GLCD_SetBackColor(Black);
		GLCD_DisplayString (0,6, 1, "MEME-MORY");
		GLCD_DisplayString (2,4,1,"Select level");
		
		joystick_val = get_button();	//read the joystick		
		if (joystick_val !=joystick_prev)	//this means that the user used the joystick
			  {
					if (joystick_val == KBD_DOWN)
						{	
							selector +=1;	//we are have only 3 modes so only 3 values of selector
							selector = (selector %4);		//are accepted, overflow is mapped back to beginning
						}
					else if (joystick_val == KBD_UP)
						{
							selector -=1;
							if (selector <=0)
									selector = 3;
						}
					else if(joystick_val == KBD_RIGHT)
						{
							if (selector == 1)
							{
								create_level(1);
								done = start_game(1);
							}
							else if (selector == 2)
							{
								create_level(2);
								done = start_game(2);
							}
							else if (selector == 3)
							{
								create_level(3);
								done = start_game(3);
							}
						}
					GLCD_DisplayString (3, 4, 1,"   ");	
					GLCD_DisplayString (4, 4, 1,"   ");
					GLCD_DisplayString (5,4,1,"   ");
					GLCD_DisplayString (6,4,1,"   ");
					GLCD_DisplayString (7,4,1,"   ");
					GLCD_DisplayString (selector+3,4,1,">>");
					joystick_prev = joystick_val;
				}
		}	//end of while(!done)
		for (delay=0xFFFFFF;delay>=0;delay--){}
		GLCD_Clear(Black);
		GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Red);
		GLCD_DisplayString(2,7,1,"THANK");
		GLCD_DisplayString(3,8,1,"YOU");
		GLCD_DisplayString(4,8,1,"FOR");
		GLCD_DisplayString(5,6,1,"PLAYING");	
		for (delay=0x2FFFFFF;delay>=0;delay--){}
		return 0;
}	//end of MAIN	



