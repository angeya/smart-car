int leftSpeed = 5;
int rightSpeed = 6;

int left1 = 3;
int left2 = 4;

int right1 = 7;
int right2 = 2;

String prefix = "ESP-";
String suffix = "-8266";

int stopThreshold = 70;

// 转圈速度
int spinSpeed = 120;
// 之前带符号的速度，正负代表方向，用于转弯后恢复速度
int lastSignSpeed = 0;


// 左向前
void leftForwords() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
}
// 左后退
void leftBackwards() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
}

// 右向前
void rightForwords() {
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}
// 右后退
void rightBackwards() {
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}

// 向前走
void forwards(int speed) {
  setLeftSpeed(speed);
  leftForwords();
  setRightSpeed(speed);
  rightForwords();
}

// 向后走
void backwards(int speed) {
  setLeftSpeed(speed);
  leftBackwards();
  setRightSpeed(speed);
  rightBackwards();
}

// 设置左边速度
void setLeftSpeed(int speed) {
  analogWrite(leftSpeed, speed);
}

// 设置右边边速度
void setRightSpeed(int speed) {
  analogWrite(rightSpeed, speed);
}

// 停止速度和方向
void allStop() {
  analogWrite(leftSpeed, 0);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);

  analogWrite(rightSpeed, 0);
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
}

// 改变速度
void changeSpeed(int signSpeed) {
  lastSignSpeed = signSpeed;
  if (-stopThreshold < signSpeed && signSpeed < stopThreshold) {
    allStop();
    return;
  }
  if (signSpeed >= stopThreshold) {
    forwards(signSpeed);
  } else {
    // 改变方向，速度使用正值
    backwards(-signSpeed);
  }
}

// 左转圈
void leftSpin() {
  setLeftSpeed(spinSpeed);
  setRightSpeed(spinSpeed);
  leftBackwards();
  rightForwords();
}
// 右转圈
void rightSpin() {
  setLeftSpeed(spinSpeed);
  setRightSpeed(spinSpeed);
  rightBackwards();
  leftForwords();
}


void listenMsgFromEsp8266() {
  // 如果有数据可读
  if (Serial.available() <= 0) {
    return;
  }
  String receivedData = Serial.readStringUntil('\n');  // 读取串行数据
  // 去掉换行符
  receivedData.remove(receivedData.length() - 1, 1);
  // 检查字符串是否以 "ESP" 开头并以 "8266" 结尾
  if (receivedData.startsWith(prefix) && receivedData.endsWith(suffix)) {
    String msg = receivedData.substring(prefix.length(), receivedData.length() - suffix.length());
    Serial.println("接收到8266数据: " + msg);
    handleMsg(msg);
  }
}


// 处理esp8266消息
void handleMsg(String msg) {
  String speed = "speed";
  String turn = "turn";
  String leftSpinStart = "leftSpinStart";
  String rightSpinStart = "rightSpinStart";
  String spinEnd = "spinEnd";

  if (msg.startsWith(speed)) {
    Serial.println("速度是: " + msg);
    // 去掉speed标识
    msg.remove(0, speed.length());
    int signSpeed = msg.toInt();
    changeSpeed(signSpeed);
  }

  else if (msg.startsWith(leftSpinStart)) {
    leftSpin();
  }
  else if (msg.startsWith(rightSpinStart)) {
    rightSpin();
  }
  else if (msg.startsWith(spinEnd)) {
    changeSpeed(lastSignSpeed);
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
}
