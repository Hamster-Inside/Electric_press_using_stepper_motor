# 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
# 11 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
long current_encoder_state = 0;
long incremental_stepper_motor_current_step = 0;
int delay_between_steps = 60;
int fastest_possible_delay_between = 60;
long set_position = 198000;
volatile long Encoder_state = 0;
bool go_to_base_signal = false;
bool is_based = false;
bool can_go_to_base = false;
bool can_go_auto = false;

void setup()
{
  pinMode(2, 0x2);
  pinMode(3, 0x2);
  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : ((2) >= 18 && (2) <= 21 ? 23 - (2) : -1))), encoder_input_a, 1);
  attachInterrupt(((3) == 2 ? 0 : ((3) == 3 ? 1 : ((3) >= 18 && (3) <= 21 ? 23 - (3) : -1))), encoder_input_b, 1);
  pinMode(22, 0x0);
  pinMode(23, 0x0);
  pinMode(24, 0x0);
  pinMode(A0, 0x0);
  pinMode(29, 0x0); // set the input pin as input
  pinMode(27, 0x1); // set the first output pin as output
  pinMode(26, 0x1); // set the second output pin as output
  pinMode(28, 0x1); // set the third output pin as output
  digitalWrite(26, 0x1);
  digitalWrite(28, 0x0);
  digitalWrite(27, 0x1);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(22) == 0x0 && digitalRead(23) == 0x0)
  {
    int analogValue = analogRead(A0); // Read analog value from A0
    delay_between_steps = map(analogValue, 0, 1023, 300, 40); // Map the analog value to a range of 0-255
  }
  if (digitalRead(22) == 0x1 && digitalRead(23) == 0x0)
  {
    move_down();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(incremental_stepper_motor_current_step));
  }
  if (digitalRead(23) == 0x1 && digitalRead(22) == 0x0)
  {
    move_up();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(incremental_stepper_motor_current_step));
  }
  while (digitalRead(24) == 0x0)
  {
    base_move_decider();
  }
  if (can_go_to_base)
  {
    go_to_base();
    can_go_to_base = false;
  }
}
# 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Encoder.ino"
// INTERRUPTY Z ENCODERA
void encoder_input_a()
{

  if (digitalRead(2) == 0x1)
  {
    if (digitalRead(3) == 0x0)
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
    if (digitalRead(3) == 0x0)
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

  if (digitalRead(3) == 0x1)
  {
    if (digitalRead(2) == 0x0)
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
    if (digitalRead(2) == 0x0)
    {
      Encoder_state++;
    }
    else
    {
      Encoder_state--;
    }
  }
}
# 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void go_to_base()
{

  if (!is_base_end_stop_clicked())
  {
    go_to_base_signal = true;
  }
  delay_between_steps = fastest_possible_delay_between;
  move_up();
  Encoder_state = 0;
  incremental_stepper_motor_current_step = 0;
  Serial.println("BASED");
  can_go_auto = true;
}
void move_up()
{
  set_direction_to_go_up();
  while ((digitalRead(29) == 0x1 && digitalRead(23) == 0x1) || go_to_base_signal == true)
  {
    make_step();
    if (is_base_end_stop_clicked())
    {
      go_to_base_signal = false;
    }
  }
}

void move_down()
{
  set_direction_to_go_down();
  while (digitalRead(22) == 0x1)
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
  if (digitalRead(22) == 0x1)
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
  return (digitalRead(24) == 0x0) ? true : false;
}

bool is_base_end_stop_clicked()
{
  return (digitalRead(29) == 0x1) ? false : true;
}
# 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Stepper_Movement.ino"
void make_step()
{
  digitalWrite(27, !digitalRead(27));
  delayMicroseconds(delay_between_steps);
  if (digitalRead(28) == 0x1)
  {
    incremental_stepper_motor_current_step += 1;
  }
  else
  {
    incremental_stepper_motor_current_step -= 1;
  }
}

void set_direction_to_go_up()
{
  if (digitalRead(28) == 0x1)
  {
    digitalWrite(28, 0x0);
  }
}

void set_direction_to_go_down()
{
  if (digitalRead(28) == 0x0)
  {
    digitalWrite(28, 0x1);
  }
}
