#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// shortcuts
#define log Serial.println
#define diputer .write
#define bacaBenda (!digitalRead(IRSensor1)) || (!digitalRead(IRSensor2))
#define detik *1000

// inputs
#define metalSensor 6
#define IRSensor1 4
#define IRSensor2 5

// distanceSensor
#define echo 12
#define triger 13

// global variables
const int inputs[] = {metalSensor, IRSensor1, IRSensor2};

// servo
#define servoAtas 13
#define servoBawah 12

const int servoPin[] = {servoAtas, servoBawah};
Servo servo[sizeof(servoPin) / sizeof(servoPin[0])];
#define atas [0]
#define bawah [1]

// rotations
#define posisiTumpah 0
#define posisiTutup 15
#define posisiBuka 150
#define metalRotation 130
#define nonMetalRotation 13

// LCD
#define LCD_ROW 2
#define LCD_COLMN 16
LiquidCrystal_I2C lcd_(0x27, LCD_COLMN, LCD_ROW);

// lcd function
struct
{
    String currentText[2];
    unsigned long int nextUpdate, updateDelay;
    bool comingUpdate = false;
    bool initialized = false;

    // replace all text with this one
    void print(String top, String bottom = "", bool force = false)
    {
        if (top == currentText[0] && bottom == currentText[1] && !force)
            return;
        if (!initialized)
        {
            lcd_.begin();
            initialized = true;
        }

        String text[2] = {top, bottom};
        if (force)
            lcd_.clear();
        // update the text
        for (int y = 0; y < LCD_ROW; y++)
        {
            lcd_.setCursor(0, y);
            if (force)
                lcd_.print(text[y]);
            else
                for (int x = 0; x < LCD_COLMN + 1; x++)
                {
                    lcd_.setCursor(x, y);
                    if (!force && currentText[y][x] != text[y][x])
                        if (text[y].length() > x)
                            lcd_.print(String(text[y][x]));
                        else
                            lcd_.print(F(" "));
                }
        }
        currentText[0] = top;
        currentText[1] = bottom;
    }
} lcd;

void setup()
{
    Serial.begin(115200);

    // pinmodes
    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++)
        pinMode(inputs[i], INPUT);
    for (int i = 0; i < sizeof(servoPin) / sizeof(servoPin[0]); i++)
        servo[i].attach(servoPin[i]);
    servo atas diputer(posisiBuka);
    lcd.print(F("Selamat datang"), F("Robobin"));
    delay(1 detik);
}

void loop()
{
    lcd.print(F("Silahkan letakan"), F("sampah mu disini"));
    bool trashDetected = bacaBenda;

    // if theris trash
    if (trashDetected)
    {
        String bottom = F("terdeteksi");
        for (int i = 0; i < 3; i++)
        {
            bottom += F(".");
            lcd.print(F("sampah"), bottom);
            delay(500);
        }
        bool isMetal = !digitalRead(metalSensor);
        lcd.print(isMetal ? F("sampah metal") : F("bukan sampah"), isMetal ? F("terdeteksi") : F("metal"));
        trashDetected = bacaBenda;

        if (!trashDetected){
            lcd.print(F("Eits tapi bo'ong"));
            delay(1 detik);
            return;
        }

        servo bawah diputer(isMetal ? metalRotation : nonMetalRotation);
        delay(300);
        servo atas diputer(posisiTumpah);
        delay(1000);
        servo atas diputer(posisiBuka);
        delay(1000);
    }
}