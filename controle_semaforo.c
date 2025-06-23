#include <avr/io.h>
#include <util/delay.h>
#include "bits.h"
#include "controle_semaforo.h"

static unsigned long t_ms = 0;
static unsigned int tempo_fase = 0;
static unsigned char fase = 0;

void controle_semaforo_init(void)
{
    DDRC = 0b00111110;
    PORTC = 0b00000001; // Pull-up no botão (PC0)
    t_ms = 0;
    tempo_fase = 0;
    fase = 0; // 0: Verde, 1: Amarelo, 2: Vermelho
}

void tarefa_semaforo_100ms(void)
{
    static uint8_t pisca = 0;
    static uint8_t cont_pisca = 0;
    static uint8_t iniciou_pisca = 0;

    t_ms += 100;
    tempo_fase += 100;

    // Leitura do botão
    uint8_t botao = PINC & (1 << PC0);

    // Fase VERDE
    if (fase == 0) 
    {
        SETBIT(PORTC, 3); // Verde veículo
        SETBIT(PORTC, 2); // Vermelho pedestre
        CLRBIT(PORTC, 1); CLRBIT(PORTC, 4); CLRBIT(PORTC, 5);

        if (!botao && tempo_fase < 32000) tempo_fase = 32000;
        if (tempo_fase >= 42000) 
        {
            fase = 1;
            tempo_fase = 0;
    
        }
    }

    // Fase AMARELO
    else if (fase == 1) 
    {
        SETBIT(PORTC, 4); // Amarelo veículo
        SETBIT(PORTC, 2); // Vermelho pedestre
        CLRBIT(PORTC, 3); CLRBIT(PORTC, 1); CLRBIT(PORTC, 5);

        if (tempo_fase >= 3000) 
        {
            fase = 2;
            tempo_fase = 0;
        }
    }
    // Fase VERMELHO
    else if (fase == 2) 
    {
        SETBIT(PORTC, 5); // Vermelho veículo
        CLRBIT(PORTC, 3); CLRBIT(PORTC, 4);

        if (tempo_fase < 12000) 
        {
            SETBIT(PORTC, 1); // Verde pedestre
            CLRBIT(PORTC, 2); // Vermelho pedestre
        } 
        else if (tempo_fase < 15000) 
        {
            // Iniciar pisca vermelho pedestre (3 vezes = 6 ciclos de 500ms)
            CLRBIT(PORTC, 1); // Verde pedestre OFF

            if (!iniciou_pisca) {
                iniciou_pisca = 1;
                cont_pisca = 0;
                pisca = 1;
            }

            if (++cont_pisca % 5 == 0) { // a cada 500ms
                pisca = !pisca;
                if (pisca) SETBIT(PORTC, 2);
                else       CLRBIT(PORTC, 2);
            }
        } 
        else 
        {
            fase = 0;
            tempo_fase = 0;
        }
    }
}

unsigned char controle_semaforo_display(void)
{
    if (fase == 2 && tempo_fase < 15000) return (15 - tempo_fase / 1000);
    return 0;
}
