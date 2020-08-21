#include <Bounce2.h>

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[] = {5, 6, 11, 12, 14, 15, 18, 21};
Bounce* buttons = new Bounce[NUM_BUTTONS];
const int DEBOUNCE_MS = 25;

const int LED_PIN_LEFT = 9;
const int LED_PIN_RIGHT = 10;

const int MODIFIER_KEY_NONE = 0;
int modifierKeys = MODIFIER_KEY_NONE;

void DescribeModifiers()
{
    Serial.print("Modifiers: 0x");
    Serial.print(modifierKeys, HEX);
    if ((modifierKeys & 0xFF & MODIFIERKEY_CTRL) > 0)
        Serial.print(" CTRL");
    if ((modifierKeys & 0xFF & MODIFIERKEY_ALT) > 0)
        Serial.print(" ATL");
    if ((modifierKeys & 0xFF & MODIFIERKEY_SHIFT) > 0)
        Serial.print(" SHIFT");
    Serial.println("");
}

void setModifierKey(int modifier)
{
    modifierKeys |= modifier;
    Keyboard.set_modifier(modifierKeys);
    //DescribeModifiers();
}

void clearModifierKey(int modifier)
{
    modifierKeys &= ~modifier;
    Keyboard.set_modifier(modifierKeys);
    //DescribeModifiers();
}

void pressLeft(int modifier)
{
    setModifierKey(modifier);
    Keyboard.set_key1(KEY_LEFT);
}

void releaseLeft(int modifier)
{
    clearModifierKey(modifier);
    Keyboard.set_key1(0);
}

void pressRight(int modifier)
{
    setModifierKey(modifier);
    Keyboard.set_key2(KEY_RIGHT);
}

void releaseRight(int modifier)
{
    clearModifierKey(modifier);
    Keyboard.set_key2(0);
}

void pressUp()
{
    Keyboard.set_key3(KEY_UP);
}

void releaseUp()
{
    Keyboard.set_key3(0);
}

void pressDown()
{
    Keyboard.set_key4(KEY_DOWN);
}

void releaseDown()
{
    Keyboard.set_key4(0);
}

void setup()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].attach(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i].interval(DEBOUNCE_MS);
    }

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
/*
    pinMode(LED_PIN_LEFT, OUTPUT);
    digitalWrite(LED_PIN_LEFT, LOW);

    pinMode(LED_PIN_RIGHT, OUTPUT);
    digitalWrite(LED_PIN_RIGHT, LOW);
*/
}

void loop()
{
    bool anyButtonDown = false;

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i].update();

        if (buttons[i].read() == LOW)
        {
            anyButtonDown = true;
        }

        if (buttons[i].fell())
        {
            switch (i)
            {
                case 0: pressLeft(MODIFIERKEY_CTRL); break;
                case 1: pressLeft(MODIFIERKEY_ALT); break;
                case 2: pressLeft(MODIFIERKEY_SHIFT); break;
                case 3: pressDown(); break;
                case 4: pressUp(); break;
                case 5: pressRight(MODIFIERKEY_SHIFT); break;
                case 6: pressRight(MODIFIERKEY_ALT); break;
                case 7: pressRight(MODIFIERKEY_CTRL); break;
            }
            Keyboard.send_now();
        }
        else if (buttons[i].rose())
        {
            switch (i)
            {
                case 0: releaseLeft(MODIFIERKEY_CTRL); break;
                case 1: releaseLeft(MODIFIERKEY_ALT); break;
                case 2: releaseLeft(MODIFIERKEY_SHIFT); break;
                case 3: releaseDown(); break;
                case 4: releaseUp(); break;
                case 5: releaseRight(MODIFIERKEY_SHIFT); break;
                case 6: releaseRight(MODIFIERKEY_ALT); break;
                case 7: releaseRight(MODIFIERKEY_CTRL); break;
            }
            Keyboard.send_now();
        }
    }

    if (anyButtonDown)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        //digitalWrite(LED_PIN_LEFT, HIGH);
        //digitalWrite(LED_PIN_RIGHT, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
        //digitalWrite(LED_PIN_LEFT, LOW);
        //digitalWrite(LED_PIN_RIGHT, LOW);
    }
}
