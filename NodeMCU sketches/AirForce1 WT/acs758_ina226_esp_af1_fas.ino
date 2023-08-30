  #include <Arduino.h>
  #include <U8g2lib.h>
  #ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
  #endif
  #ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
  #endif
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
  #include <InfluxDbClient.h>
  #include <InfluxDbCloud.h>
  #include <INA226.h>
  
  // Pocket router WiFi AP SSID
  #define WIFI_SSID ""
  // Pocket router WiFi password
  #define WIFI_PASSWORD ""

  #define TRIGGER_PIN 0

  //influxDB info
  #define INFLUXDB_URL ""
  #define INFLUXDB_TOKEN "" //ESP token
  #define INFLUXDB_ORG "" //org ID
  #define INFLUXDB_BUCKET "" //bucket name
  #define TURBINE_NAME "" //measurement
  #define LOCATION "" //tag key
  float delT = 1;
  float voltage, current, power, output_voltage, wind_speed, energy, powerPrev = 0.0;
  double totalEnergy;
  double totalEnergyPrev = 0.0;
  
  // Time zone info
  #define TZ_INFO "UTC6"
  INA226 INA(0x40);
  U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
  // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  
  // Declare Data point
  Point sensor(TURBINE_NAME);

  const int analogInPin = A0;
  
  void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    pinMode(A0, INPUT);
    u8g2.begin();
    Serial.println("Hello!");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_originalsans_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("Restarted...");
    u8g2.sendBuffer();
    Serial.println();

    Serial.println("\n Starting");
    u8g2.begin();
    Serial.println("Hello!");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_HelvetiPixel_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("Restarted...");
    u8g2.sendBuffer();
    Serial.println();

    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.println("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
      }
    delay(3000);
    Serial.println("connected...");
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_HelvetiPixel_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("connecting to wifi...");
    u8g2.sendBuffer();  
    delay(3000);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_HelvetiPixel_tr); // choose a suitable font
    u8g2.setCursor(5,20);
    u8g2.print("connected");
    u8g2.sendBuffer();    
    if (!INA.begin() )
    {
      Serial.println("could not connect. Fix and Reboot");
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
      u8g2.setCursor(0,20);
      u8g2.print("INA226 error!!!");
      while (1) {
        delay(10);
        }
        }
        
     INA.setAverage(2);
        
     pinMode(TRIGGER_PIN, INPUT);
  
    // Accurate time is necessary for certificate validation and writing in batches
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
   
    // Add tags to the data point
    sensor.addTag("location", LOCATION);
  
    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());

      delay(1000);

      String query = "from(bucket: \"";
      query += INFLUXDB_BUCKET;
      query += "\") \|>range(start: -30d)\|>filter(fn: (r) => r._measurement == \"";
      query += TURBINE_NAME;
      query += "\" and r._field == \"Total\ Energy\ (kWh)\" and r.location == \"";
      query += LOCATION;
      query += "\")\|>last()";

      // Print ouput header
      Serial.print("==========\n");
      // Send query to the server and get result
      FluxQueryResult result = client.query(query);

      // Iterate over rows. Even there is just one row, next() must be called at least once.
      while (result.next()) {
        Serial.print("Total energy: ");
        // Get converted value for flux result column '_value' where there is RSSI value
        totalEnergyPrev = result.getValueByName("_value").getDouble();
      }
        // Check if there was an error
      if(result.getError() != "") {
        Serial.print("Query result error: ");
        Serial.println(result.getError());
      }

      // Close the result
      result.close();  
      Serial.print(totalEnergyPrev,18);
      Serial.print("\n==========\n");
      } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  }

  
  const float iniVolt = x.xx; //ACS758 initial voltage w/o current flow. calibrate if needed.
  
  void loop(){
  float shuntvoltage = 0;
  float busvoltage = 0;
  float loadvoltage = 0;
  
  float voltage = 0.0, current = 0.0;
  for(int i = 0; i < 1000; i++) 
  {
    int adc = analogRead(analogInPin);
    voltage = adc*5/1023.0;
    float cur = (voltage-iniVolt)/0.04; //sensitivity for 50A ACS758 is 0.04 V/A
    current = abs(current + cur/1000);   
    delay(1);
  }

  if (current <= 0.16){
    current = 0.0;
    }

  shuntvoltage = INA.getShuntVoltage_mV();
  busvoltage = INA.getBusVoltage();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  if (loadvoltage < 1.0) {
    loadvoltage = 0.0;
  }

  power = loadvoltage * current;
  
  
  Serial.print("acs758-V: ");
  Serial.print(voltage);
  Serial.print("\t");
  Serial.print("Load-V: ");
  Serial.print(loadvoltage);
  Serial.print(" V");
  Serial.print("\t");
  Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" A");
  Serial.print("\t");
  Serial.print("Power: ");
  Serial.print(power);
  Serial.print(" W");
  Serial.print("\t");
  //delay(500);

  // Store measured value into point
  sensor.clearFields();

  if(powerPrev > 0.0) {
    energy = 0.5 * (powerPrev + power) * delT;
    totalEnergy = totalEnergy + ( energy / (3600 * 1000));
    powerPrev = power;
    } 
  else {
    totalEnergy = totalEnergyPrev;
    powerPrev = power;
    energy = 0.0;
    }
  sensor.addField("Voltage (V)", loadvoltage);
  sensor.addField("Current (A)", current);
  sensor.addField("Power (W)", power);
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // Serial.println(totalEnergy, 18);

  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  String waitingMessage = "Wait ";
  waitingMessage += String(delT);
  waitingMessage += " sec\n";
  Serial.println(waitingMessage);
  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_HelvetiPixel_tr); // choose a suitable font
  u8g2.setCursor(5,10);
  u8g2.print("Vol: ");
  u8g2.setCursor(50,10);
  u8g2.print(loadvoltage);
  u8g2.setCursor(80,10);
  u8g2.print(" V");
  u8g2.setCursor(5,25);
  u8g2.print("Cur: ");
  u8g2.setCursor(50,25);
  u8g2.print(current);
  u8g2.setCursor(80,25);
  u8g2.print(" A"); 
  u8g2.setCursor(5,40);
  u8g2.print("Pow: ");
  u8g2.setCursor(50,40);
  u8g2.print(power);
  u8g2.setCursor(80,40);
  u8g2.print(" W");
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
}
