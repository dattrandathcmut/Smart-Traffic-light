#include "simulate_machine.h"
#include "..\button_matrix\button.h"
#include "..\lcd\lcd.h"

#define INIT_RECEIVE		0
#define WAIT_HEADER_1		1
#define WAIT_HEADER_2		2
#define RECEIVE_DATA		3
#define RECEIVE_DATA_X1		4
#define RECEIVE_DATA_X2		5
#define RECEIVE_DATA_X3		6
#define RECEIVE_DATA_X4		7
#define RECEIVE_DATA_Y1		8
#define RECEIVE_DATA_Y2		9
#define RECEIVE_DATA_Y3		10
#define RECEIVE_DATA_Y4		11
#define END_OF_RECEIVE_1	12
#define END_OF_RECEIVE_2	13
#define MANUAL_0            14
#define MANUAL_1            15
#define SLOW                16
#define     INIT_SYSTEM     0
#define     CALL            1
#define     MESSAGE         2
#define     WAITING         3
#define     MESSAGE_GPRS         4
#define     WAITING_GPRS         5
// Noi khai bao bien toan cuc
unsigned char statusSim900 = INIT_SYSTEM;
unsigned char WaitResponse = 0;
unsigned char trafficMode = INIT_SYSTEM;
unsigned char manualMode = INIT_SYSTEM;
unsigned char i;
unsigned char statusReceive = WAIT_HEADER_1;
unsigned char flagOfDataReceiveComplete = 0;
unsigned char indexOfMachine = 0;
unsigned char indexOfDataReceive = 0;
unsigned char indexOfdataSend = 0;
unsigned char numberOfDataReceive = 8;
unsigned char dataReceive [50];
unsigned char numberOfDataSend = 5;
unsigned char dataSend [50];

unsigned char tempCount = 0, TEMP_value = 0, pH_value = 0, FLOW_value = 0;
unsigned char COD_value = 0, TSS_value = 0, NO3_value = 0, NH4_value = 0;
unsigned int timeChangedataSend = 0;
unsigned char isButtonMessage();
unsigned char isButtonCall();
unsigned char isButtonGPRS();
unsigned char isButtonClear();
unsigned char countReceive = 0;///////

unsigned char tempReceive_x1;
unsigned char tempReceive_x2;
unsigned char tempReceive_x3;
unsigned char tempReceive_x4;

unsigned char tempReceive_y1;
unsigned char tempReceive_y2;
unsigned char tempReceive_y3;
unsigned char tempReceive_y4;

unsigned char tempReceive;                                                                                                                                                                                                                                                                                                                                                      
unsigned char number;
unsigned char enable_flag = 0;

void uart_isr_simulate_machine()
{

    LcdClearS();
    tempReceive = RCREG;
    uart_putchar(tempReceive);
    switch(statusReceive)
    {
        case WAIT_HEADER_1:
            if(countReceive == 0){
                UartSendString("\r\n");
                UartSendString("-----------------\r\n");
                UartSendString("Press a change to setting time mode\r\n");
                UartSendString("Press b change to manual mode\r\n");
                UartSendString("Press c change to slow down mode\r\n");
                countReceive = 1;
            }            
            if (tempReceive == 97){
                indexOfDataReceive = 0;                
                countReceive = 0;
                UartSendString("\r\n");
                UartSendString("-----------------\r\n");
                UartSendString("-SETTING MODE: ON-\r\n");
                UartSendString("Phase1-Green:(X1)(X2)s--Phase1-Yellow:(X3)(X4)s\r\n");
                UartSendString("Phase2-Green:(Y1)(Y2)s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                UartSendString("-----------------\r\n");
                UartSendString("Update X1:");
                statusReceive = RECEIVE_DATA_X1;
            }
            else if (tempReceive == 98){
                indexOfDataReceive = 0;                
                countReceive = 0;
                UartSendString("\r\n");
                UartSendString("-----------------\r\n");
                UartSendString("-MANUAL MODE: ON-\r\n");
                UartSendString("press a to change led!\r\n");
                UartSendString("press b to come back to traffic mode!\r\n");
                trafficMode = 1;
                statusReceive = MANUAL_0;
            }
            else if (tempReceive == 99){
                indexOfDataReceive = 0;                
                countReceive = 0;
                UartSendString("\r\n");
                UartSendString("-----------------\r\n");
                UartSendString("-SLOW MODE: ON-\r\n");
                UartSendString("press b to come back to traffic mode!\r\n");
                trafficMode = 2;
                statusReceive = SLOW;
            }
            break;
        case RECEIVE_DATA_X1:
            LcdPrintStringS(0,0,"X1=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_x1 = tempReceive;
                    dataReceive[0] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[0]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    UartSendString("(X2)s--Phase1-Yellow:(X3)(X4)s\r\n");
                    UartSendString("Phase2-Green:(Y1)(Y2)s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                    UartSendString("Reupdate X1 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update X1:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update X2:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_X2:
            LcdPrintStringS(0,0,"X2=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_x2 = tempReceive;
                    dataReceive[1] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[1]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:(X3)(X4)s\r\n");
                    UartSendString("Phase2-Green:(Y1)(Y2)s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                    UartSendString("Reupdate X2 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update X2:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update X3:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_X3:
            LcdPrintStringS(0,0,"X3=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_x3 = tempReceive;
                    dataReceive[2] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[2]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    UartSendString("(X4)s\r\n");
                    UartSendString("Phase2-Green:(Y1)(Y2)s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                    UartSendString("Reupdate X3 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update X3:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update X4:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_X4:
            LcdPrintStringS(0,0,"X4=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_x4 = tempReceive;
                    dataReceive[3] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[3]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    uart_putchar(tempReceive_x4);
                    UartSendString("s\r\n");
                    UartSendString("Phase2-Green:(Y1)(Y2)--Phase2-Yellow:(Y3)(Y4)\r\n");
                    UartSendString("Reupdate X4 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update X4:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update Y1:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_Y1:
            LcdPrintStringS(0,0,"Y1=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_y1 = tempReceive;
                    dataReceive[4] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[4]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    uart_putchar(tempReceive_x4);
                    UartSendString("s\r\n");
                    UartSendString("Phase2-Green:");
                    uart_putchar(tempReceive_y1);
                    UartSendString("(Y2)s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                    UartSendString("Reupdate Y1 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update Y1:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update Y2:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_Y2:
            LcdPrintStringS(0,0,"Y2=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_y2 = tempReceive;
                    dataReceive[5] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[5]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    uart_putchar(tempReceive_x4);
                    UartSendString("s\r\n");
                    UartSendString("Phase2-Green:");
                    uart_putchar(tempReceive_y1);
                    uart_putchar(tempReceive_y2);
                    UartSendString("s--Phase2-Yellow:(Y3)(Y4)s\r\n");
                    UartSendString("Reupdate X2 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update Y2:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update Y3:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_Y3:
            LcdPrintStringS(0,0,"Y3=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_y3 = tempReceive;
                    dataReceive[6] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[6]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    uart_putchar(tempReceive_x4);
                    UartSendString("s\r\n");
                    UartSendString("Phase2-Green:");
                    uart_putchar(tempReceive_y1);
                    uart_putchar(tempReceive_y2);
                    UartSendString("s--Phase2-Yellow:");
                    uart_putchar(tempReceive_y3);
                    UartSendString("(Y4)s\r\n");
                    UartSendString("Reupdate Y3 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update Y3:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update Y4:");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case RECEIVE_DATA_Y4:
            LcdPrintStringS(0,0,"Y4=");      
            if(enable_flag == 0 || (tempReceive >= 48 && 57 >= tempReceive)){
                if(tempReceive >= 48 && 57 >= tempReceive){
                    UartSendString("\r\n");
                    tempReceive_y4 = tempReceive;
                    dataReceive[7] = tempReceive - 48;
                    LcdPrintNumS(0,4,dataReceive[7]); 
                    UartSendString("-----------------\r\n");
                    UartSendString("Phase1-Green:");
                    uart_putchar(tempReceive_x1);
                    uart_putchar(tempReceive_x2);
                    UartSendString("s--Phase1-Yellow:");
                    uart_putchar(tempReceive_x3);
                    uart_putchar(tempReceive_x4);
                    UartSendString("s\r\n");
                    UartSendString("Phase2-Green:");
                    uart_putchar(tempReceive_y1);
                    uart_putchar(tempReceive_y2);
                    UartSendString("s--Phase2-Yellow:");
                    uart_putchar(tempReceive_y3);
                    uart_putchar(tempReceive_y4);
                    UartSendString("s\r\n");
                    UartSendString("Reupdate X4 or press a to countinue\r\n");
                    enable_flag = 1;
                }else{
                    UartSendString(" => Wrong value \r\n");
                    UartSendString("Update Y4:");
                }
            }else if(enable_flag == 1 && tempReceive == 97){
                    indexOfDataReceive ++;
                    UartSendString("\r\n");
                    UartSendString("-----------------\r\n");
                    UartSendString("Update all setting:\r\n");
                    UartSendString("Yes -> Press a\r\n");
                    UartSendString("No -> Press b\r\n");
                    statusReceive++;
                    enable_flag = 0;
            }
            break;
        case END_OF_RECEIVE_1:
            if (tempReceive == 97)
            {
                flagOfDataReceiveComplete = 1;
                statusReceive = WAIT_HEADER_1;
                UartSendString("\r\n");
                UartSendString("Update success!\r\n");
            }
            else if (tempReceive == 98){
                statusReceive = WAIT_HEADER_1;
                UartSendString("Skipped data!\r\n");
            }
            break;
        case MANUAL_0:
            if (tempReceive == 97)
            {
                UartSendString("\r\n");
                UartSendString("change led complete!\r\n");
                UartSendString("press a to change led!\r\n");
                UartSendString("press b to come back to traffic mode!\r\n");
                manualMode = 1;
                statusReceive = MANUAL_1;
            }
            else if (tempReceive == 98){
                UartSendString("\r\nchanged to traffic mode!\r\n");
                trafficMode = 5;
                statusReceive = WAIT_HEADER_1;
            }            
            break;
        case MANUAL_1:
            if (tempReceive == 97)
            {
                UartSendString("\r\n");
                UartSendString("change led complete!\r\n");
                UartSendString("press a to change led!\r\n");
                UartSendString("press b to come back to traffic mode!\r\n");
                manualMode = 0;
                statusReceive = MANUAL_0;
            }
            else if (tempReceive == 98){
                UartSendString("\r\nChanged to traffic mode!\r\n");
                trafficMode = 5;
                statusReceive = WAIT_HEADER_1;
            }
            break;
        case SLOW:
            if (tempReceive == 98)
                UartSendString("Changed to traffic mode!\r\n");
                statusReceive = WAIT_HEADER_1;
            break;
            
        default:
            statusReceive = INIT_RECEIVE;
            break;
    }   
    //LcdPrintNumS(0,7,dataReceive[indexOfDataReceive]);
}

void SimulatedataSend()
{
   
}

void DisplayDataReceive()
{
    if(flagOfDataReceiveComplete == 1)
    {
        //flagOfDataReceiveComplete = 0;
        LcdPrintStringS(0,0,"                ");
        for (i = 0; i<numberOfDataReceive;i++)
            LcdPrintNumS(0,3*i,dataReceive[i]);
        LcdPrintNumS(0,15,statusReceive);
    }

}
