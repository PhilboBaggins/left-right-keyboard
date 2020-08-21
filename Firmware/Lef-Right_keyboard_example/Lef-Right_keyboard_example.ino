#include <Bounce2.h>

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[] = {5, 6, 11, 12, 14, 15, 18, 21};
Bounce* buttons = new Bounce[NUM_BUTTONS + 1];
const int DEBOUNCE_MS = 25;

#define LED_PIN_LEFT 9
#define LED_PIN_RIGHT 10

bool ledState = false;

const int DELAY_BETWEEN_PRESS_AND_RELEASE_MS = 0;

void sendModifiedKey(int modifier, int key)
{
    Keyboard.set_modifier(modifier);
    Keyboard.set_key1(key);
    Keyboard.send_now();

    delay(DELAY_BETWEEN_PRESS_AND_RELEASE_MS);

    Keyboard.set_key1(0);
    Keyboard.set_modifier(0);
    Keyboard.send_now();
}

void setup()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].attach(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i].interval(DEBOUNCE_MS);
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, ledState);
/*
    pinMode(LED_PIN_LEFT, OUTPUT);
    digitalWrite(LED_PIN_LEFT, ledState);

    pinMode(LED_PIN_RIGHT, OUTPUT);
    digitalWrite(LED_PIN_RIGHT, ledState);
*/
}

void loop()
{
    bool needToToggleLed = false;

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].update();
        if (buttons[i].fell())
        {
            Serial.print("Button number ");
            Serial.print(i);
            Serial.println(" pressed");
            needToToggleLed = true;
        }
    }

    if (needToToggleLed)
    {
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        //digitalWrite(LED_PIN_LEFT, ledState);
        //digitalWrite(LED_PIN_RIGHT, ledState);
    }
}
