//E:\程式
#include <516.h>
#include <stdio.h>
char keytr[3]={'9','9','b'};
char keymatch=0;
int r=0,rr=0;
int sp=0;
int t=0,k=0,kk=0,key=0;//t is distance data of UART2 k is -48 of loop
int SW1=0,SW2=0,SW3=0;//99b
char a=' ';//S2BUF
char ang=' ';//SBUF
int angle=0;//SW of start bit
int cad=0;
int count=0;//drlay
#define angle_MAX 3//UART1 RX angle
unsigned char an[angle_MAX];
unsigned char ancount=0;
#define encodedata_MAX 4//UART2 RX data
char encodedata[encodedata_MAX];
char endatacont=0;
#define TX_length_MAX 16  //UART1 TX cad
unsigned char TXBUF[TX_length_MAX];
unsigned char TXposistion=0;
#define test_MAX 3
unsigned char test[test_MAX];
unsigned char testn=0;
main()
{
	//TMOD=0x20;
	//PCON=0x80;//2
	//TH1=253;
	//TR1=1;
	SCON=0x50;
	S2CON=0x50;//4
	S2BRT=238;//1
	AUXR2|=0x54;//3on
	AUXR=0x80;
	AUXIE|=0x10;
	IE=0x90;
	test[0]='1';
	while(1);
		
}

void delay(int count)
{
	int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<1500;j++)
	;
}

void S2CON_int(void) interrupt 12
{
	if(SW3==1)
	{
		S2BUF='\n';
		while(!(S2CON&0x02));
		S2CON&=0xfd;
		S2BUF=t/1000+48;
		while(!(S2CON&0x02));
		S2CON&=0xfd;
		S2BUF=(t%1000)/100+48;
		while(!(S2CON&0x02));
		S2CON&=0xfd;
		S2BUF=(t%100)/10+48;
		while(!(S2CON&0x02));
		S2CON&=0xfd;
		S2BUF=t%10+48;
		while(!(S2CON&0x02));
		S2CON&=0xfd;
		SW3=0;
	}
	if(S2CON&0x02==0x02)
	{
		S2CON&=0xfd;
	}
	if(S2CON&0x01==0x01)//RI
	{
		S2CON&=0xfe;
		a=S2BUF;
		if(SW2!=1)
		{			
			if(SW1!=1)
			{
			if(keymatch>2)
				keymatch=0;
			if(a==keytr[keymatch])
			{
				keymatch++;
			}else{
				keymatch=0;
			}
			S2BUF=keymatch+48;
			while(!(S2CON&0x02));
			S2CON&=0xfd;
			if(keymatch==3)
				SW1=1;
			if(keymatch>2)
				keymatch=0;
			}else
			{
				if(a=='\n')
					sp++;			
				if(sp<2)
				{
				S2BUF=sp+48;
				while(!(S2CON&0x02));
				S2CON&=0xfd;
				}
				if(sp==2&&a!='\n')
				{ 
					if(endatacont>3)
						endatacont=0;
					if(endatacont<encodedata_MAX)
					{
						encodedata[endatacont]=a;
						endatacont++;
					}
					if(endatacont==4)
					{
						SW2=1;
						sp=0;
					}					
					S2BUF=encodedata[endatacont-1];
					while(!(S2CON&0x02));
					S2CON&=0xfd;
					if(endatacont>3)
						endatacont=0;
				}
			}
		}else
		{
			kk=0;
			S2BUF='p';
	  	while(!(S2CON&0x02));
	  	S2CON&=0xfd;
			for(k=0;k<3;k++)
			{
				kk=kk+encodedata[k];
			}
			key=(kk&0x3f)+48;
			if(key!=encodedata[3])
			{
				SW2=0;
				SW1=0;
			}
			if(SW2==1)
			{
				for(k=0;k<3;k++)
				{
				encodedata[k]-=48;
				}
				t=(int)encodedata[0];
				t=t<<6;
				t+=encodedata[1];
				t=t<<6;
				t+=encodedata[2];
				endatacont=0;
				SW3=1;
				SW1=0;
				SW2=0;
			}
		}
		
		/*
		if(a=='9'){
			if(SW1==0){
				SW1++;
			}else if(SW1==1){
				SW2=1;
			}else{
				SW1=0;
				SW2=0;
			}
		}else{
			if(a=='b'&&SW2==1){
				SW3=1;
				S2BUF='7';
				while(!(S2CON&0x02));
				S2CON&=0xfd;
			}else{
				SW1=0;
				SW2=0;
				SW3=0;
			}
		}*//*
		if(SW2!=1)
		{
			if(a=='9')
				SW1=1;
			else if(SW1==1&&a=='b')
			{
				SW2=1;
				SW1=0;
			}
			else SW1=0;
		}*/
		
		/*if(SW3!=1)
		{
			if(a=='9')
			{
				if(SW1==1)
				{
					SW2=2;
					//test[1]=SW2;
				}
				SW1=1;
				//test[0]=SW1;
			}else	SW1=0;
			S2CON&=0xfe;
			if(SW2==2)
			{
				if(a=='b')
				{
					SW3=1;
					//test[2]=SW3;
					SW1=0;
					SW2=0;
				}
				else if(a!='9')
					{
						SW2=0;
					}
			}
		}
		/*else//////////////
		{
			S2CON&=0xfe;
			if(a=='\n')
				sp++;			
			if(sp==2)
			{
				if(endatacont<encodedata_MAX)
				{
					encodedata[endatacont]=a;
					endatacont++;
				}else 
				{
					for(k=0;k<3;k++)
					{
						kk=kk+encodedata[k];
					}
					key=(kk&0x3f)+48;
					if(key!=encodedata[3])
						SW3=0;
					else
					{
						for(k=0;k<3;k++)
						{
						encodedata[k]-=48;
						}
						t=(int)encodedata[0];
						t=t<<6;
						t+=encodedata[1];
						t=t<<6;
						t+=encodedata[2];
						endatacont=0;
				}
			}
		}
		endatacont=0;			
		}*/
		
	}
	/*
	if(S2CON&0x02==0x02)
	{
		S2CON&=0xfd;
		while(testn<test_MAX)
		{
			S2BUF=test[testn];
			while(!(S2CON&0x02));
			S2CON&=0xfd;
			testn++;
		}
		testn=0;
	}
	S2CON&=0xfd;
	/*
	S2CON&=0xfe;
	S2BUF=SW1+48;
	while(!(S2CON&0x02));
	S2CON&=0xfd;
	S2BUF=SW2+48;
	while(!(S2CON&0x02));
	S2CON&=0xfd;
	S2BUF=SW3+48;
	while(!(S2CON&0x02));
	S2CON&=0xfd;	
	S2BUF='\n';
	while(!(S2CON&0x02));
	S2CON&=0xfd;*/
}

void SCON_int(void) interrupt 4
{
	if(RI)
	{
		ang=SBUF;
		RI=0;
		if(ang=='a')
			angle=1;
		if(ang=='b')
		{
			angle=0;
			SBUF='\n';
		}
		if(angle==1&&ang!='a')
			{
				if(ancount<angle_MAX)
				{
					an[ancount]=ang;
					ancount++;
				}else
				{
					ancount=0;
					TXBUF[0]='M';
					TXBUF[1]='D';
					TXBUF[2]='0';
					TXBUF[3]=an[0];
					TXBUF[4]=an[1];
					TXBUF[5]=an[2];
					TXBUF[6]='0';
					TXBUF[7]=an[0];
					TXBUF[8]=an[1];
					TXBUF[9]=an[2];
					TXBUF[10]='0';
					TXBUF[11]='1';
					TXBUF[12]='0';
					TXBUF[13]='0';
					TXBUF[14]='0';
					TXBUF[15]='\n';
				}
			}
		}
		if(TI)
		{
			TI=0;
			if(TXposistion<TX_length_MAX)
			{
				SBUF=TXBUF[TXposistion];
				TXposistion++;
			}else
			{
				TXposistion=0;
			}
		}
		TI=0;
	/*
	if(TI)
	{
		TI=0;
		if(cad)
		{
			if(TXposistion<TX_length_MAX)
				{
					SBUF=TXBUF[TXposistion];					
					TXposistion++;
				}else 
				{
					TXposistion=0;
					cad=0;
					TI=0;
				}
			}
		}
			
	if(RI)
	{
		RI=0;
		a=SBUF;
		SBUF=a;
		RI=0;
		if(a=='\n')
			count++;
			if(count==20)
			{
				S2BUF=count;
				IE=0x81;
			}
	}*/
}