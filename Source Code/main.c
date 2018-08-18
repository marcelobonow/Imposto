#include <main.h>
#include <LCD_KIT.c>

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int16 counter;
int32 hideCounter;
int running;

#int_timer0
void timerTick()
{
    if (running)
        hideCounter++;
    if (hideCounter % 1000 == 0)
    {
        counter++;
    }
}

void intToString(int16 value, char *buffer)
{

    buffer[0] = (value / 10000) % 10;
    buffer[1] = (value / 1000) % 10;
    buffer[2] = (value / 100) % 10;
    buffer[3] = (value / 10) % 10;
    buffer[4] = value % 10;

    buffer[0] += 48;
    buffer[1] += 48;
    buffer[2] += 48;
    buffer[3] += 48;
    buffer[4] += 48;

    buffer[5] = 0;
}

void main()
{
    counter = 0;
    hideCounter = 0;
    running = 1;

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
    set_timer0(0);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);

    InitializeLcd();
    lcd_pos_xy(1, 1);
    printf(lcd_escreve, " Inicializando");
    delay_ms(20);
    do
    {
        lcd_pos_xy(1, 1);
        if (input(button1) == 0)
        {
            running = 1;
        }
        if (input(button2) == 0)
        {
            running = 0;
        }
        lcd_pos_xy(1, 2);
        printf(lcd_escreve, "C:");

        char string[5];
        intToString(counter, string);

        printf(lcd_escreve, string);
        delay_ms(100);
    } while (1);
}
