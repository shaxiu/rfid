#include <Arduino.h>

#include "sent_data.hpp"

SentData data;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  InfoData info = data.info();
  Serial.print("type:");
  Serial.println(info.type);
  Serial.print("version:");
  Serial.println(info.version.c_str());
  delay(1000);
}
