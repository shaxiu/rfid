#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  // put your setup code here, to run once:
}

void info();
void loop()
{
  info();
  
  delay(1000);
}

void info()
{
  char s1[8] = {0XBB, 0, 0X03, 0, 0X01, 0, 0X04, 0X7E};
  for (auto i : s1)
  {
    Serial2.print(i);
  }
  delay(1);
  while (Serial2.available() > 0)
  {
    char tmp = Serial2.read();
    Serial.print(tmp);
  }
}
