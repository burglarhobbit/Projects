/*
 * FinalProject.c
 *
 * Created: 25-04-2016 12:30:52
 * Author : User
 */ 

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Note that all the values in the coming functions and variables are in terms of cms

float strlenX = 11, strlenY = 23; // Global Variables for accessing string lengths of Stepper Motors X and Y
float lenA = 3.5, lenB = 13.5; // Horizontal and Vertical displacement of pointer w.r.t Stepper Motor X
float lenXY = 24; // Distance between Stepper Motors X and Y

//Previous motor positions and respective arrays of stepvalues of stepper motors
unsigned char prevMotorPosX = 3;
unsigned char prevMotorPosY = 3;
unsigned char stepValuesX[] = {0X90,0XC0,0X60,0X30};
unsigned char stepValuesY[] = {0X09,0X0C,0X06,0X03};

//Explicit function definition
void rotateMotor(float newX, float newY);

// moveXY function will receive variables a and b as positive X displacement and negative Y displacement in cms respectively
// and it will update string lengths of stepper motors X and Y and displacement of pointer according to the Pythagoras theorem.
// Negating the values passed in arguments will result in reverse motion of stepper motors.
void moveXY(float a, float b) {
	float newstrX,newstrY;
	lenA += a;
	lenB += b;
	newstrX = sqrt((lenA*lenA)+(lenB*lenB));
	newstrY = sqrt(((lenXY-lenA)*(lenXY-lenA))+(lenB*lenB));
	rotateMotor(newstrX,newstrY); //calling the function with new string values
}

// According to the new string values received, the rotateMotor function will rotate motors with 4 different cases
// of combination of decreased-increased new motor lengths X and Y w.r.t old motor lengths and with precision of 0.325 cms
// while updating new String lengths and previous motor positions in real-time
void rotateMotor(float newX, float newY) {

	while(newX-strlenX>0&&newY-strlenY>0){
		_delay_ms(100);
		if(newX-strlenX>0.325&&newY-strlenY>0.325) {
			prevMotorPosX = (prevMotorPosX + 1) % 4;
			prevMotorPosY = (prevMotorPosY - 1 + 4) % 4;

			PORTC = stepValuesX[prevMotorPosX]|stepValuesY[prevMotorPosY];
			strlenX += 0.325;
			strlenY += 0.325;
		}
		else if(newX-strlenX>0.325) {
			prevMotorPosX = (prevMotorPosX + 1) % 4;
			PORTC = stepValuesX[prevMotorPosX]|(PORTC&0x0F);
			strlenX += 0.325;
		}
		else if(newY-strlenY>0.325) {
			prevMotorPosY = (prevMotorPosY - 1 + 4) % 4;
			PORTC = stepValuesY[prevMotorPosY]|(PORTC&0xF0);
			strlenY += 0.325;
		}
		else
			break;
	}
	
	while(newX-strlenX<0&&newY-strlenY>0){
		_delay_ms(100);
		if(newX-strlenX<-0.325&&newY-strlenY>0.325) {
			prevMotorPosX = (prevMotorPosX - 1 + 4) % 4;
			prevMotorPosY = (prevMotorPosY - 1 + 4) % 4;

			PORTC = stepValuesX[prevMotorPosX]|stepValuesY[prevMotorPosY];
			strlenX -= 0.325;
			strlenY += 0.325;
		}
		else if(newX-strlenX<-0.325) {
			prevMotorPosX = (prevMotorPosX - 1 + 4) % 4;
			PORTC = stepValuesX[prevMotorPosX]|(PORTC&0x0F);
			strlenX -= 0.325;
		}
		else if(newY-strlenY>0.325) {
			prevMotorPosY = (prevMotorPosY -1 + 4) % 4;
			PORTC = stepValuesY[prevMotorPosY]|(PORTC&0xF0);
			strlenY += 0.325;
		}
		else
			break;
	}
	
	while(newX-strlenX>0&&newY-strlenY<0){
		_delay_ms(100);
		if(newX-strlenX>0.325&&newY-strlenY<-0.325) {
			prevMotorPosX = (prevMotorPosX + 1) % 4;
			prevMotorPosY = (prevMotorPosY + 1) % 4;

			PORTC = stepValuesX[prevMotorPosX]|stepValuesY[prevMotorPosY];
			strlenX += 0.325;
			strlenY -= 0.325;
		}
		else if(newX-strlenX>0.325) {
			prevMotorPosX = (prevMotorPosX + 1) % 4;
			PORTC |= stepValuesX[prevMotorPosX]|(PORTC&0x0F);
			strlenX += 0.325;
		}
		else if(newY-strlenY<-0.325) {
			prevMotorPosY = (prevMotorPosY + 1) % 4;
			PORTC |= stepValuesY[prevMotorPosY]|(PORTC&0xF0);
			strlenY -= 0.325;
		}
		else
			break;
	}
	
	while(newX-strlenX<0&&newY-strlenY<0){
		_delay_ms(100);
		if(newX-strlenX<-0.325&&newY-strlenY<-0.325) {
			prevMotorPosX = (prevMotorPosX - 1 + 4) % 4;
			prevMotorPosY = (prevMotorPosY + 1) % 4;

			PORTC = stepValuesX[prevMotorPosX]|stepValuesY[prevMotorPosY];
			strlenX -= 0.325;
			strlenY -= 0.325;
		}
		else if(newX-strlenX<-0.325) {
			prevMotorPosX = (prevMotorPosX + 1) % 4;
			PORTC = stepValuesX[prevMotorPosX]|(PORTC&0x0F);
			strlenX -= 0.325;
		}
		else if(newY-strlenY<-0.325) {
			prevMotorPosY = (prevMotorPosY + 1) % 4;
			PORTC = stepValuesY[prevMotorPosY]|(PORTC&0xF0);
			strlenY -= 0.325;
		}
		else
			break;
	}	
}

//Defining motion of pointer for each character
void printChar(unsigned char ch) {
	float one=3,two=6,three=9,lena=3*lenA; //prescalar in cms for one, two and three units
	if(ch=='a'||ch=='A') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(0,one);
		moveXY(-two,0);
		moveXY(two,0);
		moveXY(0,one);
	}
	else if(ch=='b'||ch=='B') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(0,one);
		moveXY(-two,0);
		moveXY(two,0);
		moveXY(0,one);
		moveXY(-two,0);
		moveXY(two,0);
	}
	else if(ch=='c' || ch=='C') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,two);
		moveXY(two,0);
	}
	else if(ch=='d' || ch=='D') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(0,two);
		moveXY(-two,0);
		moveXY(two,0);
	}
	else if(ch=='e' || ch=='E') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(two,0);
	}
	else if(ch=='f' || ch=='F') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(one,0);
		moveXY(-one,0);
		moveXY(0,one);
	}
	else if(ch=='g' || ch=='G') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,two);
		moveXY(two,0);
		moveXY(0,-one);
		moveXY(0,one);
	}
	else if(ch=='h' || ch=='H') {
		moveXY(0,-two);
		moveXY(0,one);
		moveXY(two,0);
		moveXY(0,-one);
		moveXY(0,two);
	}
	else if(ch=='i' || ch=='I') {
		moveXY(one,0);
		moveXY(0,-two);
		moveXY(-one,0);
		moveXY(two,0);
		moveXY(-one,0);
		moveXY(0,two);
		moveXY(one,0);
	}
	else if(ch=='j' || ch=='J') {
		moveXY(0,-one);
		moveXY(0,one);
		moveXY(two,0);
		moveXY(0,-two);
		moveXY(-two,0);
		moveXY(two,0);
		moveXY(0,two);
	}
    else if(ch=='k' || ch=='K') {
		moveXY(0,-two);
		moveXY(0,one);
		moveXY(one,-one);
		moveXY(-one,one);
		moveXY(one,one);
	}
	else if(ch=='l' || ch=='L') {
		moveXY(0,-two);
		moveXY(0,two);
		moveXY(two,0);
	}
	else if(ch=='n' || ch=='N') {
		moveXY(0,-two);
		moveXY(two,two);
		moveXY(0,-two);
		moveXY(0,two);
	}
	else if(ch=='o' || ch=='O') {
		moveXY(one,0);
		moveXY(-one,-one);
		moveXY(one,-one);
		moveXY(one,one);
		moveXY(-one,one);
	}
	else if(ch=='p' || ch=='P') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(0,one);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(one,0);
	}
	else if(ch=='q' || ch=='Q') {
		moveXY(two,0);
		moveXY(0,-two);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(two,0);
		moveXY(0,one);
	}
	else if(ch=='r' || ch=='R') {
		moveXY(0,-two);
		moveXY(two,0);
		moveXY(0,one);
		moveXY(-two,0);
		moveXY(one,one);
	}
	else if(ch=='s' || ch=='S') {
		moveXY(two,0);
		moveXY(0,-one);
		moveXY(-two,0);
		moveXY(0,-one);
		moveXY(two,0);
		moveXY(-two,0);
		moveXY(0,one);
		moveXY(two,0);
		moveXY(0,one);
	}
	else if(ch=='t' || ch=='T') {
		moveXY(one,0);
		moveXY(0,-two);
		moveXY(0,one);
		moveXY(one,0);
		moveXY(-one,0);
		moveXY(0,one);
		moveXY(one,0);
	}
	else if(ch=='u' || ch=='U') {
		moveXY(0,-two);
		moveXY(0,two);
		moveXY(two,0);
		moveXY(0,-two);
		moveXY(0,two);
	}
	else if(ch=='v' || ch=='V') {
		moveXY(one,0);
		moveXY(-one,-one);
		moveXY(0,-one);
		moveXY(0,one);
		moveXY(one,one);
		moveXY(one,-one);
		moveXY(0,-one);
		moveXY(0,one);
		moveXY(-one,one);
		moveXY(one,0);
	}
	else if(ch=='w' || ch=='W') {
		moveXY(0,-two);
		moveXY(0,two);
		moveXY(one,0);
		moveXY(0,-one);
		moveXY(0,one);
		moveXY(one,0);
		moveXY(0,-two);
		moveXY(0,two);
	}
	else if(ch=='x' || ch=='X') {
		moveXY(two,-two);
		moveXY(-one,one);
		moveXY(-one,-one);
		moveXY(two,two);
	}
	else if(ch=='y' || ch=='Y') {
		moveXY(one,0);
		moveXY(0,-one);
		moveXY(-one,-one);
		moveXY(one,one);
		moveXY(one,-one);
		moveXY(-one,one);
		moveXY(0,one);
		moveXY(one,0);
	}
	else if(ch=='z' || ch=='Z') {
		moveXY(two,-two);
		moveXY(-two,0);
		moveXY(two,0);
		moveXY(-two,two);
		moveXY(two,0);
	}
	else if(ch==' '&&lenXY-lenA>2) {
		moveXY(one,0);
	}
	else if(ch==' '&&lenXY-lenA<=2) {
		moveXY(0,one);
		moveXY(lena,0);
		moveXY(0,three);
		moveXY(one,0);
	}
	if(ch!=' '&&lenXY-lenA>2) {
		moveXY(one,0);
	}
	else if(ch!=' ') {
		moveXY(0,one);
		moveXY(lena,0);
		moveXY(0,three);
		moveXY(one,0);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Initializing usart 
void usart_initialize() {
	UCSRB=0x18;
	UCSRC=0x86;
	UBRRL=0x33;
}

void usart_send(unsigned char ch) {
	while(!(UCSRA & (1<<UDRE))); // Wait till UDR is empty
	UDR=ch; //Write the value to be Tx
}
 
 /*
//LCD Connections
#define LCD_RS  PORTA.B2   // RS
#define LCD_EN  PORTA.B6   //Enable
#define LCD_D4  PORTB.B4   //Data Bit 4
#define LCD_D5  PORTB.B5   //Data Bit 5
#define LCD_D6  PORTB.B6   //Data Bit 6
#define LCD_D7  PORTB.B7   //Data Bit 7

*/
// to provide data to LCD
void LCD_Data(unsigned char Data) {
	PORTB=Data&0xF0; // Send Higher nibble (D7-D4)
	
	PORTA|=(1<<2);       // Register Select =1 (for data select register)
	PORTA|=(1<<6);      //Enable=1 for H to L pulse
	
	_delay_us(5);
	
	PORTA^=(1<<6);
	PORTB=((Data<<4)&0xF0); // Send Lower nibble (D3-D0)
	PORTA|=(1<<6);               //Enable=1 for H to L pulse
	 
	_delay_us(5);
	 
	PORTA^=(1<<6);

	_delay_us(100);
}


// to print string to LCD
void LCD_Print(char * str) {
	unsigned char i=0;
	// Till NULL character is reached, take each character
	while(*(str+i)!=0) {
		LCD_Data(*(str+i)); // Data sent to LCD data register
		i++;
		_delay_ms(10);
	}
}

// to provide command to LCD
void lcdcommand(unsigned char command) {
	PORTB=command&0xF0; // Send Higher nibble (D7-D4)
	
	PORTA&=~(1<<2); // Register Select =0 (for Command register)
	PORTA|=(1<<6); //Enable=1 for H to L pulse
	
	_delay_us(5);
	PORTA^=(1<<6);
	_delay_us(100);

	PORTB=((command<<4)&0xF0);  // Send Lower nibble (D3-D0)
	PORTA|=(1<<6); //Enable=1 for H to L pulse
	_delay_us(5);
	PORTA^=(1<<6);

	_delay_us(40);
}

 // Cursor Position
void Cursor_Position(unsigned short int x,unsigned short int y) {
	unsigned char firstcharadd[] ={0x80,0xC0}; // First line address 0X80
	//Second line address 0XC0
	lcdcommand((firstcharadd[x-1]+y-1));// to get address line on LCD
}

// To clear LCD previous data
void clear() {
	lcdcommand(0x01); // to clear LCD
	_delay_ms(2);
}

 //LCD Initialize
void LCD_Initialize() {
	PORTA&=~(1<<6);
	
	lcdcommand(0x33); // Initialize LCD for 4 bit mode
	lcdcommand(0x32); // Initialize LCD for 4 bit mode
	lcdcommand(0x28); // Initialize LCD for 5X7 matrix mode
	lcdcommand(0x0E); //Display on,cursor blinking
	clear();
	lcdcommand(0x06); //Shift cursor to right
	lcdcommand(0x80);
}


int main(void) {
  char pass[100]={};
  unsigned char flag=0,i=0,k=0,j=0;
  usart_initialize();
  //Set-up PORTS for LCD
  DDRB=0xF0;  // For D3-D0
  DDRC=0xFF;
  PORTC=0x00;
  DDRA|=(1<<2);                    //Port D as output Pin0-3 controls motor 1 while Pin 4-7 controls motor 2
  DDRA|=(1<<6);

  LCD_Initialize(); //Initialize
  
  clear();

 while(flag!=1) {
     while((UCSRA&(1<<RXC))==0);
         pass[i++] = UDR;
         usart_send(pass[i-1]);
         if(UDR=='\r'||pass[i-1]=='\r')
             { flag=1;pass[i-1]='\0';}
 }
 
 
 for(k=0;k<2;k++) {
 for(j=1;j+(k*16)<=(i-1)&&j<=16;j++) {
     Cursor_Position(k+1,j);
     LCD_Data(pass[j-1+(k*16)]);
     _delay_ms(20);
 }
 }
// Cursor_Position(1,1);
 //LCD_Print(pass);
 
 for(k=0;pass[k]!='\0';k++) {
	printChar(pass[k]);
 }
	return 0;
 }
