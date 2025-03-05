
const int trig = 14;     // D5 chân trig của HC-SR04
const int echo = 12;     // D6 chân echo của HC-SR04
int gui = 1800000; //30 phút gửi mail 1 lần
int het = 10;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>         //Thêm thư viện đi kèm trong thư mục đi kèm
LiquidCrystal_I2C lcd(0x27,20,4);
#include "Arduino.h"
#include <EMailSender.h>
#include <ESP8266WiFi.h>
int but = D3; //Nút nhấn gửi mail
const char* ssid = "Thu Thao";
const char* password = "12052002";
long times;
uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("hungdaoduy0703@gmail.com", "Hung0703");   //Mail và mật khẩu mail cần gửi tin nhắn

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}
  
void setup()
{
    Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
    lcd.init();
    lcd.backlight();
    times = millis();
    pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
    pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
    pinMode(but,INPUT_PULLUP);
    lcd.setCursor(2,0);
  lcd.print("DO MUC NUOC");
  connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    
    Serial.println("Sending status: ");

}
 
void loop()
{
    unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    
    /* In kết quả ra Serial Monitor */
    Serial.print(distance);
    Serial.println("cm");
    lcd.setCursor(0,1);
  lcd.print("T:           Cm");
  lcd.setCursor(3,1);
  lcd.print(distance);   
    delay(200);
  if(millis()- times >= gui){
  EMailSender::EMailMessage message;
    message.subject = "HE THONG DO MUC NUOC";
    message.message = String("MUC NUOC HIEN TAI LA:  ") + String(distance) + String("  Cm ") ;

    EMailSender::Response resp = emailSend.send("tranminhquyet20@gmail.com", message);  ///Mail nhận tin nhắn 
  times = millis();
  }
  if(digitalRead(but) == 0){
    delay(300);
    Serial.println("DA GUI MAIL");
    EMailSender::EMailMessage message;
    message.subject = "HE THONG DO MUC NUOC";
    message.message = String("NUT NHAN DUOC NHAN <br> MUC NUOC HIEN TAI LA:  ") + String(distance) + String("  Cm ") ;//<br>

    EMailSender::Response resp = emailSend.send("tranminhquyet20@gmail.com", message);
  }
  else{};
}
