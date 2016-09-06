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
                  
#include <LPC17xx.H> /* LPC17xx definitions */
#include "PHOTO_VIEWER.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"



extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

// extern unsigned char face[];
// extern unsigned char face2[];
// extern unsigned char fish[];
// extern unsigned char artsy[];



void display_image(int which)		//function for displaying image
{
	int delay = 0;
	if (which==0)
	{
		GLCD_Clear(Black);
		//GLCD_Bitmap(80,60,160,120,face2);
	}
	else if (which==1)
	{
		GLCD_Clear(Black);
		//GLCD_Bitmap(80,60,160,120,fish);
	}
	else if (which==2)
	{
		GLCD_Clear(Black);
		//GLCD_Bitmap(80,60,160,120,artsy);
	}
}

void zoom_in(unsigned char *picture_ptr)
{
	int joystick_prev = get_button();
	int joystick_val = get_button();
	int x_offset=40;
	int y_offset=30;
	int exit=0;
	GLCD_BitmapZoom(x_offset,y_offset,4,picture_ptr);
	while(!exit)
	{
		joystick_val = get_button();
		if (joystick_val !=joystick_prev)
		{
			if (joystick_val == KBD_LEFT)
			{
				if (x_offset >=10)
					x_offset = x_offset - 10;
				GLCD_BitmapZoom(x_offset, y_offset, 4, picture_ptr);
			}
			else if (joystick_val == KBD_RIGHT)
			{
				if (x_offset <= 70)
					x_offset = x_offset + 10;	//maximum value of x_offset can only be 120
				GLCD_BitmapZoom(x_offset, y_offset, 4, picture_ptr);
			}
			else if (joystick_val == KBD_UP)
			{
				if (y_offset <=50)
					y_offset = y_offset + 10;			//maximum value of y_offset can only be 50
				GLCD_BitmapZoom(x_offset, y_offset, 4 ,picture_ptr);
			}
			else if(joystick_val == KBD_DOWN)
			{
				if (y_offset >10)	//minimum value of y_offset is 0
					y_offset = y_offset - 10;
				GLCD_BitmapZoom(x_offset, y_offset, 4, picture_ptr);
			}
			else if (joystick_val == KBD_SELECT)
			{
				GLCD_BitmapFull(picture_ptr);
				exit =1;
			}
			joystick_prev = joystick_val;
		}
	}
	return;
}

void photo_viewer (int mode)
{	
	int zoom=0;
	int pic     =  0, DELAY = 0;		//variable that saves which picture to display
	int timer   =  0;
	unsigned char *picture_ptr =0;
	int joystick_prev = get_button();
	int joystick_val = get_button();
	counter =0;
	display_image(pic);
	while (timer <1)		//if the joystick has not pressed twice, we stay on photo viewer
	{	
		joystick_val = get_button();		//read the joystick
		if (joystick_val != joystick_prev)	//if sth change, then know what change it was
			{
				if (joystick_val == KBD_RIGHT)
					{
							pic = pic+1; //increment pic
							pic = pic%3;	//we are only displaying 3 pictures
						  display_image(pic);	//display whatever image
							zoom = 0;
					}
				else if (joystick_val ==KBD_LEFT)
					{
						pic = pic-1; //decrement pic
						if (pic < 0)
								pic = 2;			
						display_image(pic);	//display whatever image
						zoom = 0;
					}
				else if (joystick_val == KBD_UP)
					{
						
// 						if (zoom ==0)
// 						{
// 							if (pic==0)
// 									//picture_ptr = &face2[0];
// 							else if (pic==1)
// 									//picture_ptr =&fish[0];
// 							else if (pic==2)
// 								//picture_ptr = &artsy[0];
// 							GLCD_BitmapFull(picture_ptr);
// 							zoom =1;
// 						}
// 						else if (zoom ==1)
// 						{
// 							zoom_in(picture_ptr);
// 							zoom = 0;
// 						}
					}
				else if (joystick_val ==KBD_SELECT)
				{
					timer ++;
				}
			  joystick_prev = joystick_val;
				counter =0;
		}
		if (counter >= 50)	//for slide show purposes
		{
			pic = pic+1; //increment pic	
			pic = pic%3;	//we are only displaying 3 pictures
			display_image(pic);
			counter =0;
			zoom = 0;
			
		}	
	}
	timer = 0;	//before going out, set the timer back to zero
	GLCD_Clear(White);
	GLCD_DisplayString (0, 0, 1, "COE 718 Project Demo");
	GLCD_DisplayString (1, 6, 1, "MAIN MENU");
}





