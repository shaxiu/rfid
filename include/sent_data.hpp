#pragma once
#include <Arduino.h>

class SentData
{
private:
    void sent(const uint8_t type,const uint8_t command, const vector<uint8_t>& parameter);

    const uint8_t head = 0xBB;
    const uint8_t end = 0x7E;

    SemaphoreHandle_t xSemaphore = NULL;
public:
    SentData();
    ~SentData() = default;
};

