// This is an example to interface a incremental rotary encoder with a digital potentiometer in STM32. 
// Make sure core stm32duino core is installed and configured properly. 
// For arduino, call encoder_read() in main loop though it recommended for testing only. 

#include <DigiPotX9Cxxx.h>

// Encoder
#define ENCODER_SW PB6
#define ENCODER_CLK PB7
#define ENCODER_DT PB8
#define ANTI_CLOCKWISE -1
#define CLOCKWISE 1

#define POT_DIR PA1
#define POT_INC PA2
#define POT_CS PA3

DigiPot pot(POT_INC, POT_DIR, POT_CS);
bool setup_end = false;

void setup() 
{
  Serial.begin(115200);
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  setup_end = true;
  pot.reset();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    uint8_t value = cmd.toInt();
    pot.set(value);
    // Serial.println(pot.get());
  }
}

void encoder_read()
{
  volatile static uint8_t ABs = 0;
  int8_t dir = 0;
  ABs = (ABs << 2) & 0x0f;
  ABs |= (digitalRead(ENCODER_CLK) << 1) | digitalRead(ENCODER_DT);
  switch(ABs)
  {
    // CLOCKWISE
    case 0x0d:
    dir = 1;
    pot.decrease(1);
    break;

    //ANIT-CLOCKWISE
    case 0x0e:
    dir = -1;
    pot.increase(1);
    break;
  }
  if(dir)
  {
    Serial.println(pot.get());
  }
}

void HAL_SYSTICK_Callback()
{
  if (setup_end)
  {
    encoder_read();
  }
}
