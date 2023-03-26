#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     LED7    PORTC
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM        255
#define     ENTER_PASSWORD     1
#define     CHECK_PASSWORD     2
#define     UNLOCK_DOOR        3
#define     WRONG_PASSWORD     4

#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD

#define PORTC_OUT   PORTC
#define TRISC_OUT   TRISC

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
unsigned char isButtonMotorOn();
unsigned char isButtonMotorOff();
void MotorOn();
void MotorOff();
void BaiTap_Motor();
void Test_KeyMatrix();
//Chuong trinh Password Door
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [5][4]= {
  {1,2,3,4},
  {2,7,8,9},
  {3,3,3,3},
  {4,8,6,8},
  {5,'A','B','C'},
};
unsigned char arrayPassword[4];
unsigned char statusPassword = INIT_SYSTEM;
unsigned char indexOfNumber = 0;
unsigned char timeDelay = 0;
unsigned char enable = 0;

void App_PasswordDoor();
unsigned char CheckPassword();
unsigned char isButtonNumber();
unsigned char numberValue;
unsigned char isButtonEnter();
void UnlockDoor();
void LockDoor();
// Den giao thong
void Phase1_GreenOn();
void Phase1_GreenOff();
void Phase1_YellowOn();
void Phase1_YellowOff();
void Phase1_RedOn();
void Phase1_RedOff();

void Phase2_GreenOn();
void Phase2_GreenOff();
void Phase2_YellowOn();
void Phase2_YellowOff();
void Phase2_RedOn();
void Phase2_RedOff();

void Walking_GreenOn();
void Walking_GreenOff();
void Walking_RedOn();
void Walking_RedOff();

void Display_number();

unsigned char isButton4Up();
unsigned char isButton4Down();
unsigned char isButton5Up();
unsigned char isButton6Up();
unsigned char isButtonUp();

#define     INIT_SYSTEM         255
#define     PHASE1_GREEN        0
#define     PHASE1_YELLOW       1
#define     PHASE2_GREEN        2
#define     PHASE2_YELLOW       3
#define     WAIT                4

#define     MODE_0              0
#define     MODE_1              1
#define     MODE_2              2
#define     MODE_3              3
#define     MODE_4              4
#define     KEEP_SETTING        0
#define     WAITING_NEXT_MODE   5

#define     MODE_TRAFFIC_LIGHT  10
#define     MODE_PASSWORD       9
#define     MODE_CHOOSE_MODE    8

#define     MODE_SETTING_TIME   0
#define     MODE_MANUAL         1
#define     MODE_SLOW           2
#define     MODE_UART           3

#define     MODE_MANUAL_0       0
#define     MODE_MANUAL_1       1

unsigned char statusOfLight = INIT_SYSTEM;
//unsigned char trafficMode = INIT_SYSTEM;
//unsigned char manualMode = INIT_SYSTEM;
unsigned char settingMode = INIT_SYSTEM;

unsigned char timeOfGreenPhase1 = 10;
unsigned char timeOfYellowPhase1 = 4;
unsigned char timeOfGreenPhase2 = 10;
unsigned char timeOfYellowPhase2 = 4;

unsigned char temp_G_1 = 10;
unsigned char temp_Y_1 = 4;
unsigned char temp_G_2 = 10;
unsigned char temp_Y_2 = 4;

unsigned char count;
unsigned char waiting_time = 0;

unsigned char num_mode = 4;
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char waitingTimeTrafficLight = 0;
unsigned char nextModeFlag = 0;

void LedTrafficLight();
void LcdTrafficLight();
void TimeTrafficLight();
void WaitingTimeTrafficLight();
void Traffic_fsm();
void Manual_Adjustment();
void Slow_mode();
void setting_time();
void UartDataReceiveProcess();
void UartDataReceiveProcess_ElectronicDeviceControl();
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
	init_system();
        delay_ms(1000);
        trafficMode = INIT_SYSTEM;
        manualMode = INIT_SYSTEM;
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            scan_key_matrix(); // 8 button
            //scan_key_matrix_with_uart(); // 8 button
            
            //AppTrafficLight();
            Traffic_fsm();
            DisplayLcdScreenOld(); //Output process 14ms vs 10ms with no timer           
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
   PORTD_OUT = 0x00;
//   TRISE_OUT = 0x00;
//   PORTE_OUT = 0x00;
}

void init_system(void)
{
    init_output();
    lcd_init();
    init_key_matrix();
    init_interrupt();
    lcd_clear();
    LcdClearS();
    delay_ms(500);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    //PORTAbits.RA0 = 1;
    init_uart();
}
/////////normal led////////
void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}
}
void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}
void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}
void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn()
{
    OpenOutput(0);
}
void Phase1_GreenOff()
{
    CloseOutput(0);
}

void Walking_GreenOn()
{
    OpenOutput(2);
}
void Walking_GreenOff()
{
    CloseOutput(2);
}

void Phase1_YellowOn()
{
    OpenOutput(4);
}
void Phase1_YellowOff()
{
    CloseOutput(4);
}
void Phase1_RedOn()
{
    OpenOutput(6);
}
void Phase1_RedOff()
{
    CloseOutput(6);
}
void Phase2_GreenOn()
{
    OpenOutput(1);
}
void Phase2_GreenOff()
{
    CloseOutput(1);
}

void Walking_RedOn()
{
    OpenOutput(3);
}
void Walking_RedOff()
{
    CloseOutput(3);
}

void Phase2_YellowOn()
{
    OpenOutput(5);
}
void Phase2_YellowOff()
{
    CloseOutput(5);
}
void Phase2_RedOn()
{
    OpenOutput(7);
}
void Phase2_RedOff()
{
    CloseOutput(7);
}
/////////7 led////////
void Display_number(int index){
    switch(index){
        case 1:
            
            break;
    }
}
/////////////Main fsm/////////////
void Traffic_fsm(){
    switch(trafficMode){
        case INIT_SYSTEM:
            cntOfLight = 0;
            timeOfLight = 0;
            trafficMode = MODE_TRAFFIC_LIGHT;
            break;
        case MODE_TRAFFIC_LIGHT:
            LedTrafficLight();
            TimeTrafficLight();
            LcdTrafficLight();
            if(isButton4Up()){
                trafficMode = MODE_PASSWORD;
            }
            if(isButton6Up()){
                LcdClearS();
                trafficMode = MODE_UART;
            }
            break;
        case MODE_PASSWORD:
            LedTrafficLight();
            TimeTrafficLight();
            App_PasswordDoor();
            break;
        case MODE_CHOOSE_MODE:
            LedTrafficLight();
            TimeTrafficLight();
            waiting_time++;
            if (count == 0){
                LcdClearS();
                LcdPrintStringS(0,0,"1:SETTING TIME <");
                LcdPrintStringS(1,0,"2:MANUAL MODE");
            }else if(count == 1){
                LcdClearS();
                LcdPrintStringS(0,0,"1:SETTING TIME");
                LcdPrintStringS(1,0,"2:MANUAL MODE  <");
            }else if(count == 2){
                LcdClearS();
                LcdPrintStringS(0,0,"3:SLOW MODE <");
                LcdPrintStringS(1,0,"4:UART MODE");
            }else if(count == 3){
                LcdClearS();
                LcdPrintStringS(0,0,"3:SLOW MODE");
                LcdPrintStringS(1,0,"4:UART MODE <");
            }
            if(isButton4Up()){
                count=(count+1)%num_mode;
                waiting_time = 0;
            }
            if(isButton5Up()){
                if(count == 0)count = num_mode;
                count=(count-1)%num_mode;
                waiting_time = 0;
            }
            if(isButton6Up()){
                LcdClearS();
                trafficMode = count;
            }
            if(waiting_time == 500)trafficMode = MODE_TRAFFIC_LIGHT;
            break;
        case MODE_SETTING_TIME:
            LedTrafficLight();
            TimeTrafficLight();
            setting_time();
            break;
        case MODE_MANUAL:
            Manual_Adjustment();
            TimeTrafficLight();
            break;
        case MODE_SLOW:
            Slow_mode();
            TimeTrafficLight();
            break;
        case MODE_UART:
            LedTrafficLight();
            TimeTrafficLight();
            UartDataReceiveProcess();
            break;
        default:
            trafficMode = INIT_SYSTEM;
            break;
    }
}
//////////Traffic Light///////////
void LcdTrafficLight(){
    LcdPrintStringS(0,0,"TRAFFIC LIGHT:ON");
    switch (statusOfLight){
        case PHASE1_GREEN:
            LcdPrintStringS(1,0,"PHASE1_GREEN:   ");
        break;
        case PHASE1_YELLOW:
            LcdPrintStringS(1,0,"PHASE1_YELLOW:  ");            
        break;
        case PHASE2_GREEN:
            LcdPrintStringS(1,0,"PHASE2_GREEN:   ");
        break;
        case PHASE2_YELLOW:
            LcdPrintStringS(1,0,"PHASE2_YELLOW:  ");   
        break;
    }
    LcdPrintNumS(1,14,timeOfLight);
}
void LedTrafficLight()
{
    switch (statusOfLight)
    {
        case PHASE1_GREEN:
            Phase1_GreenOn();
            Phase1_YellowOff();
            Phase1_RedOff();
            
            Walking_RedOn();
            Walking_GreenOff();
            
            Phase2_RedOn();
            Phase2_GreenOff();
            Phase2_YellowOff();
            break;
        case PHASE1_YELLOW:
            Phase1_YellowOn();
            Phase1_GreenOff();
            Phase1_RedOff();
            
            Phase2_RedOn();
            Phase2_GreenOff();
            Phase2_YellowOff();
            
//            if((cntOfLight % 5)<=3){
//                Walking_RedOn();
//            }
//            else Walking_RedOff();
            break;
        case PHASE2_GREEN:
            Phase2_GreenOn();
            Phase2_YellowOff();
            Phase2_RedOff();
            
            Walking_GreenOn();
            Walking_RedOff();
            
            Phase1_RedOn();
            Phase1_GreenOff();
            Phase1_YellowOff();            
            break;
        case PHASE2_YELLOW:
            Phase2_GreenOff();
            Phase2_YellowOn();
            Phase2_RedOff();
            
            Phase1_RedOn();
            Phase1_GreenOff();
            Phase1_YellowOff(); 
//            if((cntOfLight % 5)<=3){
//                Walking_GreenOn();
//            }
//            else Walking_GreenOff();
            break;
    }
}
void TimeTrafficLight(){
    cntOfLight = (cntOfLight + 1)%20;
    if (cntOfLight == 0)
        timeOfLight --;
    switch (statusOfLight)
    {
        case PHASE1_GREEN:
            nextModeFlag = 0;
            //waitingTimeTrafficLight = timeOfLight + timeOfYellowPhase1;
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE1_YELLOW;
                timeOfLight = timeOfYellowPhase1;
            }
            break;
        case PHASE1_YELLOW:
            //waitingTimeTrafficLight = timeOfLight;
            //if(waitingTimeTrafficLight == 0)nextModeFlag = 1;
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE2_GREEN;
                timeOfLight = timeOfGreenPhase2;
                nextModeFlag = 1;
            }
            break;
            
        case PHASE2_GREEN:
            //waitingTimeTrafficLight = timeOfLight + timeOfYellowPhase2;
            nextModeFlag = 0;
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE2_YELLOW;
                timeOfLight = timeOfYellowPhase2;
            }
            break;
        case PHASE2_YELLOW:
            //waitingTimeTrafficLight = timeOfLight;
            //if(waitingTimeTrafficLight == 0)nextModeFlag = 1;
            if (timeOfLight == 0)
            {
                statusOfLight = PHASE1_GREEN;
                timeOfLight = timeOfGreenPhase1;
                nextModeFlag = 1;
            }
            break;
        default:
            statusOfLight = PHASE1_GREEN;
            break;
    }
}
void WaitingTimeTrafficLight(){
    switch (statusOfLight)
    {
        case PHASE1_GREEN:
            waitingTimeTrafficLight = timeOfLight + timeOfYellowPhase1;
            break;
        case PHASE1_YELLOW:
            waitingTimeTrafficLight = timeOfLight;            
            break;
        case PHASE2_GREEN:
            waitingTimeTrafficLight = timeOfLight + timeOfYellowPhase2;
            break;
        case PHASE2_YELLOW:
            waitingTimeTrafficLight = timeOfLight;
            break;
    }
    //if(waitingTimeTrafficLight == 0)nextModeFlag = 1;
}
unsigned char isButton4Up()
{
    if (key_code[4] == 1 || (key_code[4] >= 20 && key_code[4]%2 == 1))
        return 1;
    else
        return 0;
}
unsigned char isButton5Up()
{
    if (key_code[5] == 1 || (key_code[5] >= 20 && key_code[5]%2 == 1))
        return 1;
    else
        return 0;
}
unsigned char isButton6Up()
{
    if (key_code[6] == 1 || (key_code[6] >= 20 && key_code[6]%2 == 1))
        return 1;
    else
        return 0;
}
unsigned char isButton8Up()
{
    if (key_code[8] == 1 || (key_code[8] >= 20 && key_code[8]%2 == 1))
        return 1;
    else
        return 0;
}
/////////uart mode/////////

void UartDataReceiveProcess()
{
    //uart_isr_simulate_machine();
    
    if(flagOfDataReceiveComplete == 1)
    {
        flagOfDataReceiveComplete = 0;
        //if (dataReceive[4] == 0)
        //{
            timeOfGreenPhase1 = dataReceive[0]*10 + dataReceive[1];
            timeOfYellowPhase1 = dataReceive[2]*10 + dataReceive[3];
            timeOfGreenPhase2 = dataReceive[4]*10 + dataReceive[5];
            timeOfYellowPhase2 = dataReceive[6]*10 + dataReceive[7];
        //}
        LcdClearS();
        //LcdPrintStringS(0,0,"RECEIVE:");
        LcdPrintStringS(0,0,"G1:");
        LcdPrintNumS(0,4,timeOfGreenPhase1);
        LcdPrintStringS(0,8,"Y1:");
        LcdPrintNumS(0,11,timeOfYellowPhase1);
        LcdPrintStringS(1,0,"G2:");
        LcdPrintNumS(1,4,timeOfGreenPhase2);
        LcdPrintStringS(1,8,"Y2:");
        LcdPrintNumS(1,11,timeOfYellowPhase2);
    }
    LcdPrintNumS(0,15,statusReceive);
    
    if(isButton4Up()){
        trafficMode = INIT_SYSTEM;
    }
}
/////////setting time mode/////////
void setting_time(){
    LcdClearS();
    LcdPrintStringS(0,0,"P1 G:");
    LcdPrintNumS(0,5,temp_G_1);
    LcdPrintStringS(0,9,"Y:");
    LcdPrintNumS(0,11,temp_Y_1);
    
    LcdPrintStringS(1,0,"P2 G:");
    LcdPrintNumS(1,5,temp_G_2);
    LcdPrintStringS(1,9,"Y:");
    LcdPrintNumS(1,11,temp_Y_2);
    switch(settingMode){
        case MODE_1:
//            Phase1_GreenOn();
//            Phase2_GreenOff();
//            Phase1_RedOff();
//            Phase2_RedOff();
//            Phase1_YellowOff();
//            Phase2_YellowOff();
            LcdPrintStringS(0,8,"<");            
            if(isButton4Up()){
                settingMode = MODE_2;
            }
            if(isButton5Up()){               
                if(temp_G_1 == 99) temp_G_1 = 0;
                else temp_G_1++;
            }  
            if(isButton6Up()){                
                if(temp_G_1 == 0) temp_G_1 = 99;
                else temp_G_1--;
            }
            break;
        case MODE_2:
//            Phase1_GreenOff();
//            Phase2_GreenOn();
//            Phase1_RedOff();
//            Phase2_RedOff();
//            Phase1_YellowOff();
//            Phase2_YellowOff();
            LcdPrintStringS(1,8,"<");
            if(isButton4Up()){
                settingMode = MODE_3;
            }
            if(isButton5Up()){               
                if(temp_G_2 == 99) temp_G_2 = 0;
                else temp_G_2++;
            }  
            if(isButton6Up()){                
                if(temp_G_2 == 0) temp_G_2 = 99;
                else temp_G_2--;
            }
            break;
        case MODE_3:
//            Phase1_GreenOff();
//            Phase2_GreenOff();
//            Phase1_RedOff();
//            Phase2_RedOff();
//            Phase1_YellowOn();
//            Phase2_YellowOff();
            LcdPrintStringS(0,14,"<");
            if(isButton4Up()){
                settingMode = MODE_4;
            }
            if(isButton5Up()){               
                if(temp_Y_1 == 99) temp_Y_1 = 0;
                else temp_Y_1++;
            }  
            if(isButton6Up()){                
                if(temp_Y_1 == 0) temp_Y_1 = 99;
                else temp_Y_1--;
            }
            break;
        case MODE_4:
//            Phase1_GreenOff();
//            Phase2_GreenOff();
//            Phase1_RedOff();
//            Phase2_RedOff();
//            Phase1_YellowOff();
//            Phase2_YellowOn();
            LcdPrintStringS(1,14,"<");
            if(isButton4Up()){
                settingMode = KEEP_SETTING;
                //trafficMode = INIT_SYSTEM;
            }
            if(isButton5Up()){               
                if(temp_Y_2 == 99) temp_Y_2 = 0;
                else temp_Y_2++;
            }  
            if(isButton6Up()){                
                if(temp_Y_2 == 0) temp_Y_2 = 99;
                else temp_Y_2--;
            }
            break;
        case KEEP_SETTING:
            LcdClearS();
            LcdPrintStringS(0,0,"KEEP SETTING?");
            LcdPrintStringS(1,0,"YES  NO  RESET");
            if (count == 0){
                LcdPrintStringS(1,0,"YES< NO  RESET");
            }else if(count == 1){
                LcdPrintStringS(1,0,"YES  NO< RESET");
            }else if(count == 2){
                LcdPrintStringS(1,0,"YES  NO  RESET<");
            }
            if(isButton4Up()){
                count=(count+1)%3;
            }
            if(isButton5Up()){
                if(count == 0)count = 3;
                count=(count-1)%3;
            }
            if(isButton6Up()){
                LcdClearS();
                if (count == 0){
                    timeOfGreenPhase1 = temp_G_1;
                    timeOfYellowPhase1 = temp_Y_1;
                    timeOfGreenPhase2 = temp_G_2;
                    timeOfYellowPhase2 = temp_Y_2;
                    settingMode = WAITING_NEXT_MODE;                 
                }else if(count == 1){
                    temp_G_1 = timeOfGreenPhase1;
                    temp_Y_1 = timeOfYellowPhase1;
                    temp_G_2 = timeOfGreenPhase2;
                    temp_Y_2 = timeOfYellowPhase2;
                    settingMode = MODE_1;
                    trafficMode = MODE_TRAFFIC_LIGHT;
                }else if(count == 2){
                    temp_G_1 = timeOfGreenPhase1;
                    temp_Y_1 = timeOfYellowPhase1;
                    temp_G_2 = timeOfGreenPhase2;
                    temp_Y_2 = timeOfYellowPhase2;
                    
                    LcdClearS();
                    LcdPrintStringS(0,0,"P1 G:");
                    LcdPrintNumS(0,5,temp_G_1);
                    LcdPrintStringS(0,9,"Y:");
                    LcdPrintNumS(0,11,temp_Y_1);

                    LcdPrintStringS(1,0,"P2 G:");
                    LcdPrintNumS(1,5,temp_G_2);
                    LcdPrintStringS(1,9,"Y:");
                    LcdPrintNumS(1,11,temp_Y_2);
                    
                    settingMode = MODE_1;
                }
            }
            break;
        case WAITING_NEXT_MODE:  
            WaitingTimeTrafficLight();
            if(nextModeFlag == 1){
                settingMode = MODE_1;
                trafficMode = MODE_TRAFFIC_LIGHT;                
                nextModeFlag = 0;
            } else {
                LcdClearS();
                LcdPrintStringS(0,0," UPDATE SETTING ");
                LcdPrintStringS(1,0,"    IN:  ");
                LcdPrintNumS(1,8,waitingTimeTrafficLight);
            }            
            break;
        default:
            settingMode = MODE_1;
            break;
    }
}
/////////////slow mode//////////////
void Slow_mode(){   
    Phase1_RedOff();
    Phase1_GreenOff();                
    Phase2_GreenOff();
    Phase2_RedOff();
    
    Walking_GreenOff();
    Walking_RedOff();
    LcdClearS();
    LcdPrintStringS(1,0,"GO OUT: PRESS 4 ");
    LcdPrintStringS(0,0,"SLOW MODE: ON");

    count = (count + 1)%100;
    if(count > 50){
        LcdPrintStringS(1,0,"");
        Phase1_YellowOff();
        Phase2_YellowOff();
    }
    else{
        Phase1_YellowOn();
        Phase2_YellowOn();
    }
    if(isButton4Up()){
        count = 0;
        trafficMode = MODE_TRAFFIC_LIGHT;
    }
}
///////////manual adjustment////////
void Manual_Adjustment(){
    switch(manualMode){
        case INIT_SYSTEM:
            LcdClearS();
            LcdPrintStringS(0,0,"TRAFFIC MODE:4");
            LcdPrintStringS(1,0,"CHANGE MANUAL:5");
            timeDelay = 0;
            if(isButton4Up()){
                trafficMode = WAITING_NEXT_MODE;
            }
            if(isButton5Up()){
                if(statusOfLight == PHASE1_GREEN || statusOfLight == PHASE1_YELLOW)
                manualMode = MODE_MANUAL_0;
                else manualMode = MODE_MANUAL_1;
            }
        break;
        case MODE_MANUAL_0:
            timeDelay++;
            if(timeDelay%100 < 50){
                LcdClearS();
                LcdPrintStringS(0,0,"MANUAL MODE:ON");   
                if(enable == 0){
                    LcdPrintStringS(1,0,"CHANGE LED IN:");
                    LcdPrintNumS(1,14,(100 - timeDelay)/20);
                }
            }
            else{
                LcdClearS();
                LcdPrintStringS(0,0,"TRAFFIC MODE:4");
                LcdPrintStringS(1,0,"CHANGE MANUAL:5");
            }
            if(timeDelay < 100 && enable == 0){
                Phase1_YellowOn();
                Phase1_GreenOff();
                Phase1_RedOff();
                
                Phase2_YellowOff();
                Phase2_GreenOff();
                Phase2_RedOn();
                
                Walking_RedOn();
                Walking_GreenOff();
            }
            else {
                Phase1_YellowOff();
                Phase1_RedOn();
                Phase1_GreenOff();
                
                Phase2_GreenOn();
                Phase2_RedOff();
                Phase2_YellowOff();
                
                Walking_RedOff();
                Walking_GreenOn();
                if(enable == 0){                
                    enable = 1;
                }        
            }
            if(timeDelay > 100)timeDelay = 0;
            if(isButton4Up()){
                timeDelay = 0;
                enable = 0;
                manualMode = WAITING_NEXT_MODE;
            }
            if(isButton5Up()){
                timeDelay = 0;
                enable = 0;
                manualMode = MODE_MANUAL_1;
            }
        break;        
        case MODE_MANUAL_1:
            timeDelay++;
            
            if(timeDelay%100 < 50){
                LcdClearS();
                LcdPrintStringS(0,0,"MANUAL MODE:ON");   
                if(enable == 0){
                    LcdPrintStringS(1,0,"CHANGE LED IN:");
                    LcdPrintNumS(1,14,(100 - timeDelay)/20);
                }                
            }
            else{
                LcdClearS();
                LcdPrintStringS(0,0,"TRAFFIC MODE:4");
                LcdPrintStringS(1,0,"CHANGE MANUAL:5");
            }
            if(timeDelay < 100 && enable == 0){
                Phase2_YellowOn();
                Phase2_GreenOff();
                Phase2_RedOff();
                
                Phase1_YellowOff();
                Phase1_GreenOff();
                Phase1_RedOn();
                
                Walking_RedOff();
                Walking_GreenOn();
            }
            else{
                Phase2_YellowOff();
                Phase2_RedOn();
                
                Phase1_GreenOn();
                Phase1_RedOff();
                
                Walking_RedOn();
                Walking_GreenOff();
                if(enable == 0){
                    enable = 1;
                }
            }
            if(timeDelay > 100)timeDelay = 0;
            if(isButton4Up()){
                timeDelay = 0;
                enable = 0;
                manualMode = WAITING_NEXT_MODE;
            }
            if(isButton5Up()){
                timeDelay = 0;
                enable = 0;
                manualMode = MODE_MANUAL_0;
            }
        break;
        case WAITING_NEXT_MODE:
            WaitingTimeTrafficLight();
            if(nextModeFlag == 1){
                trafficMode = MODE_TRAFFIC_LIGHT;
                manualMode = INIT_SYSTEM;
                nextModeFlag = 0;
            } else {
                LcdClearS();
                LcdPrintStringS(0,0," CHANGE MODE ");
                LcdPrintStringS(1,0,"    IN:  ");
                LcdPrintNumS(1,10,waitingTimeTrafficLight);
            }           
        break;
    }            
}
///////////password door//////////
void App_PasswordDoor()
{
    switch (statusPassword)
    {
        case INIT_SYSTEM:
            LcdPrintStringS(0,0,"PRESS 4 FOR PASS");
            LcdPrintStringS(1,0,"                ");
            LockDoor();
            if (isButton4Up())
            {
                indexOfNumber = 0;
                timeDelay = 0;
                statusPassword = ENTER_PASSWORD;
            }
            break;
        case ENTER_PASSWORD:
            LcdPrintStringS(0,0,"ENTER PASSWORD  ");
            timeDelay ++;
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if (indexOfNumber >= 4)
                statusPassword = CHECK_PASSWORD;
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            if (isButtonEnter())
                statusPassword = INIT_SYSTEM;
            break;
        case CHECK_PASSWORD:
            timeDelay = 0;
            if (CheckPassword())
                statusPassword = UNLOCK_DOOR;
            else
                statusPassword = WRONG_PASSWORD;
            break;
        case UNLOCK_DOOR:
            timeDelay ++;
            LcdPrintStringS(0,0,"OPENING DOOR    ");
            UnlockDoor();
            if (timeDelay >= 100)
                statusPassword = INIT_SYSTEM;
            break;
        case WRONG_PASSWORD:
            timeDelay ++;
            LcdPrintStringS(0,0,"PASSWORD WRONG  ");
            if (timeDelay >= 40)
                statusPassword = INIT_SYSTEM;
            break;
        default:
            break;

    }
}
unsigned char CheckPassword()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i<5;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayPassword[j] != arrayMapOfPassword[i][j])
                result = 0;
        }
        if (result == 1)
            return (i+1);
    }

}
unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}

unsigned char isButtonEnter()
{
    if (key_code[12] == 1)
        return 1;
    else
        return 0;
}
void UnlockDoor()
{    
    trafficMode = MODE_CHOOSE_MODE;
    count = 0;
    statusPassword = INIT_SYSTEM;
}
void LockDoor()
{
    timeDelay++;
    if (timeDelay >= 200){
        LcdPrintStringS(0,0,"TRAFFIC LED     ");
        trafficMode = MODE_TRAFFIC_LIGHT;
        statusPassword = INIT_SYSTEM;
        timeDelay = 0;
    }    
}