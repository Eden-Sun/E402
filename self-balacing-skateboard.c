#include <516.h>
#define SABER_MOTOR1_FULL_FORWARD 127
#define SABER_MOTOR1_FULL_REVERSE 1

#define SABER_MOTOR2_FULL_FORWARD 255
#define SABER_MOTOR2_FULL_REVERSE 129

int STD_LOOP_TIME = 9;
int lastLoopTime = 9;
int lastLoopUsefulTime =9;
unsigned long loopStartTime = 0;

int initial_balancetrim=362;
sbit issteer=P2^0;       //??????
sbit dir=P2^3;
sbit green=P2^2;
sbit yellow=P2^1;
sbit sysclk=P0^0;
sbit t2clk=P0^1;
sbit pulseR=P3^6;
sbit pulseL=P3^7;
sbit turnR=P2^6;
sbit turnL=P2^7;
char steer_control=0;
float forward_control=0;
int tt,tt2;
int er=0;
int sync=0;
int er_p=0;
int er_sum=0;
float level=0;
float aa = 0.005; //this means 0.5% of the accelerometer reading is fed into angle of tilt calculation with every loop of program (to correct the gyro).
//accel is sensitive to vibration which is why we effectively average it over time in this manner. You can increase aa if you want experiment. 
//too high though and the board may become too vibration sensitive.
float Steering;
float SteerValue;
float SteerCorrect;
float steersum;
int Steer = 0;
void PWM(unsigned char buf) ;
float accraw;
float x_acc;
float accsum;
float x_accdeg;

bit ppulseL;
bit ppulseR;
int pulsecountL=0;
int pulsecountR=0;
int pulsetimeL=0;
int pulsetimeR=0;
int gyrosum;
float hiresgyrosum;

#define g 238   //gyro
int     s=	303;	 //steer
#define t 238   //higyro

//float g,s,t; // g=275  s=300  t=277


float gangleratedeg;
float gangleratedeg2;

int adc1;
int adc4;
int adc5;


float gangleraterads;
float gyroscalingfactor = 2.2;

float overallgain=0.3; 

float gyroangledt;
float angle;
float anglerads;
float balance_torque;
float softstart;

float cur_speed;
//Need to know cycle time as gyro measures rate of turning. Needs to know time between each measurement
//so it can then work out angle it has turned through since the last measurement - so it can know angle of tilt from vertical.
float cycle_time = 0.01;
float balancetrim;

int balleft;
int balright;

float a0, a1, a2, a3, a4, a5, a6;  //Sav Golay variables. The TOBB bot describes this neat filter for accelerometer called Savitsky Golay filter.
//More on this plus links on my website.
float adcsteer;
float b0, b1, b2, b3, b4, b5, b6;
int i;
int j;
int tipstart;
signed char Motor1percent;
signed char Motor2percent;
#define TX_length_MAX 18  //
unsigned char TXBUF[TX_length_MAX];
unsigned char TXposistion=0;
bit softL=0;
bit softR=0;
bit softControl=0;
unsigned int analogRead(char channel)
{
    AUXR&=0x8F;                                 //3]ADRJ=0
    channel &= 0x07;                            
    ADCTL = 0x88|channel;                       //?}clAa�?  //-i?� ADCTL = 0x88|channel;
    while(!(ADCTL & 0x10));                    //�_�EAa�?-P�a                 
    return ((unsigned int)(ADCH<<2) | (ADCL & 3));
}

char map(char x, char in_min, char in_max, char out_min, char out_max)
{
  return  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void sample_inputs()  {
  gyrosum = 0;
  steersum = 0;
  hiresgyrosum = 0;
  
  accraw = analogRead(2); //read the accelerometer pin (0-1023)
  adc4 = analogRead(0);
	sysclk=0;
	//Take a set of 7 readings very fast
  for (j=0; j<7; j++) {
       adc1 = analogRead(1);
       
       adc5 = analogRead(3);
       gyrosum =  gyrosum + adc1; //sum of the 7 readings
       steersum=adc4;// = (float) steersum + adc4; //sum of the 7 readings
       hiresgyrosum = (float)hiresgyrosum +adc5; //sum of the 7 readings
                       }
                       

  a0 = a1;       
  a1 = a2;
  a2 = a3;
  a3 = a4;
  a4 = a5;
  a5 = a6;
  a6 = (float) accraw;
  
  accsum = (float) ((-2*a0) + (3*a1) + (6*a2) + (7*a3) + (6*a4) + (3*a5) + (-2*a6))/21; 

	b0 = b1;        
  b1 = b2;
  b2 = b3;
  b3 = b4;
  b4 = b5;
  b5 = b6;
  b6 = (float) adc4;
  
  adcsteer = (float) ((-2*b0) + (3*b1) + (6*b2) + (7*b3) + (6*b4) + (3*b5) + (-2*b6))/21;  
 
  gangleratedeg2 = (float) ((steersum/7) - s) * 2.44;  //divide by 0.41 as for low resolution balance gyro i.e. multiply by 2.44
	
 
  	//steer
		 SteerCorrect = 0;
		 
		 er=(tt2-tt)>10?10:(tt2-tt);
		 er=(tt2-tt)<-10?-10:(tt2-tt);
		 sync=1.0*er+0.3*(er-er_p)+0.1*er_sum;
		 er_p=er;
		 er_sum+=er;
		 if(er_sum>5000)er_sum=5000;
		 if(er_sum<-5000)er_sum=-5000;
	
		/*if(!issteer)SteerValue=512-((adcsteer) - s)* 6;	
		if(issteer)SteerValue=512;	                            //?? ????
		else {
			SteerValue=512-((adc4) - s)* 2;
		}
		if(SteerValue<524&&SteerValue>500)SteerValue=512;
		//else SteerValue=512+steer_control+50-sync;//+((adcsteer) - s)* 16;
		*/
		if(softControl){
			if(softR&&softL)SteerValue=512;
			else if(softR)SteerValue=512+30;
			else if(softL)SteerValue=512-30;
			else SteerValue=512;
		}
		else {
			if(turnR==0&&turnL==0)SteerValue=512;
			else if(turnR==0)SteerValue=512-30;
			else if(turnL==0)SteerValue=512+30;
			else SteerValue=512;
		}
		if (SteerValue < 1) {
								SteerValue = 1;
										} 
		if (SteerValue > 1023) {
								SteerValue = 1023;
											 }
		// steer											 
  x_accdeg = (float)((accsum - (initial_balancetrim + balancetrim))* 0.923315);  //0.862); //approx 1.16 steps per degree so divide by 1.16 i.e. multiply by 0.862 
  if (x_accdeg < -72) x_accdeg = -72; //rejects silly values to stop it going berserk!
  if (x_accdeg > 72) x_accdeg = 72;
  

  gangleratedeg = (float)(0+(g-(gyrosum/7) )*2.44); //divide by 0.41 for low res balance gyro i.e. multiply by 2.44
  
  if (gangleratedeg < -450) gangleratedeg = -450; //stops crazy values entering rest of the program
  if (gangleratedeg > 450) gangleratedeg = 450;
  

	tt=accsum;
	tt2=gyrosum/7;
  //..BUT...Hires gyro ideally used to re-calculate the rate of tipping in degrees per sec, i.e. use to calculate gangleratedeg IF rate is less than 100 deg per sec
  if (gangleratedeg < 100 && gangleratedeg > -100) {
      gangleratedeg = (float)(0+(t-(hiresgyrosum/7) )*0.538); //divide by 1.86 i.e. multiply by 0.538
        if (gangleratedeg < -110) gangleratedeg = -110;
        if (gangleratedeg > 110) gangleratedeg = 110;
                           }
 
  gyroangledt = (float) gyroscalingfactor * cycle_time * gangleratedeg;
    
  gangleraterads = (float) gangleratedeg * 0.017453; //convert to radians - just a scaling issue from history
  angle = (float) ((1-aa) * (angle + gyroangledt)) - (aa * x_accdeg); // minus sign here is because accel is wrong way around relative to the gyro
  //aa allows us to feed a bit (0.5%) of the accelerometer data into the angle calculation
  //so it slowly corrects the gyro (which drifts slowly with tinme remember). Accel sensitive to vibration though so aa does not want to be too large.
  //this is why these boards do not work if an accel only is used. We use gyro to do short term tilt measurements because it is insensitive to vibration
  //the video on my instructable shows the skateboard working fine over a brick cobbled surface - vibration +++ !
  anglerads = (float) angle * 0.017453; //converting to radians again a historic scaling issue from past software
  
  
  
  balance_torque = (float) (4.5 * anglerads) + (0.5 * gangleraterads);
   cur_speed = (float) (cur_speed + (anglerads * 6 * cycle_time)) * 0.999;
  
 
  level = (float)(balance_torque + cur_speed) * overallgain+forward_control;  
	//SteerValue=512+level*200
	//level = (float)balance_torque * overallgain+forward_control; 
	//level =0.18;
  
}

 /*void sendduty(char left,char power){          //?????
	unsigned char back=(power<0);
	unsigned char num=left>0?131:135;
	unsigned char duty=power*1.26*(power<0?-1:1);
		
	

	
 	S2BUF=num;                       //????2  ??
	while((S2CON&0x02)==0);
	S2CON&=0xfd;
 
 	S2BUF=back;
	while((S2CON&0x02)==0);
	S2CON&=0xfd;
	
	S2BUF=duty;
	while((S2CON&0x02)==0);
	S2CON&=0xfd;
	
	S2BUF=((num+back+duty) & 0x7f);
	while((S2CON&0x02)==0);
	S2CON&=0xfd;
}
  */
void set_motor()   {
  
	unsigned char cSpeedVal_Motor1 = 0;
  unsigned char cSpeedVal_Motor2 = 0;
  
  level = level * 200; //changes it to a scale of about -100 to +100
  if (level < -100) {level = -100;}
  if (level > 100) {level = 100;}
  
  
  Steer = (float) SteerValue - SteerCorrect;  //at this point is on the 0-1023 scale 
  //SteerValue is either 512 for dead ahead or bigger/smaller if you are pressing steering switch left or right
  //SteerCorrect is the "adjustment" made by the second gyro that resists sudden turns if one wheel hits a small object for example.
  Steer = (Steer - 512) * 0.19;   //gets it down from 0-1023 (with 512 as the middle no-steer point) to -100 to +100 with 0 as the middle no-steer point on scale

  
//set motors using the simplified serial Sabertooth protocol (same for smaller 2 x 5 Watt Sabertooth by the way) 
 if(level>0){               
 Motor1percent = (signed char) level + Steer;
 Motor2percent = (signed char) level - Steer;
 }
 else	{               
 Motor1percent = (signed char) level - Steer;
 Motor2percent = (signed char) level + Steer;
 }
	 
 if (Motor1percent > 100) Motor1percent = 100;
 if (Motor1percent < -100) Motor1percent = -100;
 if (Motor2percent > 100) Motor2percent = 100;
 if (Motor2percent < -100) Motor2percent = -100;
 
 cSpeedVal_Motor1 = map (Motor1percent, -100, 100,    SABER_MOTOR1_FULL_REVERSE, SABER_MOTOR1_FULL_FORWARD);
                         
 cSpeedVal_Motor2 = map (Motor2percent, -100, 100,    SABER_MOTOR2_FULL_REVERSE,  SABER_MOTOR2_FULL_FORWARD);

	//sendduty(0,Motor1percent);
	//sendduty(1,Motor2percent);
	                  
		PWM(cSpeedVal_Motor1);
		PWM(cSpeedVal_Motor2);
	sysclk=1;
	
		TXBUF[0]='L';         //??!!!
		TXBUF[1]='=';
		if (tt<0){
			TXBUF[2]	=	'-';
			tt=-tt;
		}
		else TXBUF[2]	=	' ';
		TXBUF[3]	=	(tt/10000)+48;
		TXBUF[4]	=	((tt % 10000)/1000)+48;
		TXBUF[5]  =	((tt % 1000)/100)+48;
		TXBUF[6]	=	((tt%100)/10)+48;
		TXBUF[7]	=	(tt%10)+48;
		TXBUF[8]	='R';
		TXBUF[9]	='=';
    if (tt2<0){
		TXBUF[10]	=	'-';
			tt2=-tt2;
		}
		else TXBUF[10]	=	' ';
		
		TXBUF[11]	=	(tt2/10000)+48;
		TXBUF[12]	=	((tt2 % 10000)/1000)+48;
		TXBUF[13]	=	((tt2 % 1000)/100)+48;
		TXBUF[14]	=	((tt2%100)/10)+48;
		TXBUF[15]	=	(tt2%10)+48;
		TXBUF[16]	=	13;
		TXBUF[17]	=	10;
		SBUF=TXBUF[0];
		
	sysclk=1;
   //Serial.write (cSpeedVal_Motor1);
  // Serial.write (cSpeedVal_Motor2);  
}

void waitstartloop ()   {   
  
  tipstart = 0;
  overallgain = 0;
  cur_speed = 0;
  level = 0;
  Steer = 0;
  balancetrim = 0;
  for (i=0; i<200; i++) {
  	
		S2BUF=0xaa;
		while((S2CON&0x02)==0);
		S2CON&=0xfd;
		s=analogRead(0);
                         }
                      
    //now you have stepped away from baord having turned it on, we get 7 readings from each gyro (and a hires reading from the balance gyro)                     
    //g = (float) gyrosum/7;  //gyro balance value when stationary i.e. 1.35V
    //s = (float) adc4;  //steer gyro value when stationary i.e. about 1.32V
    //t = (float) hiresgyrosum/7; //hiresgyro balance output when stationary  i.e. about 1.38V
    
    
		green=0;
			while (tipstart < 5) {
        for (i=0; i<10; i++) {
                  sample_inputs();
                  
                             }
	
  //x_accdeg is tilt angle from accelerometer in degrees                      
              if (x_accdeg < -4 || x_accdeg > 4) {
              //*****ADJUST THESE LIMITS TO SUIT YOUR BOARD SO TILTSTART KICKS IN WHERE YOU WANT IT TO*******
              //MY IMU IS NOT QUITE VERTICALLY MOUNTED
                                  tipstart = 5;
                                  overallgain = 0;
                                  cur_speed = 0;
                                  level = 0;
                                  Steer = 0;
                                  balancetrim = 0; 
																
                                                   }
              else {
                     tipstart = 5;
                    }   
       
    }
	yellow=0;
  

overallgain = 0.3;  //softstart value. Gain will now rise to final of 0.5 at rate of 0.005 per program loop. 
//i.e. it will go from 0.3 to 0.5 over the first 4 seconds after tipstart has been activated


angle = 0;
cur_speed = 0;
Steering = 512;
SteerValue = 512;
balancetrim = 0;
		
		T2CON=0x04;  // TR2=1;
         
           
  }




main()
	{
	
	TMOD=0x22;  
	TH0=253;   //PWM=f/256/18=400MHZ @22M
	TL0=253;  //PWM
	TH1=250; //255  
	TL1=250;  //255
	AUXR2=0xd8;  //t1 t0 X12  S2MOD=1
		TR1=1;
	TR0=1;
		SCON=0x50;
	TL2=0;
	TH2=256-36;        //?????
	RCAP2L=TL2;
	RCAP2H=TH2;	  
	IT0=1;
	//PCON|=0x80;  //BR*2
		
	CMOD=0x04;
	CR=1;
		
	IE=0xb1;
	P2=0xff;
	P3=0xff;
	P1M0=0x0f;
	S2CON|=0x50;
	S2BRT=256-12;
	waitstartloop();
	while(1);
	
	
}
	void EX0_int(void) interrupt 0     
	{
		if(!pulseR)pulsecountR++;
		else pulsecountR--;
	}
	void uart_inter(void) interrupt 4            //??SEGWAY   ????
{
	if(TI){
		TI=0;
		if (TXposistion<TX_length_MAX){
			SBUF=TXBUF[TXposistion];
			TXposistion++;
		}else {
			TXposistion=0;
		}
	}
	if(RI){	
		RI=0;
		switch(SBUF) {
				case 'w':
					forward_control+=0.09;
					break;
				case 's':
					forward_control-=0.09;
					break;
				case 'a':
				case 'A':
					softL=1;
					softR=0;
					break;
				case 'D':
				case 'd':
					softR=1;
					softL=0;
					break;	
				case 'q':
					softControl=1;
					break;
				case 'e':
					softControl=0;
					break;				
				case ' ':
				case 'r':
					forward_control=0;	
					steer_control=0;
					softL=0;
					softR=0;
					break;
				
				case 'u':
					initial_balancetrim+=10;
					steer_control=0;
					break;
				
				case 'j':
					initial_balancetrim-=10;
					steer_control=0;
					break;
				case 'i':
					initial_balancetrim+=1;
					steer_control=0;
					break;
				
				case 'k':
					initial_balancetrim-=1;
					steer_control=0;
					break;
				
				default :
					forward_control=0;
					steer_control=0;
					break;
			}
	}
}	
	void t2(void) interrupt 5 
{

TF2=0;	
t2clk=~t2clk;	

  
  sample_inputs();
  set_motor();


   if (overallgain < 0.5) {
       overallgain = (float)overallgain + 0.005;
                          }
   if (overallgain > 0.5) {overallgain = 0.5;}
 //XXXXXXXXXXXXXXX end of softstart code XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

  

	
}

	void PWM(unsigned char buf)     //??PWM
{
	
		if(buf==0){  //all stop
			CCAPM2=0x00;
			CCAPM3=0x00;
			CCAPM4=0x00;
			CCAPM5=0x00;
		}
		else if(buf<64){
			
			CCAP2H=255-(buf*4);
			CCAPM2=0x42;
			CCAPM3=0x00;
		}else if(buf<128){
			
			CCAPM2=0x00;
			CCAPM3=0x42;
			CCAP3H=(buf-64)*4;            //2.3??
			
		}else if(buf<192){
			
			CCAPM4=0x42;
			CCAP4H=255-((buf-128)*4);
			CCAPM5=0x00;
			
		}else {
			
			CCAPM4=0x00;
			CCAPM5=0x42;
			CCAP5H=((buf-192)*4);         //4.5??  
			
		}
	
}
