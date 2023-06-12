//
// Created by zr on 23-4-20.
//

#ifndef TINYMQTT_MQTT_SESSION_H
#define TINYMQTT_MQTT_SESSION_H
#include "net/mqtt_tcp_conn.h"
#include "mqtt/mqtt_types.h"

typedef struct sending_packet
{
    struct sending_packet* next;
    int64_t send_time;
    uint16_t packet_id;
    tmq_any_packet_t* packet;
} sending_packet;

typedef enum session_state_e{OPEN, CLOSED} session_state_e;
typedef void(*new_message_cb)(void* upstream, char* topic, tmq_message* message, uint8_t retain);

typedef struct tmq_session_s
{
    tmq_str_t client_id;
    tmq_tcp_conn_t* conn;
    session_state_e state;
    uint8_t clean_session;
    tmq_map(char*, uint8_t) subscriptions;
    void* upstream;
    new_message_cb on_new_message;
    uint16_t keep_alive;
    int64_t last_pkt_ts;
    uint16_t next_packet_id;
    uint8_t max_inflight;
    uint8_t inflight_packets;
    sending_packet* sending_queue_head, *sending_queue_tail;
    sending_packet* pending_pointer;
    tmq_timerid_t resend_timer;
} tmq_session_t;

tmq_session_t* tmq_session_new(void* upstream, new_message_cb on_new_message,
                               tmq_tcp_conn_t* conn, tmq_str_t client_id,
                               uint8_t clean_session, uint16_t keep_alive, uint8_t max_inflight);
void tmq_session_send_packet(tmq_session_t* session, tmq_any_packet_t* pkt);
void tmq_session_publish(tmq_session_t* session, char* topic, char* payload, uint8_t qos, uint8_t retain);

#endif //TINYMQTT_MQTT_SESSION_H
