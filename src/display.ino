#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED FeatherWing buttons map to different pins depending on board.
// The I2C (Wire) bus may also be different.
#define WIRE Wire
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);
byte ReadID1[8] = { 0x01, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F
byte ReadID2[8] = { 0x01, 0x2F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F
byte ReadID3[8] = { 0x01, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F
byte ReadID4[8] = { 0x01, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F
#define MENU_ITEMS_COUNT 4
int selectedMenuItem = 1;

void clear() {
    display.setCursor(0,0);
    display.clearDisplay();
    display.display();
}

void setup_display(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    //clear adafrout logo buffer
    display.clearDisplay();

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Domofon Clone Tool");
    display.println("      v1.0.0      ");
    display.display();
    delay(2000);
}

void display_sleep(){
    display.clearDisplay();
    display.display();
}

void display_menu(){
    display.clearDisplay();
    display.setCursor(0,0);
    for (int i = 1; i <= MENU_ITEMS_COUNT; i++)
    {
        print_item(i);
    }
    display.display();
}

byte* get_item(int item)
{
    if (item == 1){
        return ReadID1;
    }
    if (item == 2){
        return ReadID2;
    }
    if (item == 3){
        return ReadID3;
    }
    if (item == 4){
        return ReadID4;
    }

    return ReadID1;
}

void select_next_menu_item() 
{
    selectedMenuItem++;
    if (selectedMenuItem > MENU_ITEMS_COUNT)
    {
        selectedMenuItem = 1;
    }
    ibutton_read_mode();
    display_menu();
}

void clear_current_menu_item() 
{
    set_default_key(selectedMenuItem);
}

void print_item(int item)
{
    if (selectedMenuItem == item)
    {
        if (ibutton_get_mode() == 0){
            // display.print(">");
            display.print(selectedMenuItem);
            display.print(">");
        }
        else
        {
            display.print("w>");
        }
    } else {
        display.print("  ");
    }
    byte* currentId = get_item(item);
    char buffer[2];
    for (int i = 0; i < 8; i++)
    {
        //Пропустим первый байт если он 01 -- это значение по дефолту для всех меток.
        if (i == 0 && currentId[i] == 1){
            continue;
        }
        sprintf(buffer, "%02X", currentId[i]);
        display.print(buffer);
    }
    display.println();
}