#define AUTOXDELAY 1
#define AUTOYDELAY 2
#define XDELAY 6
#define YDELAY 10
#define KEYPADDELAY 250
#define LOCKDELAY 100

#define XDEG 180
#define YDEG 90
#define MAXX 1200
#define MAXY 358
#define XCONST 6.67
#define YCONST 3.98
#define STARTX 580
#define STARTY 320

#define START 0x00
#define GET_MODE 0x01
#define CALIBRATE 0x02
#define GET_INPUTS 0x03
#define TARGET 0x04
#define GETPOWER 0x05
#define CHARGE 0x06
#define FUN 0x07

#define SPI_SER_B
#define SPI_CLK_DEVISOR 5
#use SPI.LIB

void init_LCD();
void write_control(char);
void write_string(char*, char);
void write_char(char);
void flush_lcdstring();
void flush_temp();
char read_keypad();
int readADC(int, int);
void stepX(int);
void stepY(int);
void calibrate(char);
int getDesiredX(int);
int getDesiredY(int);

char lcdstring[16], step[8], temp[4];
int xCount, yCount, setAngle[2], desiredX, desiredY, xSign, ySign, x, y;

void main()
{
   char state, key, mode, cal, xcal, power, setPower, i, z;

   WrPortI(SPCR, &SPCRShadow, 0x84); //init port A
   WrPortI(PADR, &PADRShadow, 0x00); //write 0's

	WrPortI(PCFR, &PCFRShadow, 0xff); //init port C - alt function

   WrPortI(PDFR, &PDFRShadow, 0x00); //init port D i/o
   WrPortI(PDDDR, &PDDDRShadow, 0xf0); //set pins 0 - 3 as input, 4 - 7 as output (PDDDR = Port D Data Direction Register)
   WrPortI(PDDCR, &PDDCRShadow, 0xff); //set output pins as open drain
   WrPortI(PDDR, &PDDRShadow, 0x00); //write 0's

   WrPortI(PEFR, &PEFRShadow, 0x00); //init port E i/0
	WrPortI(PEDDR, &PEDDRShadow, 0x8f); //pins 0 - 3 and 7 are output, 4 - 6 are input
	WrPortI(PEDR, &PEDRShadow, 0x00); //write 0's

   //define step sequence
	step[0] = 1; step[1] = 5; step[2] = 4; step[3] = 6; step[4] = 2; step[5] = 10; step[6] = 8; step[7] = 9;

   state = START;
   cal = 0x00;
   desiredX = 0x00;
   desiredY = 0x00;
   x = 0x00;
   y = 0x00;
   init_LCD();
   SPIinit();

   while(1)
   {costate{
      switch(state)
      {
      	case START:
         		write_control(0x01);
               waitfor(DelayMs(5));
         		i = 0x00;
               z = 0x00;
               xcal = 0x00;
					key = 0x00;
				   mode = 0x00;
               BitWrPortI(PEDR, &PEDRShadow, 0, 3); //deactivate charger
         		flush_lcdstring();
         		sprintf(lcdstring, "1)Auto 2)Manual:");
               write_string(lcdstring, sizeof(lcdstring));
               state = GET_MODE;
         break;

			case GET_MODE:
      			waitfor(key = read_keypad());
         		waitfor(DelayMs(KEYPADDELAY));
               if(key == '1') //auto control
               {
               	mode = 0x01;
                  flush_lcdstring();
                  write_control(0x01);
            		waitfor(DelayMs(5));
                  sprintf(lcdstring, "Calibrating...");
                  write_string(lcdstring, sizeof(lcdstring));
                  state = CALIBRATE;
               }
               else if(key == '2') //manual control
               {
               	mode = 0x02;
                  write_control(0x01);
            		waitfor(DelayMs(5));
                  sprintf(lcdstring, "Manual control..");
                  write_string(lcdstring, sizeof(lcdstring));
                  flush_lcdstring();
                  state = TARGET;
               }
               else if(key == 'C') //calibrate
               {
               	cal = 0x00;
                  mode = 0x00;
                  flush_lcdstring();
                  write_control(0x01);
            		waitfor(DelayMs(5));
                  sprintf(lcdstring, "Calibrating...");
                  write_string(lcdstring, sizeof(lcdstring));
                  state = CALIBRATE;
               }
               else if(key == 'B')
               {
               	cal = 0x00;
                  mode = 0x03;
                  flush_lcdstring();
                  write_control(0x01);
                  waitfor(DelayMs(5));
                  sprintf(lcdstring, "Weeeee!!!!");
                  write_string(lcdstring, sizeof(lcdstring));
                  state = CALIBRATE;
               }
         break;

			case CALIBRATE:
               if(cal != 1)//go to zero positions
               {
               	//calibrate X
               	if(xcal != 0x01 && BitRdPortI(PEDR, 5) != 0x01)
               	{
               		stepX(-1);
               		waitfor(DelayMs(AUTOXDELAY));
               	}
                  //calibrate Y
               	else if(BitRdPortI(PEDR, 6) != 0x01)
                  {
                  	xcal = 0x01;
                     stepY(-1);
                  	waitfor(DelayMs(AUTOYDELAY));
                  }
                  else
                  {
                   	cal = 1;
                     xCount = 0x00;
                     yCount = 0x00;
                  }
               }
               else if(mode == 0x03)
               	state = FUN;
               else //go to starting position
               {
                  if(yCount < STARTY)
                  {
                  	stepY(1);
                     waitfor(DelayMs(AUTOYDELAY));
                  }
                  else if(yCount > STARTY)
                  {
                  	stepY(-1);
                     waitfor(DelayMs(AUTOYDELAY));
                  }
						else if(xCount < STARTX)
                  {
                  	stepX(1);
                     waitfor(DelayMs(AUTOXDELAY));
                  }
                  else if(xCount > STARTX)
                  {
                  	stepX(-1);
                     waitfor(DelayMs(AUTOXDELAY));
                  }
						else //at starting positions
                  {
	                  flush_lcdstring();
                     flush_temp();
                     write_control(0x01);
                     waitfor(DelayMs(5));
	                  sprintf(lcdstring, "x angle: ");
	                  write_string(lcdstring, sizeof(lcdstring));
                     if(mode != 0x00)
                     	state = GET_INPUTS;
                     else
                     	state = START;
                     waitfor(DelayMs(LOCKDELAY)); //lock motors until inertia dies out
               		WrPortI(PADR, &PADRShadow, 0x00); //turn off the coils
                  }
               }
         break;

      	case GET_INPUTS:
               waitfor(key = read_keypad());
               waitfor(DelayMs(KEYPADDELAY));

               switch(key)
               {
               	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                     if(i < 4)
                     {
                     	if(i == 0 && key == '0')
                        	key = '-';
                        temp[i] = key;
                        write_char(key);
                     	i++;
                     }
                  break;

                  case '#':
                  	setAngle[z] = atoi(temp);
                  	i = 0;
                     write_control(0x01);
                     waitfor(DelayMs(5));
                     flush_lcdstring();
                     flush_temp();
                     if(z == 0)
                     {
                        sprintf(lcdstring, "y angle: ");

                     	z++;
                     }
                     else
                     {
                     	sprintf(lcdstring, "Targeting...");
                        desiredX = getDesiredX(setAngle[0]);
                        desiredY = getDesiredY(setAngle[1]);
                        if(desiredX != 0x00)
                        	xSign = desiredX/abs(desiredX);
                        else
                        	xSign = 0x00;
                        ySign = (desiredY)/abs(desiredY);
                        state = TARGET;
                     }
                     write_string(lcdstring, sizeof(lcdstring));
                  break;

                  case 'B':
                  	if(i != 0)
                     	i--;
                     temp[i] = 0x00;
                     write_control(0x89 + i);
                     waitfor(DelayMs(5));
                     write_char(0x20);
                     write_control(0x89 + i);
                     waitfor(DelayMs(5));
                  break;

                  case 'A':
                  	state = START;
                  break;

               }//end switch
         break; //break GET_INPUTS

         case TARGET:
         	if(mode == 0x01)
            {
               if(desiredX != 0x00)
               {
               	stepX(xSign);
                  waitfor(DelayMs(AUTOXDELAY));
                  desiredX = desiredX - xSign;
               }
               else if(desiredY != 0x00)
               {
                  stepY(ySign);
                  waitfor(DelayMs(AUTOYDELAY));
                  desiredY = desiredY - ySign;
               }
               else
               {
               	flush_lcdstring();
	               sprintf(lcdstring, "Power? 0 - 254:");
	               write_control(0x01);
	               waitfor(DelayMs(5));
	               write_string(lcdstring, sizeof(lcdstring));
	               write_control(0xC0);
	               waitfor(DelayMs(KEYPADDELAY));
	               state = GETPOWER;
                  waitfor(DelayMs(LOCKDELAY)); //lock motors until inertia dies out
               	WrPortI(PADR, &PADRShadow, 0x00); //turn off coils
               }
            }
            else if(mode == 0x02)
            {
					waitfor(key = read_keypad());
               switch(key)
               {
               	case '2':
               		stepY(-1);
                  	waitfor(DelayMs(YDELAY));
               	break;

               	case '8':
               		stepY(1);
                  	waitfor(DelayMs(YDELAY));
						break;

                  case '4':
               		stepX(-1);
                  	waitfor(DelayMs(XDELAY));
                  break;

               	case '6':
               		stepX(1);
                  	waitfor(DelayMs(XDELAY));
						break;

						case '#':
                  	flush_lcdstring();
	                  flush_temp();
	                  sprintf(lcdstring, "Power? 0 - 254:");
	                  write_control(0x01);
	                  waitfor(DelayMs(5));
	                  write_string(lcdstring, sizeof(lcdstring));
	                  write_control(0xC0);
	                  waitfor(DelayMs(KEYPADDELAY));
	                  state = GETPOWER;
						break;

               	case '*':
                  	BitWrPortI(PEDR, &PEDRShadow, 1, 7); //trigger scr...
	                  waitfor(DelayMs(50));                //wait...
	                  BitWrPortI(PEDR, &PEDRShadow, 0, 7); //release
						break;

                  case 'A':
                  	state = START;
                  break;
               }

               if(state != GETPOWER)
               {
              		WrPortI(PADR, &PADRShadow, 0x00); //turn off the coils when no button is pressed
	               write_control(0x01);
	               waitfor(DelayMs(5));
	               flush_lcdstring();
	               sprintf(lcdstring, "x: %d  y: %d", (int)((float)xCount / (float)XCONST - 90.0), (int)((float)yCount / (float)YCONST - 44.0));
	               write_string(lcdstring, sizeof(lcdstring));
               }
            }//end else if
         break;

         case GETPOWER:
               waitfor(key = read_keypad());
               waitfor(DelayMs(KEYPADDELAY));

               switch(key)
               {
               	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                     if(i < 3)
                     {
                     	temp[i] = key;
                     	write_char(key);
                     	i++;
                     }
                  break;

                  case '#':
                  	setPower = atoi(temp);
                  	i = 0;
                     write_control(0x01);
                     waitfor(DelayMs(5));
                     flush_lcdstring();
                     sprintf(lcdstring, "Charging... %d", power);
                     write_string(lcdstring, sizeof(lcdstring));
							state = CHARGE;
                  break;

                  case 'B':
                  	if(i != 0)
                     	i--;
                     temp[i] = 0x00;
                     write_control(0xC0 + i);
                     waitfor(DelayMs(5));
                     write_char(0x20);
                     write_control(0xC0 + i);
                     waitfor(DelayMs(5));
                  break;

                  case 'A':
                  	state = START;
                  break;
               }//end switch
         break; //break GETPOWER

         case CHARGE:
         		costate
               {
               	flush_lcdstring();
               	power = readADC(0, 50);
                  sprintf(lcdstring, "%d", power);
                  write_control(0x8D);
                  write_char(0x20);
                  write_char(0x20);
                  write_control(0x8C);
                  write_string(lcdstring, sizeof(lcdstring));
               	if(power <= setPower)
               		BitWrPortI(PEDR, &PEDRShadow, 1, 3); //activate charger
               	else
               		BitWrPortI(PEDR, &PEDRShadow, 0, 3); //deactivate charger
               	waitfor(DelayMs(200));
               }

               costate
               {
               	waitfor(key = read_keypad());
                  if(key == '*')
                  {
                  	waitfor(power == setPower);
                     BitWrPortI(PEDR, &PEDRShadow, 1, 7); //trigger scr...
               		waitfor(DelayMs(50));               //wait...
               		BitWrPortI(PEDR, &PEDRShadow, 0, 7); //release
                     key = 0x00;
                  }
                  else if(key == 'A')
                  	state = START;
               }
         break;

         case FUN:
         	costate
            {
               if(xCount != desiredX)
               {
               	stepX(xSign);
                  waitfor(DelayMs(AUTOXDELAY));
                  //desiredX = desiredX - xSign;
               }
               else if(yCount != desiredY)
               {
                  stepY(ySign);
                  waitfor(DelayMs(AUTOYDELAY));
                  //desiredY = desiredY - ySign;
               }
               else
               {
               	desiredX = (int)(rand() * (float)MAXX);
                  desiredY = (int)(rand() * (float)MAXY);
                  if((desiredX - xCount) != 0x00)
                  	xSign = (desiredX - xCount)/abs(desiredX - xCount);
                  else
                  	xSign = 0x00;

                  if((desiredY - xCount) != 0x00)
                  	ySign = (desiredY - yCount)/abs(desiredY - yCount);
                  else
                  	ySign = 0x00;
               }
            }
            costate
            {
            	waitfor(key = read_keypad());
            	if(key == 'A')
              	{
               	state = START;
                  WrPortI(PADR, &PADRShadow, 0x00); //turn off coils
               }
            }
         break;
      }//end state machine
   }}//end while & costate
}//end main

//									  ^^ State Machine ^^
//###########################################################################
//###########################################################################
//								         vv LCD vv

void init_LCD()
{
	char init;
   init = 1;
   while(init)
   {
   	costate {
		waitfor(DelayMs(16));
   	write_control(0x30);
   	waitfor(DelayMs(5));
   	write_control(0x30);
   	waitfor(DelayMs(1));
   	write_control(0x30);
   	write_control(0x20);
   	write_control(0x0f);
   	write_control(0x01);
		waitfor(DelayMs(5));
      init = 0;
      }//end costate
   }//end while
}

void write_control(char controlval)
{
	char high_nibble, low_nibble;

   BitWrPortI(PEDR, &PEDRShadow, 0, 1); //set as control write
   high_nibble = (controlval & 0xf0);
   low_nibble = (controlval & 0x0f) * 16;

   WrPortI(PDDR, &PDDRShadow, high_nibble);
   BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   BitWrPortI(PEDR, &PEDRShadow, 0, 0);

   WrPortI(PDDR, &PDDRShadow, low_nibble);
   BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   BitWrPortI(PEDR, &PEDRShadow, 0, 0);
}

void write_string(char *ptr_data, char length)
{
	char i, temp1, temp2, high_nibble, low_nibble;
   BitWrPortI(PEDR, &PEDRShadow, 1, 1); //set as data write
   for(i=0; i<length; i++)
   {
   	temp1 = *(ptr_data+i);
      temp2 = *(ptr_data+i);
      if(temp1!=0 && temp2!=0)
      {
      	high_nibble = (temp1 & 0xf0);
   		low_nibble = (temp2 & 0x0f) * 16;

   		WrPortI(PDDR, &PDDRShadow, high_nibble);
   		BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   		BitWrPortI(PEDR, &PEDRShadow, 0, 0);

   		WrPortI(PDDR, &PDDRShadow, low_nibble);
   		BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   		BitWrPortI(PEDR, &PEDRShadow, 0, 0);
      }
   }
}

void write_char(char dataval)
{
	char high_nibble, low_nibble;
   BitWrPortI(PEDR, &PEDRShadow, 1, 1); //set as data write
   high_nibble = (dataval & 0xf0);
   low_nibble = (dataval & 0x0f) * 16;

   WrPortI(PDDR, &PDDRShadow, high_nibble);
   BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   BitWrPortI(PEDR, &PEDRShadow, 0, 0);

   WrPortI(PDDR, &PDDRShadow, low_nibble);
   BitWrPortI(PEDR, &PEDRShadow, 1, 0);
   BitWrPortI(PEDR, &PEDRShadow, 0, 0);
}

void flush_lcdstring()
{
	char i;
	for(i=0; i<16; i++)
   	lcdstring[i] = 0x00;
}

//								      	^^ LCD ^^
//###########################################################################
//###########################################################################
//										vv Other Functions vv

void flush_temp()
{
	char i;
   for(i=0; i<4; i++)
   	temp[i] = 0x00;
}

char read_keypad()
{
	char in1, in2, inmask, colmask, rowmask, row, col;
	static const char_key[4][4] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
   rowmask = '\B00010000';
   colmask = '\B00000001';

   for(row=0; row<4; row++)
   {
      WrPortI(PDDR, &PDDRShadow, ~(rowmask << row));
      for(col=0; col<4; col++)
      {
      	in1 = RdPortI(PDDR);
         inmask = ~(colmask<<col);
         in2 = in1 & inmask;
         if(in1 == in2)
            return char_key[col][row];
      }
   }
}

void stepX(int dir)
{
	//static int x; // maybe make this global if this doesn't work

   x = x + dir;

   if(x <= -1)
      x = 7;
   else if(x >= 8)
      x = 0;

   xCount = xCount + dir;

	//WrPortI(PADR, &PADRShadow, step[x]);
   WrPortI(PADR, &PADRShadow, (step[y] << 4) | step[x]); //x is low nibble, y is upper nibble
}

void stepY(int dir)
{
	//static int y; // maybe make this global if this doesn't work

   y = y + dir;

   if(y <= -1)
      y = 7;
   else if(y >= 8)
      y = 0;

   yCount = yCount + dir;

   //WrPortI(PADR, &PADRShadow, (step[y] << 4));
   WrPortI(PADR, &PADRShadow, (step[y] << 4) | step[x]);
}

int readADC(int Channel, int Samples)
{
	int Count, j, k;
   unsigned char Command[3];
   unsigned long sum;
   unsigned char Data[3];
   unsigned char Datum;

   Command[0] = 0xC0;
   Command[1] = 0x00;
   Command[2] = 0x00;

   if(Channel == 1) Command[0]=Command[0]|0x20;
   if(Channel == 2) Command[0]=Command[0]|0x10;
   if(Channel == 3) Command[0]=Command[0]|0x30;

   sum = 0;
   for(Count=1; Count<=Samples; Count++)
   {
   	BitWrPortI(PEDR, &PEDRShadow, 0, 2);
      SPIWrRd(Command, Data, 3);
   	BitWrPortI(PEDR, &PEDRShadow, 1, 2);
      Datum = Data[0]<<5 | Data[1]>>3;
      sum += Datum;
   }
   return(sum / Samples);
}

int getDesiredX(int angle)
{
	int steps;

   if(angle > 100)
   	angle = 100;
   else if(angle < -90)
   	angle = -90;

   steps = (int)(((float)angle + 90) * (float)XCONST);
   steps = steps - xCount;

   return steps;
}

int getDesiredY(int angle)
{
	int steps;

   if(angle > 50)
   	angle = 50;
   else if(angle < 0)
   	angle = 0;

   steps = (int)(((float)angle + 44)  * (float)YCONST);
   steps = steps - yCount;

	return steps;
}