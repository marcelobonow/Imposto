#include <main.h>
#include <Motor1.c>
#include <LCD_KIT.c>

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int running;
const char blankScreen[] = "                ";

#ifdef DEBUG
int16 pwm;
#endif

int16 ad1, ad2, ad3, ad4;
int16 timerBase;
int8 adCounter;

void ClearScreen();
void Setup();

#int_timer0
void timerTick()
{
    if (running == 1)
    {
        if (adCounter == 1)
        {
            ad1 = read_adc();
        }
        else if (adCounter == 2)
        {
            ad2 = read_adc();
        }
        else if (adCounter == 3)
        {
            ad3 = read_adc();
        }
        else
        {
            adCounter = 0;
            ad4 = read_adc();
        }

        set_adc_channel(adCounter + 1);

        // if (ad1 + ad2 < 400)
        // {
        //     if ((float)ad1 / ad1 + ad2 > 0.52f)
        //     {
        //         SetLeft(900);
        //         //SetRight((((float)ad2 / ad1 + ad2) * 200) + 800);
        //         frente();
        //     }
        //     else if ((float)ad2 / ad1 + ad2 > 0.52f)
        //     {
        //         SetRight(900);
        //         //SetLeft((((float)ad1 / ad1 + ad2) * 200) + 800);
        //         frente();
        //     }
        //     else
        //     {
        //         set_pwm(900);
        //         frente();
        //     }
        // }
        // else
        // {
        //     set_pwm(0);
        //     //para();
        // }

        if (timerBase == 2000)
        {
            timerBase = 0;
            ClearScreen();
            lcd_pos_xy(1, 1);
            printf(lcd_escreve, "1: ");
            printf(lcd_escreve, "%04ld", ad1);
            printf(lcd_escreve, " 2: ");
            printf(lcd_escreve, "%04ld", ad2);
            lcd_pos_xy(1, 2);
            printf(lcd_escreve, "3: ");
            printf(lcd_escreve, "%04ld", ad3);
            printf(lcd_escreve, " 4: ");
            printf(lcd_escreve, "%04ld", ad4);
        }
        timerBase++;
        adCounter++;
        ///Seta para pegar no proximo tick
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
            lcd_pos_xy(1,1);
            printf(lcd_escreve, "Frente");
            frente();
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
    adCounter = 1;

    setup_adc_ports(AN0_TO_AN3);
    setup_adc(ADC_CLOCK_DIV_4);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
    set_timer0(0);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);

    lcd_ini();
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
