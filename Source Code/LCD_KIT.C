/************************************************************************/
/*  MOD_LCD.C - Biblioteca de manipula��o de m�dulo LCD                 */
/*                                                                      */
/*  Autor: F�bio Pereira                                                */
/*                                                                      */
/************************************************************************/
/*
lcd_cursor_on()                        fun��es LCD
lcd_cursor_pisca()
lcd_cursor_off()
lcd_corre_esquerda()
lcd_corre_direita()
lcd_envia_nibble( byte dado )
lcd_le_byte()
lcd_ini()
lcd_pos_xy( byte x, byte y)
lcd_escreve( char c)                  \f  Clear display                            ////
                                            \n  Go to start of second line               ////
                                  \b  Move back one position                   ////

lcd_le( byte x, byte y)
lcd_apaga()
{

*/
// As defini��es a seguir s�o utilizadas para acesso aos pinos do display
// caso o pino RW n�o seja utilizado, comente a defini��o lcd_rw
#ifndef lcd_enable
   #define lcd_rs         pin_d2      // pino rs do LCD
   #define lcd_enable     pin_d3      // pino enable do LCD
   #define lcd_d4         pin_d4      // pino de dados d4 do LCD
   #define lcd_d5         pin_d5      // pino de dados d5 do LCD
   #define lcd_d6         pin_d6      // pino de dados d6 do LCD
   #define lcd_d7         pin_d7   // pino de dados d7 do LCD
#endif

#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 linhas
#define lcd_seg_lin 0x40    // Endere�o da segunda linha na RAM do LCD

// a constante abaixo define a seq��ncia de inicializa��o do m�dulo LCD
byte CONST INI_LCD[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6};

/*   O 0x20 eh p configurar o tipo do display e talz.
   O Segundo numero configura o modo do cursor, sendo:
               -> 0xf Cursor piscante
               -> 0xe Cursor comum
               -> 0xc Sem cursor
   O terceiro (1) limpa o display
   O Quarto volta com o cursor.
*/

byte lcd_le_byte()
// l� um byte do LCD (somente com pino RW)
{
   byte dado;
   // configura os pinos de dados como entradas
   input(lcd_d4);
   input(lcd_d5);
   input(lcd_d6);
   input(lcd_d7);
   // se o pino rw for utilizado, coloca em 1
   #ifdef lcd_rw
      output_high(lcd_rw);
   #endif
   output_high(lcd_enable); // habilita display
   dado = 0;   // zera a vari�vel de leitura
   // l� os quatro bits mais significativos
   if (input(lcd_d7)) bit_set(dado,7);
   if (input(lcd_d6)) bit_set(dado,6);
   if (input(lcd_d5)) bit_set(dado,5);
   if (input(lcd_d4)) bit_set(dado,4);
   // d� um pulso na linha enable
   output_low(lcd_enable);
   output_high(lcd_enable);
   // l� os quatro bits menos significativos
   if (input(lcd_d7)) bit_set(dado,3);
   if (input(lcd_d6)) bit_set(dado,2);
   if (input(lcd_d5)) bit_set(dado,1);
   if (input(lcd_d4)) bit_set(dado,0);
   output_low(lcd_enable);   // desabilita o display
   return dado;   // retorna o byte lido
}

void lcd_envia_nibble( byte dado )
// envia um dado de quatro bits para o display
{
   // coloca os quatro bits nas saidas
   output_bit(lcd_d4,bit_test(dado,0));
   output_bit(lcd_d5,bit_test(dado,1));
   output_bit(lcd_d6,bit_test(dado,2));
   output_bit(lcd_d7,bit_test(dado,3));
   // d� um pulso na linha enable
   output_high(lcd_enable);
   output_low(lcd_enable);
}


void lcd_envia_byte( boolean endereco, byte dado )
{
   // coloca a linha rs em 0
   output_low(lcd_rs);
   // aguarda o display ficar desocupado
   //while ( bit_test(lcd_le_byte(),7) ) ;
   // configura a linha rs dependendo do modo selecionado
   output_bit(lcd_rs,endereco);
   delay_us(100);   // aguarda 100 us
   // caso a linha rw esteja definida, coloca em 0
   #ifdef lcd_rw
      output_low(lcd_rw);
   #endif
   // desativa linha enable
   output_low(lcd_enable);
   // envia a primeira parte do byte
   lcd_envia_nibble(dado >> 4);
   // envia a segunda parte do byte
   lcd_envia_nibble(dado & 0x0f);
}


void lcd_ini()
// rotina de inicializa��o do display
{
   byte conta;
   output_low(lcd_d4);
   output_low(lcd_d5);
   output_low(lcd_d6);
   output_low(lcd_d7);
   output_low(lcd_rs);
   #ifdef lcd_rw
      output_high(lcd_rw);
   #endif
   output_low(lcd_enable);
   delay_ms(15);
   // envia uma seq��ncia de 3 vezes 0x03
   // e depois 0x02 para configurar o m�dulo
   // para modo de 4 bits
   for(conta=1;conta<=3;++conta)
   {
      lcd_envia_nibble(3);
      delay_ms(5);
   }
   lcd_envia_nibble(2);
   // envia string de inicializa��o do display
   for(conta=0;conta<=3;++conta) lcd_envia_byte(0,INI_LCD[conta]);
}

void lcd_pos_xy( byte x, byte y)
{
   byte endereco;
   if(y!=1)
      endereco = lcd_seg_lin;
   else
      endereco = 0;
   endereco += x-1;
   lcd_envia_byte(0,0x80|endereco);
}

void lcd_escreve( char c)
// envia caractere para o display
{
   switch (c)
   {
     case '\f'    :   lcd_envia_byte(0,1);
              delay_ms(2);
            break;
     case '\n'   :
     case '\r'    :   lcd_pos_xy(1,2);
              break;
     case '\b'    :   lcd_envia_byte(0,0x10);
              break;
     default   :   lcd_envia_byte(1,c);
              break;
   }
}

char lcd_le( byte x, byte y)
// le caractere do display
{
   char valor;
   // seleciona a posi��o do caractere
   lcd_pos_xy(x,y);
   // ativa rs
   output_high(lcd_rs);
   // l� o caractere
   valor = lcd_le_byte();
   // desativa rs
   output_low(lcd_rs);
   // retorna o valor do caractere
   return valor;
}

void lcd_apaga()
{
   lcd_envia_byte(0,1);
   delay_ms(2);
}

void lcd_cursor_on()
    {
lcd_envia_byte(0,0x0E);
    }

void lcd_cursor_pisca()
    {
lcd_envia_byte(0,0x0F);
    }

void lcd_cursor_off()
    {
 lcd_envia_byte(0,0x0C);
    }

void lcd_corre_esquerda()
    {
 lcd_envia_byte(0,0x18);
    }

void lcd_corre_direita()
    {
lcd_envia_byte(0,0x1C);
    }

