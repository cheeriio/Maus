#include <irq.h>
#include "messages.h"
#include "prio.h"
#include "utils.h"

#define _MESSAGES_Q_LEN 10

typedef struct {
    char* message;
    int length;
} Message;

static Message mes_queue[_MESSAGES_Q_LEN];
static int first = 0;
static int q_size = 0;

static void start_dma_transmission(char* message, int length) {
    DMA1_Stream6->M0AR = (uint32_t)message;
    DMA1_Stream6->NDTR = length;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void send_message(char* message) {
    // irq_level_t irq_level = IRQprotect(LOW_IRQ_PRIO);
    if((DMA1_Stream6->CR & DMA_SxCR_EN) == 0 &&
       (DMA1->HISR & DMA_HISR_TCIF6) == 0) {
        // DMA available. Start transmission.
        // IRQunprotect(irq_level);
        start_dma_transmission(message, string_len(message));
    } else {
        // DMA occupied. Add message to queue.
        int index = (first + q_size) % _MESSAGES_Q_LEN;
        q_size++;
        mes_queue[index].message = message;
        mes_queue[index].length = string_len(message);
        // IRQunprotect(irq_level);
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
