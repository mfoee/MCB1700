#ifndef ___PHOTO_VIEWER_H
#define ___PHOTO_VIEWER_H
extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned char face[];
extern unsigned char fish[];
extern unsigned char artsy[];
extern void display_image(int which); 
extern void photo_viewer(int mode);
#endif
