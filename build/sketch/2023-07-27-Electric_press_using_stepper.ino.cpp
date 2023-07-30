#include <Arduino.h>
#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
#define interrupt_PIN_a 2
#define interrupt_PIN_b 3
#define PUL 27
#define EN 26
#define DIR 28
#define DOWN_BUTTON 22
#define UP_BUTTON 23
#define BASE_BUTTON 24
#define END_STOP 29
#define pot A0
long current_encoder_state = 0;
long stepper_motor_current_step = 0;
int delay_between_steps = 60;
int fastest_possible_delay_between = 60;
long set_position = 198000;
volatile long Encoder_state = 0;
bool go_to_base_signal = false;
bool is_based = false;
bool can_go_to_base = false;
bool can_go_auto = false;

#line 22 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
void setup();
#line 42 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
void loop();
#line 2 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Encoder.ino"
void encoder_input_a();
#line 28 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Encoder.ino"
void encoder_input_b();
#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void go_to_base();
#line 11 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void move_up();
#line 47 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void move_down();
#line 56 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void move_to_set_position();
#line 97 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
void base_move_decider();
#line 114 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
bool is_base_button_clicked();
#line 120 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
bool is_base_end_stop_clicked();
#line 126 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
bool is_UP_BUTTON_clicked();
#line 132 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
bool is_DOWN_BUTTON_clicked();
#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Stepper_Movement.ino"
void make_step();
#line 15 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Stepper_Movement.ino"
void set_direction_to_go_up();
#line 23 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Stepper_Movement.ino"
void set_direction_to_go_down();
#line 22 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\2023-07-27-Electric_press_using_stepper.ino"
void setup()
{
  pinMode(interrupt_PIN_a, INPUT_PULLUP);
  pinMode(interrupt_PIN_b, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_PIN_a), encoder_input_a, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interrupt_PIN_b), encoder_input_b, CHANGE);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(UP_BUTTON, INPUT);
  pinMode(BASE_BUTTON, INPUT);
  pinMode(pot, INPUT);
  pinMode(END_STOP, INPUT); // set the input pin as input
  pinMode(PUL, OUTPUT);     // set the first output pin as output
  pinMode(EN, OUTPUT);      // set the second output pin as output
  pinMode(DIR, OUTPUT);     // set the third output pin as output
  digitalWrite(EN, HIGH);
  digitalWrite(DIR, LOW);
  digitalWrite(PUL, HIGH);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(DOWN_BUTTON) == LOW && digitalRead(UP_BUTTON) == LOW)
  {
    int analogValue = analogRead(pot);                        // Read analog value from A0
    delay_between_steps = map(analogValue, 0, 1023, 300, 40); // Map the analog value to a range of 0-255
  }
  if (digitalRead(DOWN_BUTTON) == HIGH && digitalRead(UP_BUTTON) == LOW)
  {
    move_down();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(stepper_motor_current_step));
  }
  if (digitalRead(UP_BUTTON) == HIGH && digitalRead(DOWN_BUTTON) == LOW)
  {
    move_up();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(stepper_motor_current_step));
  }
  while (digitalRead(BASE_BUTTON) == LOW)
  {
    base_move_decider();
  }
  if (can_go_to_base)
  {
    go_to_base();
    can_go_to_base = false;
  }
}

#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Encoder.ino"
// INTERRUPTY Z ENCODERA
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

#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Press_Logic.ino"
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
#line 1 "D:\\Programowanie Arduino\\2023-07-27-Electric_press_using_stepper\\Stepper_Movement.ino"
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

