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
// #include "PHOTO_VIEWER.h"
#include "usbdmain.h"
#include "game_on.h"
#include "snake.h"

extern int audio_main (void);


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) 
{  	/* Main Program                       */
	int selector = 0; //selector to see which program is user the choosing
										//'1' for photo viewer, '2' for audio file, '3' for game
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();

  LED_Init ();
  GLCD_Init();

  GLCD_Clear  (White);
  SysTick_Config(SystemCoreClock/100); 
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
	GLCD_DisplayString (0, 0, 1, "COE 718 Project Demo");
	GLCD_DisplayString (1, 6, 1, "MAIN MENU");

  for (;;)		//loop forever
	{
			joystick_val = get_button();	//read the joystick
			
			if (joystick_val !=joystick_prev)	//this means that the user used the joystick
			  {
					if (joystick_val == KBD_DOWN)
						{
							selector +=1;	//we are have only 3 modes so only 3 values of selector
							selector = selector %5;		//are accepted, overflow is mapped back to beginning
						}
					else if (joystick_val == KBD_UP)
						{
							selector -=1;
							if (selector <=0)
									selector = 1;
						}
					else if(joystick_val == KBD_RIGHT)
							{
								if (selector == 1)
								{
									//photo_viewer(1);	//start the photo viewer function
									selector = 0;
								}
								else if (selector == 2)
								{
									audio_main();
									delay(10);
									GLCD_Clear(White);
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Blue);
									GLCD_DisplayString (0, 0, 1, "COE 718 Project Demo");
									GLCD_DisplayString (1, 6, 1, "MAIN MENU");									
									selector =0;
								}
								else if (selector == 3)
								{
									GLCD_Clear(White);
									game_on();	//after the game is done, we reset the LCD to display the main menu
									GLCD_Clear(White);
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Blue);
									GLCD_DisplayString (0, 0, 1, "COE 718 Project Demo");
									GLCD_DisplayString (1, 6, 1, "MAIN MENU");
									selector = 0;
								}
								else if(selector ==4)
								{
									GLCD_Clear(White);
									snakegame();
									GLCD_Clear(White);
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Blue);
									GLCD_DisplayString (0, 0, 1, "COE 718 Project Demo");
									GLCD_DisplayString (1, 6, 1, "MAIN MENU");
									selector = 0;
								}
							}
					joystick_prev = joystick_val;	//we good
			}
			
			/*this is to update the LCD each time the user interfaces with the joystick*/
			if (selector == 1)	//for displaying now
				{
						GLCD_SetBackColor(Red);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(4,0,1, "Photo Viewer");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "Audio");
						GLCD_DisplayString(6,0,1, "MEME-MORY");
						GLCD_DisplayString(7,0,1, "SNAKE");
						
				}
			else if(selector == 2)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "Photo Viewer");
						GLCD_SetBackColor(Red);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(5,0,1, "Audio");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "MEME-MORY");
						GLCD_DisplayString(7,0,1, "SNAKE");
				}
			else if(selector == 3)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "Photo Viewer");
						GLCD_DisplayString(5,0,1, "Audio");
						GLCD_SetBackColor(Red);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(6,0,1, "MEME-MORY");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(7,0,1, "SNAKE");
				}
			else if (selector == 4)
			{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "Photo Viewer");
						GLCD_DisplayString(5,0,1, "Audio");
						GLCD_DisplayString(6,0,1, "MEME-MORY");
						GLCD_SetBackColor(Red);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(7,0,1, "SNAKE");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
			}
			else
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "Photo Viewer");
						GLCD_DisplayString(5,0,1, "Audio");
						GLCD_DisplayString(6,0,1, "MEME-MORY");
						GLCD_DisplayString(7,0,1, "SNAKE");
				}
	}
}


