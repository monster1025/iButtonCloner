#include <EEPROM.h>

void read_items(){
    for (int i = 1; i <= 4; i++)
    {
        read_item(i);
    }
}

void read_item(int itemNumber)
{
    byte* currentItem = get_item(itemNumber);
    int address = itemNumber * 8;
    bool isEmpty = true;
    for (int i = 0; i < 8; i++)
    {
        EEPROM.get(address + i, currentItem[i]);
        if (currentItem[i] != 0xFF){
            isEmpty = false;
        }
    }
    //universal key if empty
    if (isEmpty){
        set_default_key(itemNumber);
    }
}

void set_default_key(int itemNumber) 
{
    byte* currentItem = get_item(itemNumber);
    //universal key if empty
    currentItem[0] = 0x01;
    currentItem[1] = 0x1F;
    currentItem[2] = 0xFF;
    currentItem[3] = 0xFF;
    currentItem[4] = 0xFF;
    currentItem[5] = 0xFF;
    currentItem[6] = 0xFF;
    currentItem[7] = 0x2F;
    write_item(itemNumber);
}

void write_item(int itemNumber){
    byte* currentItem = get_item(itemNumber);
    int address = itemNumber * 8;
    for (int i = 0; i < 8; i++)
    {
        EEPROM.write(address + i, currentItem[i]);
    }
}