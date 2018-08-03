#include <main.h>
#include <Motor1.c>
#include <lcd_kit.c>

#include<SonarV1.1.c>
#include<LinhasV1.1.c>

#define  ch1    pin_a4
#define  ch2   pin_a5
#define  ch3   pin_e0
#define  ch4   pin_e1

void sonar_modos(){
   do{
   printf(lcd_escreve,"\f1. Livre\n2. Labirinto");
   delay_ms(100);
      if(input(ch1)==0){
      }
      if(input(ch2)==0){
         while(input(ch2)==0);
         do{
         sonar_labirinto();}while(input(ch3)==1);
      }
      if(input(ch3)==0){
         do{
         printf(lcd_escreve,"\f3. Display");
         delay_ms(100);
         if(input(ch1)==0){
            while(input(ch1)==0);
            do{
               sonar_display();}while(input(ch3)==1);
            }
         }while(input(ch4)==1);
      }
   }while(input(ch4)==1);
}

void main()
{
   lcd_ini();
   motor_ini();
   set_pwm(1000); 

   //float distancia1=0,distancia2=0,distancia3=0;
   printf(lcd_escreve,"\fImposto V1.2");
   delay_ms(2000);
   do{
      printf(lcd_escreve,"\f1. Linhas\n2. Sonar");
      delay_ms(20);
      if(input(ch1)==0){
         while(input(ch1)==0);
         linhas();
      }
      if(input(ch2)==0){
         while(input(ch2)==0);
         sonar_modos();
      }
   }while(1);
   
   /*
   do{
      sonar_display();
   }while(input(ch1)==1);
   
   while(input(ch1)==0);

   do{
      sonar_labirinto();
   }while(1);
*/
}







