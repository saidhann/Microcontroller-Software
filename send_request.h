#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"

float currentTemperature;
float currentLight;
int id = 2;

#define SERVER_IP "192.168.0.174"  // Replace with your server IP
#define SERVER_PORT 80            // Standard HTTP port

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_client_err(void *arg, err_t err);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        tcp_close(tpcb);
        printf("Connection failed\n");
        return err;
    }

    char post_message[30];
    char post_request[512];

    snprintf(post_message,sizeof(post_message),"i %d %f %f",id,currentTemperature,currentLight);

    snprintf(post_request, sizeof(post_request),
        "POST /sendtemperature.cgi HTTP/1.1\r\n"
        "Host: 192.168.0.174\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        strlen(post_message), post_message);

    tcp_write(tpcb, post_request, strlen(post_request), TCP_WRITE_FLAG_COPY);

    return ERR_OK;
}

static void tcp_client_err(void *arg, err_t err) {
    printf("TCP error: %d\n", err);
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    printf("Data sent\n");
    return ERR_OK;
}

void send_post_request(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Failed to create PCB\n");
        return;
    }

    ip4_addr_t server_ip;
    ip4addr_aton(SERVER_IP, &server_ip);

    tcp_arg(pcb, NULL);
    tcp_err(pcb, tcp_client_err);
    tcp_sent(pcb, tcp_client_sent);

    err_t err = tcp_connect(pcb, &server_ip, SERVER_PORT, tcp_client_connected);
    if (err != ERR_OK) {
        printf("TCP connect failed: %d\n", err);
        tcp_close(pcb);
    }
}
