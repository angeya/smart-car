

int leftSpeed = 5;
int rightSpeed = 6;

int left1 = 3;
int left2 = 4;

int right1 = 7;
int right2 = 2;

String prefix = "ESP-";
String suffix = "-8266";


// 向前走
void forwards(int speed) {
  analogWrite(leftSpeed, speed);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);

  analogWrite(rightSpeed, speed);
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}

// 向后走
void backwards(int speed) {
  analogWrite(leftSpeed, speed);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);

  analogWrite(rightSpeed, speed);
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}

void speedStop() {
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

void allStop() {
  analogWrite(leftSpeed, 0);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);

  analogWrite(rightSpeed, 0);
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
}

void changeSpeed(int speed) {
  if (-80 < speed && speed < 80) {
    allStop();
    return;
  }
  if (speed >= 80) {
    forwards(speed);
  } else {
    backwards(speed);
  }

}



void listenMsgFromEsp8266() {
  // 如果有数据可读
  if (Serial.available() <= 0) {
   return;
  }
  String receivedData = Serial.readStringUntil('\n'); // 读取串行数据
  // 去掉换行符
  receivedData.remove(receivedData.length()-1, 1);
  // Serial.println(receivedData.startsWith(prefix));
  // Serial.println(receivedData.endsWith(suffix));
  // 检查字符串是否以 "ESP" 开头并以 "8266" 结尾
  if (receivedData.startsWith(prefix) && receivedData.endsWith(suffix)) {
    String msg = receivedData.substring(prefix.length(), receivedData.length() - suffix.length());
    Serial.println("接收到8266数据: " + msg);
    handleMsg(msg);
  }
}

void handleMsg(String msg) {
  String speed = "speed";
  String turn = "turn";
  if(msg.startsWith(speed)) {
    Serial.println("速度是: " + msg);
    // 去掉speed标识
    msg.remove(0, speed.length());
    int speed = msg.toInt();
    changeSpeed(speed);
  }
  
  else if(msg.startsWith(turn)) {

  }
}


void setup() {
  // 初始化串口通讯
  Serial.begin(9600);

  // 设置L198N端口
  pinMode(leftSpeed, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);

  // 一开始是停止的
  allStop();
}

void loop() {
  listenMsgFromEsp8266();
  // put your main code here, to run repeatedly:
  // forwards(120);
  // delay(5000);
  // analogWrite(rightSpeed, 0);
  // delay(3000);
  // backwards(80);
  // delay(3000);
}







