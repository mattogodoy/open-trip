//Author: cantone-electonics
//More information welcome to : http://www.canton-electronics.com 
//Arduino 1.0.4
//Arduino uno R3
//5 DIGIT 7SEG LCD

/*---Segment Display Screen----
--5--
1---6
--2--
3---7
--4--
8 decimal point
----------------------*/

#define sbi(x, y)  (x |= (1 << y))   /*set Register x of y*/
#define cbi(x, y)  (x &= ~(1 << y))  /*Clear Register x of y*/
#define uchar unsigned char
#define uint  unsigned int
//Defined HT1621's command
#define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0
#define  RCosc      0x30  //on-chip RC oscillator(Power-on default)1000 0011 0000
#define  LCD_on     0x06  //Turn on LCD
#define  LCD_off    0x04  //Turn off LCD
#define  Sys_en     0x02  //Turn on system oscillator 1000   0000 0010
#define  CTRl_cmd   0x80  //Write control cmd
#define  Data_cmd   0xa0  //Write data cmd

// =======================
// ====== SCREEN A =======
// =======================
//Define port     HT1621 data port
#define pin_display_a_cs   pin_display_a_cs  //Pin 3 as chip selection output
#define pin_display_a_wr   pin_display_a_wr  //Pin 4 as read clock  output
#define pin_display_a_data pin_display_a_data  //Pin 5 as Serial data output

// =======================
// ====== SCREEN b =======
// =======================
//Define port     HT1621 data port
#define pin_display_b_cs   pin_display_b_cs  //Pin 6 as chip selection output
#define pin_display_b_wr   pin_display_b_wr  //Pin 7 as read clock  output
#define pin_display_b_data pin_display_b_data  //Pin 8 as Serial data output


/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,*/    
const char num[] = {0xEB, 0x60, 0xC7, 0xE5, 0x6C, 0xAD, 0xAF, 0xE0, 0xEF, 0xED, 0xEE, 0x2F, 0x8B, 0x07, 0x67, 0x8F, 0x8E};
char dispnum[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

/*
 *       a
 *       -
 *    e | | b
 *       -  <-f
 *    g | | c
 *       - . <-d
 *       h
 *       
 *   a b c d e f g h
 *   1 1 1 1 1 1 1 1 (all on) == 0b11111111
 *   
 *
 */

char char_null = 0b00000000;
char char_0    = 0b11101011;
char char_1    = 0b01100000;
char char_2    = 0b11000111;
char char_3    = 0b11100101;
char char_4    = 0b01101100;
char char_5    = 0b10101101;
char char_6    = 0b10101111;
char char_7    = 0b11100000;
char char_8    = 0b11101111;
char char_9    = 0b11101101;
char char_a    = 0b11100111;
char char_aa   = 0b11101110; // A
char char_b    = 0b00101111;
char char_c    = 0b00000111;
char char_cc   = 0b10001011; // C
char char_d    = 0b01100111;
char char_e    = 0b11001111;
char char_ee   = 0b10001111; // E
char char_f    = 0b10001110;
char char_g    = 0b11101101;
char char_h    = 0b00101110;
char char_hh   = 0b01101110; // H
char char_i    = 0b00100000;
char char_ii   = 0b01100000; // I
char char_j    = 0b01100011;
char char_jj   = 0b11100011; // J
char char_l    = 0b00001010;
char char_ll   = 0b00001011; // L
char char_n    = 0b00100110;
char char_nn   = 0b10100110; // ñ
char char_o    = 0b00100111;
char char_oo   = 0b11101011; // O
char char_p    = 0b11001110;
char char_q    = 0b11101100;
char char_r    = 0b00000110;
char char_s    = 0b10101101;
char char_t    = 0b00001111;
char char_u    = 0b00100011;
char char_uu   = 0b01101011; // U
char char_y    = 0b01101101;
char char_z    = 0b11000111;
char char_dot  = 0b00010000;
char char_less = 0b00000100; // -
char char_und  = 0b00000001; // _
char char_deg  = 0b11001100; // º

// Menus
char menu_display1[]      = { char_d, char_i, char_s, char_p, char_1, '\0' };
char menu_display2[]      = { char_d, char_i, char_s, char_p, char_2, '\0' };
char menu_circumference[] = { char_cc, char_ii, char_r, char_cc, char_null, '\0' };
char menu_declination[]   = { char_d, char_ee, char_cc, char_ll, char_null, '\0' };
char menu_light[]         = { char_ll, char_ii, char_g, char_hh, char_t, '\0' };
char menu_auto[]          = { char_aa, char_uu, char_t, char_oo, char_null, '\0' };
char menu_reset[]         = { char_r, char_ee, char_s, char_ee, char_t, '\0' };
char menu_partial[]       = { char_p, char_aa, char_r, char_t, char_null, '\0' };
char menu_total[]         = { char_t, char_oo, char_t, char_aa, char_ll, '\0' };
char menu_heading[]       = { char_hh, char_ee, char_aa, char_d, char_null, '\0' };
char menu_speed[]         = { char_s, char_p, char_ee, char_ee, char_d, '\0' };
char menu_on[]            = { char_0, char_n, char_null, char_null, char_null, '\0' };
char menu_off[]           = { char_0, char_f, char_f, char_null, char_null, '\0' };

char* menu[] = {
  menu_display1,
  menu_display2,
  menu_circumference,
  menu_declination,
  menu_light,
  menu_auto,
  menu_reset,
  menu_partial,
  menu_total,
  menu_heading,
  menu_speed,
  menu_on,
  menu_off
};


/**------------------------------------------------------------------------- 
                Name: SendBit_1621(send data) 
---------------------------------------------------------------------------*/ 

void SendBit_1621(uchar sdata, uchar cnt, int screen){ //High bit first
  uchar i;
  int DATA;
  int WR;
  
  switch(screen){
    case 0:
      DATA = pin_display_a_data;
      WR = pin_display_a_wr;
      break;
    case 1:
      DATA = pin_display_b_data;
      WR = pin_display_b_wr;
      break;
  }

  for(i = 0; i < cnt; i++){ 
    digitalWrite(WR, LOW);
    delayMicroseconds(20); 
    
    if(sdata & 0x80) 
      digitalWrite(DATA, HIGH); 
    else
      digitalWrite(DATA, LOW); 
  
    delayMicroseconds(20);
    digitalWrite(WR, HIGH);
    delayMicroseconds(20);
    sdata <<= 1; 
  } 

  delayMicroseconds(20);
}

/**------------------------------------------------------------------------- 
                  Name: SendCmd(send cmd) 
                  //Write MODE“100” AND 9 bits command 
---------------------------------------------------------------------------*/ 

void SendCmd_1621(uchar command, int screen){
  int CS;
  switch(screen){
    case 0:
      CS = pin_display_a_cs;
      break;
    case 1:
      CS = pin_display_b_cs;
      break;
  }

  digitalWrite(CS, LOW);
  SendBit_1621(0x80, 4, screen);
  SendBit_1621(command, 8, screen);
  digitalWrite(CS, HIGH);
}

/**------------------------------------------------------------------------- 
                Name: Write_1621send data and cmd) 
---------------------------------------------------------------------------*/ 

void Write_1621(uchar addr, uchar sdata, int screen){ 
  addr <<= 2;
  int CS;
  switch(screen){
    case 0:
      CS = pin_display_a_cs;
      break;
    case 1:
      CS = pin_display_b_cs;
      break;
  }

  digitalWrite(CS, LOW);
  SendBit_1621(0xa0, 3, screen);     //Write MODE“101”
  SendBit_1621(addr, 6, screen);     //Write addr high 6 bits
  SendBit_1621(sdata, 8, screen);    //Write data  8 bits
  digitalWrite(CS, HIGH);
}

/**------------------------------------------------------------------------- 
                        Name: all_off(Clear Display) 
---------------------------------------------------------------------------*/ 

void HT1621_all_off(uchar num, int screen) {
  uchar i;
  uchar addr = 0;
  for(i = 0; i < num; i++){
    Write_1621(addr, 0x00, screen);
    addr += 2;
  }
}

/**************************************************************************** 
                        Name: all_on(All lit) 
****************************************************************************/ 

// void HT1621_all_on(uchar num, int screen){
//   uchar i, j; 
//   uchar addr = 0; 
//   for(i = 0; i < num; i++){
//     Write_1621(addr, 0xff, screen);
//     addr += 2;
//   } 
// }

/**************************************************************************** 
                        Name: all_on_num(All lit,Display the same number) 
****************************************************************************/ 

// void HT1621_all_on_num(uchar num, uchar xx, int screen){
//   uchar i, j; 
//   uchar addr = 0; 
//   for(i = 0; i < num; i++){
//     Write_1621(addr, xx, screen); 
//     addr += 2;
//   } 
// } 

/**************************************************************************** 
                        Name: Init_1621(initialize 1621) 
*****************************************************************************/ 

void Init_1621(int screen){
  SendCmd_1621(Sys_en, screen);
  SendCmd_1621(RCosc, screen);
  SendCmd_1621(ComMode, screen);
  SendCmd_1621(LCD_on, screen);
} 

/**************************************************************************** 
                        Name: LCDoff(off 1621) 
*****************************************************************************/ 

void LCDoff(int screen){
  SendCmd_1621(LCD_off, screen);
}

/**************************************************************************** 
                        Name: LCDon(on 1621) 
*****************************************************************************/ 

void LCDon(int screen){
  SendCmd_1621(LCD_on, screen);
}

// void displayallnum(unsigned int n){       //Display all digital characters
//   uchar i, j; 
//   uchar addr = 0; 

//   for(i = 0; i < n; i++){
//     for(j = 0; j < 29; j++){
//       Write_1621(addr, num[j]);
//       delay(100);
//     }

//     addr += 2;
//  }
// }

// //By-bit display 8 characters and decimal point 
// void displayall8(void){
//   uchar i;
//   HT1621_all_off(6);
//   for(i = 0; i < 6; i++){
//     Write_1621(2 * i, 0b11111111);
//     delay(50);
//   }
// }


/**************************************************************************** 
   Name: display_number(unsigned long int t,int p,char bat1,char bat2,char bat3) Display  data
   t : Data to be displayed
   p : Display decimal
   s1,s2 : Left side of the Triangle
*****************************************************************************/ 

void display_number(long int t, int p, char s1, char s2, int screen){
  uchar i;

  if(t > 99999) t = 99999; // Avoid overflow

  dispnum[4] = num[t / 10000];
  dispnum[3] = num[(t / 1000) % 10];
  dispnum[2] = num[(t / 100) % 10];
  dispnum[1] = num[(t / 10) % 10];
  dispnum[0] = num[t % 10];

  switch(p){
    case 1:
      sbi(dispnum[0], 4);
      break;
    case 2:
      sbi(dispnum[1], 4);
      break;
    case 3:
      sbi(dispnum[2], 4);
      break;
    default:
      break;
  }

  // ==========================
  // This abomination is to avoid writing 0s in blank spaces. Sorry.
  if(t < 10){
    dispnum[2] = char_null;
  }
  if(t < 100){
    dispnum[2] = char_null;
  }
  if(t < 1000){
    dispnum[3] = char_null;
  }
  if(t < 10000){
    dispnum[4] = char_null;
  }
  // ==========================

  if(s1 == 1) sbi(dispnum[3], 4);
  if(s2 == 1) sbi(dispnum[4], 4);

  for(i = 0; i < 5; i++){
    Write_1621(i * 2, dispnum[i], screen);
  }
}

void display_degrees(long int t, int p, char s1, char s2, int screen){
  uchar i;

  if(t > 360) t = 360; // Only degrees here, man

  dispnum[4] = char_null;
  dispnum[3] = num[(t / 100) % 10];
  dispnum[2] = num[(t / 10) % 10];
  dispnum[1] = num[t % 10];
  dispnum[0] = char_deg;

  switch(p){
    case 1:
      sbi(dispnum[0], 4);
      break;
    case 2:
      sbi(dispnum[1], 4);
      break;
    case 3:
      sbi(dispnum[2], 4);
      break;
    default:
      break;
  }

  if(t < 10){
    dispnum[2] = char_null;
  }
  if(t < 100){
    dispnum[3] = char_null;
  }

  if(s1 == 1) sbi(dispnum[3], 4);
  if(s2 == 1) sbi(dispnum[4], 4);

  for(i = 0; i < 5; i++){
    Write_1621(i * 2, dispnum[i], screen);
  }
}


void display_word(int word, int screen){
  //HT1621_all_off(16, screen);

  char* w = menu[word];
  int i = 4; // It's 4 because the display has 5 digits (0 counts as one)
  
  for (; *w != '\0'; ++w){
      Write_1621(i * 2, *w, screen);
      i--; // Start from the end fo the screen to the beginning
  }
  //int arraySize = w - word;
}


void display_mark(int up, int screen){
  char arrow = 0x00;
  sbi(arrow, 4);

  if(up != 0)
    Write_1621(6, arrow, screen);
  else
    Write_1621(8, arrow, screen);
}

void clear_mark(int up, int screen){
  char arrow = 0x00;
  cbi(arrow, 4);

  if(up == 0)
    Write_1621(6, arrow, screen);
  else
    Write_1621(8, arrow, screen);
}

