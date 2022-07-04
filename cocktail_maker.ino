const int PUMP_COUNT = 4;
const byte power_pins[PUMP_COUNT] = { 5, 4, 7, 6 };
const byte pwm_pins[PUMP_COUNT] = { 9, 3, 10, 11 };
const byte button_pins[PUMP_COUNT] = { 2, 8, 12, 13 };

class Pump {
public:
    byte power_pin;
    byte pwm_pin;
    byte button_pin;
    int flow_rate;
    bool state;
    Pump() {};
    Pump(byte _power_pin, byte _pwm_pin, byte _button_pin, int _flow_rate) {
        power_pin = _power_pin;
        pwm_pin = _pwm_pin;
        button_pin = _button_pin;
        flow_rate = _flow_rate;
        state = false;
    };
    void change_state() {
        state = !state;
        if (state == true) {
            int flow_rate_ten_bit = map(flow_rate, 0, 100, 0, 1023);
            digitalWrite(power_pin, HIGH);
            analogWrite(pwm_pin, flow_rate_ten_bit);
        }
        if (state == false) {
            digitalWrite(power_pin, LOW);
            analogWrite(pwm_pin, 0);
        }
    }
};

class Bottle {
public:
    String contents;
    int capacity;
    int remaining;
    bool empty;
    int pump_number;
    Bottle() {};
    Bottle(String _contents, int _capacity, int _pump_number) {
        empty = false;
        capacity = _capacity;
        remaining = capacity;
        pump_number = _pump_number;
    };

};

Pump pumps[PUMP_COUNT];

void setup() {
    for (int i = 0; i < PUMP_COUNT; i++) {
        pumps[i] = Pump(power_pins[i], pwm_pins[i], button_pins[i], 100);
        digitalWrite(pumps[i].power_pin, LOW);
        analogWrite(pumps[i].pwm_pin, 0);
    }
}

void loop() {
    for (int i = 0; i < PUMP_COUNT; i++) {
        bool pump_turned_on = digitalRead(pumps[i].button_pin) == HIGH && pumps[i].state == false;
        bool pump_turned_off = digitalRead(pumps[i].button_pin) == LOW && pumps[i].state == true;
        if (pump_turned_on || pump_turned_off) {
            pumps[i].change_state();
        }
    }
}
