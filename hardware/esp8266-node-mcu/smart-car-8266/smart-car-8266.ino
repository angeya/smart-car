#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = "ThunderGod";       // WiFi名称
const char* password = "5201314asdk";  // WiFi密码
const char* websockets_server_host = "192.168.31.123";  // WebSocket 服务器地址
const uint16_t websockets_server_port = 8080; // WebSocket 服务端口
String prefix = "ESP-";
String suffix = "-8266";

using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(9600);
    // 连接wifi
    WiFi.begin(ssid, password);

    // 等待连接wifi 20s
    for(int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(500);
    }

    // 判断WiFi是否已经连接上
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // 尝试连接websocket服务
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/ws");
    if(connected) {
        Serial.println("Connecetd!");
        client.send("Hello Server");
    } else {
        Serial.println("Not Connected!");
    }
    
    // 接收到消息的回调
    client.onMessage([&](WebsocketsMessage message) {
      String msg = message.data();
      Serial.println("Got ws msg: " + msg);
      Serial.println(prefix + msg + suffix);
    });
}

void loop() {
    // 让客户端监听消息的接收
    if(client.available()) {
        client.poll();
    }
}