#ifndef CONTROLE_SEMAFORO_H
#define CONTROLE_SEMAFORO_H

void controle_semaforo_init(void);
/* Deve ser chamado a cada 100 ms */
void tarefa_semaforo_100ms(void);
/* Valor (0-99) que deve ser exibido no display */
unsigned char controle_semaforo_display(void);

#endif
