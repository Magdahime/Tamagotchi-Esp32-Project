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
  T getQueue(int ms = portMAX_DELAY);
  bool getQueue(T& elem, int ms = portMAX_DELAY);
  bool empty() { return uxQueueMessagesWaiting(messageQueue_) == 0; }
  xQueueHandle& messageQueue() { return messageQueue_; }
  int32_t elementsCount() { return uxQueueMessagesWaiting(messageQueue_); }
  int32_t size() { return size_; }

 private:
  int32_t size_;
  SemaphoreHandle_t mutex_;
  static constexpr char TAG_[] = "MessageQueue";
  xQueueHandle messageQueue_;
};

template <typename T>
MessageQueue<T>::MessageQueue(int queueSize) {
  size_ = queueSize;
  messageQueue_ = xQueueCreate(queueSize, sizeof(T));
  mutex_ = xSemaphoreCreateBinary();
}

template <typename T>
bool MessageQueue<T>::putQueue(T event) {
  if (uxQueueSpacesAvailable(messageQueue_) == 0) {
    T dummy;
    xQueueReceive(messageQueue_, &(dummy), (TickType_t)WAIT_TIME);
  }
  auto result =
      xQueueSendToBack(messageQueue_, (void*)&event, (TickType_t)WAIT_TIME);
  xSemaphoreGive(mutex_);
  return result;
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
  if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(ms)) == pdPASS) {
    xQueueReceive(messageQueue_, &(message), (TickType_t)ms);
    xSemaphoreGive(mutex_);
  }
  return message;
}

template <typename T>
bool MessageQueue<T>::getQueue(T& elem, int ms) {
  if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(ms)) == pdPASS) {
    auto result = xQueueReceive(messageQueue_, &(elem), (TickType_t)ms);
    xSemaphoreGive(mutex_);
    return result;
  }
  return false;
}

template <typename T>
void MessageQueue<T>::clearQueue() {
  xQueueReset(messageQueue_);
}

}  // namespace MessageQueue
}  // namespace tamagotchi