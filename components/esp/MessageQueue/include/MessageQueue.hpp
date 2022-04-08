#pragma once
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

namespace tamagotchi {
namespace MessageQueue {

constexpr int WAIT_TIME = 10;

template <typename T>
class MessageQueue {
 public:
  MessageQueue(int queueSize);
  bool putQueue(T event);
  void putQueueFromISR(T event);
  void clearQueue();
  T getQueue(int ms);

 private:
  SemaphoreHandle_t mutex_;
  static constexpr char TAG_[] = "MessageQueue";
  xQueueHandle messageQueue_;
};

template <typename T>
MessageQueue<T>::MessageQueue(int queueSize) {
  messageQueue_ = xQueueCreate(queueSize, sizeof(T));
  mutex_ = xSemaphoreCreateBinary();
}

template <typename T>
bool MessageQueue<T>::putQueue(T event) {
  if (uxQueueSpacesAvailable(messageQueue_) == 0) {
    T dummy;
    xQueueReceive(messageQueue_, &(dummy), (TickType_t)WAIT_TIME);
  }
  xQueueSendToBack(messageQueue_, (void*)&event, (TickType_t)WAIT_TIME);
  return xSemaphoreGive(mutex_);
}

template <typename T>
void MessageQueue<T>::putQueueFromISR(T event) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (xQueueIsQueueFullFromISR(messageQueue_)) {
    T dummy;
    xQueueReceiveFromISR(messageQueue_, (void*)&dummy,
                         &xHigherPriorityTaskWoken);
  }
  xQueueSendToBackFromISR(messageQueue_, &event, &xHigherPriorityTaskWoken);
  xSemaphoreGiveFromISR(mutex_, NULL);
}

template <typename T>
T MessageQueue<T>::getQueue(int ms) {
  T message;
  if (xSemaphoreTake(mutex_, ms) == pdPASS) {
    xQueueReceive(messageQueue_, &(message), (TickType_t)ms);
  }
  return message;
}

template <typename T>
void MessageQueue<T>::clearQueue() {
  xQueueReset(messageQueue_);
}

}  // namespace MessageQueue
}  // namespace tamagotchi