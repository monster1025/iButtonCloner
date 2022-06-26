#include <EncButton.h>

#define BTN_PIN 2        // кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)
EncButton<EB_TICK, BTN_PIN> butt1;           // просто кнопка
int hold_cnt = 0;

void isr() {
  isr_wakeup();
  butt1.tickISR();  // опрашиваем в прерывании, чтобы поймать нажатие в любом случае
}

void setup_button() {
    enable_isr();
    pinMode(BTN_PIN, INPUT_PULLUP);
}

void enable_isr() {
    attachInterrupt(1, isr, CHANGE);
}

void disable_isr(){
    detachInterrupt(1);
}

void button_loop() {
    butt1.tick();  // опрашиваем в скетче, иначе не будут работать проверки по времени!

    if (butt1.click())
    {
        reset_sleep();
        select_next_menu_item();
        display_menu();
        return;
    }
    if (butt1.held())
    {
        reset_sleep();
        ibutton_write_mode();
        display_menu();
    }
    if (butt1.hold())
    {
        hold_cnt++;
        if (hold_cnt > 100)
        {
            reset_sleep();
            clear_current_menu_item();
            display_menu();
        }
    }
    else 
    {
        hold_cnt = 0;
    }
}
