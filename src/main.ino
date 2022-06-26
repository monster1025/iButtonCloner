void setup() {
    Serial.begin(115200);

    setup_display();
    setup_ibutton();
    setup_power();
    setup_button();
    
    //init state
    read_items();
    display_menu();
}

void loop() {
    process_ibutton();
    button_loop();
    power_loop();

    delay(50);
}
