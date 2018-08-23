#include <main.h>
#include <Motor1.c>
#include <LCD_KIT.c>

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int running;
const char blankScreen[] = "                ";

int16 ad1, ad2, ad3, ad4;
int16 timerBase;
int8 adCounter;
char string[5];

void ClearScreen();
void Setup();
void IntToString(int16, char *);

#int_timer0
void timerTick()
{
    if (running)
     {
        if (adCounter == 0)
        {
            ad1 = read_adc();
        }
        else if (adCounter == 1)
        {
            ad2 = read_adc();
        }
        else if (adCounter == 2)
        {
            ad3 = read_adc();
        }
        else
        {
            adCounter = 0;
            ad4 = read_adc();
        }

        if (timerBase == 2000)
        {
            timerBase = 0;
            ClearScreen();
            lcd_pos_xy(1, 1);
            printf(lcd_escreve, "1: ");
            intToString(ad1, string);
            printf(lcd_escreve, string);
            printf(lcd_escreve, " 2: ");
            intToString(ad2, string);
            printf(lcd_escreve, string);
            lcd_pos_xy(1, 2);
            printf(lcd_escreve, "3: ");
            intToString(ad3, string);
            printf(lcd_escreve, string);
            printf(lcd_escreve, " 4: ");
            intToString(ad4, string);
            printf(lcd_escreve, string);
        }
        timerBase++;
        adCounter++;
        ///Seta para pegar no proximo tick
        set_adc_channel(adCounter + 1);
    }
}

void main()
{
    Setup();
    lcd_pos_xy(1, 1);
    printf(lcd_escreve, " Inicializando");
    delay_ms(1000);
    do
    {
        if (input(button1) == 0)
        {
            running = 1;
            frente();
        }
        else
        {
            para();
            delay_ms(200);
        }
        if (input(button2) == 0)
        {
            running = 0;
        }
    } while (1);
}

void Setup()
{
    running = 1;

    setup_adc_ports(AN0_TO_AN3);
    setup_adc(ADC_CLOCK_DIV_4);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
    set_timer0(0);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);

    InitializeLcd();
    motor_ini();
    set_pwm(900); 
}

void ClearScreen()
{
    lcd_pos_xy(1, 1);
    printf(lcd_escreve, blankScreen);
    lcd_pos_xy(1, 2);
    printf(lcd_escreve, blankScreen);
}

void IntToString(int16 value, char *buffer)
{
    buffer[0] = (value / 1000) % 10;
    buffer[1] = (value / 100) % 10;
    buffer[2] = (value / 10) % 10;
    buffer[3] = value % 10;

    buffer[0] += 48;
    buffer[1] += 48;
    buffer[2] += 48;
    buffer[3] += 48;

    buffer[4] = '\0';
}
