#include <stm32f031x6.h>
#include "display.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "musical_notes.h"
#include "sound.h"
#include "serial.h"

// #include <stm32l031xx.h>


//declaring my functions
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
int isTouching(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void mainmenu();


//creating the sprites 
volatile uint32_t milliseconds;
const uint16_t orangeBrick[]=
{
	6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,6691,
};
const uint16_t greenBrick[]=
{
	9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,
};
const uint16_t redBrick[]=
{
	40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,40224,
};
const uint16_t yellowBrick[]=
{
	24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,
};
const uint16_t rectangle[]=
{
	0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,0,
};
const uint16_t ball1[]=
{
	0,24327,24327,24327,0,24327,24327,65535,65535,24327,24327,24327,24327,65535,24327,24327,24327,24327,24327,24327,0,24327,24327,24327,0,
};
// const uint16_t deco1[]=
// {
// 	0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0,
// };
// const uint16_t deco2[]= 
// 	{
// 	0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
// 	};
// const uint16_t deco3[]= 
// {
// 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
// };
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};
//creating the structure for the bricks
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int isHit;
	char color;
} Brick;


//int main
int main()
{

	//declaring the variables
	int lives = 3;//lives
	int hinverted = 0;//x ball direction 
	int vinverted = 0;//y ball direction
	int show = 0;//show the ball
	int showrect =0;//show the rectangle
	int score = 0;//score -- GAME STATUS
	int hmoved = 0;// x moved
	int vmoved = 0;//y moved
	uint16_t x =20;//ball x coordinate
	uint16_t y = 100;//ball y coordinate
	uint16_t oldx = x;//old ball x coordinate
	uint16_t oldy = y;//old ball y coordinate
	uint16_t rectx = 50;//rectangle coorindate x
	uint16_t recty = 128;//rectangle coordinate y
	uint16_t oldrectx  = rectx;//old rectangle x coordinate
	uint16_t oldrecty = recty;//old rectangle y coordinate
	uint16_t box = 3;//x beginning coordinate of bricks
	uint16_t box1y= 38;//y coorindate of bricks

	//initialising 
	uint16_t rectSpeed;
	initClock();//initialise clock
	initSysTick();//initalise sys tick
	
	setupIO();

	//setting output pins -- additional output -> LED's
	pinMode(GPIOB,0,1);//setting red led port A bit 2 as an output
	pinMode(GPIOA,9,1);//setting yellow led port A bit 9 as an output
	pinMode(GPIOA,10,1);//setting green led port B bit 3 as an output

	initSound();//initialise sound
	initSerial();
	
	//declare some variables
	int menu = 1;//1 so it starts off in the menu
	int pause =0;
	int power = 0;
	
	mainmenu();//main menu function 
	delay(100);
	hinverted = vinverted = 6;//setting the x and y speed of the ball
	rectSpeed = 10;//setting the rectangle speed

	//creating an array of brick structures
	Brick bricks[16];
	
    //setting the original y coordinate for the bricks
    int yStart = 5;
    


	//put the positions of the bricks in an array
   	for (int i = 0; i < 16; i+=4) {
		int xStart = 3;
		
        bricks[i].x = xStart;
        bricks[i].y = yStart;
        bricks[i].width = 30;
        bricks[i].height = 10;
        bricks[i].isHit = 0;
		// bricks[i].color = colorArr[i/4];


		bricks[i+1].x = xStart+31;
        bricks[i+1].y = yStart;
        bricks[i+1].width = 30;
        bricks[i+1].height = 10;
        bricks[i+1].isHit = 0;
		// bricks[i+1].color = colorArr[i/4];


		bricks[i+2].x = xStart+62;
        bricks[i+2].y = yStart;
        bricks[i+2].width = 30;
        bricks[i+2].height = 10;
        bricks[i+2].isHit = 0;
		// bricks[i+2].color = colorArr[i/4];


		bricks[i+3].x = xStart+93;
        bricks[i+3].y = yStart;
        bricks[i+3].width = 30;
        bricks[i+3].height = 10;
        bricks[i+3].isHit = 0;
        // bricks[i+3].color = colorArr[i/4];
		
		yStart += 11;

    }
	char input = 0;
	int inputtingChar =0;//to make sure a char is only pressed once 
	//begin the while loop for the game
	while(1)
	{	


		//serial communications, remote control getting a char from the keyboard waiting in the background rather than foreground
		if(dataavailable()){
			
			input = egetchar();
			inputtingChar = 1;
		}
		//set the LED's based on the amount of lives
		if (lives == 3){
			GPIOB->ODR |= (1 << 0);//Set PA2 high (turn on red LED) 
			GPIOA->ODR |= (1 << 10); // Set PB3 high (turn on Green LED)
			GPIOA->ODR |= (1 << 9); // Set PA9 high (turn on Yellow LED)
		}
		else if(lives == 2){
			GPIOA->ODR &= ~(1 << 10);//Set PA2 low (turn off red LED) 
			GPIOA->ODR |= (1 << 9); // Set PA9 high (turn on Yellow LED)
			GPIOB->ODR |= (1 << 0); // Set PB3 high (turn on Green LED)
			
		}
		else if(lives ==1){
			GPIOA->ODR &= ~(1 << 10);//Set PA2 low (turn off red LED)
			GPIOA->ODR &= ~(1 << 9); // Set PA9 low (turn off Yellow LED) 
			GPIOB->ODR |= (1 << 0); // Set PB3 high (turn on Green LED)
			
		}
		else if(lives == 0){//GAME OVER
			GPIOA->ODR &= ~(1 << 10);//Set PA2 low (turn off red LED) 
			GPIOA->ODR &= ~(1 << 9); // Set PA9 low (turn off Yellow LED)
			GPIOB->ODR &= ~(1 << 0); // Set PB3 low (turn off Green LED)

			fillRectangle(0,0,200,200,0);//clear screen
			printTextX2("GAME OVER",16,50,RGBToWord(0xff,0xff,0xff),0);//display game over
			//reset everything
			hmoved = vmoved = 0;
			show = 0;//show ball
			showrect =0;//show rectangle
			score =0;//score
			lives = 3;//reset
			//setting the bricks to original form
			for (int i = 0; i < 16; i++) {
				bricks[i].isHit =0;
			}
			delay(2000);
			mainmenu();//main menu function
			
		}
		//print the score
		printNumber(score, 80, 140, RGBToWord(0xff,0xff,0xff), 0);
		
	
		if((GPIOA->IDR & (1 << 8)) == 0 &&  (GPIOA->IDR & (1 << 11)) == 0){//EASTER EGG
			//debugging
			// printText("Both up and down", 20, 50, RGBToWord(0xff,0xff,0), 0);

			
			power= 1;//special power to clear all of the bricks when you press
			
		}
		if(power){
			for (int i = 0; i < 16; i++) {
				delay(20);
				if (!bricks[i].isHit && isTouching(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height, x, y)) {
					// // Ball hits the brick
					
					vinverted = vinverted * -1;//invert the y directino
					for(int j=0;j<16;j++){
						fillRectangle(bricks[j].x, bricks[j].y, bricks[j].width, bricks[j].height, 0);  // Erase brick
						
						bricks[j].isHit = 1;  // Mark the brick as hit
						
						}
					score = 16;//score set to finished
					

					}
			}
			
		}

		//if user finishes the game - GAME STATUS
		if(score == 16){
			score = 0;
			delay(500);
			//display the text when you win
			printText("You Win!", 20, 40, RGBToWord(0xff,0xff,0xff), 0);
			printText("Press DOWN to", 20, 50, RGBToWord(0xff,0xff,0xff), 0);
			printText("Restart!", 20, 60, RGBToWord(0xff,0xff,0xff), 0);
			//play the melody
			playNote(C4);
			delay(500);
			playNote(F4);
			delay(500);
			playNote(C3);
			delay(500);
			playNote(C3);
			delay(500);
			playNote(0);
			//dont show ball or rectangle
			show = 0;
			showrect = 0;
			power =0;
			
		}

		//up pressed
		if (((GPIOA->IDR & (1 << 8)) == 0 && (GPIOA->IDR & (1 << 11)) != 0) || ((input == 'w') && (inputtingChar == 1))) {
			pause=1;//pause the game
			show =0;//dont move ball
			showrect=0;//dont move rectangle
			delay(200);
			//display text
			printText("PAUSED", 40, 50, RGBToWord(0xff,0xff,0xff), 0);
			printText("UP to UNPAUSE",20,60,RGBToWord(0xff,0xff,0xff), 0);
			printText("DOWN to QUIT", 25, 70, RGBToWord(0xff,0xff,0xff), 0);
			inputtingChar = 0;
		}
			//pause the game
		while(pause){
			if(dataavailable()){
			
				input = egetchar();
				inputtingChar = 1;
			}
			//if up is pressed unpause
			if (((GPIOA->IDR & (1 << 8)) == 0 && (GPIOA->IDR & (1 << 11)) != 0) || ((input == 'w')) && (inputtingChar == 1)) {
				fillRectangle(20,50,100,60,0);
				delay(100);
				show = 1;
				showrect =1;
				pause =0;
				inputtingChar =0;
			}
			delay(50);
			//if down is pressed, unpause and quit
			if ((((GPIOA->IDR & (1 << 8)) != 0 && (GPIOA->IDR & (1 << 11)) == 0) || (input == 's')) && (inputtingChar == 1)){
				pause = 0;
				menu =1;
				inputtingChar =0;
			}
			
			
		}
		//reset everything quit the game.
		if (((GPIOA->IDR & (1 << 11)) == 0 && (GPIOA->IDR & (1 << 8)) != 0) || ((input == 's') && (inputtingChar == 1))) // down pressed
		{	
			menu =1;
			
			mainmenu();//main menu functino
			delay(100);
			//set ball movement to 0
			hmoved = vmoved = 0;
			show = 0;
			showrect =0;
			score =0;

			for (int i = 0; i < 16; i++) {
				bricks[i].isHit =0;
			}
			inputtingChar =0;
			
			
		}
		//while we are in the menu
		while(menu){
				delay(100);
				if(dataavailable()){
			
					input = egetchar();
					inputtingChar = 1;
				}

				//if down is pressed go to game
				if ((((GPIOA->IDR & (1 << 8)) != 0 && (GPIOA->IDR & (1 << 11)) == 0) || ((input == 's')) && (inputtingChar == 1)))  // down pressed
				{	
					playNote(1000); //play note when button pressed
					
					delay(500);
					playNote(0); // Play a note for 500ms
					menu =0;//set menu to 0 to exit menu loop
					fillRectangle(0,0,200,200,0);//clear screen
					delay(100);
					//set the correct numbers for the ball speed and rectangle speed etc
					vinverted = 6;
					x = 50;
					y =50;
					rectx =50;
					recty = 128;
					show = 1;
					showrect =1;
					putImage(rectx,recty,35,5,rectangle,0,0);//display the rectangle
					box =3;//starting x position for bricks
					
					for(int i =0;i<4;i++){//loop to display the bricks
						
						putImage(box,box1y-33,30,10,redBrick,0,0);
						putImage(box,box1y-22,30,10,yellowBrick,0,0);
						putImage(box,box1y-11,30,10,greenBrick,0,0);
						putImage(box,box1y,30,10,orangeBrick,0,0);

						box = box+31;
					}
					inputtingChar = 0;
					
					
				}

			}
		

		//THE RECTANGLE MOVES
		//if showrect =1
		if(showrect){
			if(rectx >5){//if x is more than 5
				if ( (GPIOB->IDR & (1 << 4)) )//left
				{
					//move the rectangles x coorinate by the speed
					rectx = rectx -rectSpeed;
					hmoved = 1;
					}
				
				
			}
			if(rectx <90){//if x is less than 90
				if (GPIOB -> IDR & (1<<5))//right
				{
					//move rectangle x coordinate by the speed which is 10
					rectx = rectx +rectSpeed;
					hmoved = 1;	
					}
			}
		}
		if(inputtingChar && showrect){
			
				if(rectx >5){//if x is more than 5
					if ( input == 'a' )//left
					{
						//move the rectangles x coorinate by the speed
						rectx = rectx -rectSpeed;
						hmoved = 1;
						}
				
				
				}
			if(rectx <90){//if x is less than 90
				if (input == 'd')//right
				{
					//move rectangle x coordinate by the speed which is 10
					rectx = rectx +rectSpeed;
					hmoved = 1;	
					}
			}
			inputtingChar = 0;
			
		}
		
		//if the rectangle is moved, clear the old coordiantes of the rectangle 
		if(hmoved){
			fillRectangle(oldrectx,oldrecty,35,5,0);
			oldrectx = rectx;
			oldrecty = recty;
			
		}
		//if showrect is active(menu isnt on or pause isnt on) draw the image of the new rectangle
		if(showrect){
			putImage(rectx,recty,35,5,rectangle,0,0);
			delay(100);
		}
		
		
		//if the ball is touching the rectangle
		if (isTouching(rectx, recty, 35, 5, x, y) || isTouching(rectx, recty, 35, 5, x + 5, y + 5)) {
			// The ball is in contact with the rectangle

			//play a little note
			playNote(700);
			delay(100);
			playNote(0);

			//using a random number from 1-7 to use to invert the y velocity randomised
			double randomNumber = (rand() % 7) +3;
			vinverted = -randomNumber;

			power =0; //setting the super power to 0 so 
			}

		//looping through the array of bricks and checking if the brick is unhit & if the ball is inside the brick
		for (int i = 0; i < 16; i++) {

			if (!bricks[i].isHit && isTouching(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height, x, y)) {
				// Ball hits the brick
				GPIOB->ODR &= ~(1 << 0);//Set PA2 high (turn on red LED) 
				GPIOA->ODR &= ~(1 << 10); // Set PB3 high (turn on Green LED)
				GPIOA->ODR &= ~(1 << 9); // Set PA9 high (turn on Yellow LED)
				delay(50);

				GPIOB->ODR |= (1 << 0);//Set PA2 high (turn on red LED) 
				GPIOA->ODR |= (1 << 10); // Set PB3 high (turn on Green LED)
				GPIOA->ODR |= (1 << 9); // Set PA9 high (turn on Yellow LED)
				
				playNote(800);
				delay(100);
				playNote(0);

				score++;//increment score

				vinverted = vinverted * -1;//invert the y direction

				fillRectangle(bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height, 0);  // Erase brick

				bricks[i].isHit = 1;  // Mark the brick as hit
				

				}
		}	
		//if show is active than move the ball
		if(show){

			if (x < 3 || x > 120) {//HITS ONE OF THE SIDE WALLS

				//invert the ball x direction
				hinverted = hinverted * -1;
				x = x + 1 * hinverted;
				y = y + 1 * vinverted;

				vmoved = 1;
			}

			else if (y < 5) {//HITS THE TOP

				//invert the ball x direction
				vinverted = vinverted * -1;
				y = y + 1 * vinverted;
				x = x + 1 * hinverted;

				vmoved = 1;
			}

			else if(y > 150){//LOSE THE BALL OUT OF BOUNDS

				lives = lives-1;//if ball goes through the bottom lose a life

				fillRectangle(oldx,oldy,5,5,0);//remove the sprite of the ball
				//set the old x and y coordinates

				oldx = x;
				oldy = y;

				show = 0;//set show to 0 to not show or move the ball
				x=50;//reset the x axis for the ball
				y=50;//reset the y axis for the ball
				putImage(x,y,5,5,ball1,0,0);// but the ball back up at the top

				oldx =x;
				oldy =y;

				fillRectangle(oldx,oldy,5,5,0);//fill rectangle from the old x position
				
				delay(100);
				show=1;//show the ball
			}
			else {//THE BALL IS MOVING IN PLAY
				//increase ball x and ball y by the speed
				x = x + 1 * hinverted;
				y = y + 1 * vinverted;
				vmoved = 1;
			
			}
		}
		
		if ((vmoved))//WHILE THE BALL IS MOVING
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,5,5,0);
			oldx = x;
			oldy = y;
		}
		if(show){//WHILE THE BALL HASNT GONE OUT OF PLAY
			putImage(x,y,5,5,ball1,0,0);
			delay(100);
		}
	}

	return 0;
}
//sysTick
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
//systick_handler
void SysTick_Handler(void)
{
	milliseconds++;
}
//init clock
void initClock(void)
{
	
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
//delay function
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
//if is inside
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}
//if is touching
int isTouching(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
    // checks to see if point px, py touches or overlaps the rectangle defined by x1, y1, w, h
    uint16_t x2, y2;
    x2 = x1 + w;
    y2 = y1 + h;
    int rvalue = 0;

    // Check if the point is within or on the boundaries of the rectangle
    if ((px >= x1 && px <= x2) && (py >= y1 && py <= y2)) {
        rvalue = 1;
    }

    return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
//main menu function to clear the screen and display the text
void mainmenu(){
	
	fillRectangle(0,0,200,200,0);
	printTextX2("MAIN MENU",10,10,RGBToWord(0xff,0xff,0xff),0);
	printText("Welcome To", 30, 40, RGBToWord(0xff,0xff,0), 0);
	printText("Breakout!", 32, 50, RGBToWord(0xff,0xff,0), 0);
	printText("Press DOWN", 30, 80, RGBToWord(0xff,0xff,0), 0);
	printText("To Continue!", 26, 90, RGBToWord(0xff,0xff,0), 0);
}

