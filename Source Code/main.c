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

void ClearScreen();
void Setup();

#int_timer0
void timerTick()
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

    set_adc_channel(adCounter + 1);
    if (running == 1)
    {
        ///alto significa escuro
        if (ad1 + ad2 > 250)
        {
            //Como quanto mais escuro mais alto, isso significa que o que esta com menos tem que subir,
            //e vai subir proporcionalmente a quanto a menos ele tem, esse valor é exatamente o oposto (que esta no outro ad)
            //
            if ((float)ad1 / ad1 + ad2 < 0.48f)
            {
                SetRight(1023);
                SetLeft((((float)ad1 / ad1 + ad2) * 200) + 800);
            }
            else if ((float)ad2 / ad1 + ad2 < 0.48f)
            {
                SetLeft(1023);
                SetRight((((float)ad2 / ad1 + ad2) * 200) + 800);
            }
            else
            {
                SetBothPwm(1023);
            }
        }
        else
        {
            SetBothPwm(0);
        }
    }

    ///Seta para pegar no proximo tick
    else
    {
        SetBothPwm(0);
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
            lcd_pos_xy(1, 1);
            printf(lcd_escreve, "Frente");
            SetBothPwm(1023);
        }
        if (input(button2) == 0)
        {
            running = 0;
        }
    } while (1);
}

void Setup()
{
    adCounter = 1;
    running = 1;

    setup_adc_ports(AN0_TO_AN3);
    setup_adc(ADC_CLOCK_DIV_4);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
    set_timer0(0);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);

    lcd_ini();
    MotorInitialize();
    SetBothPwm(1023);
    frente();
}

void ClearScreen()
{
    lcd_pos_xy(1, 1);
    printf(lcd_escreve, blankScreen);
    lcd_pos_xy(1, 2);
    printf(lcd_escreve, blankScreen);
}
