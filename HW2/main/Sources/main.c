/*
  MIKRO ISLEMCILER HW2
  131044023 
  MUHAMMED SELIM DURSUN
*/

/*
  LCD panel ile ilgili kýsýmlar ödevde istenmiyordu fakat görünüþ açýsýdan kendim ekledim
  Lcd kodlarý için internetten yardým aldým.
*/
#include <hidef.h>      /* makrolar */
#include <mc9s12dp512.h>
#pragma LINK_INFO DERIVATIVE "mc9s12dp512"
unsigned int Tcount;
unsigned int x; /*nota degistirmek icin tcount a eklenecek globak deger*/
/*notalar*/
#define DO 46801
#define LA 28716
#define SOL 31612
#define SI 25291
#define FA 35364
#define RE 41858
#define MI 37407
#define DOO 23922

#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02
 /*gelen stringi size kadar ekrana basan fonksiyon*/
void printL(unsigned char* string, unsigned int size );
void COMWRT4(unsigned char);
void DATWRT4(unsigned char);
/*yasa fenerbahce muzigini calan fonksiyon*/
void playFenerbahceSong();
/*kucuk kurbaga muzigini calan fonksiyon*/
void kucukKurbagaSong();
/*bak postacý gelýyor muzigini calan fonksiyon*/
void bakPostaciSong() ;
/*dip switch 0 iken 3 müzigi ard arda totalde 15 saniye calan fonksiyon*/
void play3Song() ;
void playNote(int multiplier);
void MSDelay(unsigned int itime);
 void main(void){
         DDRB = 0xFF; //setting output 
         DDRP = 0xFF; //setting output
         PTP = 0x07;
         printL("SELIM DURSUN",12);
         MSDelay(300);
         if(PTH == 0) {
            PORTB = 0x3F;                      //showing number of the song on seven segment
            play3Song();
         }
         
        if(PTH == 1) {
            printL("YASA FENERBAHCE",15);
            PORTB = 0x06;                      //showing number of the song on seven segment
            playFenerbahceSong();
        }
        if(PTH == 2) {
            printL("BAK POSTACI GELIYOR",19);
            PORTB = 0x5B;                     //showing number of the song on seven segment
            bakPostaciSong();
        }
        if(PTH == 3) {
           printL("KUCUK KURBAGA",13);
           PORTB = 0x4F;                          //showing number of the song on seven segment
           kucukKurbagaSong();
        }
           

	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
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

 void MSDelay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<4000;j++);
 }
void playNote(int multiplier) {

  unsigned int temp = 0;
  DDRT = DDRT | 0x10;
  TSCR1 = 0x80;
  TSCR2 = 0x00;
  TIOS = 0x20;
  TCTL1 = 0x04;
  TFLG2 = 0x80;
  while(temp < multiplier *150){
    temp = temp + 1;
    Tcount = TCNT ;
    Tcount = Tcount + x;
    TC5 = Tcount;
    while(!(TFLG1 & TFLG1_C5F_MASK));
    TFLG1 = TFLG1 | TFLG1_C5F_MASK;
  }
  TSCR1 &= ~0x80; //tscr1 sýfýrlama
}
/*dip switch 0 iken 3 müzigi ard arda totalde 15 saniye calan fonksiyon*/
void play3Song() {
          printL("YASA FENERBAHCE",15);
            x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  LA;
           playNote(2);
           MSDelay(25);
           x =  SOL;
           playNote(3);
           MSDelay(25);
           x =  FA;
           playNote(2);
           MSDelay(25);
           x =  MI;
           playNote(2);
           MSDelay(35);
           x =  SOL;
           playNote(2);
           MSDelay(25);
           x =  DOO;
           playNote(2);
           MSDelay(25);
           x =  LA;
           playNote(2);
           MSDelay(25);
           x =  SI;
           playNote(2);
           printL("KUCUK KURBAGA",13);
           MSDelay(150);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           printL("BAK POSTACI GELIYOR",19);
           MSDelay(150);
           x =  SOL;
           playNote(3);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(3);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(4);
}
/*kucuk kurbaga muzigini calan fonksiyon*/
void kucukKurbagaSong() {
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  DO;
           playNote(2);
           MSDelay(15);
                  
}
/*bak postacý gelýyor muzigini calan fonksiyon*/
void bakPostaciSong() {
           x =  SOL;
           playNote(3);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(3);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(4);
           MSDelay(100);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(4);
           MSDelay(40);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI ;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(15);
           x =  DO;
           playNote(2);
}

/*yasa fenerbahce muzigini calan fonksiyon*/
void playFenerbahceSong() {      
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  SOL;
           playNote(1);
           MSDelay(35);
           x =  LA;
           playNote(2);
           MSDelay(25);
           x =  SOL;
           playNote(3);
           MSDelay(25);
           x =  FA;
           playNote(2);
           MSDelay(25);
           x =  MI;
           playNote(2);
           MSDelay(35);
           x =  SOL;
           playNote(2);
           MSDelay(25);
           x =  DOO;
           playNote(2);
           MSDelay(25);
           x =  LA;
           playNote(2);
           MSDelay(25);
           x =  SI;
           playNote(2);
           MSDelay(25);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(40);
           x =  FA;
           playNote(1);
           MSDelay(25);
           x =  FA;
           playNote(1);
           MSDelay(25);
           x =  FA;
           playNote(1);
           MSDelay(25);
           x =  FA;
           playNote(1);
           MSDelay(25);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
           MSDelay(15);
           x =  RE;
           playNote(2);
           MSDelay(40);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  SI;
           playNote(2);
           MSDelay(15);
           x =  SOL;
           playNote(2);
           MSDelay(15);
           x =  LA;
           playNote(2);
           MSDelay(15);
           x =  FA;
           playNote(2);
           MSDelay(15);
           x =  MI;
           playNote(2);
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
