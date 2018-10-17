//-----------------------------------------------------------------------------
//   File:      FX2regs.h
//   Contents:   EZ-USB FX2/FX2LP/FX1 register declarations and bit mask definitions.
//
// $Archive: /USB/Target/Inc/fx2regs.h $
// $Date: 4/13/05 4:29p $
// $Revision: 42 $
//
//
//   Copyright (c) 2005 Cypress Semiconductor, All rights reserved
//-----------------------------------------------------------------------------

#ifndef FX2REGS_H   /* Header Sentry */
#define FX2REGS_H

// #include "Fx2.h"

//-----------------------------------------------------------------------------
// FX2/FX2LP/FX1 Related Register Assignments
//-----------------------------------------------------------------------------

// The Ez-USB FX2/FX2LP/FX1 registers are defined here. We use fx2regs.h for register 
// address allocation by using "#define ALLOCATE_EXTERN". 
// When using "#define ALLOCATE_EXTERN", you get (for instance): 
// xdata volatile BYTE OUT7BUF[64]   _at_   0x7B40;
// Such lines are created from FX2.h by using the preprocessor. 
// Incidently, these lines will not generate any space in the resulting hex 
// file; they just bind the symbols to the addresses for compilation. 
// You just need to put "#define ALLOCATE_EXTERN" in your main program file; 
// i.e. fw.c or a stand-alone C source file. 
// Without "#define ALLOCATE_EXTERN", you just get the external reference: 
// extern xdata volatile BYTE OUT7BUF[64]   ;//   0x7B40;
// This uses the concatenation operator "##" to insert a comment "//" 
// to cut off the end of the line, "_at_   0x7B40;", which is not wanted.

typedef unsigned char BYTE;

__xdata volatile BYTE __at(0xE400) GPIF_WAVE_DATA;
__xdata volatile BYTE __at(0xE480) RES_WAVEDATA_END;

/* General Configuration */

__xdata volatile BYTE __at(0xE600) CPUCS;  /* Control & Status */
__xdata volatile BYTE __at(0xE601) IFCONFIG;  /* Interface Configuration */
__xdata volatile BYTE __at(0xE602) PINFLAGSAB;  /* FIFO FLAGA and FLAGB Assignments */
__xdata volatile BYTE __at(0xE603) PINFLAGSCD;  /* FIFO FLAGC and FLAGD Assignments */
__xdata volatile BYTE __at(0xE604) FIFORESET;  /* Restore FIFOS to default state */
__xdata volatile BYTE __at(0xE605) BREAKPT;  /* Breakpoint */
__xdata volatile BYTE __at(0xE606) BPADDRH;  /* Breakpoint Address H */
__xdata volatile BYTE __at(0xE607) BPADDRL;  /* Breakpoint Address L */
__xdata volatile BYTE __at(0xE608) UART230;  /* 230 Kbaud clock for T0,T1,T2 */
__xdata volatile BYTE __at(0xE609) FIFOPINPOLAR;  /* FIFO polarities */
__xdata volatile BYTE __at(0xE60A) REVID;  /* Chip Revision */
__xdata volatile BYTE __at(0xE60B) REVCTL;  /* Chip Revision Control */

/* Endpoint Configuration */

__xdata volatile BYTE __at(0xE610) EP1OUTCFG;  /* Endpoint 1-OUT Configuration */
__xdata volatile BYTE __at(0xE611) EP1INCFG;  /* Endpoint 1-IN Configuration */
__xdata volatile BYTE __at(0xE612) EP2CFG;  /* Endpoint 2 Configuration */
__xdata volatile BYTE __at(0xE613) EP4CFG;  /* Endpoint 4 Configuration */
__xdata volatile BYTE __at(0xE614) EP6CFG;  /* Endpoint 6 Configuration */
__xdata volatile BYTE __at(0xE615) EP8CFG;  /* Endpoint 8 Configuration */
__xdata volatile BYTE __at(0xE618) EP2FIFOCFG;  /* Endpoint 2 FIFO configuration */
__xdata volatile BYTE __at(0xE619) EP4FIFOCFG;  /* Endpoint 4 FIFO configuration */
__xdata volatile BYTE __at(0xE61A) EP6FIFOCFG;  /* Endpoint 6 FIFO configuration */
__xdata volatile BYTE __at(0xE61B) EP8FIFOCFG;  /* Endpoint 8 FIFO configuration */
__xdata volatile BYTE __at(0xE620) EP2AUTOINLENH;  /* Endpoint 2 Packet Length H (IN only) */
__xdata volatile BYTE __at(0xE621) EP2AUTOINLENL;  /* Endpoint 2 Packet Length L (IN only) */
__xdata volatile BYTE __at(0xE622) EP4AUTOINLENH;  /* Endpoint 4 Packet Length H (IN only) */
__xdata volatile BYTE __at(0xE623) EP4AUTOINLENL;  /* Endpoint 4 Packet Length L (IN only) */
__xdata volatile BYTE __at(0xE624) EP6AUTOINLENH;  /* Endpoint 6 Packet Length H (IN only) */
__xdata volatile BYTE __at(0xE625) EP6AUTOINLENL;  /* Endpoint 6 Packet Length L (IN only) */
__xdata volatile BYTE __at(0xE626) EP8AUTOINLENH;  /* Endpoint 8 Packet Length H (IN only) */
__xdata volatile BYTE __at(0xE627) EP8AUTOINLENL;  /* Endpoint 8 Packet Length L (IN only) */
__xdata volatile BYTE __at(0xE630) EP2FIFOPFH;  /* EP2 Programmable Flag trigger H */
__xdata volatile BYTE __at(0xE631) EP2FIFOPFL;  /* EP2 Programmable Flag trigger L */
__xdata volatile BYTE __at(0xE632) EP4FIFOPFH;  /* EP4 Programmable Flag trigger H */
__xdata volatile BYTE __at(0xE633) EP4FIFOPFL;  /* EP4 Programmable Flag trigger L */
__xdata volatile BYTE __at(0xE634) EP6FIFOPFH;  /* EP6 Programmable Flag trigger H */
__xdata volatile BYTE __at(0xE635) EP6FIFOPFL;  /* EP6 Programmable Flag trigger L */
__xdata volatile BYTE __at(0xE636) EP8FIFOPFH;  /* EP8 Programmable Flag trigger H */
__xdata volatile BYTE __at(0xE637) EP8FIFOPFL;  /* EP8 Programmable Flag trigger L */
__xdata volatile BYTE __at(0xE640) EP2ISOINPKTS;  /* EP2 (if ISO) IN Packets per frame (1-3) */
__xdata volatile BYTE __at(0xE641) EP4ISOINPKTS;  /* EP4 (if ISO) IN Packets per frame (1-3) */
__xdata volatile BYTE __at(0xE642) EP6ISOINPKTS;  /* EP6 (if ISO) IN Packets per frame (1-3) */
__xdata volatile BYTE __at(0xE643) EP8ISOINPKTS;  /* EP8 (if ISO) IN Packets per frame (1-3) */
__xdata volatile BYTE __at(0xE648) INPKTEND;  /* Force IN Packet End */
__xdata volatile BYTE __at(0xE649) OUTPKTEND;  /* Force OUT Packet End */

/* Interrupts */

__xdata volatile BYTE __at(0xE650) EP2FIFOIE;  /* Endpoint 2 Flag Interrupt Enable */
__xdata volatile BYTE __at(0xE651) EP2FIFOIRQ;  /* Endpoint 2 Flag Interrupt Request */
__xdata volatile BYTE __at(0xE652) EP4FIFOIE;  /* Endpoint 4 Flag Interrupt Enable */
__xdata volatile BYTE __at(0xE653) EP4FIFOIRQ;  /* Endpoint 4 Flag Interrupt Request */
__xdata volatile BYTE __at(0xE654) EP6FIFOIE;  /* Endpoint 6 Flag Interrupt Enable */
__xdata volatile BYTE __at(0xE655) EP6FIFOIRQ;  /* Endpoint 6 Flag Interrupt Request */
__xdata volatile BYTE __at(0xE656) EP8FIFOIE;  /* Endpoint 8 Flag Interrupt Enable */
__xdata volatile BYTE __at(0xE657) EP8FIFOIRQ;  /* Endpoint 8 Flag Interrupt Request */
__xdata volatile BYTE __at(0xE658) IBNIE;  /* IN-BULK-NAK Interrupt Enable */
__xdata volatile BYTE __at(0xE659) IBNIRQ;  /* IN-BULK-NAK interrupt Request */
__xdata volatile BYTE __at(0xE65A) NAKIE;  /* Endpoint Ping NAK interrupt Enable */
__xdata volatile BYTE __at(0xE65B) NAKIRQ;  /* Endpoint Ping NAK interrupt Request */
__xdata volatile BYTE __at(0xE65C) USBIE;  /* USB Int Enables */
__xdata volatile BYTE __at(0xE65D) USBIRQ;  /* USB Interrupt Requests */
__xdata volatile BYTE __at(0xE65E) EPIE;  /* Endpoint Interrupt Enables */
__xdata volatile BYTE __at(0xE65F) EPIRQ;  /* Endpoint Interrupt Requests */
__xdata volatile BYTE __at(0xE660) GPIFIE;  /* GPIF Interrupt Enable */
__xdata volatile BYTE __at(0xE661) GPIFIRQ;  /* GPIF Interrupt Request */
__xdata volatile BYTE __at(0xE662) USBERRIE;  /* USB Error Interrupt Enables */
__xdata volatile BYTE __at(0xE663) USBERRIRQ;  /* USB Error Interrupt Requests */
__xdata volatile BYTE __at(0xE664) ERRCNTLIM;  /* USB Error counter and limit */
__xdata volatile BYTE __at(0xE665) CLRERRCNT;  /* Clear Error Counter EC[3..0] */
__xdata volatile BYTE __at(0xE666) INT2IVEC;  /* Interupt 2 (USB) Autovector */
__xdata volatile BYTE __at(0xE667) INT4IVEC;  /* Interupt 4 (FIFOS & GPIF) Autovector */
__xdata volatile BYTE __at(0xE668) INTSETUP;  /* Interrupt 2&4 Setup */

/* Input/Output */

__xdata volatile BYTE __at(0xE670) PORTACFG;  /* I/O PORTA Alternate Configuration */
__xdata volatile BYTE __at(0xE671) PORTCCFG;  /* I/O PORTC Alternate Configuration */
__xdata volatile BYTE __at(0xE672) PORTECFG;  /* I/O PORTE Alternate Configuration */
__xdata volatile BYTE __at(0xE678) I2CS;  /* Control & Status */
__xdata volatile BYTE __at(0xE679) I2DAT;  /* Data */
__xdata volatile BYTE __at(0xE67A) I2CTL;  /* I2C Control */
__xdata volatile BYTE __at(0xE67B) XAUTODAT1;  /* Autoptr1 MOVX access */
__xdata volatile BYTE __at(0xE67C) XAUTODAT2;  /* Autoptr2 MOVX access */

#define EXTAUTODAT1 XAUTODAT1
#define EXTAUTODAT2 XAUTODAT2

/* USB Control */

__xdata volatile BYTE __at(0xE680) USBCS;  /* USB Control & Status */
__xdata volatile BYTE __at(0xE681) SUSPEND;  /* Put chip into suspend */
__xdata volatile BYTE __at(0xE682) WAKEUPCS;  /* Wakeup source and polarity */
__xdata volatile BYTE __at(0xE683) TOGCTL;  /* Toggle Control */
__xdata volatile BYTE __at(0xE684) USBFRAMEH;  /* USB Frame count H */
__xdata volatile BYTE __at(0xE685) USBFRAMEL;  /* USB Frame count L */
__xdata volatile BYTE __at(0xE686) MICROFRAME;  /* Microframe count, 0-7 */
__xdata volatile BYTE __at(0xE687) FNADDR;  /* USB Function address */

/* Endpoints */

__xdata volatile BYTE __at(0xE68A) EP0BCH;  /* Endpoint 0 Byte Count H */
__xdata volatile BYTE __at(0xE68B) EP0BCL;  /* Endpoint 0 Byte Count L */
__xdata volatile BYTE __at(0xE68D) EP1OUTBC;  /* Endpoint 1 OUT Byte Count */
__xdata volatile BYTE __at(0xE68F) EP1INBC;  /* Endpoint 1 IN Byte Count */
__xdata volatile BYTE __at(0xE690) EP2BCH;  /* Endpoint 2 Byte Count H */
__xdata volatile BYTE __at(0xE691) EP2BCL;  /* Endpoint 2 Byte Count L */
__xdata volatile BYTE __at(0xE694) EP4BCH;  /* Endpoint 4 Byte Count H */
__xdata volatile BYTE __at(0xE695) EP4BCL;  /* Endpoint 4 Byte Count L */
__xdata volatile BYTE __at(0xE698) EP6BCH;  /* Endpoint 6 Byte Count H */
__xdata volatile BYTE __at(0xE699) EP6BCL;  /* Endpoint 6 Byte Count L */
__xdata volatile BYTE __at(0xE69C) EP8BCH;  /* Endpoint 8 Byte Count H */
__xdata volatile BYTE __at(0xE69D) EP8BCL;  /* Endpoint 8 Byte Count L */
__xdata volatile BYTE __at(0xE6A0) EP0CS;  /* Endpoint  Control and Status */
__xdata volatile BYTE __at(0xE6A1) EP1OUTCS;  /* Endpoint 1 OUT Control and Status */
__xdata volatile BYTE __at(0xE6A2) EP1INCS;  /* Endpoint 1 IN Control and Status */
__xdata volatile BYTE __at(0xE6A3) EP2CS;  /* Endpoint 2 Control and Status */
__xdata volatile BYTE __at(0xE6A4) EP4CS;  /* Endpoint 4 Control and Status */
__xdata volatile BYTE __at(0xE6A5) EP6CS;  /* Endpoint 6 Control and Status */
__xdata volatile BYTE __at(0xE6A6) EP8CS;  /* Endpoint 8 Control and Status */
__xdata volatile BYTE __at(0xE6A7) EP2FIFOFLGS;  /* Endpoint 2 Flags */
__xdata volatile BYTE __at(0xE6A8) EP4FIFOFLGS;  /* Endpoint 4 Flags */
__xdata volatile BYTE __at(0xE6A9) EP6FIFOFLGS;  /* Endpoint 6 Flags */
__xdata volatile BYTE __at(0xE6AA) EP8FIFOFLGS;  /* Endpoint 8 Flags */
__xdata volatile BYTE __at(0xE6AB) EP2FIFOBCH;  /* EP2 FIFO total byte count H */
__xdata volatile BYTE __at(0xE6AC) EP2FIFOBCL;  /* EP2 FIFO total byte count L */
__xdata volatile BYTE __at(0xE6AD) EP4FIFOBCH;  /* EP4 FIFO total byte count H */
__xdata volatile BYTE __at(0xE6AE) EP4FIFOBCL;  /* EP4 FIFO total byte count L */
__xdata volatile BYTE __at(0xE6AF) EP6FIFOBCH;  /* EP6 FIFO total byte count H */
__xdata volatile BYTE __at(0xE6B0) EP6FIFOBCL;  /* EP6 FIFO total byte count L */
__xdata volatile BYTE __at(0xE6B1) EP8FIFOBCH;  /* EP8 FIFO total byte count H */
__xdata volatile BYTE __at(0xE6B2) EP8FIFOBCL;  /* EP8 FIFO total byte count L */
__xdata volatile BYTE __at(0xE6B3) SUDPTRH;  /* Setup Data Pointer high address byte */
__xdata volatile BYTE __at(0xE6B4) SUDPTRL;  /* Setup Data Pointer low address byte */
__xdata volatile BYTE __at(0xE6B5) SUDPTRCTL;  /* Setup Data Pointer Auto Mode */
__xdata volatile BYTE __at(0xE6B8) SETUPDAT[8];  /* 8 bytes of SETUP data */

/* GPIF */

__xdata volatile BYTE __at(0xE6C0) GPIFWFSELECT;  /* Waveform Selector */
__xdata volatile BYTE __at(0xE6C1) GPIFIDLECS;  /* GPIF Done, GPIF IDLE drive mode */
__xdata volatile BYTE __at(0xE6C2) GPIFIDLECTL;  /* Inactive Bus, CTL states */
__xdata volatile BYTE __at(0xE6C3) GPIFCTLCFG;  /* CTL OUT pin drive */
__xdata volatile BYTE __at(0xE6C4) GPIFADRH;  /* GPIF Address H */
__xdata volatile BYTE __at(0xE6C5) GPIFADRL;  /* GPIF Address L */

__xdata volatile BYTE __at(0xE6CE) GPIFTCB3;  /* GPIF Transaction Count Byte 3 */
__xdata volatile BYTE __at(0xE6CF) GPIFTCB2;  /* GPIF Transaction Count Byte 2 */
__xdata volatile BYTE __at(0xE6D0) GPIFTCB1;  /* GPIF Transaction Count Byte 1 */
__xdata volatile BYTE __at(0xE6D1) GPIFTCB0;  /* GPIF Transaction Count Byte 0 */

#define EP2GPIFTCH GPIFTCB1   /* these are here for backwards compatibility */
#define EP2GPIFTCL GPIFTCB0   /* */
#define EP4GPIFTCH GPIFTCB1   /* these are here for backwards compatibility */
#define EP4GPIFTCL GPIFTCB0   /* */
#define EP6GPIFTCH GPIFTCB1   /* these are here for backwards compatibility */
#define EP6GPIFTCL GPIFTCB0   /* */
#define EP8GPIFTCH GPIFTCB1   /* these are here for backwards compatibility */
#define EP8GPIFTCL GPIFTCB0   /* */

__xdata volatile BYTE __at(0xE6D2) EP2GPIFFLGSEL;  /* EP2 GPIF Flag select */
__xdata volatile BYTE __at(0xE6D3) EP2GPIFPFSTOP;  /* Stop GPIF EP2 transaction on prog. flag */
__xdata volatile BYTE __at(0xE6D4) EP2GPIFTRIG;  /* EP2 FIFO Trigger */
__xdata volatile BYTE __at(0xE6DA) EP4GPIFFLGSEL;  /* EP4 GPIF Flag select */
__xdata volatile BYTE __at(0xE6DB) EP4GPIFPFSTOP;  /* Stop GPIF EP4 transaction on prog. flag */
__xdata volatile BYTE __at(0xE6DC) EP4GPIFTRIG;  /* EP4 FIFO Trigger */
__xdata volatile BYTE __at(0xE6E2) EP6GPIFFLGSEL;  /* EP6 GPIF Flag select */
__xdata volatile BYTE __at(0xE6E3) EP6GPIFPFSTOP;  /* Stop GPIF EP6 transaction on prog. flag */
__xdata volatile BYTE __at(0xE6E4) EP6GPIFTRIG;  /* EP6 FIFO Trigger */
__xdata volatile BYTE __at(0xE6EA) EP8GPIFFLGSEL;  /* EP8 GPIF Flag select */
__xdata volatile BYTE __at(0xE6EB) EP8GPIFPFSTOP;  /* Stop GPIF EP8 transaction on prog. flag */
__xdata volatile BYTE __at(0xE6EC) EP8GPIFTRIG;  /* EP8 FIFO Trigger */
__xdata volatile BYTE __at(0xE6F0) XGPIFSGLDATH;  /* GPIF Data H (16-bit mode only) */
__xdata volatile BYTE __at(0xE6F1) XGPIFSGLDATLX;  /* Read/Write GPIF Data L & trigger transac */
__xdata volatile BYTE __at(0xE6F2) XGPIFSGLDATLNOX;  /* Read GPIF Data L, no transac trigger */
__xdata volatile BYTE __at(0xE6F3) GPIFREADYCFG;  /* Internal RDY,Sync/Async, RDY5CFG */
__xdata volatile BYTE __at(0xE6F4) GPIFREADYSTAT;  /* RDY pin states */
__xdata volatile BYTE __at(0xE6F5) GPIFABORT;  /* Abort GPIF cycles */

/* UDMA */

__xdata volatile BYTE __at(0xE6C6) FLOWSTATE; /*Defines GPIF flow state */
__xdata volatile BYTE __at(0xE6C7) FLOWLOGIC; /*Defines flow/hold decision criteria */
__xdata volatile BYTE __at(0xE6C8) FLOWEQ0CTL; /*CTL states during active flow state */
__xdata volatile BYTE __at(0xE6C9) FLOWEQ1CTL; /*CTL states during hold flow state */
__xdata volatile BYTE __at(0xE6CA) FLOWHOLDOFF;
__xdata volatile BYTE __at(0xE6CB) FLOWSTB; /*CTL/RDY Signal to use as master data strobe */
__xdata volatile BYTE __at(0xE6CC) FLOWSTBEDGE; /*Defines active master strobe edge */
__xdata volatile BYTE __at(0xE6CD) FLOWSTBHPERIOD; /*Half Period of output master strobe */
__xdata volatile BYTE __at(0xE60C) GPIFHOLDAMOUNT; /*Data delay shift */
__xdata volatile BYTE __at(0xE67D) UDMACRCH; /*CRC Upper byte */
__xdata volatile BYTE __at(0xE67E) UDMACRCL; /*CRC Lower byte */
__xdata volatile BYTE __at(0xE67F) UDMACRCQUAL; /*UDMA In only, host terminated use only */


/* Debug/Test */
/* The following registers are for Cypress's internal testing purposes only. */
/* These registers are not documented in the datasheet or the Technical Reference */
/* Manual as they were not designed for end user application usage */
__xdata volatile BYTE __at(0xE6F8) DBUG;  /* Debug */
__xdata volatile BYTE __at(0xE6F9) TESTCFG;  /* Test configuration */
__xdata volatile BYTE __at(0xE6FA) USBTEST;  /* USB Test Modes */
__xdata volatile BYTE __at(0xE6FB) CT1;  /* Chirp Test--Override */
__xdata volatile BYTE __at(0xE6FC) CT2;  /* Chirp Test--FSM */
__xdata volatile BYTE __at(0xE6FD) CT3;  /* Chirp Test--Control Signals */
__xdata volatile BYTE __at(0xE6FE) CT4;  /* Chirp Test--Inputs */

/* Endpoint Buffers */

__xdata volatile BYTE __at(0xE740) EP0BUF[64];  /* EP0 IN-OUT buffer */
__xdata volatile BYTE __at(0xE780) EP1OUTBUF[64];  /* EP1-OUT buffer */
__xdata volatile BYTE __at(0xE7C0) EP1INBUF[64];  /* EP1-IN buffer */
__xdata volatile BYTE __at(0xF000) EP2FIFOBUF[1024];  /* 512/1024-byte EP2 buffer (IN or OUT) */
__xdata volatile BYTE __at(0xF400) EP4FIFOBUF[1024];  /* 512 byte EP4 buffer (IN or OUT) */
__xdata volatile BYTE __at(0xF800) EP6FIFOBUF[1024];  /* 512/1024-byte EP6 buffer (IN or OUT) */
__xdata volatile BYTE __at(0xFC00) EP8FIFOBUF[1024];  /* 512 byte EP8 buffer (IN or OUT) */

/* Error Correction Code (ECC) Registers (FX2LP/FX1 only) */

__xdata volatile BYTE __at(0xE628) ECCCFG;  /* ECC Configuration */
__xdata volatile BYTE __at(0xE629) ECCRESET;  /* ECC Reset */
__xdata volatile BYTE __at(0xE62A) ECC1B0;  /* ECC1 Byte 0 */
__xdata volatile BYTE __at(0xE62B) ECC1B1;  /* ECC1 Byte 1 */
__xdata volatile BYTE __at(0xE62C) ECC1B2;  /* ECC1 Byte 2 */
__xdata volatile BYTE __at(0xE62D) ECC2B0;  /* ECC2 Byte 0 */
__xdata volatile BYTE __at(0xE62E) ECC2B1;  /* ECC2 Byte 1 */
__xdata volatile BYTE __at(0xE62F) ECC2B2;  /* ECC2 Byte 2 */

/* Feature Registers  (FX2LP/FX1 only) */
__xdata volatile BYTE __at(0xE50D) GPCR2;  /* Chip Features */

/*-----------------------------------------------------------------------------
   Special Function Registers (SFRs)
   The byte registers and bits defined in the following list are based
   on the Synopsis definition of the 8051 Special Function Registers for EZ-USB. 
    If you modify the register definitions below, please regenerate the file 
    "ezregs.inc" which includes the same basic information for assembly inclusion.
-----------------------------------------------------------------------------*/

__sfr __at(0x80) IOA;
         /*  IOA  */
         __sbit __at( 0x80 + 0) PA0;
         __sbit __at( 0x80 + 1) PA1;
         __sbit __at( 0x80 + 2) PA2;
         __sbit __at( 0x80 + 3) PA3;

         __sbit __at( 0x80 + 4) PA4;
         __sbit __at( 0x80 + 5) PA5;
         __sbit __at( 0x80 + 6) PA6;
         __sbit __at( 0x80 + 7) PA7;
__sfr __at(0x81) SP;
__sfr __at(0x82) DPL;
__sfr __at(0x83) DPH;
__sfr __at(0x84) DPL1;
__sfr __at(0x85) DPH1;
__sfr __at(0x86) DPS;
         /*  DPS  */
         /* sbit SEL   = 0x86+0; */
__sfr __at(0x87) PCON;
         /*  PCON  */
         /*sbit IDLE   = 0x87+0; */
         /*sbit STOP   = 0x87+1; */
         /*sbit GF0    = 0x87+2; */
         /*sbit GF1    = 0x87+3; */
         /*sbit SMOD0  = 0x87+7; */
__sfr __at(0x88) TCON;
         /*  TCON  */
         __sbit __at( 0x88+0) IT0;
         __sbit __at( 0x88+1) IE0;
         __sbit __at( 0x88+2) IT1;
         __sbit __at( 0x88+3) IE1;
         __sbit __at( 0x88+4) TR0;
         __sbit __at( 0x88+5) TF0;
         __sbit __at( 0x88+6) TR1;
         __sbit __at( 0x88+7) TF1;
__sfr __at(0x89) TMOD;
         /*  TMOD  */
         /*__sbit M00    = 0x89+0; */
         /*__sbit M10    = 0x89+1; */
         /*__sbit CT0    = 0x89+2; */
         /*__sbit GATE0  = 0x89+3; */
         /*__sbit M01    = 0x89+4; */
         /*__sbit M11    = 0x89+5; */
         /*__sbit CT1    = 0x89+6; */
         /*__sbit GATE1  = 0x89+7; */
__sfr __at(0x8A) TL0;
__sfr __at(0x8B) ATL1;
__sfr __at(0x8C) BTH0;
__sfr __at(0x8D) CTH1;
__sfr __at(0x8E) DCKCON;
         /*  CKCON  */
         /*__sbit MD0    = 0x89+0; */
         /*__sbit MD1    = 0x89+1; */
         /*__sbit MD2    = 0x89+2; */
         /*__sbit T0M    = 0x89+3; */
         /*__sbit T1M    = 0x89+4; */
         /*__sbit T2M    = 0x89+5; */
__sfr __at(0x8F) SPC_FNC; /* Was WRS in Reg320 */
         /*  CKCON  */
         /*__sbit WRS    = 0x8F+0; */
__sfr __at(0x90) IOB;
         /*  IOB  */
         __sbit __at( 0x90 + 0) PB0;
         __sbit __at( 0x90 + 1) PB1;
         __sbit __at( 0x90 + 2) PB2;
         __sbit __at( 0x90 + 3) PB3;

         __sbit __at( 0x90 + 4) PB4;
         __sbit __at( 0x90 + 5) PB5;
         __sbit __at( 0x90 + 6) PB6;
         __sbit __at( 0x90 + 7) PB7;
__sfr __at(0x91) EXIF; /* EXIF Bit Values differ from Reg320 */
         /*  EXIF  */
         /*__sbit USBINT = 0x91+4; */
         /*__sbit I2CINT = 0x91+5; */
        /* __sbit IE4    = 0x91+6; */
         /*__sbit IE5    = 0x91+7; */
__sfr __at(0x92) MPAGE;
__sfr __at(0x98) SCON0;
         /*  SCON0  */
         __sbit __at( 0x98+0) RI;
         __sbit __at( 0x98+1) TI;
         __sbit __at( 0x98+2) RB8;
         __sbit __at( 0x98+3) TB8;
         __sbit __at( 0x98+4) REN;
         __sbit __at( 0x98+5) SM2;
         __sbit __at( 0x98+6) SM1;
         __sbit __at( 0x98+7) SM0;
__sfr __at(0x99) SBUF0;

#define AUTOPTR1H AUTOPTRH1 /* for backwards compatibility with examples */
#define AUTOPTR1L AUTOPTRL1 /* for backwards compatibility with examples */
#define APTR1H AUTOPTRH1 /* for backwards compatibility with examples */
#define APTR1L AUTOPTRL1 /* for backwards compatibility with examples */

/* this is how they are defined in the TRM */
__sfr __at(0x9A) AUTOPTRH1; 
__sfr __at(0x9B) AUTOPTRL1; 
__sfr __at(0x9D) AUTOPTRH2;
__sfr __at(0x9E) AUTOPTRL2; 

__sfr __at(0xA0) IOC;
         /*  IOC  */
         __sbit __at( 0xA0 + 0) PC0;
         __sbit __at( 0xA0 + 1) PC1;
         __sbit __at( 0xA0 + 2) PC2;
         __sbit __at( 0xA0 + 3) PC3;

         __sbit __at( 0xA0 + 4) PC4;
         __sbit __at( 0xA0 + 5) PC5;
         __sbit __at( 0xA0 + 6) PC6;
         __sbit __at( 0xA0 + 7) PC7;
__sfr __at(0xA1) INT2CLR;
__sfr __at(0xA2) INT4CLR;

__sfr __at(0xA8) IE;
         /*  IE  */
         __sbit __at( 0xA8+0) EX0;
         __sbit __at( 0xA8+1) ET0;
         __sbit __at( 0xA8+2) EX1;
         __sbit __at( 0xA8+3) ET1;
         __sbit __at( 0xA8+4) ES0;
         __sbit __at( 0xA8+5) ET2;
         __sbit __at( 0xA8+6) ES1;
         __sbit __at( 0xA8+7) EA;

__sfr __at(0xAA) EP2468STAT;
         /* EP2468STAT */
         /*__sbit EP2E   = 0xAA+0; */
         /*__sbit EP2F   = 0xAA+1; */
         /*__sbit EP4E   = 0xAA+2; */
         /*__sbit EP4F   = 0xAA+3; */
         /*__sbit EP6E   = 0xAA+4; */
         /*__sbit EP6F   = 0xAA+5; */
         /*__sbit EP8E   = 0xAA+6; */
         /*__sbit EP8F   = 0xAA+7; */

__sfr __at(0xAB) EP24FIFOFLGS;
__sfr __at(0xAC) EP68FIFOFLGS;
__sfr __at(0xAF) AUTOPTRSETUP;
         /* AUTOPTRSETUP */
         /*   __sbit EXTACC  = 0xAF+0; */
         /*   __sbit APTR1FZ = 0xAF+1; */
         /*   __sbit APTR2FZ = 0xAF+2; */

__sfr __at(0xB0) IOD;
         /*  IOD  */
         __sbit __at( 0xB0 + 0) PD0;
         __sbit __at( 0xB0 + 1) PD1;
         __sbit __at( 0xB0 + 2) PD2;
         __sbit __at( 0xB0 + 3) PD3;

         __sbit __at( 0xB0 + 4) PD4;
         __sbit __at( 0xB0 + 5) PD5;
         __sbit __at( 0xB0 + 6) PD6;
         __sbit __at( 0xB0 + 7) PD7;
__sfr __at(0xB1) IOE;
__sfr __at(0xB2) OEA;
__sfr __at(0xB3) OEB;
__sfr __at(0xB4) OEC;
__sfr __at(0xB5) OED;
__sfr __at(0xB6) OEE;

__sfr __at(0xB8) IP;
         /*  IP  */
         __sbit __at( 0xB8+0) PX0;
         __sbit __at( 0xB8+1) PT0;
         __sbit __at( 0xB8+2) PX1;
         __sbit __at( 0xB8+3) PT1;
         __sbit __at( 0xB8+4) PS0;
         __sbit __at( 0xB8+5) PT2;
         __sbit __at( 0xB8+6) PS1;

__sfr __at(0xBA) EP01STAT;
__sfr __at(0xBB) GPIFTRIG;
                
__sfr __at(0xBD) GPIFSGLDATH;
__sfr __at(0xBE) GPIFSGLDATLX;
__sfr __at(0xBF) GPIFSGLDATLNOX;

__sfr __at(0xC0) SCON1;
         /*  SCON1  */
         __sbit __at( 0xC0+0) RI1;
         __sbit __at( 0xC0+1) TI1;
         __sbit __at( 0xC0+2) RB81;
         __sbit __at( 0xC0+3) TB81;
         __sbit __at( 0xC0+4) REN1;
         __sbit __at( 0xC0+5) SM21;
         __sbit __at( 0xC0+6) SM11;
         __sbit __at( 0xC0+7) SM01;
__sfr __at(0xC1) SBUF1;
__sfr __at(0xC8) T2CON;
         /*  T2CON  */
         __sbit __at( 0xC8+0) CP_RL2;
         __sbit __at( 0xC8+1) C_T2;
         __sbit __at( 0xC8+2) TR2;
         __sbit __at( 0xC8+3) EXEN2;
         __sbit __at( 0xC8+4) TCLK;
         __sbit __at( 0xC8+5) RCLK;
         __sbit __at( 0xC8+6) EXF2;
         __sbit __at( 0xC8+7) TF2;
__sfr __at(0xCA) RCAP2L;
__sfr __at(0xCB) RCAP2H;
__sfr __at(0xCC) TL2;
__sfr __at(0xCD) CTH2;
__sfr __at(0xD0) PSW;
         /*  PSW  */
         __sbit __at( 0xD0+0) P;
         __sbit __at( 0xD0+1) FL;
         __sbit __at( 0xD0+2) OV;
         __sbit __at( 0xD0+3) RS0;
         __sbit __at( 0xD0+4) RS1;
         __sbit __at( 0xD0+5) F0;
         __sbit __at( 0xD0+6) AC;
         __sbit __at( 0xD0+7) CY;
__sfr __at(0xD8) EICON; /* Was WDCON in DS80C320; Bit Values differ from Reg320 */
         /*  EICON  */
         __sbit __at( 0xD8+3) INT6;
         __sbit __at( 0xD8+4) RESI;
         __sbit __at( 0xD8+5) ERESI;
         __sbit __at( 0xD8+7) SMOD1;
__sfr __at(0xE0) ACC;
__sfr __at(0xE8) EIE; /* EIE Bit Values differ from Reg320 */
         /*  EIE  */
         __sbit __at( 0xE8+0) EUSB;
         __sbit __at( 0xE8+1) EI2C;
         __sbit __at( 0xE8+2) EIEX4;
         __sbit __at( 0xE8+3) EIEX5;
         __sbit __at( 0xE8+4) EIEX6;
__sfr __at(0xF0) B;
__sfr __at(0xF8) EIP; /* EIP Bit Values differ from Reg320 */
         /*  EIP  */
         __sbit __at( 0xF8+0) PUSB;
         __sbit __at( 0xF8+1) PI2C;
         __sbit __at( 0xF8+2) EIPX4;
         __sbit __at( 0xF8+3) EIPX5;
         __sbit __at( 0xF8+4) EIPX6;

/*-----------------------------------------------------------------------------
   Bit Masks
-----------------------------------------------------------------------------*/

#define bmBIT0		(1<<0)
#define bmBIT1		(1<<1)
#define bmBIT2		(1<<2)
#define bmBIT3		(1<<3)
#define bmBIT4		(1<<4)
#define bmBIT5		(1<<5)
#define bmBIT6		(1<<6)
#define bmBIT7		(1<<7)

/* CPU Control & Status Register (CPUCS) */
#define bmPRTCSTB    bmBIT5
#define bmCLKSPD     (bmBIT4 | bmBIT3)
#define bmCLKSPD1    bmBIT4
#define bmCLKSPD0    bmBIT3
#define bmCLKINV     bmBIT2
#define bmCLKOE      bmBIT1
#define bm8051RES    bmBIT0
/* Port Alternate Configuration Registers */
/* Port A (PORTACFG) */
#define bmFLAGD      bmBIT7
#define bmINT1       bmBIT1
#define bmINT0       bmBIT0
/* Port C (PORTCCFG) */
#define bmGPIFA7     bmBIT7
#define bmGPIFA6     bmBIT6
#define bmGPIFA5     bmBIT5
#define bmGPIFA4     bmBIT4
#define bmGPIFA3     bmBIT3
#define bmGPIFA2     bmBIT2
#define bmGPIFA1     bmBIT1
#define bmGPIFA0     bmBIT0
/* Port E (PORTECFG) */
#define bmGPIFA8     bmBIT7
#define bmT2EX       bmBIT6
#define bmINT6       bmBIT5
#define bmRXD1OUT    bmBIT4
#define bmRXD0OUT    bmBIT3
#define bmT2OUT      bmBIT2
#define bmT1OUT      bmBIT1
#define bmT0OUT      bmBIT0

/* I2C Control & Status Register (I2CS) */
#define bmSTART      bmBIT7
#define bmSTOP       bmBIT6
#define bmLASTRD     bmBIT5
#define bmID         (bmBIT4 | bmBIT3)
#define bmBERR       bmBIT2
#define bmACK        bmBIT1
#define bmDONE       bmBIT0
/* I2C Control Register (I2CTL) */
#define bmSTOPIE     bmBIT1
#define bm400KHZ     bmBIT0
/* Interrupt 2 (USB) Autovector Register (INT2IVEC) */
#define bmIV4        bmBIT6
#define bmIV3        bmBIT5
#define bmIV2        bmBIT4
#define bmIV1        bmBIT3
#define bmIV0        bmBIT2
/* USB Interrupt Request & Enable Registers (USBIE/USBIRQ) */
#define bmEP0ACK     bmBIT6
#define bmHSGRANT    bmBIT5
#define bmURES       bmBIT4
#define bmSUSP       bmBIT3
#define bmSUTOK      bmBIT2
#define bmSOF        bmBIT1
#define bmSUDAV      bmBIT0
/* Breakpoint register (BREAKPT) */
#define bmBREAK      bmBIT3
#define bmBPPULSE    bmBIT2
#define bmBPEN       bmBIT1
/* Interrupt 2 & 4 Setup (INTSETUP) */
#define bmAV2EN      bmBIT3
#define INT4IN       bmBIT1
#define bmAV4EN      bmBIT0
/* USB Control & Status Register (USBCS) */
#define bmHSM        bmBIT7
#define bmDISCON     bmBIT3
#define bmNOSYNSOF   bmBIT2
#define bmRENUM      bmBIT1
#define bmSIGRESUME  bmBIT0
/* Wakeup Control and Status Register (WAKEUPCS) */
#define bmWU2        bmBIT7
#define bmWU         bmBIT6
#define bmWU2POL     bmBIT5
#define bmWUPOL      bmBIT4
#define bmDPEN       bmBIT2
#define bmWU2EN      bmBIT1
#define bmWUEN       bmBIT0
/* End Point 0 Control & Status Register (EP0CS) */
#define bmHSNAK      bmBIT7
/* End Point 0-1 Control & Status Registers (EP0CS/EP1OUTCS/EP1INCS) */
#define bmEPBUSY     bmBIT1
#define bmEPSTALL    bmBIT0
/* End Point 2-8 Control & Status Registers (EP2CS/EP4CS/EP6CS/EP8CS) */
#define bmNPAK       (bmBIT6 | bmBIT5 | bmBIT4)
#define bmEPFULL     bmBIT3
#define bmEPEMPTY    bmBIT2
/* Endpoint Status (EP2468STAT) SFR bits */
#define bmEP8FULL    bmBIT7
#define bmEP8EMPTY   bmBIT6
#define bmEP6FULL    bmBIT5
#define bmEP6EMPTY   bmBIT4
#define bmEP4FULL    bmBIT3
#define bmEP4EMPTY   bmBIT2
#define bmEP2FULL    bmBIT1
#define bmEP2EMPTY   bmBIT0
/* SETUP Data Pointer Auto Mode (SUDPTRCTL) */
#define bmSDPAUTO    bmBIT0
/* Endpoint Data Toggle Control (TOGCTL) */
#define bmQUERYTOGGLE  bmBIT7
#define bmSETTOGGLE    bmBIT6
#define bmRESETTOGGLE  bmBIT5
#define bmTOGCTLEPMASK bmBIT3 | bmBIT2 | bmBIT1 | bmBIT0
/* IBN (In Bulk Nak) enable and request bits (IBNIE/IBNIRQ) */
#define bmEP8IBN     bmBIT5
#define bmEP6IBN     bmBIT4
#define bmEP4IBN     bmBIT3
#define bmEP2IBN     bmBIT2
#define bmEP1IBN     bmBIT1
#define bmEP0IBN     bmBIT0

/* PING-NAK enable and request bits (NAKIE/NAKIRQ) */
#define bmEP8PING     bmBIT7
#define bmEP6PING     bmBIT6
#define bmEP4PING     bmBIT5
#define bmEP2PING     bmBIT4
#define bmEP1PING     bmBIT3
#define bmEP0PING     bmBIT2
#define bmIBN         bmBIT0

/* Interface Configuration bits (IFCONFIG) */
#define bmIFCLKSRC    bmBIT7
#define bm3048MHZ     bmBIT6
#define bmIFCLKOE     bmBIT5
#define bmIFCLKPOL    bmBIT4
#define bmASYNC       bmBIT3
#define bmGSTATE      bmBIT2
#define bmIFCFG1      bmBIT1
#define bmIFCFG0      bmBIT0
#define bmIFCFGMASK   (bmIFCFG0 | bmIFCFG1)
#define bmIFGPIF      bmIFCFG1

/* EP 2468 FIFO Configuration bits (EP2FIFOCFG,EP4FIFOCFG,EP6FIFOCFG,EP8FIFOCFG) */
#define bmINFM       bmBIT6
#define bmOEP        bmBIT5
#define bmAUTOOUT    bmBIT4
#define bmAUTOIN     bmBIT3
#define bmZEROLENIN  bmBIT2
#define bmWORDWIDE   bmBIT0

/* Chip Revision Control Bits (REVCTL) - used to ebable/disable revision specidic
   features */ 
#define bmNOAUTOARM    bmBIT1
#define bmSKIPCOMMIT   bmBIT0

/* Fifo Reset bits (FIFORESET) */
#define bmNAKALL       bmBIT7

/* Chip Feature Register (GPCR2) */
#define bmFULLSPEEDONLY    bmBIT4

#endif   /* FX2REGS_H */
