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
int8 IsCenterAndNotMargins();
int8 IsCenterInside();
int8 IsMarginsInside();
int8 IsMarginsOutside();
int8 IsAllOutside();
///Curva a direita
int8 IsRight();
int8 IsLeft();
int8 IsAllAlign();
///uma linha preta sobre todos
void Align();
void InvertedAlign();

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
        ClearScreen();
        lcd_pos_xy(1, 1);
        // printf(lcd_escreve, "1: ");
        // printf(lcd_escreve, "%04ld", ad1);
        // printf(lcd_escreve, " 2: ");
        // printf(lcd_escreve, "%04ld", ad2);
        // lcd_pos_xy(1, 2);
        // printf(lcd_escreve, "3: ");
        // printf(lcd_escreve, "%04ld", ad3);
        // printf(lcd_escreve, " 4: ");
        // printf(lcd_escreve, "%04ld", ad3);
        printf(lcd_escreve, " d: %d", decisionCounter);
        printf(lcd_escreve, " c: %d", decisionInCoolDown);

        if (decisionInCoolDown > 0)
        {
            decisionInCoolDown--;
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
        // if (decisionCounter == 0)
        // {
        //     ///Esperar até ser colocado na linha de partida
        //     if (IsAllAlign())
        //     {
        //         lcd_pos_xy(1, 2);
        //         printf(lcd_escreve, "TA NA POSICAO");
        //         if (running)
        //         {
        //             decisionCounter++;
        //             decisionInCoolDown = 5;
        //         }
        //     }
        //     else
        //     {
        //         if (running)
        //         {
        //             Align();
        //         }
        //     }
        // }
        // else if (decisionCounter == 1 && running)
        // {
        //     ///Se alinha ate achar a curva a direita
        //     Align();
        //     if (IsRight() && !decisionInCoolDown)
        //     {
        //         ///Começa a fazer a curva a direita girando um pouco para a direita e dando uma ré
        //         SetLeft(900);
        //         SetRight(950);
        //         RightBackwardLeftForward();
        //         delay_ms(400);
        //         decisionCounter++;
        //         decisionInCoolDown = 5;
        //     }
        // }
        // else if (decisionCounter == 2 && running)
        // {
        //     if (IsCenterAlign() && !decisionInCoolDown)
        //     {
        //         decisionCounter++;
        //         decisionInCoolDown = 5;
        //     }
        //     else
        //     {
        //         ///Continua Girando do jeito que estava ate ficar com os 2 sensores no centro
        //     }
        // }
        // else if (decisionCounter == 3)
        // {
        //     ///Anda alinhado até achar a linha preta
        //     Align();
        //     if (IsAllAlign() && !decisionInCoolDown)
        //     {
        //         SetLeft(900);
        //         SetRight(950);
        //         RightBackwardLeftForward();
        //         delay_ms(500);
        //         decisionCounter++;
        //         decisionInCoolDown = 25;
        //     }
        // }
        // else if (decisionCounter == 4)
        // {
        //     ///Primeira linha de parada
        //     Align();
        //     if (IsAllAlign() && !decisionInCoolDown)
        //     {
        //         decisionCounter++;
        //         decisionInCoolDown = 10;
        //     }
        // }
        // else if (decisionCounter == 5)
        // {
        //     ///Logo após passar a primeira linha de parada, anda alinhado até o decisioninCoolDown e para
        //     Align();
        //     if (!decisionInCoolDown)
        //     {
        //         SetBothPwm(0);
        //         delay_ms(2000);
        //         SetBothPwm(920);
        //         decisionCounter++;
        //     }
        // }
        // else if (decisionCounter == 6)
        // {
        //     ///Logo após sair da parada, anda alinhado até achar a proxima linha completa
        //     Align();
        //     if (IsAllAlign() && !decisionInCoolDown)
        //     {
        //         decisionCounter++;
        //         decisionInCoolDown = 5;
        //     }
        // }

        //Era 7
        else if (decisionCounter == 0)
        {
            ///Passou pela linha completa do desafio de parar anda alinhado até a linha da esquerda do desafio de andar invertido
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 0;
            }
        }
        else if (decisionCounter == 1)
        {
            ///Anda alinhado até chegar no desafio da linha invertida, a linha invertida começa quando o centro esta fora e as extremidades dentro

            Align();
            if (IsMarginsInside() && !decisionInCoolDown)
            {
                ///Iniciou o desafio de linha invertido
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 2)
        {
            ///Desafio da linha invertida
            InvertedAlign();
            ///Anda ate não ter linha nas extremidades
            if (IsMarginsOutside() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 2;
            }
        }
        else if (decisionCounter == 3)
        {
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 5;
            }
        }
        else if (decisionCounter == 4)
        {
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                SetLeft(950);
                SetRight(900);
                LeftBackwardRightForward();
                delay_ms(500);
                decisionCounter++;
                decisionInCoolDown = 8;
            }
        }
        else if (decisionCounter == 5)
        {

            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 5;
            }
        }
        else if (decisionCounter == 6)
        {
            ///Desafio de andar cego
            ///!!!!!!!!!!   AQUI ELE TERIA QUE ESTAR MUITO BEM ALINHADO, SERIA BOM TER A FUNÇÃO DE ALINHAR E COLOCAR PARA ALINHAR BEM AQUI
            Align();
            if (IsAllOutside() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 3;
            }
        }
        else if (decisionCounter == 7)
        {
            ///ESTA NO DESAFIO DE ANDAR SEM LINHA
            SetBothPwm(870);
            if (IsCenterInside() && !decisionInCoolDown)
            {
                Align();
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 8)
        {
            ///Passou pelo desafio de andar sem linha
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 9)
        {
            Align();
            if (IsRight() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 2;
            }
        }
        else if (decisionCounter == 10)
        {
            Align();
            if (IsRight() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 2;
            }
        }
        else if (decisionCounter == 11)
        {
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                SetLeft(950);
                SetRight(900);
                LeftBackwardRightForward();
                delay_ms(500);
                decisionCounter++;
                decisionInCoolDown = 7;
            }
        }
        else if (decisionCounter == 12)
        {
            Align();
            if (IsAllAlign() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 0;
            }
        }

        else if (decisionCounter == 13)
        {
            ///TODO: desvia do desafio de remover o bloco
            SetBothPwm(0);
            lcd_pos_xy(1, 1);
            printf(lcd_escreve, "Fim do programado");
        }

        else
        {
            ///Se ja passou por todas as decisões
            SetBothPwm(0);
        }

        if (running == 0)
        {
            SetBothPwm(0);
            SetBothPwm(0);
        }
    } while (1);
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
int8 IsCenterAndNotMargins()
{
    if (ad1 < 300 && ad2 >= 300 && ad3 >= 300 && ad4 < 300)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsCenterInside()
{
    if (ad2 >= 300 && ad3 >= 300)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsMarginsInside()
{
    if (ad1 >= 300 && ad4 >= 400)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsMarginsOutside()
{
    if (ad1 < 300 && ad4 < 400)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int8 IsAllOutside()
{
    if (ad1 < 300 && ad2 < 300 && ad3 < 300 && ad4 < 300)
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

        lcd_pos_xy(1, 1);

        //O ad2 é o da direita, se este esta maior, significa que o robo esta a direita
        if (ad2Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a direita");
            SetBothPwm(950);
            LeftBackwardRightForward();
        }
        else if (ad3Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a esquerda");
            SetBothPwm(950);
            RightBackwardLeftForward();
        }
        else
        {

            ///850 é o minimo que o motor vai ter (ele precisa de uns 800 pra não ficar travado) e vai somar no maximo 173
            ///Por isso que o maximo dele é 1023 (850 + 173), é possivel fazer com que o valor de maior que 1023 e limitar
            ///Com um if, da para testar isso
            long pwm1 = ad2Ratio * 173 + 850;
            long pwm2 = ad3Ratio * 173 + 850;
            SetLeft(pwm1);
            SetRight(pwm2);
            frente();
            // SetLeft((long)(ad2Ratio * 100) + 850);
            // SetRight((long)(ad3Ratio * 100) + 850);
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

///BASICAMENTE faz a mesma coisa que o algin, mas troca os sensores (coloca o da esquerda na direitea)
void InvertedAlign()
{
    float ad2Ratio = (float)ad2 / (float)(ad2 + ad3);
    float ad3Ratio = (float)ad3 / (float)(ad2 + ad3);

    ///alto significa escuro
    if (ad2 + ad3 <= 400)
    {
        //Como quanto mais escuro mais alto, isso significa que o que esta com menos tem que subir,
        //e vai subir proporcionalmente a quanto a menos ele tem, esse valor é exatamente o oposto (que esta no outro ad)

        lcd_pos_xy(1, 1);

        //O ad2 é o da direita, se este esta maior, significa que o robo esta a direita
        if (ad3Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a direita");
            SetBothPwm(950);
            LeftBackwardRightForward();
        }
        else if (ad2Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a esquerda");
            SetBothPwm(950);
            RightBackwardLeftForward();
        }
        else
        {

            ///850 é o minimo que o motor vai ter (ele precisa de uns 800 pra não ficar travado) e vai somar no maximo 173
            ///Por isso que o maximo dele é 1023 (850 + 173), é possivel fazer com que o valor de maior que 1023 e limitar
            ///Com um if, da para testar isso
            long pwm1 = ad2Ratio * 173 + 850;
            long pwm2 = ad3Ratio * 173 + 850;
            SetLeft(pwm2);
            SetRight(pwm1);
            frente();
            // SetLeft((long)(ad2Ratio * 100) + 850);
            // SetRight((long)(ad3Ratio * 100) + 850);
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
