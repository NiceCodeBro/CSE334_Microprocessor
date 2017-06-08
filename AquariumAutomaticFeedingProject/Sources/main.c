/*
131044023 Muhammed Selim Dursun
BIL334 Mikro iþlemciler projesi

Zaman ayarlý otomatik yemleme, karanlýk olunca otomatik akvaryum aydýnlatmasý çalýþýr

*Dip switches (00 iken ýþýk sensörü aktif olur)
*Light sensor
*Servo motor
*Led lambalar
*Seven segment
*Lcd modul
*timer interrupt


*/
 
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
unsigned char COUNT;
unsigned int second = 0;
unsigned int countOfFood = 0;  //toplam atýlan yem sayýsý
#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02
//seven segment için atamalar
#define ZERO 0x3F
#define ONE 0x06
#define TWO 0x5B
#define THREE 0x4F
#define FOUR 0x66
#define FIVE 0x6D
#define SIX 0x7D
#define SEVEN 0x07
#define EIGHT 0x7F
#define NINE 0x6F

void MSDelay(unsigned int);   //pooling delay
void yemiAl(void );                          //yemi hazneden alma metodu
void sevenSegment(unsigned int count);     //o ana kadar atýlmýs yemlerin sayýsýný seven segmentte gösterme
void printL(unsigned char* string, unsigned int size );   // ekrana istenilen stringi basma fonksiyonu
void DATWRT4(unsigned char data);
void COMWRT4(unsigned char command);
void yemiBirak(void);       //yemi asagý býrakma metodu
interrupt (((0x10000-Vtimovf)/2)-1) void TOF_ISR(void);   //timer interrupt fonksiyonu
void main(void) {
  /* put your own code here */
  
    
    DDRT = 0x03;
    DDRH = 0x0;
    PTH = 0b00000000;
    PTT = 0b00000000;
    DDRB = 0xFF;    //PORTB as output
    DDRJ = 0xFF;    //PTJ as output for Dragon12+ LEDs
    PTJ=0x0;        //Allow the LEDs to dsiplay data on PORTB pins
    
 ATD0CTL2 = 0x80;     //Turn on ADC,..No Interrupt
    MSDelay(5);
    ATD0CTL3 = 0x08;  //one conversion, no FIFO
    ATD0CTL4 = 0xEB;  //8-bit resolu, 16-clock for 2nd phase,
                      //prescaler of 24 for Conversion Freq=1MHz
                      
      /////timer intarrupt code init
  
   COUNT = 0;
    TSCR1 = 0x80;
    TSCR2 = 0x83;                   
    TFLG2 = TFLG2 | TFLG2_TOF_MASK;
    __asm CLI;
    
    
    DDRP = 0xFF;
    PTP = 0x07;
  //timer interrupt code init    
    for(;;)
    {
    
           sevenSegment(countOfFood);

         if(COUNT==255) {
            second++;
            COUNT = 0;
         }
         if(second == 2) { // if second 1, past 5.5 second
            second = 0;
            ++countOfFood;
            yemiBirak();
             yemiAl();

            printL(" ",1);

         }
    
    
    ATD0CTL5 = 0x84;  //Channel 4 (right justified, unsigned,single-conver,one chan only) 
  
      
    while(!(ATD0STAT0 & 0x80));
    
    if(PTH == 0)
      if(ATD0DR0L < 9) {
        PTT= 0b00000001; 

      }
      else {
         PTT= 0b00000000; 
      }
MSDelay(1);  //optional
    }
    

  }
void sevenSegment(unsigned int count) {
  count = count% 10;
  if(count == 0)
    PORTB = ZERO;
  else if(count == 1)
    PORTB = ONE;
  else if(count == 2)
    PORTB = TWO;
  else if(count == 3)
    PORTB = THREE;
  else if(count == 4)
    PORTB = FOUR;
  else if(count == 5)
    PORTB = FIVE;
  else if(count == 6)
    PORTB = SIX;
  else if(count == 7)
    PORTB = SEVEN;
  else if(count == 8)
    PORTB = EIGHT;
  else if(count == 9)
    PORTB = NINE;
 

}

 void MSDelay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<4000;j++);
  }
  
 

void yemiAl(void ) {
   printL("Yem Aliniyor..",14);
   MSDelay(1000);

      PWMPRCLK=0x04; //ClockA=Fbus/2**4=24MHz/16=1.5MHz	
	PWMSCLA=100; 	 //ClockSA=1.5MHz/2x125=6000 Hz
	PWMCLK=0b00010000; 	 //ClockSA for chan 4
	PWMPOL=0x10; 		     //high then low for polarity
	PWMCAE=0x1; 		     //left aligned
	PWMCTL=0x0;		         //8-bit chan, PWM during freeze and wait
	PWMPER4=100; 	 //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
	PWMDTY4=5; 	 //50% duty cycle           AND THIS TO SEE THE EFFECT ON MOTOR. TRY LESS THAN 10%
	
	PWMCNT4=10;		 //clear initial counter. This is optional
	PWME=0x10; 	   //Enable chan 4 PWM


    MSDelay(1000);

}


void yemiBirak(void ) {
printL("Yem Atiliyor..",14);
              MSDelay(1000);

      PWMPRCLK=0x04; //ClockA=Fbus/2**4=24MHz/16=1.5MHz	
	PWMSCLA=100; 	 //ClockSA=1.5MHz/2x125=6000 Hz
	PWMCLK=0b00010000; 	 //ClockSA for chan 4
	PWMPOL=0x10; 		     //high then low for polarity
	PWMCAE=0x1; 		     //left aligned
	PWMCTL=50;		         //8-bit chan, PWM during freeze and wait
	PWMPER4=100; 	 //PWM_Freq=ClockSA/100=6000Hz/100=60Hz. CHANGE THIS
	PWMDTY4=35; 	 //50% duty cycle           AND THIS TO SEE THE EFFECT ON MOTOR. TRY LESS THAN 10%
	
	PWMCNT4=10;		 //clear initial counter. This is optional
	PWME=0x10; 	   //Enable chan 4 PWM


    MSDelay(1000);

}

  interrupt (((0x10000-Vtimovf)/2)-1) void TOF_ISR(void) {
    COUNT++;
   // PORTB = COUNT;
    TFLG2 = TFLG2 | TFLG2_TOF_MASK;
 }
 
 
 void COMWRT4(unsigned char command)
  {
        unsigned char x;
        
        x = (command & 0xF0) >> 2;         
        LCD_DATA =LCD_DATA & ~0x3C;        
        LCD_DATA = LCD_DATA | x;        
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~RS;        
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;          
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;        
        MSDelay(15);                      
        x = (command & 0x0F)<< 2;          
        LCD_DATA =LCD_DATA & ~0x3C;         
        LCD_DATA =LCD_DATA | x;            
        LCD_CTRL = LCD_CTRL | EN;         
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         
        MSDelay(15);
  }

  void DATWRT4(unsigned char data)
  {
        unsigned char x;
        x = (data & 0xF0) >> 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | RS;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(5); 
        x = (data & 0x0F)<< 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(15);
  }
  
  /*gelen stringi size kadar ekrana basan fonksiyon*/
void printL(unsigned char* string, unsigned int size ) {
        unsigned int i = 0;
        DDRK = 0xFF;   
        COMWRT4(0x33);  
        MSDelay(2);
        COMWRT4(0x32);   //data sheet'e göre resetleme
        MSDelay(2);
        COMWRT4(0x28);   
        MSDelay(2);
        COMWRT4(0x06); 
        MSDelay(2);
        COMWRT4(0x0E);  
        MSDelay(2);
        COMWRT4(0x01);  //ekraný sýfýrla
        MSDelay(2);
        COMWRT4(0x80);  //baslangýc pozisyonuna ayarla
  
	
	for(i = 0;i < size ;++i) {
	       DATWRT4(string[i]);
          MSDelay(2);
	}      
	
}