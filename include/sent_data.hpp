#pragma once
#include <Arduino.h>

#include <string>

struct InfoData {
  uint8_t type;
  std::string version;
};

struct Agreement {
  uint8_t type;
  uint8_t command;
  vector<uint8_t> data;

  Agreement() : type(0), command(0), data() {}
  Agreement(const uint8_t type, const uint8_t command,
            const vector<uint8_t> &data)
      : type(type), command(command), data(data) {}
};

class SentData {
 private:
  void sent(const Agreement &parameter);
  Agreement receive();

  int8_t check(const Agreement &parameter) const;

  const uint8_t head = 0xBB;
  const uint8_t end = 0x7E;

  int timeOut;

  SemaphoreHandle_t sentLock = NULL;
  SemaphoreHandle_t receiveLock = NULL;

 public:
  SentData();
  ~SentData() = default;

  InfoData info();
};
