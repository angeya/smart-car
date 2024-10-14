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
// 上次尝试连接的时间
unsigned long lastAttemptTime = 0;
// 重连间隔时间，5秒
const long reconnectInterval = 5000;

void connectWebSocket() {
    Serial.println("Connecting to WebSocket server...");
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/ws");
    if (connected) {
        Serial.println("Connected to WebSocket server!");
        client.send("Hello Server");
    } else {
        Serial.println("WebSocket connection failed!");
    }
}

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
    connectWebSocket();
    
    // 接收到消息的回调
    client.onMessage([&](WebsocketsMessage message) {
      String msg = message.data();
      Serial.println("Got ws msg: " + msg);
      Serial.println(prefix + msg + suffix);
    });
}

void loop() {
    // 检查WebSocket连接状态
    if (WiFi.status() == WL_CONNECTED) {
        if (client.available()) {
            // WebSocket 连接可用时处理消息
            client.poll();
        } else if (millis() - lastAttemptTime > reconnectInterval) {
            // 如果WebSocket连接不可用，并且超过了重连间隔时间，则尝试重连
            Serial.println("WebSocket connection lost, trying to reconnect...");
            connectWebSocket();
            lastAttemptTime = millis();
        }
    } else {
        Serial.println("WiFi disconnected, trying to reconnect to WiFi...");
        // 如果WiFi断开，可以在这里重新连接WiFi
        WiFi.begin(ssid, password);
        delay(1000); // 给WiFi连接一点时间
    }
}