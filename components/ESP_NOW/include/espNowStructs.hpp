#ifndef ESP_NOW_STRUCTS
#define ESP_NOW_STRUCTS

/* ESPNOW can work in both station and softap mode. It is configured in menuconfig. */
#if CONFIG_STATION_MODE
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#else
#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_AP
#endif

#define ESPNOW_QUEUE_SIZE           6

#define IS_BROADCAST_ADDR(addr) (memcmp(addr, example_broadcast_mac, ESP_NOW_ETH_ALEN) == 0)

enum class espNowEventID {
    espNowSendCallback,
    espNowReceiveCallback,
};

typedef struct {
    uint8_t macAddress[ESP_NOW_ETH_ALEN];
    esp_now_send_status_t status;
} espNowEventSendCallback;

typedef struct {
    uint8_t macAddress[ESP_NOW_ETH_ALEN];
    uint8_t *data;
    int dataLength;
} espNowEventReceiveCallback;

typedef union {
    espNowEventSendCallback sendCallback;
    espNowEventReceiveCallback receiveCallback;
} espNowEventInfo;

typedef struct {
    espNowEventID id;
    espNowEventInfo info;
} espNowEvent;

enum class espNowCommunicationType{
    ESPNOW_DATA_BROADCAST,
    ESPNOW_DATA_UNICAST,
    ESPNOW_DATA_MAX,
};

typedef struct {
    uint8_t type;                         //Broadcast or unicast ESPNOW data.
    uint8_t state;                        //Indicate that if has received broadcast ESPNOW data or not.
    uint16_t sequenceNumber;              //Sequence number of ESPNOW data.
    uint16_t crcValue;                    //CRC16 value of ESPNOW data.
    uint32_t magic;                       //Magic number which is used to determine which device to send unicast ESPNOW data.
    uint8_t payload[0];                   //Real payload of ESPNOW data.
} __attribute__((packed)) espNowData;


typedef struct {
    bool unicast;                         //Send unicast ESPNOW data.
    bool broadcast;                       //Send broadcast ESPNOW data.
    uint8_t state;                        //Indicate that if has received broadcast ESPNOW data or not.
    uint32_t magic;                       //Magic number which is used to determine which device to send unicast ESPNOW data.
    uint16_t count;                       //Total count of unicast ESPNOW data to be sent.
    uint16_t delay;                       //Delay between sending two ESPNOW data, unit: ms.
    int len;                              //Length of ESPNOW data to be sent, unit: byte.
    uint8_t *buffer;                      //Buffer pointing to ESPNOW data.
    uint8_t dest_mac[ESP_NOW_ETH_ALEN];   //MAC address of destination device.
} espNowParams;

#endif