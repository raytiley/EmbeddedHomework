#include <pololu/orangutan.h>
#define TRUE 1
#define FALSE 0
#define ENCODER_COUNT_PER_REVOLUTION 333.33

void print_count(float count)
{
  //Some ugly stuff to print out a floating point number.
  lcd_goto_xy(0,1);
  print("Revs: ");
  print_long(count);
  print_character('.');
  int decimal = (count - (int)count) * 100;
  
  if(decimal < 10)
    print_character('0');

  print_long(decimal);
}

int main()
{
  // Initialize the encoders
  encoders_init(IO_D2, IO_D3, IO_A3, IO_A2);
  float count = 0;
  char forward = TRUE;
  int motor_speed = 100;

  //set_motors(100, 255);
  while(1)
  {
    clear();
    lcd_goto_xy(0,0);

    //Calculate the count
    count = encoders_get_counts_m1() / ENCODER_COUNT_PER_REVOLUTION;

    // Increment and decrement speed if top or bottom buttons were pressed.
    unsigned char button = get_single_debounced_button_press(ANY_BUTTON);

    if ((button & TOP_BUTTON) && motor_speed < 250) // if top button pressed
      motor_speed = motor_speed + 5;
    if ((button & BOTTOM_BUTTON) && motor_speed > 80) // if bottom button pressed
      motor_speed = motor_speed - 5;

    //If the user is holding the middle button we want to stop the motor.
    if(button_is_pressed(MIDDLE_BUTTON) & MIDDLE_BUTTON)
    {
      print("Paused ");
      set_motors(0,0);
    }
    else
    {
      // Set motor speed dependent on direction.
      if(forward)
      {
        set_motors(motor_speed, 0);
        print("Forward ");
      }
      else
      {
        set_motors(-motor_speed, 0);
        print("Reverse ");
      }

      // At 0 and 2 we need to switch directions.
      if(count >= 2 && forward)
        forward = FALSE;

      if(count <= 0 && !forward)
        forward = TRUE;
    }

    // Print the motor speed and revolutions
    print_long(motor_speed); 
    print_count(count); 

    //Delay so LCD doesn't flicker too much
    delay_ms(50);
  }
}
