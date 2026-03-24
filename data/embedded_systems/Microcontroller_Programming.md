# Microcontroller Programming

This file contains comprehensive microcontroller programming examples in C, including GPIO operations, timers, PWM, ADC, communication protocols, interrupts, and power management for embedded systems.

## 📚 Microcontroller Overview

### 🔌 Hardware Components
- **GPIO**: General Purpose Input/Output pins
- **Timers/Counters**: Precise timing and pulse generation
- **ADC**: Analog to Digital Conversion
- **Communication**: SPI, I2C, UART protocols
- **Memory**: EEPROM and Flash storage
- **Power Management**: Sleep modes and watchdog

### 🎯 Programming Concepts
- **Register Manipulation**: Direct hardware access
- **Interrupts**: Event-driven programming
- **Real-time Constraints**: Timing-critical operations
- **Resource Management**: Limited memory and processing

## 🔌 GPIO (General Purpose I/O)

### Pin Configuration
```c
// Pin definitions
#define PA0 0
#define PA1 1
#define PA2 2
#define PB0 0
#define PB1 1

// Bit manipulation macros
#define SET_BIT(reg, bit)    ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1 << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1)
```

### GPIO Operations
```c
void pinMode(uint8_t port, uint8_t pin, uint8_t mode) {
    if (port == 'A') {
        if (mode == 0) {       // Input
            CLEAR_BIT(regs.DDRA, pin);
        } else {                // Output
            SET_BIT(regs.DDRA, pin);
        }
    } else if (port == 'B') {
        if (mode == 0) {       // Input
            CLEAR_BIT(regs.DDRB, pin);
        } else {                // Output
            SET_BIT(regs.DDRB, pin);
        }
    }
}

void digitalWrite(uint8_t port, uint8_t pin, uint8_t value) {
    if (port == 'A') {
        if (value == 0) {
            CLEAR_BIT(regs.PORTA, pin);
        } else {
            SET_BIT(regs.PORTA, pin);
        }
    } else if (port == 'B') {
        if (value == 0) {
            CLEAR_BIT(regs.PORTB, pin);
        } else {
            SET_BIT(regs.PORTB, pin);
        }
    }
}

uint8_t digitalRead(uint8_t port, uint8_t pin) {
    if (port == 'A') {
        return READ_BIT(regs.PINA, pin);
    } else if (port == 'B') {
        return READ_BIT(regs.PINB, pin);
    }
    return 0;
}
```

### LED Blinking Example
```c
void blinkLED() {
    // Configure LED pin as output
    pinMode('A', PA0, 1);
    
    while (1) {
        digitalWrite('A', PA0, 1);  // LED ON
        delay(1000);                  // Wait 1 second
        
        digitalWrite('A', PA0, 0);  // LED OFF
        delay(1000);                  // Wait 1 second
    }
}
```

### Button Reading Example
```c
void readButton() {
    // Configure button pin as input
    pinMode('B', PB0, 0);
    
    // Enable internal pull-up
    digitalWrite('B', PB0, 1);
    
    while (1) {
        if (digitalRead('B', PB0) == 0) {
            // Button pressed
            digitalWrite('A', PA0, 1);  // Turn on LED
        } else {
            digitalWrite('A', PA0, 0);  // Turn off LED
        }
        
        delay(50); // Debounce delay
    }
}
```

## ⏱️ Timer Operations

### Timer Configuration
```c
// Timer prescaler values
#define TIMER_NO_PRESCALER   1
#define TIMER_PRESCALER_8    8
#define TIMER_PRESCALER_64   64
#define TIMER_PRESCALER_256  256
#define TIMER_PRESCALER_1024 1024

// Timer modes
#define TIMER_NORMAL     0
#define TIMER_CTC        (1 << WGM01)
#define TIMER_PWM        (1 << WGM00) | (1 << WGM01)

void timerInit(uint8_t mode, uint8_t prescaler) {
    // Set wave generation mode
    regs.TCCR0A &= ~((1 << WGM01) | (1 << WGM00));
    regs.TCCR0A |= mode;
    
    // Set prescaler
    regs.TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    switch (prescaler) {
        case TIMER_NO_PRESCALER:
            regs.TCCR0B |= (1 << CS00);
            break;
        case TIMER_PRESCALER_8:
            regs.TCCR0B |= (1 << CS01);
            break;
        case TIMER_PRESCALER_64:
            regs.TCCR0B |= (1 << CS01) | (1 << CS00);
            break;
        case TIMER_PRESCALER_256:
            regs.TCCR0B |= (1 << CS02);
            break;
        case TIMER_PRESCALER_1024:
            regs.TCCR0B |= (1 << CS02) | (1 << CS00);
            break;
    }
}
```

### Timer Interrupt Example
```c
volatile uint16_t milliseconds = 0;

void timerInterrupt() {
    milliseconds++;
}

void setupTimerInterrupt() {
    // Configure timer for 1ms interrupts
    timerInit(TIMER_CTC, TIMER_PRESCALER_64);
    timerSetCompare(250);  // 16MHz / 64 / 250 = 1kHz
    
    // Enable compare match interrupt
    SET_BIT(regs.TIMSK0, OCIE0A);
    
    // Enable global interrupts
    sei();
}
```

## 🌊 PWM (Pulse Width Modulation)

### PWM Configuration
```c
void pwmInit(uint8_t pin, uint16_t frequency) {
    // Configure timer for Fast PWM mode
    regs.TCCR0A |= (1 << WGM00) | (1 << WGM01);
    regs.TCCR0A |= (1 << COM0A1);  // Non-inverting mode
    
    // Calculate prescaler for desired frequency
    uint16_t prescaler = F_CPU / (frequency * 256UL) - 1;
    if (prescaler <= 1) {
        regs.TCCR0B |= (1 << CS00);
    } else if (prescaler <= 8) {
        regs.TCCR0B |= (1 << CS01);
    } else if (prescaler <= 64) {
        regs.TCCR0B |= (1 << CS01) | (1 << CS00);
    } else if (prescaler <= 256) {
        regs.TCCR0B |= (1 << CS02);
    } else {
        regs.TCCR0B |= (1 << CS02) | (1 << CS00);
    }
}

void pwmSetDuty(uint8_t duty) {
    regs.OCR0A = duty;
}
```

### LED Dimming Example
```c
void fadeLED() {
    pwmInit(PA0, 1000);  // 1kHz PWM
    
    while (1) {
        // Fade in
        for (uint8_t i = 0; i <= 255; i++) {
            pwmSetDuty(i);
            delay(10);
        }
        
        // Fade out
        for (uint8_t i = 255; i > 0; i--) {
            pwmSetDuty(i);
            delay(10);
        }
    }
}
```

## 📊 ADC (Analog to Digital Converter)

### ADC Configuration
```c
#define ADC_CHANNELS 8
#define ADC_RESOLUTION 1024

// ADC reference voltage
#define ADC_REF_AREF   0
#define ADC_REF_AVCC   1
#define ADC_REF_INTERNAL 3

// ADC prescaler values
#define ADC_PRESCALER_2    1
#define ADC_PRESCALER_4    2
#define ADC_PRESCALER_8    3
#define ADC_PRESCALER_16   4
#define ADC_PRESCALER_32   5
#define ADC_PRESCALER_64   6
#define ADC_PRESCALER_128  7

void adcInit(uint8_t reference, uint8_t prescaler) {
    // Set reference voltage
    adc_regs.ADMUX &= ~((1 << REFS1) | (1 << REFS0));
    adc_regs.ADMUX |= (reference << REFS0);
    
    // Set prescaler
    adc_regs.ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
    adc_regs.ADCSRA |= (prescaler << ADPS0);
    
    // Enable ADC
    SET_BIT(adc_regs.ADCSRA, ADEN);
}

uint16_t adcRead(uint8_t channel) {
    // Select channel
    adc_regs.ADMUX &= 0xF0;
    adc_regs.ADMUX |= (channel & 0x0F);
    
    // Start conversion
    SET_BIT(adc_regs.ADCSRA, ADSC);
    
    // Wait for conversion complete
    while (READ_BIT(adc_regs.ADCSRA, ADSC));
    
    // Read result
    uint16_t result = (adc_regs.ADCH << 8) | adc_regs.ADCL;
    return result;
}

float adcToVoltage(uint16_t adc_value, float vref) {
    return (adc_value * vref) / ADC_RESOLUTION;
}
```

### Sensor Reading Examples
```c
// Temperature sensor (LM35)
float readTemperature() {
    uint16_t adc_value = adcRead(0); // Read from ADC channel 0
    return adcToVoltage(adc_value, 5.0) * 100.0; // LM35: 10mV/°C
}

// Light sensor (LDR)
float readLightLevel() {
    uint16_t adc_value = adcRead(1); // Read from ADC channel 1
    return adcToVoltage(adc_value, 5.0);
}

// Potentiometer
float readPotentiometer() {
    uint16_t adc_value = adcRead(2); // Read from ADC channel 2
    return (adc_value * 100.0) / ADC_RESOLUTION; // 0-100%
}
```

## 📡 Communication Protocols

### SPI (Serial Peripheral Interface)
```c
void spiInit(uint8_t mode, uint8_t prescaler) {
    // Enable SPI, set as master
    regs.SPCR |= (1 << SPE) | (1 << MSTR);
    
    // Set clock polarity and phase
    regs.SPCR &= ~((1 << CPOL) | (1 << CPHA));
    regs.SPCR |= (mode << CPHA);
    
    // Set prescaler
    regs.SPCR &= ~((1 << SPR1) | (1 << SPR0));
    regs.SPCR |= (prescaler << SPR0);
}

uint8_t spiTransfer(uint8_t data) {
    regs.SPDR = data;
    
    // Wait for transmission complete
    while (!READ_BIT(regs.SPSR, SPIF));
    
    return regs.SPDR;
}

void spiWrite(uint8_t data) {
    spiTransfer(data);
}

uint8_t spiRead() {
    return spiTransfer(0xFF); // Dummy transfer to read
}
```

### I2C (Inter-IC Communication)
```c
void i2cInit(uint32_t frequency) {
    // Set bit rate
    uint8_t bit_rate = (F_CPU / frequency - 16) / (2 * 1); // Prescaler = 1
    i2c_regs.TWBR = bit_rate;
    
    // Enable TWI
    SET_BIT(i2c_regs.TWCR, TWEN);
}

uint8_t i2cStart(uint8_t address) {
    // Send START condition
    SET_BIT(i2c_regs.TWCR, TWSTA);
    SET_BIT(i2c_regs.TWCR, TWINT);
    CLEAR_BIT(i2c_regs.TWCR, TWSTO);
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
    
    // Send device address
    i2c_regs.TWDR = (address << 1) | 0; // Write mode
    SET_BIT(i2c_regs.TWCR, TWINT);
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
    
    return (i2c_regs.TWSR & 0xF8) == 0x18; // ACK received
}

void i2cWrite(uint8_t data) {
    i2c_regs.TWDR = data;
    SET_BIT(i2c_regs.TWCR, TWINT);
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
}

uint8_t i2cRead(uint8_t ack) {
    SET_BIT(i2c_regs.TWCR, TWINT);
    if (ack) {
        SET_BIT(i2c_regs.TWCR, TWEA);
    } else {
        CLEAR_BIT(i2c_regs.TWCR, TWEA);
    }
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
    
    return i2c_regs.TWDR;
}

void i2cStop() {
    SET_BIT(i2c_regs.TWCR, TWSTO);
    SET_BIT(i2c_regs.TWCR, TWINT);
}
```

### UART (Universal Asynchronous Receiver/Transmitter)
```c
void uartInit(uint32_t baudrate) {
    // Calculate baud rate register value
    uint16_t baud_prescale = (F_CPU / (16UL * baudrate)) - 1;
    regs.UBRR = baud_prescale;
    
    // Enable transmitter and receiver
    SET_BIT(regs.UCSRB, TXEN);
    SET_BIT(regs.UCSRB, RXEN);
    
    // Set frame format: 8 data bits, 1 stop bit
    CLEAR_BIT(regs.UCSRC, UCSZ0);
    SET_BIT(regs.UCSRC, UCSZ1);
}

void uartTransmit(uint8_t data) {
    // Wait for empty transmit buffer
    while (!READ_BIT(regs.UCSRA, UDRE));
    
    // Put data into buffer
    regs.UDR = data;
}

uint8_t uartReceive() {
    // Wait for data to be received
    while (!READ_BIT(regs.UCSRA, RXC));
    
    // Get and return received data
    return regs.UDR;
}

void uartPrintString(const char* str) {
    while (*str) {
        uartTransmit(*str++);
    }
}
```

## ⚡ Interrupts

### Interrupt Configuration
```c
#define INT0_VECTOR 0
#define INT1_VECTOR 1

void enableInterrupt(uint8_t vector) {
    int_ctrl.enabled[vector] = 1;
}

void disableInterrupt(uint8_t vector) {
    int_ctrl.enabled[vector] = 0;
}

void setInterruptHandler(uint8_t vector, void (*handler)(void)) {
    int_ctrl.handlers[vector] = handler;
}
```

### External Interrupt Example
```c
void buttonPressInterrupt() {
    static uint8_t button_count = 0;
    button_count++;
    
    // Toggle LED on button press
    if (button_count % 2 == 0) {
        digitalWrite('A', PA0, 0);
    } else {
        digitalWrite('A', PA0, 1);
    }
}

void setupExternalInterrupt() {
    // Configure interrupt pin as input with pull-up
    pinMode('B', PB2, 0);
    digitalWrite('B', PB2, 1);
    
    // Set up interrupt handler
    setInterruptHandler(INT0_VECTOR, buttonPressInterrupt);
    enableInterrupt(INT0_VECTOR);
    
    // Configure interrupt for falling edge
    SET_BIT(EICRA, ISC01);
    CLEAR_BIT(EICRA, ISC00);
    
    // Enable external interrupt
    SET_BIT(EIMSK, INT0);
}
```

### Timer Interrupt Example
```c
volatile uint16_t timer_count = 0;

void timerOverflowInterrupt() {
    timer_count++;
}

void setupTimerInterrupt() {
    // Configure timer for overflow interrupt
    timerInit(TIMER_NORMAL, TIMER_PRESCALER_64);
    
    // Enable overflow interrupt
    SET_BIT(regs.TIMSK0, TOIE0);
    
    // Enable global interrupts
    sei();
}
```

## 🔋 Power Management

### Sleep Modes
```c
#define SLEEP_MODE_IDLE     0
#define SLEEP_MODE_ADC     1
#define SLEEP_MODE_PWR_DOWN 2
#define SLEEP_MODE_PWR_SAVE 3
#define SLEEP_MODE_STANDBY 4
#define SLEEP_MODE_EXT_STANDBY 5

void setSleepMode(uint8_t mode) {
    // Set sleep mode bits
    SMCR = (SMCR & ~((1 << SM2) | (1 << SM1) | (1 << SM0))) | (mode << SM0);
}

void sleep() {
    // Enable sleep
    SET_BIT(SMCR, SE);
    
    // Execute SLEEP instruction
    __asm__ __volatile__ ("sleep" ::);
    
    // Disable sleep
    CLEAR_BIT(SMCR, SE);
}
```

### Watchdog Timer
```c
void watchdogInit(uint8_t prescaler) {
    wdt.WDTCSR = prescaler;
    wdt.enabled = true;
}

void watchdogReset() {
    // Reset watchdog timer
    __asm__ __volatile__ ("wdr");
}

void watchdogEnable() {
    SET_BIT(wdt.WDTCSR, WDE);
}

void watchdogDisable() {
    // Disable watchdog (requires timed sequence)
    CLEAR_BIT(wdt.WDTCSR, WDE);
}
```

### Power Saving Example
```c
void powerSavingMode() {
    // Configure button interrupt to wake up
    setupExternalInterrupt();
    
    while (1) {
        // Main application code
        
        // Enter sleep mode
        setSleepMode(SLEEP_MODE_PWR_DOWN);
        sleep();
        
        // Woke up from sleep
    }
}
```

## 💾 EEPROM Operations

### EEPROM Functions
```c
#define EEPROM_SIZE 512

uint8_t eepromRead(uint16_t address) {
    if (address >= EEPROM_SIZE) return 0;
    
    // Wait for EEPROM to be ready
    while (READ_BIT(EECR, EEPE));
    
    // Set address to read
    EEAR = address;
    
    // Start EEPROM read
    SET_BIT(EECR, EERE);
    
    // Return data
    return EEDR;
}

void eepromWrite(uint16_t address, uint8_t data) {
    if (address >= EEPROM_SIZE) return;
    
    // Wait for EEPROM to be ready
    while (READ_BIT(EECR, EEPE));
    
    // Set address and data
    EEAR = address;
    EEDR = data;
    
    // Write logical one to EEMPE
    SET_BIT(EECR, EEMPE);
    
    // Start EEPROM write
    SET_BIT(EECR, EEPE);
}

void eepromUpdate(uint16_t address, uint8_t data) {
    uint8_t current_data = eepromRead(address);
    if (current_data != data) {
        eepromWrite(address, data);
    }
}
```

### EEPROM Usage Examples
```c
void saveSettings() {
    struct Settings {
        uint8_t brightness;
        uint8_t volume;
        uint8_t mode;
    } settings = {50, 75, 1};
    
    // Save settings to EEPROM
    eepromWrite(0, settings.brightness);
    eepromWrite(1, settings.volume);
    eepromWrite(2, settings.mode);
}

void loadSettings() {
    struct Settings settings;
    
    // Load settings from EEPROM
    settings.brightness = eepromRead(0);
    settings.volume = eepromRead(1);
    settings.mode = eepromRead(2);
    
    // Apply settings
    pwmSetDuty(settings.brightness);
}
```

## 💡 Advanced Topics

### 1. DMA (Direct Memory Access)
```c
void dmaTransfer(uint8_t channel, void* src, void* dst, uint16_t count) {
    // Configure DMA channel
    DMA->CHANNEL[channel].SRCADDR = (uint32_t)src;
    DMA->CHANNEL[channel].DSTADDR = (uint32_t)dst;
    DMA->CHANNEL[channel].TCNT = count;
    
    // Start transfer
    DMA->CHANNEL[channel].CTRL |= DMA_CH_ENABLE;
}
```

### 2. Bootloader
```c
void bootloader() {
    // Check if application update requested
    if (applicationUpdateRequested()) {
        // Enter programming mode
        enterProgrammingMode();
    } else {
        // Jump to application
        jumpToApplication();
    }
}
```

### 3. Real-Time Operating System (RTOS)
```c
typedef struct {
    void (*task)(void);
    uint16_t period;
    uint16_t next_run;
} Task;

Task tasks[MAX_TASKS];
uint8_t task_count = 0;

void addTask(void (*task_func)(void), uint16_t period_ms) {
    if (task_count < MAX_TASKS) {
        tasks[task_count].task = task_func;
        tasks[task_count].period = period_ms;
        tasks[task_count].next_run = 0;
        task_count++;
    }
}

void scheduler() {
    uint16_t current_time = getSystemTime();
    
    for (uint8_t i = 0; i < task_count; i++) {
        if (current_time >= tasks[i].next_run) {
            tasks[i].task();
            tasks[i].next_run = current_time + tasks[i].period;
        }
    }
}
```

### 4. Firmware Update Over-the-Air (OTA)
```c
void otaUpdate() {
    // Connect to update server
    if (connectToServer()) {
        // Download new firmware
        if (downloadFirmware()) {
            // Verify firmware integrity
            if (verifyFirmware()) {
                // Boot into bootloader
                reboot();
            }
        }
    }
}
```

## 📊 Performance Optimization

### 1. Code Size Optimization
```c
// Use smaller data types
uint8_t small_var;    // Instead of int
uint16_t medium_var;  // Instead of long

// Use bit fields
struct Flags {
    uint8_t flag1 : 1;
    uint8_t flag2 : 1;
    uint8_t flag3 : 1;
};

// Use function attributes
void __attribute__((noinline)) critical_function();
void __attribute__((section(".fastcode"))) fast_function();
```

### 2. Power Optimization
```c
// Use sleep modes
void enterLowPowerMode() {
    setSleepMode(SLEEP_MODE_PWR_DOWN);
    sleep();
}

// Disable unused peripherals
void disableUnusedPeripherals() {
    CLEAR_BIT(ADCSRA, ADEN);  // Disable ADC
    CLEAR_BIT(ACSR, ACIE);    // Disable analog comparator
}

// Optimize clock speed
void setClockSpeed(uint8_t prescaler) {
    CLKPR = (1 << CLKPCE);  // Enable prescaler change
    CLKPR = prescaler;       // Set prescaler
}
```

### 3. Memory Optimization
```c
// Use PROGMEM for constants
const uint8_t lookup_table[] PROGMEM = {0x01, 0x02, 0x04, 0x08};

uint8_t getValue(uint8_t index) {
    return pgm_read_byte(&lookup_table[index]);
}

// Use unions for memory sharing
union SharedMemory {
    uint8_t bytes[4];
    uint32_t value;
};
```

## ⚠️ Common Pitfalls

### 1. Race Conditions
```c
// Wrong - Interrupt can modify variable during read
volatile uint16_t counter;

uint16_t readCounter() {
    uint8_t low = counter & 0xFF;      // Interrupt might occur here
    uint8_t high = (counter >> 8) & 0xFF; // Inconsistent value
    return (high << 8) | low;
}

// Right - Disable interrupts during read
uint16_t readCounterSafe() {
    uint8_t old_sreg = SREG;  // Save interrupt state
    cli();                    // Disable interrupts
    
    uint16_t value = counter;
    
    SREG = old_sreg;           // Restore interrupt state
    return value;
}
```

### 2. Stack Overflow
```c
// Wrong - Large arrays on stack
void problematicFunction() {
    uint8_t large_buffer[1000]; // May cause stack overflow
}

// Right - Use static allocation or heap
void safeFunction() {
    static uint8_t large_buffer[1000]; // Static allocation
    // Or use malloc if available
}
```

### 3. Timing Issues
```c
// Wrong - Delay depends on compiler optimization
void delay_ms(uint16_t ms) {
    for (volatile uint16_t i = 0; i < ms * 1000; i++); // Unreliable
}

// Right - Use hardware timers
void delay_ms(uint16_t ms) {
    uint16_t start = timerGetValue();
    while ((timerGetValue() - start) < ms);
}
```

### 4. Register Volatility
```c
// Wrong - Compiler might optimize away register access
void problematicAccess() {
    regs.PORTA = 0xFF;
    regs.PORTA = 0x00; // Might be optimized away
}

// Right - Use volatile keyword
volatile uint8_t* port_a = &regs.PORTA;
void safeAccess() {
    *port_a = 0xFF;
    *port_a = 0x00;
}
```

## 🔧 Real-World Applications

### 1. Smart Home Controller
```c
void smartHomeController() {
    // Read temperature sensor
    float temp = readTemperature();
    
    // Control fan based on temperature
    if (temp > 25.0) {
        digitalWrite('A', PA0, 1); // Turn on fan
    } else {
        digitalWrite('A', PA0, 0); // Turn off fan
    }
    
    // Read light sensor
    float light = readLightLevel();
    
    // Control LED based on light level
    uint8_t brightness = (uint8_t)(light * 51); // 0-255
    pwmSetDuty(brightness);
    
    // Sleep for power saving
    setSleepMode(SLEEP_MODE_IDLE);
    sleep();
}
```

### 2. Data Logger
```c
void dataLogger() {
    struct DataPoint {
        uint16_t timestamp;
        float temperature;
        float humidity;
        uint8_t status;
    };
    
    struct DataPoint data;
    
    // Read sensors
    data.timestamp = getSystemTime();
    data.temperature = readTemperature();
    data.humidity = readHumidity();
    data.status = getSystemStatus();
    
    // Store in EEPROM
    uint16_t eeprom_addr = getLoggingIndex() * sizeof(data);
    eepromWrite(eeprom_addr, data.timestamp);
    eepromWrite(eeprom_addr + 2, (uint8_t)(data.temperature * 100));
    eepromWrite(eeprom_addr + 3, (uint8_t)(data.humidity * 100));
    eepromWrite(eeprom_addr + 4, data.status);
    
    // Transmit via UART
    uartPrintString("Data logged: ");
    uartTransmit((data.timestamp >> 8) & 0xFF);
    uartTransmit(data.timestamp & 0xFF);
}
```

### 3. Motor Controller
```c
void motorController() {
    // Read potentiometer for speed control
    uint16_t pot_value = adcRead(2);
    uint8_t speed = (pot_value * 255) / ADC_RESOLUTION;
    
    // Set motor speed via PWM
    pwmSetDuty(speed);
    
    // Read current sensor
    uint16_t current_adc = adcRead(3);
    float current = adcToVoltage(current_adc, 5.0) / 0.1; // 0.1V/A
    
    // Overcurrent protection
    if (current > 2.0) {
        digitalWrite('A', PA1, 0); // Stop motor
        uartPrintString("Overcurrent detected!\n");
    }
    
    // Temperature monitoring
    float motor_temp = readTemperature();
    if (motor_temp > 60.0) {
        pwmSetDuty(speed / 2); // Reduce speed
    }
}
```

### 4. Wireless Sensor Node
```c
void wirelessSensorNode() {
    // Read sensors
    float temp = readTemperature();
    float humidity = readHumidity();
    uint16_t light = adcRead(1);
    
    // Prepare data packet
    uint8_t packet[8];
    packet[0] = 0xAA; // Start byte
    packet[1] = (uint8_t)(temp);
    packet[2] = (uint8_t)((temp - (uint8_t)temp) * 100);
    packet[3] = (uint8_t)(humidity);
    packet[4] = (uint8_t)((humidity - (uint8_t)humidity) * 100);
    packet[5] = (light >> 8) & 0xFF;
    packet[6] = light & 0xFF;
    packet[7] = 0x55; // End byte
    
    // Transmit via SPI to wireless module
    digitalWrite('B', PB2, 0); // Select wireless module
    for (int i = 0; i < 8; i++) {
        spiTransfer(packet[i]);
    }
    digitalWrite('B', PB2, 1); // Deselect wireless module
    
    // Enter sleep mode
    setSleepMode(SLEEP_MODE_PWR_DOWN);
    sleep();
    
    // Wake up after interrupt
}
```

## 🎓 Best Practices

### 1. Register Access
```c
// Use bit manipulation macros
#define SET_BIT(reg, bit)    ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))

// Use volatile for hardware registers
volatile uint8_t* const PORTA = (volatile uint8_t*)0x3B;
```

### 2. Interrupt Safety
```c
// Always protect shared variables
void updateSharedVariable() {
    uint8_t old_sreg = SREG;
    cli();
    
    shared_variable++;
    
    SREG = old_sreg;
}
```

### 3. Resource Management
```c
// Initialize all peripherals
void initPeripherals() {
    adcInit(ADC_REF_AVCC, ADC_PRESCALER_128);
    spiInit(0, 4);
    uartInit(9600);
    timerInit(TIMER_CTC, TIMER_PRESCALER_64);
}
```

### 4. Error Handling
```c
// Always check return values
if (!i2cStart(0x27)) {
    uartPrintString("I2C error!\n");
    return;
}
```

### 5. Documentation
```c
/**
 * @brief Initialize ADC with specified reference and prescaler
 * @param reference Voltage reference (ADC_REF_AREF, ADC_REF_AVCC)
 * @param prescaler ADC prescaler (ADC_PRESCALER_128, etc.)
 */
void adcInit(uint8_t reference, uint8_t prescaler);
```

Microcontroller programming in C provides direct hardware control for embedded systems. Master these concepts to build efficient, reliable embedded applications!
