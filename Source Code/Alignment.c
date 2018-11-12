#include <LCD_KIT.c>

//Quantos porcento pra um lado ele pode estar antes de ser considerado desalinhado
float alignRatio = 0.7f;

//extremo esquerda, centro esquerda, centro direita, extrema direita
int16 ad1, ad2, ad3, ad4;

void SetAds(int16 newAd1, int16 newAd2, int16 newAd3, int16 newAd4)
{
    ad1 = newAd1;
    ad2 = newAd2;
    ad3 = newAd3;
    ad4 = newAd4;
}

int8 IsCenterAlign()
{
    float ad2Ratio = (float)ad2 / (float)(ad1 + ad2);
    float ad3Ratio = (float)ad3 / (float)(ad1 + ad2);

    if (ad2 + ad3 > 300 && ad2Ratio < alignRatio && ad3Ratio < alignRatio)
        return 1;
    else
        return 0;
}
int8 IsRight()
{
    if (ad1 < 300 && ad2 >= 300 && ad3 >= 300 && ad4 >= 300)
        return 1;
    else
        return 0;
}
int8 IsCenterAndNotMargins()
{
    if (ad1 < 300 && ad2 >= 300 && ad3 >= 300 && ad4 < 300)
        return 1;
    else
        return 0;
}
int8 IsCenterInside()
{
    if (ad2 >= 300 && ad3 >= 300)
        return 1;
    else
        return 0;
}
int8 IsMarginsInside()
{
    if (ad1 >= 300 && ad4 >= 400)
        return 1;
    else
        return 0;
}
int8 IsMarginsOutside()
{
    if (ad1 < 300 && ad4 < 400)
        return 1;
    else
        return 0;
}
int8 IsAllOutside()
{
    if (ad1 < 300 && ad2 < 300 && ad3 < 300 && ad4 < 300)
        return 1;
    else
        return 0;
}
int8 IsLeft()
{
    if (ad1 >= 300 && ad2 >= 300 && ad3 >= 300 && ad4 < 300)
        return 1;
    else
        return 0;
}
int8 IsAllAlign()
{
    if (ad1 >= 300 && ad2 >= 300 && ad3 >= 300 && ad4 >= 300)
        return 1;
    else
        return 0;
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
            SetLeftBackwardRightForward();
        }
        else if (ad3Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a esquerda");
            SetBothPwm(950);
            SetRightBackwardLeftForward();
        }
        else
        {

            ///850 é o minimo que o motor vai ter (ele precisa de uns 800 pra não ficar travado) e vai somar no maximo 173
            ///Por isso que o maximo dele é 1023 (850 + 173), é possivel fazer com que o valor de maior que 1023 e limitar
            ///Com um if, da para testar isso
            long pwm1 = ad2Ratio * 123 + 900;
            long pwm2 = ad3Ratio * 123 + 900;
            SetLeftPWM(pwm1);
            SetRightPWM(pwm2);
            SetAllForward();
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
            SetLeftBackwardRightForward();
        }
        else if (ad2Ratio > alignRatio)
        {
            printf(lcd_escreve, "Esta a esquerda");
            SetBothPwm(950);
            SetRightBackwardLeftForward();
        }
        else
        {

            ///850 é o minimo que o motor vai ter (ele precisa de uns 800 pra não ficar travado) e vai somar no maximo 173
            ///Por isso que o maximo dele é 1023 (850 + 173), é possivel fazer com que o valor de maior que 1023 e limitar
            ///Com um if, da para testar isso
            long pwm1 = ad2Ratio * 173 + 850;
            long pwm2 = ad3Ratio * 173 + 850;
            SetLeftPWM(pwm2);
            SetRightPWM(pwm1);
            SetAllForward();
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
