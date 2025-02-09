# Projeto: Comunicação Serial e Controle de LEDs com RP2040

## Descrição
Este projeto tem como objetivo explorar as funcionalidades da placa BitDogLab, utilizando comunicação serial (UART e I2C) para interagir com LEDs e um display OLED SSD1306. Além disso, são implementadas interrupções e debounce para controle de botões.

## Componentes Utilizados
- **Matriz 5x5 de LEDs WS2812** (GPIO 7)
- **LED RGB** (GPIOs 11, 12 e 13)
- **Botão A** (GPIO 5)
- **Botão B** (GPIO 6)
- **Display OLED SSD1306 via I2C** (GPIOs 14 e 15)

## Funcionalidades
1. **Entrada de Caracteres via PC**
   - Caracteres digitados no Serial Monitor do VS Code são exibidos no display OLED.
   - Números entre 0 e 9 são representados também na matriz de LEDs WS2812.

2. **Interação com o Botão A**
   - Alterna o estado do LED RGB Verde.
   - Exibe mensagem no display OLED e envia log para o Serial Monitor.

3. **Interação com o Botão B**
   - Alterna o estado do LED RGB Azul.
   - Exibe mensagem no display OLED e envia log para o Serial Monitor.

## Requisitos Implementados
- **Uso de interrupções (IRQ) para os botões**
- **Debouncing via software**
- **Controle de LEDs comuns e LEDs WS2812**
- **Exibição de caracteres no display SSD1306 (maiúsculas e minúsculas)**
- **Envio de informações via UART para o Serial Monitor**

## Como Executar
1. Compile e carregue o código na Raspberry Pi Pico utilizando o VS Code com o ambiente adequado.
2. Conecte-se ao Serial Monitor para enviar caracteres e visualizar logs.
3. Pressione os botões para interagir com os LEDs e visualizar os resultados no display OLED.

## Vídeo
https://drive.google.com/file/d/1g7TH814wlXaarvvlX7LwuWNHjxPsujEL/view?usp=sharing


