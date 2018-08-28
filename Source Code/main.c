#include <main.h>
#include <Motor1.c>
#include <LCD_KIT.c>

#define button1 pin_a4
#define button2 pin_a5
#define button3 pin_e0
#define button4 pin_e1

int running;

//extremo esquerda, centro esquerda, centro direita, extrema direita
int16 ad1, ad2, ad3, ad4;
int16 timerBase = 1;
int8 adCounter;

///Em qual decisão ele ta
int8 decisionCounter = 0;
///Isso aqui serve para ele so contabilizar as tomadas de decisão a cadada 200ms (no timerBase)
int8 decisionInCoolDown = 0;

//Quantos porcento pra um lado ele pode estar antes de ser considerado desalinhado
float alignRatio = 0.7f;

void ClearScreen();
void Setup();
void AndarReto();
///Se tiver retorna 1
int8 IsCenterAlign();
///Curva a direita
int8 IsRight();
int8 IsLeft();
int8 IsAllAlign();
///uma linha preta sobre todos
void Align();

#int_timer0
void timerTick()
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
    else if (adCounter == 3)
    {
        ad4 = read_adc();
    }
    if (timerBase == 2000)
    {
        timerBase = 0;
        //ClearScreen();
        lcd_pos_xy(1, 1);
        printf(lcd_escreve, "1: ");
        printf(lcd_escreve, "%04ld", ad1);
        printf(lcd_escreve, " 2: ");
        printf(lcd_escreve, "%04ld", ad2);
        lcd_pos_xy(1, 2);
        printf(lcd_escreve, "3: ");
        printf(lcd_escreve, "%04ld", ad3);
        // printf(lcd_escreve, " 4: ");
        // printf(lcd_escreve, "%04ld", ad3);
        printf(lcd_escreve, " d: %d", decisionCounter);

        if (decisionInCoolDown)
        {
            decisionInCoolDown = 0;
        }
    }
    timerBase++;
    adCounter++;

    if (adCounter > 4)
    {
        adCounter = 0;
    }

    set_adc_channel(adCounter);
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
        if (decisionCounter == 0)
        {
            ///Esperar até ser colocado na linha de partida
            if (IsAllAlign())
            {
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 1 && !decisionInCoolDown)
        {
            if (running == 1)
            {
                Align();
            }
            else
            {
                SetBothPwm(0);
            }
            if (IsAllAlign())
            {
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 2 && !decisionInCoolDown)
        {
            SetBothPwm(0);
            delay_ms(2000);
            SetBothPwm(900);
        }
        delay_ms(200);
    } while (1);
}

void AndarReto()
{
    SetBothPwm(1023);
}

int8 IsCenterAlign()
{
    float ad2Ratio = (float)ad2 / (float)(ad1 + ad2);
    float ad3Ratio = (float)ad3 / (float)(ad1 + ad2);
    if (ad2 + ad3 > 300 && ad2Ratio < alignRatio && ad3Ratio < alignRatio)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsRight()
{
    if (ad1 < 300 && ad2 >= 300 && ad3 >= 300 && ad4 >= 300)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsLeft()
{
    if (ad1 >= 300 && ad2 >= 300 && ad3 >= 300 && ad4 < 300)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsAllAlign()
{
    if (ad1 >= 300 && ad2 >= 300 && ad3 >= 300 && ad4 >= 300)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Align()
{
    float ad2Ratio = (float)ad2 / (float)(ad2 + ad3);
    float ad3Ratio = (float)ad3 / (float)(ad2 + ad3);

    ///alto significa escuro
    if (ad2 + ad3 > 400)
    {
        //Como quanto mais escuro mais alto, isso significa que o que esta com menos tem que subir,
        //e vai subir proporcionalmente a quanto a menos ele tem, esse valor é exatamente o oposto (que esta no outro ad)

        lcd_pos_xy(1, 2);

        //O ad1 é o da direita, se este esta maior, significa que o robo esta a direit
        if (ad2Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a direita");
            SetRight(1000);
            SetLeft(0);
        }
        else if (ad3Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a esquerda");
            SetRight(0);
            SetLeft(1000);
        }
        else
        {
            SetLeft((long)(ad2Ratio * 100) + 850);
            SetRight((long)(ad3Ratio * 100) + 850);
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
    lcd_escreve('\f');
}
