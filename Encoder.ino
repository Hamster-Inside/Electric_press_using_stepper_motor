void encoder_input_a()
{

  if (digitalRead(interrupt_PIN_a) == HIGH)
  {
    if (digitalRead(interrupt_PIN_b) == LOW)
    {
      Encoder_state++;
    }
    else
    {
      Encoder_state--;
    }
  }
  else
  {
    if (digitalRead(interrupt_PIN_b) == LOW)
    {
      Encoder_state--;
    }
    else
    {
      Encoder_state++;
    }
  }
}
void encoder_input_b()
{

  if (digitalRead(interrupt_PIN_b) == HIGH)
  {
    if (digitalRead(interrupt_PIN_a) == LOW)
    {
      Encoder_state--;
    }
    else
    {
      Encoder_state++;
    }
  }
  else
  {
    if (digitalRead(interrupt_PIN_a) == LOW)
    {
      Encoder_state++;
    }
    else
    {
      Encoder_state--;
    }
  }
}