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
int8 decisionCounter = 3;//0
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
         /*if (decisioncounter == 0)
         {
             ///esperar até ser colocado na linha de partida
             if (isallalign())
             {
                 lcd_pos_xy(1, 2);
                 printf(lcd_escreve, "ta na posicao");
                 if (running)
                 {
                     decisioncounter++;
                     decisionincooldown = 5;
                 }
             }
             else
             {
                 if (running)
                 {
                     align();
                 }
             }
         }
		 ///Começa desafio da caixa
         else if (decisioncounter == 1 && running)
         {
             ///se alinha ate achar a curva a direita
             align();
             if (isright() && !decisionincooldown)
             {
                 decisioncounter++;
                 decisionincooldown = 5;
             }
         }
		 ///Termina desafio caixa
		 else if (decisioncounter == 2 && running)
		 {
			 align();
			 if (isright() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }*/
		 ///Primeira curva
		 else if (decisioncounter == 3)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(920);
				 setright(1000);
				 rightbackwardleftforward();
				 delay_ms(350);
				 decisioncounter = decisioncounter + 7;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 10)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Começa desvio do Fixo
		 else if (decisioncounter == 11)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(920);
				 setright(1000);
				 rightbackwardleftforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 12)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 13)
		 {
			 align();
			 if (isLeft() && !decisionincooldown)
			 {
				 setleft(1000);
				 setright(920);
				 leftbackwardrightforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 14)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 15)
		 {
			 align();
			 if (isLeft() && !decisionincooldown)
			 {
				 setleft(1000);
				 setright(920);
				 leftbackwardrightforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 16)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Volta pra pista
		 else if (decisioncounter == 17)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(950);
				 setright(1000);
				 rightbackwardleftforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 18)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Alinha no zigzag
		 ///Dobra pra fazer desafio sem linha
		 else if (decisioncounter == 19)
		 {
			 align();
			 if (isLeft() && !decisionincooldown)
			 {
				 setleft(1000);
				 setright(920);
				 leftbackwardrightforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 20)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Começa desafio sem linha
		 else if (decisionCounter == 21)
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
		 ///Termina desafio sem linha
		 else if (decisionCounter == 22)
		 {
			 ///ESTA NO DESAFIO DE ANDAR SEM LINHA
			 SetBothPwm(890);
			 if (IsCenterInside() && !decisionInCoolDown)
			 {
				 Align();
				 decisionCounter++;
				 decisionInCoolDown = 1;
			 }
		 }
		 ///Curva Direita
		 else if (decisioncounter == 23)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(920);
				 setright(1000);
				 rightbackwardleftforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 24)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 25)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 2;
			 }
		 }
		 ///Começa desvio da lombada
		 else if (decisioncounter == 26)
		 {
			 ///se alinha ate achar a curva a direita
			 align();
			 if (isright() && !decisionincooldown)
			 {
				 ///começa a fazer a curva a direita girando um pouco para a direita e dando uma ré
				 setleft(920);
				 setright(1000);
				 rightbackwardleftforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 27)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 28)
		 {
			 align();
			 if (isLeft() && !decisionincooldown)
			 {
				 setleft(1000);
				 setright(920);
				 leftbackwardrightforward();
				 delay_ms(350);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 29)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 30)
		 {
			 align();
			 if (isLeft() && !decisionincooldown)
			 {
				 setleft(950);
				 setright(900);
				 leftbackwardrightforward();
				 delay_ms(400);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 31)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Volta pra pista
		 else if (decisioncounter == 32)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(900);
				 setright(950);
				 rightbackwardleftforward();
				 delay_ms(400);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 33)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 ///Curva direita final
		 else if (decisioncounter == 34)
		 {
			 align();
			 if (isRight() && !decisionincooldown)
			 {
				 setleft(900);
				 setright(950);
				 rightbackwardleftforward();
				 delay_ms(400);
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 35)
		 {
			 if (iscenteralign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
			 else
			 {
				 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
			 }
		 }
		 else if (decisioncounter == 36)
		 {
			 align();
			 if (isRight() && !decisioninCoolDown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 ///Primeira linha do desafio de parada
		 else if (decisioncounter == 37)
		 {
			 ///primeira linha de parada
			 align();
			 if (isallalign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 10;
			 }
		 }
		 else if (decisioncounter == 38)
		 {
			 ///logo após passar a primeira linha de parada, anda alinhado até o decisionincooldown e para
			 align();
			 if (!decisionincooldown)
			 {
				 setbothpwm(0);
				 delay_ms(2000);
				 setbothpwm(920);
				 decisioncounter++;
			 }
		 }
		 ///Segunda linha do desafio de parada
		 else if (decisioncounter == 39)
		 {
			 ///logo após sair da parada, anda alinhado até achar a proxima linha completa
			 align();
			 if (isallalign() && !decisionincooldown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 else if (decisioncounter == 40) {
			 align();
			 if (isRight() && !decisioninCoolDown)
			 {
				 decisioncounter++;
				 decisionincooldown = 5;
			 }
		 }
		 ///Teste da linha final
		 else if (decisionCounter == 41)
		 {
			 Align();
			 if (IsAllAlign() && !decisionInCoolDown)
			 {
				 decisionCounter++;
				 decisionInCoolDown = 0;
			 }
		 }
		 ///Encerra
		 else if (decisionCounter == 42)
		 {
			 ///TODO: desvia do desafio de remover o bloco
			 SetBothPwm(0);
			 lcd_pos_xy(1, 1);
			 printf(lcd_escreve, "Fim do programado");
		 }
         /*else if (decisioncounter == 2 && running)
         {
             if (iscenteralign() && !decisionincooldown)
             {
                 decisioncounter++;
                 decisionincooldown = 5;
             }
             else
             {
                 ///continua girando do jeito que estava ate ficar com os 2 sensores no centro
             }
         }
         else if (decisioncounter == 3)
         {
             ///anda alinhado até achar a linha preta
             align();
             if (isallalign() && !decisionincooldown)
             {
                 setleft(900);
                 setright(950);
                 rightbackwardleftforward();
                 delay_ms(500);
                 decisioncounter++;
                 decisionincooldown = 25;
             }
         }
         else if (decisioncounter == 4)
         {
             ///primeira linha de parada
             align();
             if (isallalign() && !decisionincooldown)
             {
                 decisioncounter++;
                 decisionincooldown = 11;
             }
         }
         else if (decisioncounter == 5)
         {
             ///logo após passar a primeira linha de parada, anda alinhado até o decisionincooldown e para
             align();
             if (!decisionincooldown)
             {
                 setbothpwm(0);
                 delay_ms(2000);
                 setbothpwm(920);
                 decisioncounter++;
             }
         }
         else if (decisioncounter == 6)
         {
             ///logo após sair da parada, anda alinhado até achar a proxima linha completa
             align();
             if (isallalign() && !decisionincooldown)
             {
                 decisioncounter++;
                 decisionincooldown = 5;
             }
         }

        //Era 7
        else if (decisionCounter == 7)
        {
            ///Passou pela linha completa do desafio de parar anda alinhado até a linha da esquerda do desafio de andar invertido
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 0;
            }
        }
        else if (decisionCounter == 8)
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
        else if (decisionCounter == 9)
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
        else if (decisionCounter == 10)
        {
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 5;
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
                decisionInCoolDown = 8;
            }
        }
        else if (decisionCounter == 12)
        {

            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 5;
            }
        }
        else if (decisionCounter == 13)
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
        else if (decisionCounter == 14)
        {
            ///ESTA NO DESAFIO DE ANDAR SEM LINHA
            SetBothPwm(890);
            if (IsCenterInside() && !decisionInCoolDown)
            {
                Align();
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 15)
        {
            ///Passou pelo desafio de andar sem linha
            Align();
            if (IsLeft() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 1;
            }
        }
        else if (decisionCounter == 16)
        {
            Align();
            if (IsRight() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 2;
            }
        }
        else if (decisionCounter == 17)
        {
            Align();
            if (IsRight() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 2;
            }
        }
        else if (decisionCounter == 18)
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
        else if (decisionCounter == 19)
        {
            Align();
            if (IsAllAlign() && !decisionInCoolDown)
            {
                decisionCounter++;
                decisionInCoolDown = 0;
            }
        }

        else if (decisionCounter == 20)
        {
            ///TODO: desvia do desafio de remover o bloco
            SetBothPwm(0);
            lcd_pos_xy(1, 1);
            printf(lcd_escreve, "Fim do programado");
        }*/

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
            long pwm1 = ad2Ratio * 123 + 900;
            long pwm2 = ad3Ratio * 123 + 900;
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
