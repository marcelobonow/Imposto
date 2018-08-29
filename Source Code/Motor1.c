#build(reset = 0x1000, interrupt = 0x1008)
#org 0x000, 0xfff {}

#define m1p1 pin_e2
#define m1p2 pin_c0
#define m2p1 pin_d0
#define m2p2 pin_d1

void AllForward()
{
	output_bit(m1p1, 1);
	output_bit(m1p2, 0);
	output_bit(m2p1, 0);
	output_bit(m2p2, 1);
}

void AllBackwards()
{
	output_bit(m1p1, 0);
	output_bit(m1p2, 1);
	output_bit(m2p1, 1);
	output_bit(m2p2, 0);
}

//Vai para direita pra frente -- Motor da esquerda acelera, Motor da direita desliga
void RightForward()
{
	output_bit(m1p1, 0);
	output_bit(m1p2, 0);
	output_bit(m2p1, 0);
	output_bit(m2p2, 1);
}

//Vai para esquerda pra frente -- Motor da esquerda desliga, Motor da direita acelera
void LeftForward()
{
	output_bit(m1p1, 1);
	output_bit(m1p2, 0);
	output_bit(m2p1, 0);
	output_bit(m2p2, 0);
}

//Vai para direita pra trás -- Motor da esquerda ré, Motor da direita desliga
void RightBackward()
{
	output_bit(m1p1, 0);
	output_bit(m1p2, 0);
	output_bit(m2p1, 1);
	output_bit(m2p2, 0);
}

//Vai para esquerda pra trás -- Motor da esquerda desliga, Motor da direita ré
void LeftBackward()
{
      output_bit(m1p1, 0);
      output_bit(m1p2, 1);
      output_bit(m2p1, 0);
      output_bit(m2p2, 0);
}

void RightBackwardLeftForward()
{
	output_bit(m1p1, 0);
	output_bit(m1p2, 1);
	output_bit(m2p1, 0);
	output_bit(m2p2, 1);
}

void LeftBackwardRightForward()
{
	output_bit(m1p1, 1);
	output_bit(m1p2, 0);
	output_bit(m2p1, 1);
	output_bit(m2p2, 0);
}

//void para()
//{
//    output_bit(m1p1, 0);
//    output_bit(m1p2, 0);
//    output_bit(m2p1, 0);
//	  output_bit(m2p2, 0);
//	  output_bit(m2p2, 0);
//}

void MotorInitialize()
{
      setup_timer_2(T2_DIV_BY_16, 255, 1); //341 us overflow, 341 us interrupt

      setup_ccp1(CCP_PWM | CCP_SHUTDOWN_AC_L | CCP_SHUTDOWN_BD_L);
      setup_ccp2(CCP_PWM);
}

void SetBothPwm(int16 vel)
{
      set_pwm1_duty((int16)vel);
      set_pwm2_duty((int16)vel); //-50);
}

void SetRight(int16 vel)
{
      set_pwm1_duty((int16)vel);
}
void SetLeft(int16 vel)
{
      set_pwm2_duty((int16)vel);
}
