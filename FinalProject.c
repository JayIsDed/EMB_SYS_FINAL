// Name: Devin Dicocco
// Date: 11/21/2021
// Course: ELEC3371-01
//
unsigned int RESET=0;
unsigned int STV=1;             //start condition
unsigned int AR=0;              //ARM/Reset
unsigned int R1A=0;             //Room 1 Armed
unsigned int R2A=0;             //Room 2 armed
unsigned int R3A=0;             //room 3 Armed
unsigned int R1B=0;             //Room 1 Breach
unsigned int R2B=0;             //Room 2 Breach
unsigned int R3B=0;             //room 3 Breach
unsigned int count=15 ;        //counter for led E
unsigned int SARM=0;             //Start arm
unsigned int BRCH=0;            //breach variable
unsigned int MSD=0;
unsigned int LSD=0;

void PinConfiguration();    // Forward declaration of sub function used for pin configuration
void Timer1Configuration(); // Forward declaration of sub function used for TIMER1 configuration
void Timer4Configuration(); // Forward declaration of sub function used for TIMER4 configuration
void Timer2Configuration(); // Forward declaration of sub function used for TIMER2 configuration
void InitializeUSART1();        // Sub function which initialize usart 1
void ExternalIntConfiguration(); // Forward declaration of sub function used for
//INTERUPT CALL
void EXTIPA0 () iv IVT_INT_EXTI0{                //A0 starts and stops timer
EXTI_PR.B0 = 1;     //Clear pending interrupt flag for PA0
  
  if(STV==0 && BRCH==0){                         //if A0 is pressed
  SARM=1;                                      //SARM=1 to enable the next line
  R1A=0;
  }
 
  if(BRCH==1){                   //reset the alarm
  R3B=0;
  R2B=0;
  R1B=0;
  R1A=0;
  R2A=0;
  R3A=0;
  GPIOE_ODR=0;
  GPIOD_ODR=0;
  BRCH=0;
  STV=1;
  count=15;
  }
  
  
}
void main () {
PinConfiguration();
Timer1Configuration();
Timer4Configuration();
Timer2Configuration();
ExternalIntConfiguration();
InitializeUSART1();
TIM1_ARR=0;                     //timer 1
TIM2_ARR=0;                     //timer 2

for (;;){

while(STV==1){     //Swapped to a while --
AR=0;              //ARM/Reset
SARM=0;             //Start arm
BRCH=0;            //breach variable

if(GPIOA_IDR.B6==1){                //if a6 is pressed
if(R3A==0){
R3A=1;
}
else{                                //r3a switches from on and off
R3A=0;
}
}
if(GPIOA_IDR.B5==1){                //if a5 is pressed
if(R2A==0){
R2A=1;
}
else{                                //r2a switches from on and off
R2A=0;
}
}
if(GPIOA_IDR.B4==1){                //if a4 is pressed
if(R1A==0){
R1A=1;
}
else{                                //r1a switches from on and off
R1A=0;
}
}
GPIOD_ODR.B2=R3A;                      //D2 led on
GPIOD_ODR.B1=R2A;                      //D1 led on
GPIOD_ODR.B0=R1A;                      //D0 led on
} ///if(STV==0)

if(R1A==1 && R2A==1 && R3A==1){
STV=0;
TIM1_ARR=1000;                          //start timer 1
GPIOD_ODR.B3=1;                        //D3 led goes on
if (TIM1_SR.UIF == 1) { // If you have multiple timers, can use IF to check status register
TIM1_SR.UIF = 0;   // Clear this flag which will reset the timer
GPIOD_ODR.B2=~GPIOD_ODR.B2;
GPIOD_ODR.B1=~GPIOD_ODR.B1;
GPIOD_ODR.B0=~GPIOD_ODR.B0;
}

}  //if(R1A==1 && R2A==1 && R3A==1)
if(SARM==1){
GPIOD_ODR.B2=0;
GPIOD_ODR.B1=0;
GPIOD_ODR.B0=0;
TIM1_ARR=1000;
count=3;                                  ////change later to 15
while(count>0){
if (TIM1_SR.UIF == 1) { // If you have multiple timers, can use IF to check status register
TIM1_SR.UIF = 0;   // Clear this flag which will reset the timer
count--;
}
if(count==0){
TIM1_ARR=0;            //timmer 1 off
GPIOD_ODR.B3=0;
GPIOD_ODR.B7=1;             //sys armed
//USART
SARM=0;
BRCH=1;

}
GPIOE_ODR=count<<8;
}
Count=15;
}//if(SARM==1)

if(BRCH==1){

GPIOD_ODR.B3=0;                        //turn off ready to arm lights
GPIOD_ODR.B2=0;
GPIOD_ODR.B1=0;
GPIOD_ODR.B0=0;
if(GPIOA_IDR.B6==1){                //if a6 is pressed
 R3B=1;                             //r3 breach
GPIOD_ODR.B6=1;
 }
 if(GPIOA_IDR.B5==1){                //if a5 is pressed
 R2B=1;                             //r2 breach
GPIOD_ODR.B5=1;
 }
 if(GPIOA_IDR.B4==1){                //if a4 is pressed
 R1B=1;                             //r1 breach
GPIOD_ODR.B4=1;
 }

 if(R3B!=0 || R2B!=0 || R1B!=0){           //if any is breached
 GPIOD_ODR.B7=0;
 GPIOD_ODR.B3=0;

TIM1_ARR=1000;
if (TIM1_SR.UIF == 1) { // If you have multiple timers, can use IF to check status register
TIM1_SR.UIF = 0;   // Clear this flag which will reset the timer
 if(count>0){
count--;
 }

if(R3B!=0){
GPIOD_ODR.B6=~GPIOD_ODR.B6;                      //flash D6
}else{
GPIOD_ODR.B6=0;
}
if(R2B!=0){
GPIOD_ODR.B5=~GPIOD_ODR.B5;                    //flash D5
}else{
GPIOD_ODR.B5=0;
}
if(R1B!=0){
GPIOD_ODR.B4=~GPIOD_ODR.B4;                         //flash D4
}else{
GPIOD_ODR.B4=0;
}
}
GPIOE_ODR=count<<8;                                   //show a count down on port E
}
}//if(brch)








  } //for
} //main
//subfunctions
void PinConfiguration() {
RCC_APB2ENR |= 1 << 2;  // Enable GPIOE clock - necessary to use GPIOA
RCC_APB2ENR |= 1 << 3;  // Enable GPIOB clock - necessary to use GPIOB
RCC_APB2ENR |= 1 << 5;  // Enable GPIOD clock - necessary to use GPIOD
RCC_APB2ENR |= 1 << 6;  // Enable GPIOE clock - necessary to use GPIOE
GPIOA_CRL = 0x44444444;    //inputs configured on board
GPIOA_CRH = 0x44444444;
GPIOB_CRL = 0x44444444;
GPIOB_CRH = 0x44444444;
GPIOE_CRL = 0x33333333;    //outouts configured on board
GPIOE_CRH = 0x33333333;
GPIOD_CRL = 0x33333333;
GPIOD_CRH = 0x33333333;
}
void Timer1Configuration(){
RCC_APB2ENR |= (1 << 11);   // Enable TIMER1 clock. RCC: Clock Configuration Register
// Different clocks may use different registers.
// Ex. TIMER4 uses RCC_APB1ENR
TIM1_CR1 = 0x0000;  // Disable timer until configuration is complete
// If reset value of RCC_CFGR is used, then the 8MHz clock will
// be the clock source for timer
TIM1_PSC = 7999;    // Clock to TIMx_CNT = 72000000 (clock applied to prescaler register) /
    //                     7999 (Value in TIMx_PSC) +1) = 9000
TIM1_ARR = 9000; // Reload timer count register with this value when count register resets
// to 0 after counting from zero to this value
TIM1_CR1 = 0x0001;  // Enable TIMER1
// Notice: Bit 4 of TIM1_CR1 specifies whether the counter count up (BIT4=0) or counts down (BIT4=1)
// In this configuration this counting up is used.
}
void Timer4Configuration(){// Forward declaration of sub function used for TIMER4 configuration
  RCC_APB1ENR |= (1 << 2);        //enable timer 4 clock
  TIM4_CR1 = 0x0000;  // Disable timer until configuration is complete
// If reset value of RCC_CFGR is used, then the 8MHz clock will
// be the clock source for timer
TIM4_PSC = 7999;    // Clock to TIMx_CNT = 72000000 (clock applied to prescaler register) /
    //                     7999 (Value in TIMx_PSC) +1) = 9000
TIM4_ARR = 9000; // Reload timer count register with this value when count register resets
// to 0 after counting from zero to this value
TIM4_CR1 = 0x0001;  // Enable TIMER4
    }
void Timer2Configuration(){
RCC_APB1ENR |= (1 << 0);   // Enable TIMER1 clock. RCC: Clock Configuration Register
// Different clocks may use different registers.
// Ex. TIMER4 uses RCC_APB1ENR
TIM2_CR1 = 0x0000;  // Disable timer until configuration is complete
// If reset value of RCC_CFGR is used, then the 8MHz clock will
// be the clock source for timer
TIM2_PSC = 7999;    // Clock to TIMx_CNT = 72000000 (clock applied to prescaler register) /
    //                     7999 (Value in TIMx_PSC) +1) = 9000
TIM2_ARR = 9000; // Reload timer count register with this value when count register resets
// to 0 after counting from zero to this value
//TIM2_CR1 = 0x0001;  // Enable TIMER1
// Notice: Bit 4 of TIM1_CR1 specifies whether the counter count up (BIT4=0) or counts down (BIT4=1)
// In this configuration this counting up is used.
}
void InitializeUSART1(){ // Sub function which initializes the registers to enable USART1
        RCC_APB2ENR |= 1;                 // Enable clock for Alt. Function. USART1 uses AF for PA9/PA10
        AFIO_MAPR=0X0F000000;             // Do not mask PA9 and PA10 (becaue we are using for USART)
        RCC_APB2ENR |= 1<<2;              // Enable clock for GPIOA
        GPIOA_CRH &= ~(0xFF << 4);        // Clear PA9, PA10
        GPIOA_CRH |= (0x0B << 4);         // USART1 Tx (PA9) output push-pull
        GPIOA_CRH |= (0x04 << 8);         // USART1 Rx (PA10) input floating
        RCC_APB2ENR |= 1<<14;             // enable clock for USART1
        USART1_BRR=0X00000506;            // Set baud rate to 56000
        // Per data sheet (pg. 1010) USART1_CR1 consists of the following:
        //13 12   11  10  9    8     7    6      5      4  3  2   1   0
        //UE  M WAKE PCE PS PEIE TXEIE TCIE RXNEIE IDLEIE TE RE RWU SBK
        //rw rw  rw   rw rw   rw    rw   rw     rw     rw rw rw  rw  rw
        USART1_CR1 &= ~(1<<12);          // Force 8 data bits. M bit is set to 0.
        USART1_CR2 &= ~(3<<12);          // Force 1 stop bit
        USART1_CR3 &= ~(3<<8);           // Force no flow control and no DMA for USART1
        USART1_CR1 &= ~(3<<9);           // Force no parity and no parity control
        USART1_CR1 |= 3<<2;              // RX, TX enable
        //The following two instructions can also be used to enable RX and TX manually
        //USART1_CR1.TE=1; //TX enable
        //USART1_CR1.RE=1; //RX enable
        USART1_CR1 |= 1<<13;            // USART1 enable. This is done after configuration is complete
        Delay_ms(100);                  // Wait for USART to complete configuration and enable. This is

}
void ExternalIntConfiguration(){
RCC_APB2ENR.AFIOEN = 1;     // Enable clock for alternate pin function
AFIO_EXTICR1  = 0x00000000; // PA0 as External interrupt
AFIO_EXTICR2 |= 0x00000100; // PB6 as External interrupt
EXTI_RTSR |= 0x00000041;    // Set interrupt on rising edge for PA0 and PB6
EXTI_IMR |= 0x00000041;     // Interrupt on PA0 and PB6 are non-maskable
NVIC_ISER0 |= 1<<6;         // Enable NVIC interrupt for EXTI line zero (PA0)
                                // with position 6 in NVIC table
NVIC_ISER0.B23 = 1;        // Enable NVIC interrupt for EXTI9_% (PB6) position 23 in NVIC table
}