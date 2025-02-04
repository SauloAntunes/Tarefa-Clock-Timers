#include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes

#define PIN_LED_B 11 // Pino do LED azul
#define PIN_LED_R 12 // Pino do LED vermelho
#define PIN_LED_G 13 // Pino do LED verde
#define BUTTON_A 5   // Pino do botão A

bool led_active = false;                 // Indica se o LED está atualmente aceso (para evitar múltiplas ativações)
static volatile uint8_t apagar_led = 0; // Variável global para controlar a lógica de mudança de estado dos LEDs

// Prototipação da rotina
int64_t turn_off_callback(alarm_id_t id, void *user_data);


// Rotina principal
int main()
{
    // Inicializar o pino GPIO11 e define a direção como saída
    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);

    // Inicializar o pino GPIO12 e define a direção como saída
    gpio_init(PIN_LED_R);
    gpio_set_dir(PIN_LED_R, GPIO_OUT);

    // Inicializar o pino GPIO13 e define a direção como saída
    gpio_init(PIN_LED_G);
    gpio_set_dir(PIN_LED_G, GPIO_OUT);

    // Inicializa o pino do botão A e define a direção como entrada, com pull-up
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Loop principal do programa que verifica continuamente o estado do botão
    while (1)
    {
        // Verifica se o botão foi pressionado (nível baixo no pino) e se o LED não está ativo
        if (!gpio_get(BUTTON_A) && !led_active)
        {
            // Adiciona um pequeno atraso para debounce, evitando leituras errôneas
            sleep_ms(50);

            // Verifica novamente o estado do botão após o debounce
            if (!gpio_get(BUTTON_A))
            {
                // Liga o LED configurando o pino LED_PIN para nível alto
                gpio_put(PIN_LED_B, true);
                gpio_put(PIN_LED_R, true);
                gpio_put(PIN_LED_G, true);

                // Define 'led_active' como true para indicar que os LEDs estão acesos
                led_active = true;

                // Agenda um alarme para desligar o LED após 3 segundos (3000 ms)
                // Desliga o primero LED e aguarda 3 segundos, desliga o segundo LED e aguarda mais 3 segundos e por fim desliga o terceiro LED
                // A função 'turn_off_callback' será chamada após esse tempo
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);
                add_alarm_in_ms(6000, turn_off_callback, NULL, false);
                add_alarm_in_ms(9000, turn_off_callback, NULL, false);
            }
        }

        // Introduz uma pequena pausa de 10 ms para reduzir o uso da CPU
        // Isso evita que o loop seja executado muito rapidamente e consuma recursos desnecessários
        sleep_ms(10);
    }

    return 0;
}

// Função de callback para desligar o LED após o tempo programado
int64_t turn_off_callback(alarm_id_t id, void *user_data)
{
    // Lógica para mudança de estado dos LEDS
    if (apagar_led == 0) 
    {
        gpio_put(PIN_LED_B, false);
        apagar_led++;
    }
    else if (apagar_led == 1)
    {
        gpio_put(PIN_LED_R, false);
        apagar_led++;
    }
    else if (apagar_led == 2)
    {
        gpio_put(PIN_LED_G, false);
        apagar_led = 0;
        led_active = false;
    }
    
    // Retorna 0 para indicar que o alarme não deve se repetir
    return 0;
}