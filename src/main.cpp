#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// **电池电压采集 GPIO**
#define BATTERY_ADC_PIN 17
#define ADC_REFERENCE_VOLTAGE 3.3  // ESP32-S3 参考电压
#define ADC_RESOLUTION 4096.0      // 12-bit ADC 分辨率
#define VOLTAGE_DIVIDER_RATIO 2.0  // 分压比 2:1

// **ESP32-S3 SPI 屏幕引脚**
#define TFT_SCLK  9
#define TFT_MOSI  10
#define TFT_DC    8
#define TFT_CS    14
#define TFT_RST   18
#define TFT_BLK   13


// **SPI 屏幕对象**
SPIClass SPI_TFT(HSPI);
Adafruit_ST7789 tft = Adafruit_ST7789(&SPI_TFT, TFT_CS, TFT_DC, TFT_RST);

// **初始化**
void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-S3 initial...");

    // **初始化 SPI**
    SPI_TFT.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

    // **控制背光**
    pinMode(TFT_BLK, OUTPUT);
    digitalWrite(TFT_BLK, HIGH);

    // **初始化 TFT**
    tft.init(240, 240, SPI_MODE2);
    tft.setRotation(3);
    tft.fillScreen(ST77XX_BLACK);

    // **显示启动文本**
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(20, 20);
    tft.println("ESP32S3 - Battery Voltage");

    Serial.println("TFT successfully！");

    // **配置 ADC 读取范围**
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

}


// **主循环**
void loop() {
    // 读取电池电压
    float batteryVoltage = analogRead(BATTERY_ADC_PIN) * (ADC_REFERENCE_VOLTAGE / ADC_RESOLUTION) * VOLTAGE_DIVIDER_RATIO;

    // 显示电池电压到 TFT 屏幕
    tft.fillRect(20, 100, 200, 40, ST77XX_BLACK); // 清除旧的电压数据
    tft.setCursor(20, 100);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.printf("Voltage: %.2f V", batteryVoltage);
    Serial.println(batteryVoltage);

    delay(2000);  // 每 2 秒更新一次
}
