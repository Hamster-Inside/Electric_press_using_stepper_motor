bool is_BASE_BUTTON_clicked()
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

int get_speed_from_potentiometer()
{
    return map(analogRead(pot), 0, 1023, 300, 40);
}