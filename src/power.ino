#include <gyverpower.h>
int power_loops = 0;
#define SLEEP_MODE_DELAY 500

void setup_power(){
    power.autoCalibrate(); // автоматическая калибровка ~ 2 секунды , средняя но достаточная точность
    power.setSleepMode(POWERDOWN_SLEEP); // если нужен другой режим сна, см константы в GyverPower.h (по умолчанию POWERDOWN_SLEEP)

    //power.hardwareDisable(PWR_ADC | PWR_TIMER1); // см раздел константы в GyverPower.h, разделяющий знак " | "
    //power.setSystemPrescaler(PRESCALER_2); // см константы в GyverPower.h
    //power.setSleepMode(STANDBY_SLEEP);
    power_loops = 0;
}

void isr_wakeup() {
    power.wakeUp();
}

void power_sleepDelay(int delayMs){
    power.sleepDelay(delayMs); // спим 1.5 секунды
    //wake up display
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    //display.display();
}

void display_wake(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    clear();
}

void reset_sleep(){
    power_loops = 0;
}

void power_loop()
{
    disable_isr();

    power_loops++;
    if (power_loops > SLEEP_MODE_DELAY)
    {
        power_loops = 0;
        
        //Отключаем дисплей
        display_sleep();
        enable_isr();

        //Засыпаем до прерывания.
        power.sleep(SLEEP_FOREVER);
    }
}