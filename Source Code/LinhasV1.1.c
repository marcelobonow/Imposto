int1 linha1, linha2, linha3, linha4;

#define  ch1    pin_a4

void le_linhas()
{
      int16 ad1, ad2, ad3, ad4;

      set_adc_channel(0);
      delay_us(25);
      ad1=read_adc();
      
      set_adc_channel(1);
      delay_us(25);
      ad2=read_adc();
      
      set_adc_channel(2);
      delay_us(25);
      ad3=read_adc();
      
      set_adc_channel(3);
      delay_us(25);
      ad4=read_adc();
     
      
      if(ad1>300){
         linha1=1;
      }
      else{
         linha1=0;
      }
      
     if(ad2>300){
         linha2=1;
      }
      else{
         linha2=0;
      }
      
      if(ad3>200){
         linha3=1;
      }
      else{
         linha3=0;
      }
      
      if(ad4>200){
         linha4=1;
      }
      else{
         linha4=0;
      }
}

void alinhar(){
   le_linhas();
   
   if(linha2==1 && linha3==1){
      frente();
   }
   if(linha2==1 && linha3==0){
      gira_esquerda();
    }
    if(linha2==0 && linha3==1){
      gira_direita();
    }
}

void avancar(){
while(linha1==1 || linha4==1){
   le_linhas();
   frente();}
}

void curva_esquerda(){
avancar(); 
while(linha1==0){
   le_linhas();
   gira_esquerda();}
while(linha1==1){
   le_linhas();
   gira_esquerda();}
while(linha2==0){
   le_linhas();
   gira_esquerda();}
}

void curva_direita(){
avancar(); 
while(linha4==0){
   le_linhas();
   gira_direita();}
para();
delay_ms(3000);
while(linha4==1){
   le_linhas();
   gira_direita();}
para();
delay_ms(3000);
while(linha3==0){
   le_linhas();
   gira_direita();}
para();
delay_ms(3000);
while(linha2==0){
   le_linhas();
   gira_direita();}
para();
delay_ms(3000);
}

void linhas()
{
   lcd_ini();
   motor_ini();
   set_pwm(900);

   setup_adc_ports(AN0_TO_AN3);
   setup_adc(ADC_CLOCK_DIV_4);
   
   do{
      alinhar();
   }while(linha1==0 && linha4==0); //1
   para();
   delay_ms(1000);
   avancar();
   
   do{
      alinhar();
   }while(linha1==0 || linha4==0); //2
   para();
   delay_ms(1000);
   curva_direita();
   avancar();
   
   do{
      alinhar();
   }while(linha1==0 || linha4==0); //3
   para();/*
   delay_ms(1000);
   avancar();
   
   do{
      alinhar();
   }while(linha1==0 || linha4==0); //4
   para();
   delay_ms(1000);
   curva_direita();
   avancar();
   
   do{
      alinhar();
   }while(linha1==0 || linha4==0); //5
   para(); */
}
