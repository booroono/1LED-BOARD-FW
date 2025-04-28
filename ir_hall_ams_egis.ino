#include<Wire.h>
#include<avr/wdt.h>
#define TCA_ADDRESS 0x70
#define EGIS_ADDRESS 0x2C
#define AMS_ADDRESS 0x39
#define HALL_ADDRESS 0x10

int16_t data_z;
uint8_t IR_ADDRESS = 0;


void setup() {
  Wire.begin();
  Serial.begin(115200);
  wdt_enable(WDTO_250MS);
}
void loop() {
  wdt_reset();
  Tca_Select(0);
  IR_ADDRESS = getIRAddress();
//  IR_ADDRESS = EGIS_ADDRESS;
  initIR(IR_ADDRESS);
  HALL_init_(HALL_ADDRESS);
  uint16_t prox_data = getIRData(IR_ADDRESS);

  Serial.print("IR: ");
  Serial.print(prox_data);
  Serial.print(",");
  Serial.print("Hall: ");
  Serial.println(HALL_read_out_(HALL_ADDRESS));
}


void write_i2c(uint8_t address, uint8_t registor, uint8_t data){
  Wire.beginTransmission(address);
  Wire.write(registor); 
  Wire.write(data);
  Wire.endTransmission();
}

void read_i2c(uint8_t address, uint8_t registor, uint8_t size){
  Wire.beginTransmission(address);
  Wire.write(registor);
  Wire.endTransmission();
  
  Wire.requestFrom(address, size);
}

uint8_t getIRAddress(){
  for (uint8_t addr = 0; addr<=127; addr++) {
    if (addr == EGIS_ADDRESS or addr == AMS_ADDRESS){
      Wire.beginTransmission(addr);
      if (!Wire.endTransmission()) {
        return addr;
      }
    }
  }
  return EGIS_ADDRESS;
}

void initEGIS(uint8_t address){
  write_i2c(address, 0x84, 0xA5);
  delay(10);

  write_i2c(address, 0x00, 0x00);
  write_i2c(address, 0x40, 0x8F);
  write_i2c(address, 0x42, 0x20);
  write_i2c(address, 0x44, 0x03);
  write_i2c(address, 0x45, 0x41);
  write_i2c(address, 0x81, 0x09);
  write_i2c(address, 0x47, 0xD0);
  write_i2c(address, 0x41, 0x64);
  write_i2c(address, 0x70, 0x11);

  write_i2c(address, 0x88, 0x14);
  write_i2c(address, 0x89, 0x00);
  write_i2c(address, 0x80, 0x81);
}

void initAMS(uint8_t address){
  write_i2c(address, 0x80, 0x01);
  write_i2c(address, 0x82, 0x1F);
  write_i2c(address, 0xA9, 0x00);
  write_i2c(address, 0x8C, 0x03);
  write_i2c(address, 0x8E, 0x02);
  write_i2c(address, 0x8F, 0x65);
  write_i2c(address, 0xC0, 0x00);
  write_i2c(address, 0xC1, 0x00);
  write_i2c(address, 0xF9, 0x07);
  write_i2c(address, 0x80, 0x15);
}

void initIR(uint8_t address){
  if(address == EGIS_ADDRESS){
    initEGIS(IR_ADDRESS);
  }else if(address == AMS_ADDRESS){
    initAMS(address);
  }
}

uint16_t getIRAMS(uint8_t address){
  read_i2c(address, 0x9c, 2);
  uint8_t lowByte = Wire.read();
  uint8_t highByte = Wire.read();
  return lowByte | (highByte << 8);
}

uint16_t getIREGIS(uint8_t address){
  write_i2c(address, 0x00, 0x00);
  uint8_t V_flag = 0x00;
//  while(!(V_flag & 0x08)){
    read_i2c(address, 0x00, 1);
    V_flag = Wire.read();
//  }
  read_i2c(address, 0x02, 2);

  uint8_t LowByte = Wire.read();
  uint8_t HighByte = Wire.read();

  return LowByte | (HighByte << 8);
}

uint16_t getIRData(uint8_t address){
  if(address == EGIS_ADDRESS){
    return getIREGIS(address);
  }else if(address == AMS_ADDRESS){
    return getIRAMS(address);
  }
}

void HALL_init_(unsigned char address) {
  write_i2c(address, 0x21, 0x04);
  delay(2);
  read_i2c(address, 0x00, 8);
  Wire.read();
}


int HALL_read_out_(int address) {
  read_i2c(address, 0x17, 7*2);
  int Status = Wire.read();
  data_z = Wire.read() << 8 | Wire.read();
  return data_z;
}

void Tca_Select(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCA_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}
