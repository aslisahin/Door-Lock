#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
 #include <Servo.h>
 #include <Wire.h>
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* ssid = "SUPERONLINE_WiFi_4045";
const char* password = "PP3LXMEAFHRF";
 
int ledPin = 13; 
WiFiServer server(80);
 
void setup() {
   Wire.begin();
  lcd.begin();

  
lcd.backlight();
  servo.attach(2); //D4

servo.write(0);

delay(2000);
Serial.begin(9600);
delay(10);
 
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);
 

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

server.begin();
Serial.println("Server started");
 
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
lcd.setCursor(0, 0);
lcd.print("   Welcome!");
lcd.setCursor(0, 1);
lcd.print("   Door Lock");
 
}
 
void loop() {

WiFiClient client = server.available();
if (!client) {lcd.setCursor(0, 0);

return;
}
 
Serial.println("new client");
while(!client.available()){

delay(1);
}

String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();
 
int value = LOW;
if (request.indexOf("/Turn-On ") != -1) {
  lcd.setCursor(0, 0);
lcd.print("Door is Open!");

  servo.write(90);
delay(500);
digitalWrite(ledPin, HIGH);
value = HIGH;

}
if (request.indexOf("/Turn-Off") != -1) {
  lcd.setCursor(0, 0);
lcd.print("Door is Closed!");

servo.write(0);

delay(500);
digitalWrite(ledPin, LOW);
value = LOW;
}
 

 
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); 
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
client.println("<style>p.padding{padding-left: 0.4cm;}p{color: black;}cred{color: red}cgreen{color: green}</style>");
client.print("<br><p class=\"padding\">Door is now : ");
 
if(value == HIGH) {
 client.print("<cgreen>Unlocked<cgreen></p>");
 
} else {
client.print("<cred>Locked</cred>");

}
 client.println("<div class=\"w3-container\">");
 client.println("<br>");
 client.println("<a href=\"/Turn-On\"\"><button class=\"w3-btn w3-ripple w3-green\">Unlock the Door </button></a>");
 client.println("<a href=\"/Turn-Off\"\"><button class=\"w3-btn w3-ripple w3-red\">Lock the Door </button></a><br />");
 client.println("</div>");
client.println("</html>");
 
delay(1);
Serial.println("Client disonnected");
Serial.println("");
 
}
