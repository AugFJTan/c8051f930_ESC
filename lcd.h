#ifndef __LCD_H
#define __LCD_H		

 
 #include "time.h"
 	
#define LCD_W 128
#define LCD_H 160

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long


///////////////////////
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

typedef	unsigned char uint8_t;
typedef	unsigned int uint16_t;
typedef	 int int16_t;
typedef unsigned char u_char;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Port pin assignments 
//-----------------------------------------------------------------------------
// 1.8inch LCD ST7735
//-----------------------------------------------------------------------------
// P1.0   sck
// P1.1   miso
// P1.2   mosi
// P1.3   cs        
// P1.4   dc
// P1.6   reset


sbit LCD_REST   =P1^6;	    	
sbit LCD_AO     =P1^4;	  // dc 
sbit LCD_CS     =P1^3;
sbit LCD_SDA    =P1^2;	  //   
sbit LCD_SCK    =P1^0;	   

 

extern  uint16_t BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void Lcd_Init(void); 
void LCD_Clear(u16 Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //��ʾһ���ַ���,16����
void ST7735_printf(u8 module, const u8 *fmt, ...);

//////////////////

void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void ST7735_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

void fillCircle(u_char x, u_char y, u_char radius);
void ST7735_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h);



char* itoa_c(int value, char* result, int base) ;   

////////////////////////////////////////////////////////////////////////////////////////////
  
 


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define lcd_hght 16
#define ROW0  0*lcd_hght
#define ROW1  1*lcd_hght
#define ROW2  2*lcd_hght
#define ROW3  3*lcd_hght
#define ROW4  4*lcd_hght	
#define ROW5  5*lcd_hght
#define ROW6  6*lcd_hght	
#define ROW7  7*lcd_hght		  		 
#endif  
	 
	 



