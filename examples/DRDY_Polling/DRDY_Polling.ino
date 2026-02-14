/*
  DRDY Polling Example for ADS126X
  
  This example demonstrates how to use the DRDY (Data Ready) pin with polling mode.
  The DRDY pin goes LOW when a new conversion result is ready, and we poll it
  before reading data.
  
  Hardware Setup:
  - Connect CS pin to Arduino pin 5
  - Connect DRDY pin to Arduino pin 2
  - Connect AIN0 and AIN1 to your analog inputs
  
  This example:
  1. Sets up the DRDY pin
  2. Uses polling mode (no interrupts)
  3. Reads ADC1 with DRDY wait (polling the pin)
  4. Outputs results via Serial
*/

#include <ADS126X.h>

ADS126X adc; // create the ADS126X instance

const int chip_select = 5;  // Arduino pin connected to CS on ADS126X
const int drdy_pin = 2;     // Arduino pin connected to DRDY on ADS126X

const int pos_pin = 0;      // ADS126X pin AIN0, for positive input
const int neg_pin = 1;      // ADS126X pin AIN1, for negative input

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("ADS126X DRDY Polling Example");
  Serial.println("============================");
  
  // Initialize the ADC with chip select pin
  adc.begin(chip_select);
  
  // Setup the DRDY pin (but don't enable interrupt mode)
  adc.setDRDYPin(drdy_pin);
  
  // Set DRDY timeout to 2000ms (optional, default is 1000ms)
  adc.setDRDYTimeout(2000);
  
  // Start continuous conversion on ADC1
  adc.startADC1();
  
  Serial.println("Setup complete. Starting readings...");
  Serial.println("Note: Using polling mode (no interrupts)");
  Serial.println();
}

void loop() {
  // Read ADC1 with DRDY wait (polling mode)
  // This function will:
  // - Poll the DRDY pin until it goes LOW (data ready)
  // - Read the conversion result
  // - Return 0 if timeout occurs
  int32_t voltage = adc.readADC1_DRDY(pos_pin, neg_pin);
  
  if (voltage == 0) {
    Serial.println("Warning: DRDY timeout occurred!");
  } else {
    Serial.print("ADC1 Reading: ");
    Serial.println(voltage);
  }
  
  // You can also manually check if data is ready before reading:
  // if (adc.isDataReady()) {
  //   int32_t voltage = adc.readADC1(pos_pin, neg_pin);
  //   Serial.println(voltage);
  // }
  
  delay(1000); // wait 1 second between readings
}
