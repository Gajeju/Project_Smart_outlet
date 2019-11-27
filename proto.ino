#include <WiFi.h>
#include <IoTStarterKit_WiFi.h>
#include <Grove_LED_Bar.h>

Grove_LED_Bar bar1(5, 6, 0);  // Clock pin, Data pin, Orientation
Grove_LED_Bar bar2(7, 8, 0);
Grove_LED_Bar bar3(9, 10, 0);

/*
  Arduino
*/
#define  Relay1  2
#define  Relay2  3
#define  Relay3  4

#define TAG_ID    "control1"
#define TAG_ID_2  "current1"

#define TAG_ID_3  "control2"
#define TAG_ID_4  "current2"

#define TAG_ID_5  "control2"
#define TAG_ID_6  "current3"

/*
  WIFI Shield
*/
#define WIFI_SSID   ""                       //와이파이 SSID
#define WIFI_PASS   ""                                 //와이파이 비밀번호


/*
  IoTMakers
*/
IoTMakers g_im;

#define deviceID    ""            //디바이스 아이디
#define authnRqtNo  ""            //디바이스 패스워드
#define extrSysID   ""   //게이트웨이 연결 ID


//IoT Makers 접속을 초기화 하는 함수
void init_iotmakers()
{
  while (1)
  {
    // AP 접속
    Serial.print(F("Connect to AP..."));
    while (g_im.begin(WIFI_SSID, WIFI_PASS) < 0) {
      Serial.println(F("retrying."));
      delay(1000);
    }
    Serial.println(F("success"));

    // 인자값으로 받은 정보로 KT IoT Makers 접속
    g_im.init(deviceID, authnRqtNo, extrSysID);

    // String type 제어 핸들러
    g_im.set_strdata_handler(mycb_strdata_handler);

    // IoTMakers 플랫폼 연결
    Serial.print(F("Connect to platform... "));
    while ( g_im.connect() < 0 ) {
      Serial.println(F("retrying."));
      delay(1000);
    }
    Serial.println(F("success"));

    //IoTMakers 플랫폼 인증
    Serial.print(F("Auth... "));
    if (g_im.auth_device() >= 0) {
      Serial.println(F("success "));
      return;
    }
    Serial.println(F("fail"));
  }
}

void setup()
{
  Serial.begin(9600);
  analogReference(INTERNAL);

  pinMode(Relay1, OUTPUT);
  digitalWrite(Relay1, HIGH);

  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay2, HIGH);

  pinMode(Relay3, OUTPUT);
  digitalWrite(Relay3, HIGH);

  bar1.begin();
  bar2.begin();
  bar3.begin();

  init_iotmakers();
}


void loop()
{
  static unsigned long tick = millis();

  // 만약 플랫폼 접속이 종료되었을 경우 다시 접속
  if (g_im.isServerDisconnected() == 1)
  {
    init_iotmakers();
  }

  // 센서 값을 읽어오는 시간 설정
  if ( ( millis() - tick) > 1000 )
  {
    send_current1();
    send_current2();
    send_current3();
    tick = millis();
  }

  // IoTMakers 플랫폼 수신처리 및 keepalive 송신
  g_im.loop();
}


// 플랫폼에 보내는 내용
void mycb_strdata_handler(char *tagid, char *strval)
{
  if ( strcmp(TAG_ID, tagid) == 0 && strcmp(strval, "on") == 0
       || strcmp(TAG_ID, tagid) == 0 && strcmp(strval, "ON") == 0)
  {
    digitalWrite(Relay1, HIGH);
    Serial.println(F("Relay1 ON"));
  }

  else if ( strcmp(TAG_ID, tagid) == 0 && strcmp(strval, "off") == 0
            || strcmp(TAG_ID, tagid) == 0 && strcmp(strval, "OFF") == 0)
  {
    digitalWrite(Relay1, LOW);
    Serial.println(F("Relay1 OFF"));
  }

  if ( strcmp(TAG_ID_3, tagid) == 0 && strcmp(strval, "on") == 0
       || strcmp(TAG_ID_3, tagid) == 0 && strcmp(strval, "ON") == 0)
  {
    digitalWrite(Relay1, HIGH);
    Serial.println(F("Relay1 ON"));
  }

  else if ( strcmp(TAG_ID_3, tagid) == 0 && strcmp(strval, "off") == 0
            || strcmp(TAG_ID_3, tagid) == 0 && strcmp(strval, "OFF") == 0)
  {
    digitalWrite(Relay1, LOW);
    Serial.println(F("Relay1 OFF"));
  }

  if ( strcmp(TAG_ID_5, tagid) == 0 && strcmp(strval, "on") == 0
       || strcmp(TAG_ID_5, tagid) == 0 && strcmp(strval, "ON") == 0)
  {
    digitalWrite(Relay1, HIGH);
    Serial.println(F("Relay1 ON"));
  }

  else if ( strcmp(TAG_ID_5, tagid) == 0 && strcmp(strval, "off") == 0
            || strcmp(TAG_ID_5, tagid) == 0 && strcmp(strval, "OFF") == 0)
  {
    digitalWrite(Relay1, LOW);
    Serial.println(F("Relay1 OFF"));
  }
}

int send_current1()
{
  // 센서 값을 읽어온다.
  float currentValue = get_current(analogRead(A0));
  setLED1(currentValue);

  Serial.print(F("current1 : "));
  Serial.println(currentValue);

  if ( g_im.send_numdata(TAG_ID_2, currentValue) < 0 ) {
    Serial.println(F("fail"));
    return -1;
  }
  return 0;
}

int send_current2()
{
  // 센서 값을 읽어온다.
  float currentValue = get_current(analogRead(A1));
  setLED2(currentValue);

  Serial.print(F("current2 : "));
  Serial.println(currentValue);

  if ( g_im.send_numdata(TAG_ID_4, currentValue) < 0 ) {
    Serial.println(F("fail"));
    return -1;
  }
  return 0;
}

int send_current3()
{
  // 센서 값을 읽어온다.
  float currentValue = get_current(analogRead(A2));
  setLED3(currentValue);

  Serial.print(F("current3 : "));
  Serial.println(currentValue);

  if ( g_im.send_numdata(TAG_ID_6, currentValue) < 0 ) {
    Serial.println(F("fail"));
    return -1;
  }
  return 0;
}

float get_current(int sensor)
{
  float voltageSensor;
  float current = 0;
  float Sum = 0;
  long time = millis();
  int N = 0;
  while (millis() - time < 500)
    //0.5초에 한 번씩 측정(약 60Hz, 30 사이클)
  {
    voltageSensor = sensor * (1.1 / 1023.0);////센서 전압
    current = voltageSensor * 20.0; //current=voltageSensor*(20A/1V)
    Sum = Sum + sq(current);
    N = N + 1;
    delay(1);
  }
  Sum = Sum * 2;//음의 반주기의 제곱을 보상합니다
  current = sqrt((Sum) / N); //RMS 방정식
  return (current);
}

void setLED1(float current)
{
  if (current == 0)         bar1.setLevel(0);
  else if (current > 20)    return;
  else                      bar1.setLevel((current / 2) + 1);
}

void setLED2(float current)
{
  if (current == 0)         bar2.setLevel(0);
  else if (current > 20)    return;
  else                      bar2.setLevel((current / 2) + 1);
}

void setLED3(float current)
{
  if (current == 0)         bar3.setLevel(0);
  else if (current > 20)    return;
  else                      bar3.setLevel((current / 2) + 1);
}
