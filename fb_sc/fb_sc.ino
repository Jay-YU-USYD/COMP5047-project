#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "gitfit-1eeb0.firebaseio.com"
#define FIREBASE_AUTH "TqYPd2XaI7XUgs2fxuG614FhMRR8xHCV4HO8cNt5"
#define WIFI_SSID "Ywj"
#define WIFI_PASSWORD "19950521"

#define CLEAR_STEP      true
#define NOT_CLEAR_STEP  false 

LSM6DS3 pedometer( I2C_MODE, 0x6A );  //I2C device address 0x6A

void setup() {  
  Serial.begin(115200);   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  Firebase.setInt("TargetSteps", 0);
  Firebase.setInt("Steps", 0);
  
  if( pedometer.begin() != 0 ){
    Serial.println("Device error");
  }
  else{
      Serial.println("Device OK!");
  }
  
  //Configure LSM6DS3 as pedometer 
  if( 0 != config_pedometer(NOT_CLEAR_STEP) )
  {
    Serial.println("Configure pedometer fail!");
  }
  Serial.println("Success to Configure pedometer!");
//  pinMode(trigPin, OUTPUT);
}

int n = 0;

void loop()
{
  uint8_t dataByte = 0;
  uint16_t stepCount = 0;
  
  pedometer.readRegister(&dataByte, LSM6DS3_ACC_GYRO_STEP_COUNTER_H);
  stepCount = (dataByte << 8) & 0xFFFF;
  
  pedometer.readRegister(&dataByte, LSM6DS3_ACC_GYRO_STEP_COUNTER_L);
  stepCount |=  dataByte;
  
  Serial.print("Step: ");
  Serial.println(stepCount);
  Firebase.setInt("Steps", stepCount);
  delay(500);
}

//Setup pedometer mode
int config_pedometer(bool clearStep)
{
  uint8_t errorAccumulator = 0;
  uint8_t dataToWrite = 0;  //Temporary variable

  //Setup the accelerometer******************************
  dataToWrite = 0; 
  
  //  dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;
  dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
  dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_26Hz;

  
  // Step 1: Configure ODR-26Hz and FS-2g
  errorAccumulator += pedometer.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

  // Step 2: Set bit Zen_G, Yen_G, Xen_G, FUNC_EN, PEDO_RST_STEP(1 or 0)
  if(clearStep)
    errorAccumulator += pedometer.writeRegister(LSM6DS3_ACC_GYRO_CTRL10_C, 0x3E);
  else
    errorAccumulator += pedometer.writeRegister(LSM6DS3_ACC_GYRO_CTRL10_C, 0x3C);
  
  // Step 3:  Enable pedometer algorithm
  errorAccumulator += pedometer.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x40);
  
  //Step 4: Step Detector interrupt driven to INT1 pin, set bit INT1_FIFO_OVR
  errorAccumulator += pedometer.writeRegister( LSM6DS3_ACC_GYRO_INT1_CTRL, 0x10 );
  
  return errorAccumulator;
}
