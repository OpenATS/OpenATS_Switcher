/*
/                             OpenATS Switcher v1.0
/     
/    该项目为建设OpenATS天线射频链路切换系统搭建(build for my OpenATS earth station)
/         
/
/
/                                                     <------L-band
/       SDR --------------------------->SWITCH5(PIN6)
/                                                     <------S-band        
/                                                                                    
/       SDR1    ------>                                              <------ C-band
/                        SWITCH1(PIN2)---------------SWITCH2(PIN3)                  
/    DVB-CARD1  ------>                                              <------ X-band
/                                                                                     
/                                                                                  
/       SDR2    ------>                                              <------ Ku-band
/                        SWITCH3(PIN4)---------------SWITCH4(PIN5)                  
/    DVB-CARD2  ------>                                              <------ Ka-band
/
/   默认开关不通电状态(default):  DVB-CARD1 --- > C-band,   DVB-CARD2 ---> Ku-band  SDR--->L-band
/   低电平触发(Low Leavel Trigger)
/   Arduino
/
/  mode_a:
/    100  DVB->C
/    101  DVB->X
/    110  SDR->C
/    111  SDR->X
/   mode_b:
/    200  DVB->KU
/    201  DVB->KA
/    210  SDR->KU
/    211  SDR->KA
/   mode_c:
/      0  SDR->L
/      1  SDR->S
/
*/ 

#include <stdio.h>
#include <string.h>

#define SWITCH1 2
#define SWITCH2 3
#define SWITCH3 4
#define SWITCH4 5
#define SWITCH5 6

/*
 * This is backup options
 * 备份设置，当继电器电路板出现问题时，可远程烧写代码切换到第二个继电器板
#define SWITCH1 8
#define SWITCH2 9
#define SWITCH3 10
#define SWITCH4 11
#define SWITCH5 12
*/
 
int mode_a = 100;   //DVB->C
int mode_b = 200;   //DVB->KU
int mode_c = 0;     //SDR->L

void setup()
{
    Serial.begin(115200);
    
    pinMode(SWITCH1,OUTPUT);
    digitalWrite(SWITCH1,HIGH);         //all default off
    pinMode(SWITCH2,OUTPUT);
    digitalWrite(SWITCH2,HIGH);
    pinMode(SWITCH3,OUTPUT);
    digitalWrite(SWITCH3,HIGH);
    pinMode(SWITCH4,OUTPUT);
    digitalWrite(SWITCH4,HIGH);
    pinMode(SWITCH5,OUTPUT);
    digitalWrite(SWITCH5,HIGH);
    
    help();
}

void help()
{
    Serial.println("************************************************************");
    Serial.println("                   OpenATS Switcher v1.0        ");
    Serial.println("                                         www.openats.cn");
    Serial.println("       Send : 'Device Band' [S : SDR   D : DVB_CARD]");
    Serial.println("");
    Serial.println("          S C  : SDR --> C-band");
    Serial.println("          S X  : SDR --> X-band");
    Serial.println("          S KU : SDR --> Ku-band");
    Serial.println("          S KA : SDR --> Ka-band");
    Serial.println("");
    Serial.println("          D C  : DVB_CARD --> C-band");
    Serial.println("          D X  : DVB_CARD --> X-band");
    Serial.println("          D KU : DVB_CARD --> Ku-band");
    Serial.println("          D KA : DVB_CARD --> Ka-band");
    Serial.println("");
    //Serial.println("          S/s  : SDR --> S-band");
    //Serial.println("          L/l  : SDR --> L-band");
    Serial.println("");
    Serial.println("          R/r  : Reset to default: D->C, D->Ku");
    Serial.println("          H/h  : This HELP");
    Serial.println("");
    Serial.println("************************************************************");
    status();
}

void status()
{                                                         //返回当前状态
    Serial.println("STATUS:");
    
    if(mode_c == 0)
    {
        Serial.println("                                / <-----  L-band   ");
        Serial.println("        SDR  ------------------/ ");
        Serial.println("                                  <-----  S-band   ");
    }else{
        Serial.println("                                  <-----  L-band   ");
        Serial.println("        SDR  ------------------\\");
        Serial.println("                                \\ <-----   S-band   ");  
    }
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
    
    Serial.println("        SDR1 ------>               <------ C-band");
    if(mode_a == 100)
    {
        Serial.println("                                /       ");
        Serial.println("                       --------         ");
        Serial.println("                      /                  ");
    }else if(mode_a == 101){
        Serial.println("                                    ");
        Serial.println("                       ---------         ");
        Serial.println("                      /         \\       ");  
    }else if(mode_a == 110){
        Serial.println("                      \\         /      ");
        Serial.println("                       ---------         ");
        Serial.println("                                 ");
    }else if(mode_a == 111){
        Serial.println("                      \\                 ");
        Serial.println("                       ---------         ");
        Serial.println("                                \\       ");
    }
    Serial.println("        DVB1 ------>               <------ X-band");
    Serial.println("");
    Serial.println("");
    //Serial.println("============================================================");
    Serial.println("");
    Serial.println("");
    Serial.println("        SDR2 ------>               <------ Ku-band");
    if(mode_b == 200)
    {
        Serial.println("                                /       ");
        Serial.println("                       ---------         ");
        Serial.println("                      /         ");
    }else if(mode_b == 201){
        Serial.println("                                 ");
        Serial.println("                       ---------         ");
        Serial.println("                      /         \\       ");  
    }else if(mode_b == 210){
        Serial.println("                      \\         /      ");
        Serial.println("                       ---------         ");
        Serial.println("                                 ");
    }else if(mode_b == 211){
        Serial.println("                      \\                 ");
        Serial.println("                       ---------         ");
        Serial.println("                                \\       ");
    }
    Serial.println("        DVB2 ------>               <------ Ka-band");
    Serial.println("############################################################");
} 


//主程序循环
void loop()                                                
{
    String usbdata = "";
    char a[5]={0};
    char b[5]={0};

    //usb串口的数据格式例子：
    //
    //接收串口数据，将串口字符串数据，传递给usbdata变量
    while (Serial.available() > 0){
        usbdata += char(Serial.read());
        delay(2);
    }                                                                    


    if(usbdata.length() > 1 && usbdata.startsWith("S") || usbdata.startsWith("s"))           //SDR
    {
        const char *usb = usbdata.c_str();                       //将string数据转成字符串数据
        //Serial.println(usb);
        sscanf(usb,"%s %s",&a, &b);                              
        if(strcmp(b,"C") == 0 || strcmp(b,"c") == 0)             //SDR-->C-BAND
        {
            digitalWrite(SWITCH1,LOW);                           //switch1 通电激活
            digitalWrite(SWITCH2,HIGH);                          //switch2 不通电激活
            //Serial.println("[SDR1-->C-band   switch1 on, switch2 off]");
            mode_a = 110;
            status();
        }else if(strcmp(b,"X") == 0 || strcmp(b,"x") == 0){      //SDR-->X-BAND
            digitalWrite(SWITCH1,LOW);                           //switch1 通电激活    
            digitalWrite(SWITCH2,LOW);                           //switch2 通电激活
            //Serial.println("[SDR1-->X-band   switch1 on, switch2 on]");
            mode_a = 111;
            status();
        }else if(strcmp(b,"KU") == 0 || strcmp(b,"Ku") == 0 || strcmp(b,"ku") == 0){      //SDR-->Ku-BAND
            digitalWrite(SWITCH3,LOW);                           //switch3 通电激活
            digitalWrite(SWITCH4,HIGH);                          //switch4 不通电激活
            //Serial.println("[SDR2-->Ku-band  switch3 on, switch4 off]");
            mode_b = 210;
            status();
        }else if(strcmp(b,"KA") == 0 || strcmp(b,"Ka") == 0 || strcmp(b,"ka") == 0){      //SDR-->Ka-BAND
            digitalWrite(SWITCH3,LOW);                           //switch3 通电激活
            digitalWrite(SWITCH4,LOW);                           //switch4 通电激活               
            //Serial.println("[SDR2-->Ka-band  switch3 on, switch4 on]");
            mode_b = 211;
            status();
        }
        else{
            digitalWrite(SWITCH5,LOW);                         
            mode_c = 1;
            status();
        }
    }
    else if(usbdata.length() > 1 && usbdata.startsWith("D") || usbdata.startsWith("d"))      //DVB-CARD
    {     
        const char *usb = usbdata.c_str();                        //将string数据转成字符串数据
        //Serial.println(usb);
        sscanf(usb,"%s %s",&a, &b);                              
        if(strcmp(b,"C") == 0 || strcmp(b,"c") == 0)              //DVB-->C-BAND
        {
            digitalWrite(SWITCH1,HIGH);                           //switch1 off
            digitalWrite(SWITCH2,HIGH);                           //switch2 off
            //Serial.println("[DVB1-->C-band   switch1 off, switch2 on]");
            mode_a = 100;
            status();
        }else if(strcmp(b,"X") == 0 || strcmp(b,"x") == 0){       //DVB-->X-BAND
            digitalWrite(SWITCH1,HIGH);                           //switch1 off
            digitalWrite(SWITCH2,LOW);                            //switch2 on
            //Serial.println("[DVB1-->X-band   switch1 off, switch2 off]");
            mode_a = 101;
            status();
        }else if(strcmp(b,"KU") == 0 || strcmp(b,"Ku") == 0 || strcmp(b,"ku") == 0){     //DVB-->KU-BAND
            digitalWrite(SWITCH3,HIGH);                           //switch3 off
            digitalWrite(SWITCH4,HIGH);                           //switch4 off                     
            //Serial.println("[DVB2-->Ku-band  switch3 off, switch4 off]");
            mode_b = 200;
            status();
        }else if(strcmp(b,"KA") == 0 || strcmp(b,"Ka") == 0 || strcmp(b,"ka") == 0){     //DVB-->KA-BAND
            digitalWrite(SWITCH3,HIGH);                           //switch3 off
            digitalWrite(SWITCH4,LOW);                            //switch4 on
            //Serial.println("[DVB2-->Ka-band  switch3 off, switch4 on]");
            mode_b = 201;
            status();
        }
    }    
    else if(usbdata.length() > 0 && usbdata.startsWith("H") || usbdata.startsWith("h"))  //help   
        help();
    else if(usbdata.length() > 0 && usbdata.startsWith("R") || usbdata.startsWith("r"))  //back to default
    {
        digitalWrite(SWITCH1,HIGH);                            
        digitalWrite(SWITCH2,HIGH);
        digitalWrite(SWITCH3,HIGH);                            
        digitalWrite(SWITCH4,HIGH);
        digitalWrite(SWITCH5,HIGH);
        mode_a = 100;
        mode_b = 200;
        mode_c = 0;
        status(); 
    }
    else if(usbdata.length() > 0 && usbdata.startsWith("L") || usbdata.startsWith("l"))  //L band
    {
        digitalWrite(SWITCH5,HIGH);                            
        mode_c = 0;
        status(); 
    }  
    else if(usbdata.length() > 0)
        status();
}
//END
