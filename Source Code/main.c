//#DEFINE DEBUG
#include <main.h>
#include <Motor1.c>
#include <LCD_KIT.c>

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int running;

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
        printf(lcd_escreve, "1: ");
        printf(lcd_escreve, "%04ld", ad1);
        printf(lcd_escreve, " 2: ");
        printf(lcd_escreve, "%04ld", ad2);
        // lcd_pos_xy(1, 2);
        // printf(lcd_escreve, "3: ");
        // printf(lcd_escreve, "%04ld", ad3);
        // printf(lcd_escreve, " 4: ");
        // printf(lcd_escreve, "%04ld", ad4);
    }
    timerBase++;
    adCounter++;

    set_adc_channel(adCounter);
#ifdef DEBUG
    SetLeft(0);
    SetRight(1023);
#else
    if (running == 1)
    {
        float ad1Ratio = (float)ad1 / (float)(ad1 + ad2);
        float ad2Ratio = (float)ad2 / (float)(ad1 + ad2);

        ///alto significa escuro
        if (ad1 + ad2 > 300)
        {
            //Como quanto mais escuro mais alto, isso significa que o que esta com menos tem que subir,
            //e vai subir proporcionalmente a quanto a menos ele tem, esse valor é exatamente o oposto (que esta no outro ad)

            lcd_pos_xy(1, 2);

            //O ad1 é o da direita, se este esta maior, significa que o robo esta a direit
            if (ad1Ratio > 0.7f)
            {
                printf(lcd_escreve, "Esta a direita");
                SetRight(950);
                SetLeft(0);
            }
            else if (ad2Ratio > 0.7f)
            {
                printf(lcd_escreve, "Esta a esquerda");
                SetRight(0);
                SetLeft(950);
            }
            else
            {
                SetLeft((long)(ad1Ratio * 100) + 850);
                SetRight((long)(ad2Ratio * 100) + 850);
            }
        }
        else
        {
            ///Esta fora da pista, deve fazer algo
            //SetBothPwm(0);
            lcd_pos_xy(1, 2);

            printf(lcd_escreve, "Se perdeu");
            
        }
    }

    else
    {
        SetBothPwm(0);
    }
#endif
}

void main()
{
    Setup();
    lcd_pos_xy(1, 1);
    delay_ms(5);
    printf(lcd_escreve, " Inicializando");
    delay_ms(1000);
    do
    {
        if (input(button1) == 0)
        {
            running = 1;
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
    running = 0;

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
    lcd_escreve('\f');
}
