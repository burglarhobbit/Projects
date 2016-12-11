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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Functions to move the pointer up, down, left, right, diagonally left/right up and diagonally left/right down
void up(int iteration) {
   int i=0;
   while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0x9C;
		_delay_ms(25);
		PORTC=0x36;
		_delay_ms(25);
		PORTC=0x63;
		_delay_ms(25);
		PORTC=0xC9;
		_delay_ms(25);
		i++;
	}
}

void down(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0xC9;
		_delay_ms(25);
		PORTC=0x63;
		_delay_ms(25);
		PORTC=0x36;
		_delay_ms(25);
		PORTC=0x9C;
		_delay_ms(25);
		i++;
	}
}

void right(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0x99;
		_delay_ms(25);
		PORTC=0x33;
		_delay_ms(25);
		PORTC=0x66;
		_delay_ms(25);
		PORTC=0xCC;
		_delay_ms(25);
		i++;
   }
}

void left(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticlockwise where each step has
		a 25 ms delay*/
		PORTC=0xCC;
		_delay_ms(25);
		PORTC=0x66;
		_delay_ms(25);
		PORTC=0x33;
		_delay_ms(25);
		PORTC=0x99;
		_delay_ms(25);
		i++;
	}
}

void rightdiagup(int iteration)
{
   int i=0;
   while(i!=5*iteration){
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0x09;
		_delay_ms(25);
		PORTC=0x03;
		_delay_ms(25);
		PORTC=0x06;
		_delay_ms(25);
		PORTC=0x0C;
		_delay_ms(25);
		i++;
	}
}

void leftdiagdown(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0x0C;
		_delay_ms(25);
		PORTC=0x06;
		_delay_ms(25);
		PORTC=0x03;
		_delay_ms(25);
		PORTC=0x09;
		_delay_ms(25);
		i++;
	}
}

void leftdiagup(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0xC0;
		_delay_ms(25);
		PORTC=0x60;
		_delay_ms(25);
		PORTC=0x30;
		_delay_ms(25);
		PORTC=0x90;
		_delay_ms(25);
		i++;
	}
}

void rightdiagdown(int iteration) {
	int i=0;
	while(i!=5*iteration) {
		/*The following sequence makes motor 1 work clockwise,
		and motor 2 to work anticllockwise where each step has
		a 25 ms delay*/
		PORTC=0x90;
		_delay_ms(25);
		PORTC=0x30;
		_delay_ms(25);
		PORTC=0x60;
		_delay_ms(25);
		PORTC=0xC0;
		_delay_ms(25);
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	DDRC=0xFF;  //Port C as output Pin0-3 controls motor 1 while Pin 4-7 controls motor 2
	PORTC=0x00;
	DDRA|=(1<<2); //PORTA for LCD RS
	DDRA|=(1<<6); //PORTA for LCD EN

	LCD_Initialize(); //Initialize
  
	clear();

	while(flag!=1) {
		while((UCSRA&(1<<RXC))==0);
			pass[i++] = UDR;
			usart_send(pass[i-1]);
			if(UDR=='\r'||pass[i-1]=='\r')
				{ flag=1;pass[i-1]='\0';}
	}
	// i = received character length + 1
 
	//Dynamic for-loop for printing characters on 20x2 LCD with cases like overflow of characters printed on LCD handled.
	for(k=0;k<2;k++) {
		for(j=1;j+(k*16)<=(i-1)&&j<=16;j++) {
			Cursor_Position(k+1,j);
			LCD_Data(pass[j-1+(k*16)]);
			_delay_ms(20);
		}
	}
	
	for(k=0;pass[k]!='\0';k++) {
		if(pass[k]=='a'||pass[k]=='A') {
			up(2);
			right(2);
			down(1);
			left(2);
			right(2);
			down(1);
		}
		else if(pass[k]=='b'||pass[k]=='B') {
			up(2);
			right(2);
			down(1);
			left(2);
			right(2);
			down(1);
			left(2);
			right(2);
		}
		else if(pass[k]=='c' || pass[k]=='C') {
			up(2);
			right(2);
			left(2);
			down(2);
			right(2);
		}
		else if(pass[k]=='d' || pass[k]=='D') {
			up(2);
			right(2);
			down(2);
			left(2);
			right(2);
		}
		else if(pass[k]=='e' || pass[k]=='E') {
			up(2);
			right(2);
			left(2);
			down(1);
			right(2);
			left(2);
			down(1);
			right(2);
		}
		else if(pass[k]=='f' || pass[k]=='F') {
			up(2);
			right(2);
			left(2);
			down(1);
			right(1);
			left(1);
			down(1);
		}
		else if(pass[k]=='g' || pass[k]=='G') {
			up(2);
			right(2);
			left(2);
			down(2);
			right(2);
			up(1);
			down(1);
		}
		else if(pass[k]=='h' || pass[k]=='H') {
			up(2);
			down(1);
			right(2);
			up(1);
			down(2);
		}
		else if(pass[k]=='i' || pass[k]=='I') {
			right(1);
			up(2);
			left(1);
			right(2);
			left(1);
			down(2);
			right(1);
		}
		else if(pass[k]=='j' || pass[k]=='J') {
			up(1);
			down(1);
			right(2);
			up(2);
			left(2);
			right(2);
			down(2);
		}
		else if(pass[k]=='k' || pass[k]=='K') {
			up(2);
			down(1);
			rightdiagup(1);
			leftdiagdown(1);
			right(1);
			down(1);
			right(1);
		}
		else if(pass[k]=='l' || pass[k]=='L') {
			up(2);
			down(2);
			right(2);
		}
		else if(pass[k]=='m' || pass[k]=='M') {
			up(2);
			right(1);
			down(1);
			right(1);
			down(2);
		}
		else if(pass[k]=='n' || pass[k]=='N') {
			up(2);
			right(1);
			down(2);
			right(1);
			up(2);
			down(2);
		}
		else if(pass[k]=='o' || pass[k]=='O') {
			right(1);
			leftdiagup(1);
			rightdiagup(1);
			rightdiagdown(1);
			leftdiagdown(1);
			right(1);
		}
		else if(pass[k]=='p' || pass[k]=='P') {
			up(2);
			right(2);
			down(1);
			left(2);
			down(1);
			right(1);
		}
		else if(pass[k]=='q' || pass[k]=='Q') {
			right(2);
			up(2);
			left(2);
			down(1);
			right(2);
			down(1);
		}
		else if(pass[k]=='r' || pass[k]=='R') {
			up(2);
			right(2);
			down(1);
			left(2);
			right(1);
			down(1);
			right(1);
		}
		else if(pass[k]=='s' || pass[k]=='S') {
			right(2);
			up(1);
			left(2);
			up(1);
			right(2);
			left(2);
			down(1);
			right(2);
			down(1);
		}
		else if(pass[k]=='t' || pass[k]=='T') {
			right(1);
			up(2);
			down(1);
			right(1);
			left(1);
			down(1);
			right(1);
		}
		else if(pass[k]=='u' || pass[k]=='U') {
			up(2);
			down(2);
			right(2);
			up(2);
			down(2);
		}
		else if(pass[k]=='v' || pass[k]=='P') {
			right(1);
			leftdiagup(1);
			up(1);
			down(1);
			rightdiagdown(1);
			rightdiagup(1);
			up(1);
			down(1);
			leftdiagdown(1);
			right(1);
		}
		else if(pass[k]=='w' || pass[k]=='W') {
			up(2);
			down(2);
			right(1);
			up(1);
			down(1);
			right(1);
			up(2);
			down(2);
		}
		else if(pass[k]=='x' || pass[k]=='X') {
			right(1);
			up(1);
			left(1);
			up(1);
			down(1);
			right(1);
			up(1);
			right(1);
			left(1);
			down(1);
			right(1);
			down(1);
		}
		else if(pass[k]=='y' || pass[k]=='Y') {
			right(1);
			up(1);
			leftdiagup(1);
			rightdiagdown(1);
			rightdiagup(1);
			leftdiagdown(1);
			down(1);
			right(1);
		}
		else if(pass[k]=='z' || pass[k]=='Z') {
			up(1);
			right(2);
			up(1);
			left(2);
			right(2);
			down(1);
			left(2);
			down(1);
			right(2);
		}
	}
	while(1) {
		Cursor_Position(1,16);
		LCD_Print(pass);
		lcdcommand(0x18);
		_delay_ms(300);
	}
	return 0;
}
