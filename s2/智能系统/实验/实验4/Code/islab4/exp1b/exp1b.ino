/*
   将模块与UNO控制器正确连接，切勿接错；
   程序效果：首先使得前方无障碍物，此时避障传感器为输出接口为高电平，将“NO”发送给上位机；
            将避障模块的红外探头对准障碍物，距离为3 – 30cm厘米以内，将“YES”发送给上位机；
            然后接受上位机信号，若为0，则设定LED 为输出接口为低电平灯灭，否则高电平灯亮
*/

int Led = 13;       //定义LED 接口
int buttonpin = 3;  //定义避障传感器接口
int val;            //定义避障传感器接口的数字变量val
int prv = -1;       //定义前一次传感器收到的val
char chr;           //定义上位机的输入字符信号
int LedRED = 12;       //定义LED 接口
int Led2 = 10;
int Led2RED = 9;
int tmp_chr = 1;
void setup() {
  pinMode(Led, OUTPUT);       //定义LED 为输出接口
  pinMode(LedRED, OUTPUT);       //定义LED 为输出接口
  pinMode(Led2, OUTPUT);       //定义LED 为输出接口
  pinMode(Led2RED, OUTPUT);       //定义LED 为输出接口
  pinMode(buttonpin, INPUT);  //定义避障传感器为输出接口
  Serial.begin(9600);         //连接上位机，波特率为9600
  digitalWrite(Led, LOW);
  digitalWrite(LedRED, HIGH);
  digitalWrite(Led2, HIGH);
  digitalWrite(Led2RED, LOW);
}

void loop() {

  val = digitalRead(buttonpin);  //将数字接口3的值读取赋给val
  if (val != prv) { // 只有当val发生变化的时候才向上位机发数据
    if (val == LOW) {  //当避障传感器检测低电平时，有障碍物，传输YES
      if (tmp_chr =='1') {
        Serial.print("NSYES\n");
      }
      else if (tmp_chr == '0') {
        Serial.print("WEYES\n");
      }
    }
    else {
      Serial.print("NO\n");
    }
  }
  prv = val;

  // 接收上位机信号控制小灯开关
  chr = Serial.read();
  if (chr == '1') {  //当避障传感器检测低电平时，LED 灭
    digitalWrite(Led, HIGH);
    digitalWrite(LedRED, LOW);
    digitalWrite(Led2, LOW);
    digitalWrite(Led2RED, HIGH);
    tmp_chr = chr;

  }  else if (chr == '0') {
    digitalWrite(Led, LOW);
    digitalWrite(LedRED, HIGH);
    digitalWrite(Led2, HIGH);
    digitalWrite(Led2RED, LOW);
    tmp_chr = chr;
  }

}

