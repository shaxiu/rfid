#include "sent_data.hpp"

#include <vector>

using std::vector;

SentData::SentData() {
  Serial2.begin(115200);
  sentLock = xSemaphoreCreateMutex();
  receiveLock = xSemaphoreCreateMutex();
  timeOut = 20;
}

void SentData::sent(const Agreement &parameter) {
  if (xSemaphoreTake(sentLock, (TickType_t)50) == pdTRUE) {
    Serial2.print(head);
    Serial2.print(parameter.type);
    Serial2.print(parameter.command);
    Serial2.print(static_cast<uint8_t>(parameter.data.size() >> 8));
    Serial2.print(static_cast<uint8_t>(parameter.data.size()));
    uint8_t sum = 0;
    for (uint8_t ii : parameter.data) {
      Serial2.print(ii);
      sum += ii;
    }

    sum = check(parameter);
    Serial2.print(sum);
    Serial2.print(end);

    xSemaphoreGive(sentLock);
  }
}

Agreement SentData::receive() {
  bool complete = false;
  uint8_t flag = 0;
  uint32_t time = millis();
  Agreement data;
  if (xSemaphoreTake(receiveLock, (TickType_t)50) == pdTRUE) {
    while (1) {
      if (millis() - time > timeOut) break;
      if (complete || flag == 7) break;
      if (Serial2.available()) {
        int size = 0;
        int cnt = 0;
        uint8_t res = Serial2.read();
        switch (flag) {
          case 0:
            if (res == 0xBB) {
              flag++;
            } else {
              flag = 7;
            }
            break;
          case 1:
            data.type = res;
            flag++;
            break;
          case 2:
            data.command = res;
            flag++;
            break;
          case 3:
            size = res << 8;
            flag++;
            break;
          case 4:
            size += res;
            flag++;
            break;
          case 5:
            if (cnt < size - 1) {
              flag++;
            } else {
              cnt++;
              data.data.push_back(res);
            }
            break;
          case 6:
            if (check(data) == res) {
              complete = true;
            } else {
              flag = 7;
              complete = false;
            }
            break;
        }
      }
    }
    xSemaphoreGive(receiveLock);
    if (complete)
      return Agreement();
    else
      return data;
  }
}

int8_t SentData::check(const Agreement &parameter) const {
    uint8_t sum = 0;
    sum += parameter.type + parameter.command;
    sum += parameter.data.size() >> 8 + parameter.data.size();
    for(auto ii : parameter.data) {
        sum += ii;
    }
    return sum;
}

InfoData SentData::info() {
  vector<uint8_t> tmp = {0x01};
  sent(Agreement(0x00, 0x03, tmp));
  InfoData res;
  Agreement rec = receive();
  if(rec.command == 0x22 && rec.type == 0x02) {
       res.type = rec.data[0];
       for(int i = 1; i < rec.data.size();  i ++ ) {
           res.version += static_cast<char>(rec.data[i]);
       }
  }
  return res;
}
