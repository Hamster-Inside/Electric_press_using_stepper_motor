void go_to_base()
{

  if (!is_base_end_stop_clicked())
  {
    go_to_base_signal = true;
  }
  move_up();
}
void move_up()
{
  Serial.println("moving up");
  bool escapeOuterLoop = false;
  delayMicroseconds(300);
  set_direction_to_go_up();
  while ((!is_base_end_stop_clicked() && is_UP_BUTTON_clicked()) || go_to_base_signal == true)
  {
    make_step();
    if (is_BASE_BUTTON_clicked())
    {
      while (!escapeOuterLoop)
      {
        delayMicroseconds(300);
        if (!is_BASE_BUTTON_clicked())
        {
          escapeOuterLoop = true;
          break;
        }
      }
    }
    if (escapeOuterLoop)
    {
      break;
    }
    if (is_base_end_stop_clicked())
    {
      go_to_base_signal = false;
      can_go_auto = true;
      Encoder_state = 0;
      stepper_motor_current_step = 0;
      Serial.println("BASED");
    }
  }
}

void move_down()
{
  set_direction_to_go_down();
  while (digitalRead(DOWN_BUTTON) == HIGH)
  {
    make_step();
  }
}

void move_to_set_position()
{
  while (is_BASE_BUTTON_clicked())
  {
    delayMicroseconds(1000);
  }
  // 198000 (b) my set prev
  delayMicroseconds(1000);
  Serial.print("Going to set position = ");
  Serial.println(set_position);
  delayMicroseconds(1000);
  if (set_position > Encoder_state)
  {
    set_direction_to_go_down();
    while (set_position > Encoder_state)
    {
      make_step();
      if (is_BASE_BUTTON_clicked())
      {
        break;
      }
    }
  }
  else
  {
    set_direction_to_go_up();
    while (set_position < Encoder_state)
    {
      make_step();
      if (is_BASE_BUTTON_clicked())
      {
        break;
      }
    }
  }
  while (is_BASE_BUTTON_clicked())
  {
    delayMicroseconds(1000);
  }
}

void base_move_decider()
{
  if (digitalRead(DOWN_BUTTON) == HIGH)
  {
    can_go_to_base = false;
    if (can_go_auto)
    {
      move_to_set_position();
    }
    return;
  }
  if (!can_go_to_base)
  {
    can_go_to_base = true;
  }
}