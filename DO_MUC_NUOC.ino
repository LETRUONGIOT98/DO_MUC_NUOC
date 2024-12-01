/*
 * Kết nối:
            HCSR04                          Arduino
            VCC                               5V
            GND                               GND
            TRIG                              8
            ECHO                              7

 */

 /*
 * Kết nối: LCD
            LCD                             Arduino
            VCC                               5V
            GND                               GND
            SCL                               A5
            SDA                               A4

   Nạp code mở Serial Monitor chọn No line ending, baud 9600.
 */
const int trig = 8;     // chân trig của HC-SR04
const int echo = 7;     // chân echo của HC-SR04
 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
int docaoly = 20;  //đo chính xác độ cao từ cảm biến đến đáy ly nước cần đo
void setup()
{
    Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
    pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
    pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
    lcd.init();                      // initialize the lcd 
  lcd.backlight();
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
    if(distance < 4) distance =0;
    int mucnuoc = map(distance,0,docaoly,0,100);
    lcd.setCursor(0, 0);
  lcd.print("    DINH DUC    ");
  lcd.setCursor(0, 1);
  lcd.print("MUC NUOC:      %");
  lcd.setCursor(10, 1);
  lcd.print(mucnuoc);
    delay(200);
}
