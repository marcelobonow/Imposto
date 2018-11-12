#include <main.h>
#include <Motor.c>
#include <Alignment.c>
///O lcd ta no aligment, se adicionar aqui vai dar duplicação de #define

#build(reset = 0x1000, interrupt = 0x1008)
#org 0x000, 0xfff {}

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int running;

int16 timerBase = 1;
int8 adCounter;

///Em qual decisão ele ta
int8 decisionCounter = 0;
///Isso aqui serve para ele so contabilizar as tomadas de decisão a cadada 200ms (no timerBase)
int8 decisionInCoolDown = 0;

void ClearScreen();
void Setup();

#int_timer0
void timerTick()
{
    set_adc_channel(0);
    delay_us(10);
    ad1 = read_adc();

    set_adc_channel(1);
    delay_us(10);
    ad2 = read_adc();

    set_adc_channel(2);
    delay_us(10);
    ad3 = read_adc();

    set_adc_channel(3);
    delay_us(10);
    ad4 = read_adc();

    SetAds(ad1, ad2, ad3, ad4);

    if (timerBase == 2000)
    {
        timerBase = 0;
        if (decisionInCoolDown > 0)
            decisionInCoolDown--;
    }
    timerBase++;
}

void main()
{
    Setup();
    lcd_pos_xy(1, 1);
    printf(lcd_escreve, " Inicializando");
    delay_ms(50);
    do
    {
        if (input(button1) == 0)
            running = 1;
        if (input(button2) == 0)
            running = 0;
        if (input(button3) == 0)
            decisionCounter = 0;

        ClearScreen();
        lcd_pos_xy(1, 1);
        printf(lcd_escreve, " d: %d", decisionCounter);
        printf(lcd_escreve, " c: %d", decisionInCoolDown);

        if (running == 1)
        {
            ///Esperando uma pista com uma cruz no inicio da pista
            ///e termina quando estiver com todos os sensores fora
            if (decisionCounter == 0)
            {
                Align();
                if(IsAllAlign())
                    decisionCounter++;
            }

            else if (decisionCounter == 1)
            {
                Align();
                if(IsAllOutside())
                {
                    decisionCounter++;
                }
            }

            else
            {
                ///Se ja passou por todas as decisões
                SetBothPwm(0);
            }
        }

        else
        {
            SetBothPwm(0);
            SetBothPwm(0);
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
    // SetBothPwm(0);
    // frente();
}

void ClearScreen()
{
    lcd_escreve('\f');
}
