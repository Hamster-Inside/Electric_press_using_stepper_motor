void make_step()
{
  digitalWrite(PUL, !digitalRead(PUL));
  delayMicroseconds(delay_between_steps);
  if (digitalRead(DIR) == HIGH)
  {
    stepper_motor_current_step += 1;
  }
  else
  {
    stepper_motor_current_step -= 1;
  }
}

void set_direction_to_go_up()
{
  if (digitalRead(DIR) == HIGH)
  {
    digitalWrite(DIR, LOW);
  }
}

void set_direction_to_go_down()
{
  if (digitalRead(DIR) == LOW)
  {
    digitalWrite(DIR, HIGH);
  }
}
