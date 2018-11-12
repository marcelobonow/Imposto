#define m1p1 pin_e2
#define m1p2 pin_c0
#define m2p1 pin_d0
#define m2p2 pin_d1

void SetAllForward()
{
      output_bit(m1p1, 1);
      output_bit(m1p2, 0);
      output_bit(m2p1, 0);
      output_bit(m2p2, 1);
}
void SetAllBackwards()
{
      output_bit(m1p1, 0);
      output_bit(m1p2, 1);
      output_bit(m2p1, 1);
      output_bit(m2p2, 0);
}

void SetLeftBackwardRightForward()
{
      output_bit(m1p1, 1);
      output_bit(m1p2, 0);
      output_bit(m2p1, 1);
      output_bit(m2p2, 0);
}
void SetRightBackwardLeftForward()
{
      output_bit(m1p1, 0);
      output_bit(m1p2, 1);
      output_bit(m2p1, 0);
      output_bit(m2p2, 1);
}

void MotorInitialize()
{
      setup_timer_2(T2_DIV_BY_16, 255, 1); //341 us overflow, 341 us interrupt

      setup_ccp1(CCP_PWM | CCP_SHUTDOWN_AC_L | CCP_SHUTDOWN_BD_L);
      setup_ccp2(CCP_PWM);
}

void SetBothPwm(int16 vel)
{
      set_pwm1_duty((int16)vel);
      set_pwm2_duty((int16)vel);
}

void SetRightPWM(int16 vel)
{
      set_pwm1_duty((int16)vel);
}
void SetLeftPWM(int16 vel)
{
      set_pwm2_duty((int16)vel);
}
