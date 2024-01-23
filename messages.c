#include <irq.h>
#include "messages.h"

#include <string.h>

#define _MESSAGES_Q_LEN 100
#define _MAX_MES_LEN 200

typedef struct {
    char message[_MAX_MES_LEN];
    int length;
} Message;

static Message mes_queue[_MESSAGES_Q_LEN];
static int first = 0;
static int q_size = 0;

static void start_dma_transmission(char* message, unsigned int length) {
    DMA1_Stream6->M0AR = (uint32_t)message;
    DMA1_Stream6->NDTR = length;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void send_message(char* message, unsigned int length) {
    int index = (first + q_size) % _MESSAGES_Q_LEN;
    q_size++;
    mes_queue[index].length = length;
    memcpy(mes_queue[index].message, message, length);
    if((DMA1_Stream6->CR & DMA_SxCR_EN) == 0 &&
       (DMA1->HISR & DMA_HISR_TCIF6) == 0) {
        // DMA available. Start transmission.
        first = (first + 1) % _MESSAGES_Q_LEN;
        q_size--;
        start_dma_transmission(mes_queue[index].message, length);
    }
}

void DMA1_Stream6_IRQHandler() {
    uint32_t isr = DMA1->HISR;
    if (isr & DMA_HISR_TCIF6) {
        DMA1->HIFCR = DMA_HIFCR_CTCIF6;
        if(q_size > 0) {
            // Send awaiting message.
            Message m = mes_queue[first];
            first = (first + 1) % _MESSAGES_Q_LEN;
            q_size--;
            start_dma_transmission(m.message, m.length);
        }
    }
}
