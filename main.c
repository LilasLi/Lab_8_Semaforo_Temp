#define F_CPU 16000000UL
#include <util/delay.h>
#include "disp7seg.h"
#include "controle_semaforo.h"

int main(void)
{
    disp7seg_init();
    controle_semaforo_init();

    /* 20 × 50 ms = 1 segundo visual estável no SimulIDE */
    unsigned char cont_50ms = 0;

    while(1)
    {
        disp7seg( controle_semaforo_display() );
        _delay_ms(5);   // Mais lento, mais confiável no SimulIDE

        if (++cont_50ms >= 20)  // 1 "segundo visual"
        {
            cont_50ms = 0;
            tarefa_semaforo_100ms();  // Chama a lógica do semáforo
        }
    }
}

