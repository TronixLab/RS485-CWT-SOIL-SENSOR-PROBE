/*
  Note: 
  *Serial Port 0 is not used to connect the RS485 Converter (MAX485) because its used for debugging. 
  *The Serial Port 1 (TX1, RX1) is used for ModBus communication interface.
  *Wiring of Sensor, Arduino, and MAX485 TTL to RS485 Converter:
  ___________________________________________________________________________________________
  | Sensor (SHT20)   |   MAX485 TTL to RS485 Converter
  |  A (Yellow)      |        A (Terminal block)
  |  B (White)       |        B (Terminal block)
  |  GND (Black)     |       GND (External Supply)
  |  Vs (Red)        |      5-30V (External Supply)
  ___________________________________________________________________________________________
  | MAX485 TTL to RS485 Converter  |  Arduino (Hardware Serial)  |  Arduino (Software Serial)
  |     RO (Reciever Output)       |        D19 (RX1)            |          D9 (RX)
  |     RE (Reciever Enable)       |        D2                   |          D2
  |     DE (Driver Enable)         |        D3                   |          D3
  |     DI (Driver Input)          |        D18 (TX1)            |          D10 (TX)
  ___________________________________________________________________________________________
*/

// Define the pins used by the RS-485 module
#define MAX485_RE_NEG 2
#define MAX485_DE 3

void setup() {
  // Initialize program Serial port at 115200 baud rate
  Serial.begin(115200);

  // Initialize modbus Serial port at 4800 baud rate
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Start the Modbus serial Port, for Soil_sensor
  Serial1.begin(4800);
  delay(1000);
}

void loop() {
  // Read Soil Moisture/Humidity
  preTransmission();
  byte soil_humidity_request[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0a };
  Serial1.write(soil_humidity_request, sizeof(soil_humidity_request));
  Serial1.flush();
  postTransmission();
  byte soil_humidity_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_humidity_buff, 16);
  float humidity = word(soil_humidity_buff[3], soil_humidity_buff[4]);
  humidity = humidity / 10.0;

  // Read Soil Temperature
  preTransmission();
  byte soil_temperature_request[] = { 0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xd5, 0xca };
  Serial1.write(soil_temperature_request, sizeof(soil_temperature_request));
  Serial1.flush();
  postTransmission();
  byte soil_temperature_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_temperature_buff, 16);
  float temperature = word(soil_temperature_buff[3], soil_temperature_buff[4]);
  temperature = temperature / 10.0;

  // Read Soil Conductivity
  preTransmission();
  byte soil_conductivity_request[] = { 0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xca };
  Serial1.write(soil_conductivity_request, sizeof(soil_conductivity_request));
  Serial1.flush();
  postTransmission();
  byte soil_conductivity_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_conductivity_buff, 16);
  unsigned long int conductivity = word(soil_conductivity_buff[3], soil_conductivity_buff[4]);

  // Read Soil pH
  preTransmission();
  byte soil_pH_request[] = { 0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0a };
  Serial1.write(soil_pH_request, sizeof(soil_pH_request));
  Serial1.flush();
  postTransmission();
  byte soil_pH_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_pH_buff, 16);
  float pH = word(soil_pH_buff[3], soil_pH_buff[4]);
  pH = pH / 10.0;

  // Read Nitrogen
  preTransmission();
  byte soil_Nitrogen_request[] = { 0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xc5, 0xcb };
  Serial1.write(soil_Nitrogen_request, sizeof(soil_Nitrogen_request));
  Serial1.flush();
  postTransmission();
  byte soil_Nitrogen_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_Nitrogen_buff, 16);
  int Nitrogen = word(soil_Nitrogen_buff[3], soil_Nitrogen_buff[4]);

  // Read Phosphorus
  preTransmission();
  byte soil_Phosphorus_request[] = { 0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0b };
  Serial1.write(soil_Phosphorus_request, sizeof(soil_Phosphorus_request));
  Serial1.flush();
  postTransmission();
  byte soil_Phosphorus_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_Phosphorus_buff, 16);
  int Phosphorus = word(soil_Phosphorus_buff[3], soil_Phosphorus_buff[4]);

  // Read Potassium
  preTransmission();
  byte soil_Potassium_request[] = { 0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b };
  Serial1.write(soil_Potassium_request, sizeof(soil_Potassium_request));
  Serial1.flush();
  postTransmission();
  byte soil_Potassium_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_Potassium_buff, 16);
  int Potassium = word(soil_Potassium_buff[3], soil_Potassium_buff[4]);

  // Read Salinity
  preTransmission();
  byte soil_Salinity_request[] = { 0x01, 0x03, 0x00, 0x07, 0x00, 0x01, 0x35, 0xcb };
  Serial1.write(soil_Salinity_request, sizeof(soil_Salinity_request));
  Serial1.flush();
  postTransmission();
  byte soil_Salinity_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_Salinity_buff, 16);
  unsigned long int Salinity = word(soil_Salinity_buff[3], soil_Salinity_buff[4]);

  // Read TDS
  preTransmission();
  byte soil_TDS_request[] = { 0x01, 0x03, 0x00, 0x08, 0x00, 0x01, 0x05, 0xc8 };
  Serial1.write(soil_TDS_request, sizeof(soil_TDS_request));
  Serial1.flush();
  postTransmission();
  byte soil_TDS_buff[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  Serial1.readBytes(soil_TDS_buff, 16);
  unsigned long int TDS = word(soil_TDS_buff[3], soil_TDS_buff[4]);

  // Print sensor parameters
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  ");
  Serial.print("Conductivity: ");
  Serial.print(conductivity);
  Serial.print("us/cm  ");
  Serial.print("pH: ");
  Serial.print(pH);
  Serial.print("  ");
  Serial.print("Nitrogen: ");
  Serial.print(Nitrogen);
  Serial.print("mg/L  ");
  Serial.print("Phosphorus: ");
  Serial.print(Phosphorus);
  Serial.print("mg/L   ");
  Serial.print("Potassium: ");
  Serial.print(Potassium);
  Serial.print("mg/L   ");
  Serial.print("Salinity: ");
  Serial.print(Salinity);
  Serial.print("dS/m   ");
  Serial.print("TDS: ");
  Serial.print(TDS);
  Serial.print("ppm  ");
  Serial.println();
  delay(1000);
}

void preTransmission() {
  digitalWrite(MAX485_RE_NEG, HIGH);
  digitalWrite(MAX485_DE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, LOW);
  digitalWrite(MAX485_DE, LOW);
}