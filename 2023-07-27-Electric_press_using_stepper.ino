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
long set_position = 198000; // value based on readings from encoder, in future teach using buttons
volatile long Encoder_state = 0;
bool go_to_base_signal = false;
bool is_based = false;
bool can_go_to_base = false;
bool can_go_auto = false;

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
  if (!is_DOWN_BUTTON_clicked() && !is_UP_BUTTON_clicked())
  {
    delay_between_steps = get_speed_from_potentiometer();
  }
  if (is_DOWN_BUTTON_clicked() && !is_UP_BUTTON_clicked())
  {
    move_down();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(stepper_motor_current_step));
  }
  if (is_UP_BUTTON_clicked() && !is_DOWN_BUTTON_clicked())
  {
    move_up();
    Serial.println("Encoder state POS = " + String(Encoder_state));
    Serial.println("Stepper motor position = " + String(stepper_motor_current_step));
  }
  while (is_BASE_BUTTON_clicked())
  {
    base_move_decider();
  }
  if (can_go_to_base)
  {
    go_to_base();
    can_go_to_base = false;
  }
}
