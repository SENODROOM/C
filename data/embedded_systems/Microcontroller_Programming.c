#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// =============================================================================
// MICROCONTROLLER SIMULATION
// =============================================================================

// Simulate microcontroller registers
typedef struct {
    uint8_t PORTA;      // Port A data register
    uint8_t DDRA;       // Port A data direction register
    uint8_t PINA;       // Port A input pins register
    uint8_t PORTB;      // Port B data register
    uint8_t DDRB;       // Port B data direction register
    uint8_t PINB;       // Port B input pins register
    uint8_t TCCR0A;     // Timer/Counter 0 Control Register A
    uint8_t TCCR0B;     // Timer/Counter 0 Control Register B
    uint8_t TCNT0;      // Timer/Counter 0
    uint8_t OCR0A;      // Output Compare Register A
    uint8_t TIMSK0;     // Timer/Counter 0 Interrupt Mask Register
    uint8_t TIFR0;      // Timer/Counter 0 Interrupt Flag Register
    uint8_t SPCR;       // SPI Control Register
    uint8_t SPSR;       // SPI Status Register
    uint8_t SPDR;       // SPI Data Register
    uint8_t UCSRA;      // USART Control and Status Register A
    uint8_t UCSRB;      // USART Control and Status Register B
    uint8_t UCSRC;      // USART Control and Status Register C
    uint8_t UBRR;       // USART Baud Rate Register
    uint8_t UDR;        // USART I/O Data Register
} AVRRegisters;

// Global register simulation
AVRRegisters regs;

// =============================================================================
// GPIO (GENERAL PURPOSE I/O) OPERATIONS
// =============================================================================

// Pin definitions
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 0
#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4
#define PB5 5
#define PB6 6
#define PB7 7

// Bit manipulation macros
#define SET_BIT(reg, bit)    ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1 << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1)

// GPIO functions
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

// =============================================================================
// TIMER OPERATIONS
// =============================================================================

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

void timerSetCompare(uint8_t value) {
    regs.OCR0A = value;
}

uint8_t timerGetValue() {
    return regs.TCNT0;
}

void timerReset() {
    regs.TCNT0 = 0;
}

// =============================================================================
// PWM (PULSE WIDTH MODULATION)
// =============================================================================

void pwmInit(uint8_t pin, uint16_t frequency) {
    // Configure timer for Fast PWM mode
    regs.TCCR0A |= (1 << WGM00) | (1 << WGM01);
    regs.TCCR0A |= (1 << COM0A1);  // Non-inverting mode
    
    // Set prescaler for desired frequency
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

// =============================================================================
// ADC (ANALOG TO DIGITAL CONVERTER)
// =============================================================================

#define ADC_CHANNELS 8
#define ADC_RESOLUTION 1024

typedef struct {
    uint8_t ADMUX;    // ADC Multiplexer Selection Register
    uint8_t ADCSRA;   // ADC Control and Status Register A
    uint8_t ADCL;     // ADC Data Register Low
    uint8_t ADCH;     // ADC Data Register High
} ADCRegisters;

ADCRegisters adc_regs;

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

// =============================================================================
// INTERRUPTS
// =============================================================================

#define INT0_VECTOR 0
#define INT1_VECTOR 1

typedef struct {
    void (*handlers[8])(void);
    uint8_t enabled[8];
} InterruptController;

InterruptController int_ctrl;

// Interrupt enable/disable
void enableInterrupt(uint8_t vector) {
    int_ctrl.enabled[vector] = 1;
}

void disableInterrupt(uint8_t vector) {
    int_ctrl.enabled[vector] = 0;
}

// Set interrupt handler
void setInterruptHandler(uint8_t vector, void (*handler)(void)) {
    int_ctrl.handlers[vector] = handler;
}

// Simulate interrupt
void triggerInterrupt(uint8_t vector) {
    if (int_ctrl.enabled[vector] && int_ctrl.handlers[vector]) {
        int_ctrl.handlers[vector]();
    }
}

// =============================================================================
// COMMUNICATION PROTOCOLS
// =============================================================================

// SPI (SERIAL PERIPHERAL INTERFACE)
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

// I2C (INTER-IC COMMUNICATION)
typedef struct {
    uint8_t TWBR;    // TWI Bit Rate Register
    uint8_t TWCR;    // TWI Control Register
    uint8_t TWSR;    // TWI Status Register
    uint8_t TWDR;    // TWI Data Register
} I2CRegisters;

I2CRegisters i2c_regs;

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
    
    // Check status
    uint8_t status = i2c_regs.TWSR & 0xF8;
    if (status != 0x08 && status != 0x10) {
        return 0; // Error
    }
    
    // Send device address
    i2c_regs.TWDR = (address << 1) | 0; // Write mode
    SET_BIT(i2c_regs.TWCR, TWINT);
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
    
    return (i2c_regs.TWSR & 0xF8) == 0x18; // ACK received
}

uint8_t i2cWrite(uint8_t data) {
    i2c_regs.TWDR = data;
    SET_BIT(i2c_regs.TWCR, TWINT);
    
    // Wait for transmission complete
    while (!READ_BIT(i2c_regs.TWCR, TWINT));
    
    return (i2c_regs.TWCR & 0xF8) == 0x28; // ACK received
}

void i2cStop() {
    SET_BIT(i2c_regs.TWCR, TWSTO);
    SET_BIT(i2c_regs.TWCR, TWINT);
}

// =============================================================================
// SENSOR INTERFACING
// =============================================================================

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

// Ultrasonic distance sensor
float readDistance() {
    // Trigger pulse
    digitalWrite('B', PB0, 1);
    delayMicroseconds(10);
    digitalWrite('B', PB0, 0);
    
    // Measure echo pulse width
    uint32_t duration = pulseIn('B', PB1, 1, 30000); // 30ms timeout
    
    // Calculate distance (cm)
    return duration / 58.0;
}

// =============================================================================
// DELAY FUNCTIONS
// =============================================================================

void delay(uint32_t ms) {
    // Simulate delay (in real microcontroller, this would use timer)
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms);
}

void delayMicroseconds(uint32_t us) {
    // Simulate microsecond delay
    clock_t start = clock();
    while ((clock() - start) * 1000000 / CLOCKS_PER_SEC < us);
}

uint32_t pulseIn(uint8_t port, uint8_t pin, uint8_t state, uint32_t timeout) {
    // Simulate pulse measurement
    clock_t start = clock();
    uint32_t duration = 0;
    
    // Wait for pin to reach desired state
    while (digitalRead(port, pin) != state) {
        if ((clock() - start) * 1000000 / CLOCKS_PER_SEC > timeout) {
            return 0;
        }
    }
    
    start = clock();
    
    // Wait for pin to change state
    while (digitalRead(port, pin) == state) {
        if ((clock() - start) * 1000000 / CLOCKS_PER_SEC > timeout) {
            return 0;
        }
    }
    
    duration = (clock() - start) * 1000000 / CLOCKS_PER_SEC;
    return duration;
}

// =============================================================================
// POWER MANAGEMENT
// =============================================================================

#define SLEEP_MODE_IDLE     0
#define SLEEP_MODE_ADC     1
#define SLEEP_MODE_PWR_DOWN 2
#define SLEEP_MODE_PWR_SAVE 3
#define SLEEP_MODE_STANDBY 4
#define SLEEP_MODE_EXT_STANDBY 5

void setSleepMode(uint8_t mode) {
    // Set sleep mode (simplified)
    // In real AVR, this would set SM2, SM1, SM0 bits
}

void sleep() {
    // Enter sleep mode
    // In real AVR, this would set SE bit and execute SLEEP instruction
    delay(100); // Simulate sleep
}

void wakeUp() {
    // Wake up from sleep
    // In real AVR, this would happen automatically on interrupt
}

// =============================================================================
// EEPROM SIMULATION
// =============================================================================

#define EEPROM_SIZE 512

typedef struct {
    uint8_t data[EEPROM_SIZE];
    bool dirty;
} EEPROM;

EEPROM eeprom;

uint8_t eepromRead(uint16_t address) {
    if (address >= EEPROM_SIZE) return 0;
    return eeprom.data[address];
}

void eepromWrite(uint16_t address, uint8_t data) {
    if (address >= EEPROM_SIZE) return;
    eeprom.data[address] = data;
    eeprom.dirty = true;
}

void eepromUpdate(uint16_t address, uint8_t data) {
    if (address >= EEPROM_SIZE) return;
    if (eeprom.data[address] != data) {
        eeprom.data[address] = data;
        eeprom.dirty = true;
    }
}

// =============================================================================
// WATCHDOG TIMER
// =============================================================================

typedef struct {
    uint8_t WDTCSR;  // Watchdog Timer Control Register
    bool enabled;
    uint32_t timeout;
} WatchdogTimer;

WatchdogTimer wdt;

void watchdogInit(uint8_t prescaler) {
    wdt.WDTCSR = prescaler;
    wdt.enabled = true;
}

void watchdogReset() {
    // Reset watchdog timer
    wdt.timeout = 0;
}

void watchdogEnable() {
    wdt.enabled = true;
}

void watchdogDisable() {
    wdt.enabled = false;
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateGPIO() {
    printf("=== GPIO DEMONSTRATION ===\n");
    
    // Configure pins
    pinMode('A', PA0, 1);  // Output
    pinMode('A', PA1, 1);  // Output
    pinMode('B', PB0, 0);  // Input
    pinMode('B', PB1, 0);  // Input
    
    // LED blinking simulation
    printf("Blinking LED simulation...\n");
    for (int i = 0; i < 5; i++) {
        digitalWrite('A', PA0, 1);
        printf("LED ON\n");
        delay(500);
        
        digitalWrite('A', PA0, 0);
        printf("LED OFF\n");
        delay(500);
    }
    
    // Button reading simulation
    printf("Button reading simulation...\n");
    for (int i = 0; i < 3; i++) {
        uint8_t button_state = digitalRead('B', PB0);
        printf("Button state: %s\n", button_state ? "PRESSED" : "RELEASED");
        delay(200);
    }
    
    printf("\n");
}

void demonstratePWM() {
    printf("=== PWM DEMONSTRATION ===\n");
    
    // Initialize PWM on pin PA1
    pwmInit(PA1, 1000); // 1kHz frequency
    
    // Fade LED in and out
    printf("Fading LED simulation...\n");
    for (int i = 0; i <= 255; i++) {
        pwmSetDuty(i);
        printf("Duty cycle: %d/255 (%.1f%%)\n", i, (i * 100.0) / 255);
        delay(10);
    }
    
    for (int i = 255; i >= 0; i--) {
        pwmSetDuty(i);
        printf("Duty cycle: %d/255 (%.1f%%)\n", i, (i * 100.0) / 255);
        delay(10);
    }
    
    printf("\n");
}

void demonstrateADC() {
    printf("=== ADC DEMONSTRATION ===\n");
    
    // Initialize ADC
    adcInit(ADC_REF_AVCC, ADC_PRESCALER_128);
    
    // Read multiple channels
    printf("Reading ADC channels...\n");
    for (int channel = 0; channel < 4; channel++) {
        uint16_t adc_value = adcRead(channel);
        float voltage = adcToVoltage(adc_value, 5.0);
        printf("Channel %d: ADC=%d, Voltage=%.3fV\n", channel, adc_value, voltage);
        delay(100);
    }
    
    // Temperature sensor simulation
    printf("\nTemperature sensor reading: %.1f°C\n", readTemperature());
    printf("Light sensor reading: %.2fV\n", readLightLevel());
    
    printf("\n");
}

void demonstrateTimers() {
    printf("=== TIMER DEMONSTRATION ===\n");
    
    // Initialize timer in CTC mode
    timerInit(TIMER_CTC, TIMER_PRESCALER_64);
    timerSetCompare(255);
    
    printf("Timer demonstration...\n");
    for (int i = 0; i < 10; i++) {
        timerReset();
        delay(100);
        uint8_t timer_value = timerGetValue();
        printf("Timer value after 100ms: %d\n", timer_value);
    }
    
    printf("\n");
}

void demonstrateCommunication() {
    printf("=== COMMUNICATION DEMONSTRATION ===\n");
    
    // SPI demonstration
    spiInit(0, 4); // Mode 0, prescaler 16
    
    printf("SPI transfer demonstration...\n");
    for (int i = 0; i < 5; i++) {
        uint8_t data = i + 0x30; // Send ASCII digits
        uint8_t received = spiTransfer(data);
        printf("Sent: 0x%02X, Received: 0x%02X\n", data, received);
        delay(100);
    }
    
    // I2C demonstration
    i2cInit(100000); // 100kHz
    
    printf("\nI2C communication simulation...\n");
    if (i2cStart(0x27)) { // Address 0x27
        printf("I2C start successful\n");
        if (i2cWrite(0x42)) {
            printf("Data write successful\n");
        }
        i2cStop();
    }
    
    printf("\n");
}

void demonstratePowerManagement() {
    printf("=== POWER MANAGEMENT DEMONSTRATION ===\n");
    
    // Watchdog timer
    watchdogInit(6); // 2-second timeout
    watchdogEnable();
    
    printf("Watchdog timer enabled\n");
    
    // Simulate normal operation
    for (int i = 0; i < 3; i++) {
        printf("Normal operation cycle %d\n", i + 1);
        watchdogReset(); // Reset watchdog
        delay(1000);
    }
    
    // Simulate sleep mode
    printf("Entering sleep mode...\n");
    setSleepMode(SLEEP_MODE_PWR_DOWN);
    sleep();
    printf("Woke up from sleep\n");
    
    watchdogDisable();
    printf("Watchdog timer disabled\n");
    
    printf("\n");
}

void demonstrateEEPROM() {
    printf("=== EEPROM DEMONSTRATION ===\n");
    
    // Write some data to EEPROM
    printf("Writing data to EEPROM...\n");
    for (uint16_t i = 0; i < 10; i++) {
        eepromWrite(i, i * 10);
        printf("Address %d: %d\n", i, i * 10);
    }
    
    // Read data back
    printf("\nReading data from EEPROM...\n");
    for (uint16_t i = 0; i < 10; i++) {
        uint8_t data = eepromRead(i);
        printf("Address %d: %d\n", i, data);
    }
    
    // Demonstrate update (write only if different)
    printf("\nEEPROM update demonstration...\n");
    eepromUpdate(5, 55); // Same value
    eepromUpdate(6, 65); // Different value
    
    printf("Address 5: %d\n", eepromRead(5));
    printf("Address 6: %d\n", eepromRead(6));
    
    printf("\n");
}

void demonstrateInterrupts() {
    printf("=== INTERRUPT DEMONSTRATION ===\n");
    
    // Set up interrupt handler
    void buttonInterrupt(void) {
        printf("Button interrupt triggered!\n");
    }
    
    setInterruptHandler(INT0_VECTOR, buttonInterrupt);
    enableInterrupt(INT0_VECTOR);
    
    printf("Interrupt system initialized\n");
    
    // Simulate interrupt triggers
    for (int i = 0; i < 3; i++) {
        printf("Triggering interrupt %d...\n", i + 1);
        triggerInterrupt(INT0_VECTOR);
        delay(500);
    }
    
    printf("\n");
}

// =============================================================================
// MAIN DEMONSTRATION
// =============================================================================

int main() {
    printf("Microcontroller Programming Examples\n");
    printf("===================================\n\n");
    
    // Initialize random seed for simulations
    srand(time(NULL));
    
    demonstrateGPIO();
    demonstratePWM();
    demonstrateADC();
    demonstrateTimers();
    demonstrateCommunication();
    demonstratePowerManagement();
    demonstrateEEPROM();
    demonstrateInterrupts();
    
    printf("All microcontroller programming examples demonstrated!\n");
    printf("Note: These are simulations for demonstration purposes.\n");
    printf("Real microcontroller programming would require hardware-specific code.\n");
    
    return 0;
}
