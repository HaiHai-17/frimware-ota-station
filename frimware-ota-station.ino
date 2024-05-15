#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
const char* ssid = "Chu Quang";
const char* password = "1234567890";

const char* host = "ESP8266";
const char* updatePath = "/update";
const char* updateUsername = "admin";
const char* updatePassword = "admin";

ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdater;
//-----------------------------------------//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>ESP826</title> 
       <style> 
          body{
            text-align: center;
          }
       </style>
       <meta name="viewport" content="width=device-width,user-scalable=0" charset="UTF-8">
   </head>
   <body> 
      <div>
        <button onclick="window.location.href='/update'">UPLOAD FIRMWARE</button><br><br>
      </div>
      <script>
      </script>
   </body> 
  </html>
)=====";

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Đang khởi động chương trình...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("Kết nối Wi-Fi thất bại, đang thử lại...");
  }
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);

  httpUpdater.setup(&webServer, updatePath, updateUsername, updatePassword);
  webServer.on("/",[]{
    String s = MainPage;
    webServer.send(200,"text/html",s);
  });
  webServer.begin();
  Serial.println("Khởi động Webserer, thành công.");

  //=========Chương trình Chính=====//
  pinMode(D4,OUTPUT);
  //============End=================//
}

void loop(void){
  MDNS.update();
  webServer.handleClient();
  //====Chương trình Chính==========//
  digitalWrite(D4,!digitalRead(D4));
  delay(500);
  //=========End====================//
}
