void setup() {
  
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  
  float Irms = get_current(); //전류 (A)
  float P = Irms * 220.0; // P=IV (Watts)

  Serial.print("Irms: ");
  Serial.print(Irms, 3);
  Serial.print(" A, Power: ");
  Serial.print(P, 3);  
  Serial.println(" W");
  //delay(100);     
}

float get_current()
{
  float voltageSensor;
  float current = 0;
  float Sum = 0;
  long time = millis();
  int N = 0;
  while(millis() - time < 500)
  //0.5초에 한 번씩 측정(약 60Hz, 30 사이클)
  { 
    voltageSensor = analogRead(A0) * (1.1 / 1023.0);////센서 전압
    current = voltageSensor * 20.0; //current=voltageSensor*(20A/1V)
    Sum = Sum + sq(current);
    N = N + 1;
    delay(1);
  }
  Sum = Sum * 2;//음의 반주기의 제곱을 보상합니다
  current = sqrt((Sum) / N); //RMS 방정식
  return(current);
}
