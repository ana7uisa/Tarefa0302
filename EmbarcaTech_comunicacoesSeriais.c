#include <stdio.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "pio_matriz.pio.h"
#include "./aux/lmatriz.h"
#include "./aux/num.h"
#include "./aux/ssd1306.h"
#include "./aux/font.h"

// Definições de pinos para LEDs e botões
#define PIN_LED_AZUL 12
#define PIN_LED_VERDE 11
#define PIN_BOTAO_A 5
#define PIN_BOTAO_B 6

// Definições para comunicação I2C com o display OLED
#define I2C_INTERFACE i2c1
#define DISPLAY_ADDR 0x3C
#define PIN_SDA 14
#define PIN_SCL 15
#define TEMPO_DEBOUNCE 200 // Tempo para evitar bouncing (ms)

// Variáveis globais para controle dos botões e display
volatile bool estado_botaoA = false;
volatile bool estado_botaoB = false;
ssd1306_t display;

// Função de interrupção para alternar os LEDs e atualizar o display
void tratar_interrupcao_botao(uint pino, uint32_t eventos) {
    static absolute_time_t ultimo_evento = 0;
    absolute_time_t tempo_atual = get_absolute_time();

    if (absolute_time_diff_us(ultimo_evento, tempo_atual) > TEMPO_DEBOUNCE * 1000) {
        if (pino == PIN_BOTAO_A) {
            estado_botaoA = !estado_botaoA;
            gpio_put(PIN_LED_VERDE, estado_botaoA);

            printf("LED verde: %s\n", estado_botaoA ? "ligado" : "desligado");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, "LED", 5, 30);
            ssd1306_draw_string(&display, "verde:", 35, 30);
            ssd1306_draw_string(&display, estado_botaoA ? "ligado" : "desligado", 79, 30);
            ssd1306_send_data(&display);
        } else if (pino == PIN_BOTAO_B) {
            estado_botaoB = !estado_botaoB;
            gpio_put(PIN_LED_AZUL, estado_botaoB);

            printf("LED azul: %s\n", estado_botaoB ? "ligado" : "desligado");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, "LED", 8, 30);
            ssd1306_draw_string(&display, "azul:", 37, 30);
            ssd1306_draw_string(&display, estado_botaoB ? "ligado" : "desligado", 73, 30);
            ssd1306_send_data(&display);
        }
        ultimo_evento = tempo_atual;
    }
}

// Configuração inicial do hardware
void configurar_hardware() {
    // Inicializa LEDs como saída
    gpio_init(PIN_LED_AZUL);
    gpio_set_dir(PIN_LED_AZUL, GPIO_OUT);
    gpio_init(PIN_LED_VERDE);
    gpio_set_dir(PIN_LED_VERDE, GPIO_OUT);

    // Configura botões como entrada com pull-up e interrupção
    gpio_init(PIN_BOTAO_A);
    gpio_set_dir(PIN_BOTAO_A, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_A);
    gpio_set_irq_enabled_with_callback(PIN_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao_botao);

    gpio_init(PIN_BOTAO_B);
    gpio_set_dir(PIN_BOTAO_B, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_B);
    gpio_set_irq_enabled_with_callback(PIN_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao_botao);

    // Configuração da comunicação I2C para o display OLED
    i2c_init(I2C_INTERFACE, 400 * 1000);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SCL);

    // Inicialização do display OLED
    ssd1306_init(&display, WIDTH, HEIGHT, false, DISPLAY_ADDR, I2C_INTERFACE);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
}

int main() {
    PIO pio = pio0;
    uint sm = 0;
    uint deslocamento = pio_add_program(pio, &pio_matriz_program);
    pio_matriz_program_init(pio, sm, deslocamento, pino_matriz);

    configurar_hardware();
    stdio_init_all();

    // Loop principal para leitura de caracteres via USB Serial
    while (true) {
        if (stdio_usb_connected()) {
            char entrada = getchar();

            if (isdigit(entrada)) {
                print_num(entrada, pio, sm);
                printf("Número: %c\n", entrada);
                ssd1306_fill(&display, false);
                ssd1306_draw_char(&display, entrada, 64, 32);
                ssd1306_send_data(&display);
            } else {
                printf("Caracter: %c\n", entrada);
                clear_leds(pio, sm);
                ssd1306_fill(&display, false);
                ssd1306_draw_char(&display, entrada, 64, 32);
                ssd1306_send_data(&display);
            }
        }
        sleep_ms(10);
    }
}