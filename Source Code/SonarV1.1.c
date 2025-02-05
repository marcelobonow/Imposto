#define  echo1    pin_b6
#define  trig1    pin_b7

#define  echo2    pin_b5
#define  trig2    pin_b4

#define  echo3    pin_b2
#define  trig3    pin_b3 

float distancia1=0,distancia2=0,distancia3=0;

float le_distancia1(){

   float tempo_medido=0;
   float tempo_media=0;
   int x;
   
   for(x=0;x<3;x++)
   {
   output_bit(trig1,1);+
   delay_us(10);
   output_bit(trig1,0);

   while(input(echo1)==0);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);//FREQ 20MHz/32
   set_timer1(0);
   
   while(input(echo1)==1);
   
   setup_timer_1(T1_DISABLED);
   tempo_media=tempo_media+get_timer1();
   }
   tempo_medido=tempo_media/3;
   return ((tempo_medido*5)/(8*58));
}

float le_distancia2(){

   float tempo_medido=0;
   float tempo_media=0;
   int x;
   
   for(x=0;x<3;x++)
   {
   output_bit(trig2,1);
   delay_us(10);
   output_bit(trig2,0);

   while(input(echo2)==0);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);//FREQ 20MHz/32
   set_timer1(0);
   
   while(input(echo2)==1);
   
   setup_timer_1(T1_DISABLED);
   tempo_media=tempo_media+get_timer1();
   }
   tempo_medido=tempo_media/3;
   return ((tempo_medido*5)/(8*58));
}

float le_distancia3(){

   float tempo_medido=0;
   float tempo_media=0;
   int x;
   
   for(x=0;x<3;x++)
   {
   output_bit(trig3,1);
   delay_us(10);
   output_bit(trig3,0);

   while(input(echo3)==0);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);//FREQ 20MHz/32
   set_timer1(0);
   
   while(input(echo3)==1);
   
   setup_timer_1(T1_DISABLED);
   tempo_media=tempo_media+get_timer1();
   }
   tempo_medido=tempo_media/3;
   return ((tempo_medido*5)/(8*58));
}

void sonar_display(){

   distancia1=le_distancia1();
   distancia2=le_distancia2();
   distancia3=le_distancia3();
   
   printf(lcd_escreve,"\f%f  %f\n%f",distancia1,distancia2,distancia3);
   delay_ms(100);
}

void sonar_labirinto(){

   distancia1=le_distancia1();
   distancia2=le_distancia2();
   distancia3=le_distancia3();
   set_pwm(900); 

   if(distancia2>12 && distancia1>8 && distancia3>8)
   {
      frente();
   }
   //else para();
   if(distancia1<8 && distancia3>8 && distancia2>12)
   {
   //if(distancia2<10){tras();}
      gira_direita();
   }
   if(distancia3<8 && distancia1>8 && distancia2>12)
   {
   //if(distancia2<10){tras();}
   gira_esquerda();
   } //15 7
   //talvez passar le_distancia pra distancia denovo
   //provavelmente ocorrer� erro quando 1  menor q 10, e 3 menor q 10, podendo entrar em distancia 2 menor que 10 tbm => assim dantro tras depois de j� ter girado[corretamente?]
   if(distancia2<14) //distancia1.....
   {
      para();
      delay_ms(100); //testar 50
      //tras(); //manter tras por 100~250ms
      //delay_ms(100);
      if(distancia1 > distancia3){
         gira_esquerda();
         delay_ms(200);//testar 150 250
      }
      if(distancia3 > distancia1){
         gira_direita();
         delay_ms(200);//testar 150 250
      }
   }
   if(distancia1<8 && distancia3<8){ // && distancia2>10
      para();
   }
}

