#include "sent_data.hpp"

#include <vector>

using std::vector;

SentData::SentData() {
  Serial2.begin(115200);
  xSemaphore = xSemaphoreCreateMutex();
}

void SentData::sent(const uint8_t type, const uint8_t command,
                    const vector<uint8_t>& parameter) {
  if (xSemaphoreTake(xSemaphore, (TickType_t)50) == pdTRUE) {
    Serial2.print(head);
    Serial2.print(type);
    Serial2.print(command);
    Serial2.print(parameter.size());
    uint8_t sum = 0;
    for (uint8_t ii : parameter) {
      Serial2.print(ii);
      sum += ii;
    }

    sum = head + type + command + parameter.size();
    Serial2.print(sum);
    Serial2.print(end);

    xSemaphoreGive(xSemaphore);
  }
}