// Bibliotecas padrão e de hardware
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define PIN_LED_R 11 // Pino do LED vermelho
#define PIN_LED_Y 12 // Pino do LED amarelo
#define PIN_LED_G 13 // Pino do LED verde

// Variável global para controlar a lógica de mudança de estado dos LEDs
static volatile uint8_t acender_led = 1;

// Prototipação da rotina de temporização
bool repeating_timer_callback(struct repeating_timer *t);


// Rotina principal
int main()
{
    // Inicializa a comunicação serial, permitindo o uso de funções como printf
    stdio_init_all();

    // Inicializar o pino GPIO11 e define a direção como saída
    gpio_init(PIN_LED_R);
    gpio_set_dir(PIN_LED_R, GPIO_OUT);

    // Inicializar o pino GPIO12 e define a direção como saída
    gpio_init(PIN_LED_Y);
    gpio_set_dir(PIN_LED_Y, GPIO_OUT);

    // Inicializar o pino GPIO13 e define a direção como saída
    gpio_init(PIN_LED_G);
    gpio_set_dir(PIN_LED_G, GPIO_OUT);

    // Aciona o LED vermelho quando o programa é iniciado
    // É acionado somente uma vez, após isso a temporização será responsável por tratar a lógica de mudança de estado dos LEDs
    gpio_put(PIN_LED_R, !gpio_get(PIN_LED_R));

    // Declaração de uma estrutura de temporizador de repetição
    // Esta estrutura armazenará informações sobre o temporizador configurado
    struct repeating_timer timer;

    // Configura o temporizador para chamar a função de callback a cada 3 segundos
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop infinito que mantém o programa em execução
    while (1)
    {
        // Pausa de 1 segundo para reduzir o uso da CPU
        printf("Rotina principal de repetição\n");
        sleep_ms(1000);        
    }

    return 0;
}


// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador
bool repeating_timer_callback(struct repeating_timer *t)
{    
    // Lógica para mudança de estado dos LEDS
    if (acender_led == 0)
    {
        gpio_put(PIN_LED_G, false);
        gpio_put(PIN_LED_R, !gpio_get(PIN_LED_R));
        acender_led++;
    }
    else if (acender_led == 1)
    {   
        gpio_put(PIN_LED_R, false);
        gpio_put(PIN_LED_Y, !gpio_get(PIN_LED_Y));
        acender_led++;
    }
    else if (acender_led == 2)
    {
        gpio_put(PIN_LED_Y, false);
        gpio_put(PIN_LED_G, !gpio_get(PIN_LED_G));
        acender_led = 0;
    }
    
    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para
    return true;
}