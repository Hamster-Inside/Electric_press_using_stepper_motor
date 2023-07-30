void go_to_base()
{

  if (!is_base_end_stop_clicked())
  {
    go_to_base_signal = true;
  }
  delay_between_steps = fastest_possible_delay_between;
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
    if (is_base_button_clicked())
    {
      while (!escapeOuterLoop)
      {
        delayMicroseconds(300);
        if (!is_base_button_clicked())
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
  while (is_base_button_clicked())
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
      if (is_base_button_clicked())
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
      if (is_base_button_clicked())
      {
        break;
      }
    }
  }
  while (is_base_button_clicked())
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

bool is_base_button_clicked()
{
  // base button is NC
  return (digitalRead(BASE_BUTTON) == LOW) ? true : false;
}

bool is_base_end_stop_clicked()
{
  // End-stop switch is NC so if cable is broken or table is at the end stop it will return base as true so LOW
  return (digitalRead(END_STOP) == HIGH) ? false : true;
}

bool is_UP_BUTTON_clicked()
{
  // button to go up is NO
  return (digitalRead(UP_BUTTON) == HIGH) ? true : false;
}

bool is_DOWN_BUTTON_clicked()
{
  // button to go down is NO
  return (digitalRead(DOWN_BUTTON) == HIGH) ? true : false;
}