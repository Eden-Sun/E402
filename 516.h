// MPC82G516

// (System)
sfr ACC       = 0xE0;
sfr B         = 0xF0;
sfr SP        = 0x81;
sfr DPL       = 0x82;
sfr DPH       = 0x83;
sfr PSW       = 0xD0;

// (Interrupt)
sfr IE        = 0xA8;
sfr AUXIE     = 0xAD;
sfr XICON     = 0xC0;
sfr IP        = 0xB8;
sfr IPH       = 0xB7;
sfr AUXIP     = 0xAE;
sfr AUXIPH    = 0xAF;

// (I/O Port)
sfr P0        = 0x80;
sfr P1        = 0x90;
sfr P2        = 0xA0;
sfr P3        = 0xB0;
sfr P4        = 0xE8;
sfr P0M0      = 0x93;
sfr P0M1      = 0x94;
sfr P1M0      = 0x91;
sfr P1M1      = 0x92;
sfr P2M0      = 0x95;
sfr P2M1      = 0x96;
sfr P3M0      = 0xB1;
sfr P3M1      = 0xB2;
sfr P4M0      = 0xB3;
sfr P4M1      = 0xB4;

// (Timer)
sfr TCON      = 0x88;
sfr TMOD      = 0x89;
sfr T2CON     = 0xC8;
sfr T2MOD     = 0xC9;
sfr RCAP2L    = 0xCA;
sfr RCAP2H    = 0xCB;
sfr TL0       = 0x8A;
sfr TH0       = 0x8C;
sfr TL1       = 0x8B;
sfr TH1       = 0x8D;
sfr TL2       = 0xCC;
sfr TH2       = 0xCD;

// (Serial Port)
sfr SCON      = 0x98;
sfr SBUF      = 0x99;
sfr SADDR     = 0xA9;
sfr SADEN     = 0xB9;
sfr S2CON     = 0xAA;
sfr S2BRT     = 0xBA;
sfr S2BUF     = 0x9A;

// (ADC)
sfr ADCTL     = 0xC5;
sfr ADCH      = 0xC6;
sfr ADCL      = 0xBE;
sfr ADC       = 0xC6; //the same as ADCH, the higher 8 bits

// (Keypad Interrupt)
sfr KBCON     = 0xD6;
sfr KBPATN    = 0xD5;
sfr KBMASK    = 0xD7;

// (SPI)
sfr SPCTL     = 0x85;
sfr SPDAT     = 0x86;
sfr SPSTAT    = 0x84;

// (PCA)
sfr CCON      = 0xD8;
sfr CMOD      = 0xD9;
sfr CL        = 0xE9;
sfr CH        = 0xF9;
sfr CCAPM0    = 0xDA;
sfr CCAPM1    = 0xDB;
sfr CCAPM2    = 0xDC;
sfr CCAPM3    = 0xDD;
sfr CCAPM4    = 0xDE;
sfr CCAPM5    = 0xDF;
sfr CCAP0H    = 0xFA;
sfr CCAP0L    = 0xEA;
sfr CCAP1H    = 0xFB;
sfr CCAP1L    = 0xEB;
sfr CCAP2H    = 0xFC;
sfr CCAP2L    = 0xEC;
sfr CCAP3H    = 0xFD;
sfr CCAP3L    = 0xED;
sfr CCAP4H    = 0xFE;
sfr CCAP4L    = 0xEE;
sfr CCAP5H    = 0xFF;
sfr CCAP5L    = 0xEF;
sfr PCAPWM0   = 0xF2;
sfr PCAPWM1   = 0xF3;
sfr PCAPWM2   = 0xF4;
sfr PCAPWM3   = 0xF5;
sfr PCAPWM4   = 0xF6;
sfr PCAPWM5   = 0xF7;

// (Others)
sfr PCON      = 0x87;
sfr PCON2     = 0xC7;
sfr AUXR      = 0x8E;
sfr AUXR1     = 0xA2;
sfr AUXR2     = 0xA6;
sfr STRETCH   = 0x8F;
sfr EVRCR     = 0x97;
sfr WDTCR     = 0xE1;

// (ISP)
sfr ISPCR     = 0xE7;
sfr IFMT      = 0xE5;
sfr IFD       = 0xE2;
sfr IFADRH    = 0xE3;
sfr IFADRL    = 0xE4;
sfr SCMD      = 0xE6;

// (bits in PSW)
sbit CY       = PSW^7;
sbit AC       = PSW^6;
sbit F0       = PSW^5;
sbit RS1      = PSW^4;
sbit RS0      = PSW^3;
sbit OV       = PSW^2;
sbit P        = PSW^0;

// (bits in IE)
sbit EA       = IE^7;
sbit ET2      = IE^5;
sbit ES       = IE^4;
sbit ET1      = IE^3;
sbit EX1      = IE^2;
sbit ET0      = IE^1;
sbit EX0      = IE^0;

// (bits in IP)
sbit PT2      = IP^5;
sbit PS       = IP^4;
sbit PT1      = IP^3;
sbit PX1      = IP^2;
sbit PT0      = IP^1;
sbit PX0      = IP^0;

// (bits in SCON)
sbit FE       = SCON^7;
sbit SM0      = SCON^7;
sbit SM1      = SCON^6;
sbit SM2      = SCON^5;
sbit REN      = SCON^4;
sbit TB8      = SCON^3;
sbit RB8      = SCON^2;
sbit TI       = SCON^1;
sbit RI       = SCON^0;

// (bits in TCON)
sbit TF1      = TCON^7;
sbit TR1      = TCON^6;
sbit TF0      = TCON^5;
sbit TR0      = TCON^4;
sbit IE1      = TCON^3;
sbit IT1      = TCON^2;
sbit IE0      = TCON^1;
sbit IT0      = TCON^0;

// (bits in T2CON)
sbit TF2      = T2CON^7;
sbit EXF2     = T2CON^6;
sbit RCLK     = T2CON^5;
sbit TCLK     = T2CON^4;
sbit EXEN2    = T2CON^3;
sbit TR2      = T2CON^2;
sbit C_T2     = T2CON^1;
sbit CP_RL2   = T2CON^0;

// (bits in CCON)
sbit CF       = CCON^7;
sbit CR       = CCON^6;
sbit CCF5     = CCON^5;
sbit CCF4     = CCON^4;
sbit CCF3     = CCON^3;
sbit CCF2     = CCON^2;
sbit CCF1     = CCON^1;
sbit CCF0     = CCON^0;

// (bits in XICON)
sbit PX3      = XICON^7;
sbit EX3      = XICON^6;
sbit IE3      = XICON^5;
sbit IT3      = XICON^4;
sbit PX2      = XICON^3;
sbit EX2      = XICON^2;
sbit IE2      = XICON^1;
sbit IT2      = XICON^0;

// (bits in P0)
sbit P07      = P0^7;
sbit P06      = P0^6;
sbit P05      = P0^5;
sbit P04      = P0^4;
sbit P03      = P0^3;
sbit P02      = P0^2;
sbit P01      = P0^1;
sbit P00      = P0^0;

// (bits in P1)
sbit P17      = P1^7;
sbit P16      = P1^6;
sbit P15      = P1^5;
sbit P14      = P1^4;
sbit P13      = P1^3;
sbit P12      = P1^2;
sbit P11      = P1^1;
sbit P10      = P1^0;

// (bits in P2)
sbit P27      = P2^7;
sbit P26      = P2^6;
sbit P25      = P2^5;
sbit P24      = P2^4;
sbit P23      = P2^3;
sbit P22      = P2^2;
sbit P21      = P2^1;
sbit P20      = P2^0;

// (bits in P3)
sbit P37      = P3^7;
sbit P36      = P3^6;
sbit P35      = P3^5;
sbit P34      = P3^4;
sbit P33      = P3^3;
sbit P32      = P3^2;
sbit P31      = P3^1;
sbit P30      = P3^0;

// (bits in P4)
sbit P43      = P4^3;
sbit P42      = P4^2;
sbit P41      = P4^1;
sbit P40      = P4^0;

// (bits in P1, alternate)
sbit T2EX     = P1^1;
sbit T2       = P1^0;
sbit S2TXD    = P1^3;
sbit S2RXD    = P1^2;
sbit SPI_CLK  = P1^7;
sbit SPI_MISO = P1^6;
sbit SPI_MOSI = P1^5;
sbit SPI_SS   = P1^4;
sbit CEX5     = P1^7;
sbit CEX4     = P1^6;
sbit CEX3     = P1^5;
sbit CEX2     = P1^4;
sbit CEX1     = P1^3;
sbit CEX0     = P1^2;
sbit ECI      = P1^1;

// (bits in P3, alternate)
sbit RD       = P3^7;
sbit WR       = P3^6;
sbit T1       = P3^5;
sbit T0       = P3^4;
sbit INT1     = P3^3;
sbit INT0     = P3^2;
sbit TXD      = P3^1;
sbit RXD      = P3^0;
sbit S2CKO    = P3^5;
sbit T0CKO    = P3^4;

// (bits in P4, alternate)
sbit INT2     = P4^3;
sbit INT3     = P4^2;