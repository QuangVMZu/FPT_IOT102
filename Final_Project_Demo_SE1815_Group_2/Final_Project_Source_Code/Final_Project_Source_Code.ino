// author : IoT Group 2 //
// link YTB: https://www.youtube.com/@HộiYêuIOTNhóm2 //

#include <EEPROM.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>

#define bluetooth Serial  // Use hardware Serial for HC-05

// Pin definitions
#define TRIG_PIN 7
#define ECHO_PIN 6
#define RELAY_PIN 8
#define RST_PIN 9
#define SS_PIN 10

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { A0, A1, A2, A3 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RFID configuration
MFRC522 rfid(SS_PIN, RST_PIN);

// Password and RFID storage
char password[7] = "123456";
char inputPassword[7];
byte inputIndex = 0;
String rfidCards[4];
int rfidCount = 0;

// System states
static bool doorLocked = true;
static unsigned long lastDetectionTime = 0;
static unsigned long lastDistanceCheck = 0;
static const long DETECTION_COOLDOWN = 1000 * 45;
static bool systemActive = false;

// EEPROM addresses
const int PASSWORD_ADDR = 0;
const int RFID_ADDR = 10;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Relay off (door locked)

  Serial.begin(9600);
  bluetooth.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.begin(16, 2);
  lcd.backlight();

  loadPassword();
  loadRFIDCards();
}

void loop() {
  static unsigned long lastRFIDCheck = 0;
  if (millis() - lastRFIDCheck > 1000 * 30) {  // Reset RFID sau 30 giây không đọc
    resetRFID();
    lastRFIDCheck = millis();
  }

  if (millis() - lastDistanceCheck > 1000) {  // Chỉ đo khoảng cách mỗi 1 giây
    if (measureDistance() < 50) {

      if (millis() - lastDetectionTime < DETECTION_COOLDOWN) {

        systemActive = true;
        lcd.backlight();
        lastDetectionTime = millis();
        lcd.setCursor(0, 0);
        lcd.print("LOCKED             ");
      }
    } else {

      if (millis() - lastDetectionTime > 1000 * 10) {
        systemActive = false;
        lcd.noBacklight();
      }
    }
    lastDistanceCheck = millis();
  }

  if (systemActive) {
    checkKeypad();
    checkRFID();
    checkBluetooth();
  }
}

void resetRFID() {
  digitalWrite(RST_PIN, LOW);
  digitalWrite(RST_PIN, HIGH);
  rfid.PCD_Init();
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void checkKeypad() {
  char key = keypad.getKey();
  if (key) {
    if (key >= '0' && key <= '9') {
      if (inputIndex < 6) {
        inputPassword[inputIndex] = key;
        lcd.setCursor(inputIndex, 1);
        lcd.print("*");
        inputIndex++;

        if (inputIndex == 6) {
          inputPassword[6] = '\0';

          Serial.print("Entered: ");
          Serial.println(inputPassword);
          Serial.print("Stored : ");
          Serial.println(password);

          if (strcmp(inputPassword, password) == 0) {
            unlockDoor();
          } else {
            lcd.clear();
            lcd.print("Wrong Password ");
            lcd.setCursor(0, 1);
            lcd.print("Try again...   ");
            delay(2000);
            resetInput();
          }
        }
      }
    }

    else if (key == '#') {
      resetInput();
    }
  }
}
void checkRFID() {
  static unsigned long lastDisplay = 0;
  static bool displayActive = false;

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    char cardID[9] = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      sprintf(cardID + strlen(cardID), "%02X", rfid.uid.uidByte[i]);
    }

    bool authorized = false;
    for (int i = 0; i < rfidCount; i++) {
      if (strcmp(cardID, rfidCards[i].c_str()) == 0) {
        authorized = true;
        break;
      }
    }

    lcd.clear();
    Serial.println(cardID);
    if (authorized) {
      Serial.print("RFID Accepted: ");
      Serial.println(cardID);
      unlockDoor();
      lastDisplay = millis();
      displayActive = true;
    } else {
      lcd.print("RFID Rejected");
      Serial.print("RFID Rejected: ");
      Serial.println(cardID);
      lastDisplay = millis();
      displayActive = true;
    }

    rfid.PICC_HaltA();
  }

  // Xóa thông báo sau 2 giây
  if (displayActive && millis() - lastDisplay >= 2000) {
    lcd.clear();
    lcd.print("LOCKED");
    displayActive = false;
  }
}

void checkBluetooth() {
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim();

    if (command == "OPEN") {
      unlockDoor();
      bluetooth.println("DOOR_OPEN");
    } else if (command == "CLOSE") {
      lockDoor();
      bluetooth.println("DOOR_CLOSED");
    } else if (command.startsWith("SETPASS:")) {
      String newPass = command.substring(8);
      if (newPass.length() == 6) {
        newPass.toCharArray(password, 7);
        password[6] = '\0';
        savePassword();
        bluetooth.println("PASS_UPDATED");
      }
    } else if (command == "RESETPASS") {
      char defaultPass[7] = "123456";
      for (int i = 0; i < 6; i++) {
        password[i] = defaultPass[i];
      }
      password[6] = '\0';
      savePassword();
      bluetooth.println("PASS_RESET");
    } else if (command.startsWith("ADDRFID:")) {
      String newRFID = command.substring(8);
      if (rfidCount < 4) {
        rfidCards[rfidCount] = newRFID;
        rfidCount++;
        saveRFIDCards();
        bluetooth.println("RFID_ADDED");
      }
    } else if (command.startsWith("DELRFID:")) {
      String delRFID = command.substring(8);
      for (int i = 0; i < rfidCount; i++) {
        if (rfidCards[i] == delRFID) {
          for (int j = i; j < rfidCount - 1; j++) {
            rfidCards[j] = rfidCards[j + 1];
          }
          rfidCount--;
          saveRFIDCards();
          bluetooth.println("RFID_DELETED");
          break;
        }
      }
    } else if (command == "GETPASS") {
      bluetooth.println("PASS:" + String(password));
    } else if (command == "GETRFIDS") {
      String rfids = "RFIDS:";
      for (int i = 0; i < rfidCount; i++) {
        rfids += rfidCards[i];
        if (i < rfidCount - 1) rfids += ",";
      }
      bluetooth.println(rfids);
    }
  }
}

void unlockDoor() {
  digitalWrite(RELAY_PIN, HIGH);
  doorLocked = false;
  lcd.clear();
  lcd.print("SUCCESS!");
  lcd.setCursor(0, 1);
  lcd.print("DOOR UNLOCKED");
  bluetooth.println("DOOR_OPEN");
  delay(5000);
  lockDoor();
}

void lockDoor() {
  digitalWrite(RELAY_PIN, LOW);
  doorLocked = true;
  lcd.clear();
  lcd.print("LOCKED");
  bluetooth.println("DOOR_CLOSED");
  resetInput();
}

void resetInput() {
  inputIndex = 0;
  memset(inputPassword, 0, sizeof(inputPassword));
  lcd.setCursor(0, 0);
  lcd.print("LOCKED          ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
}

void loadPassword() {
  for (int i = 0; i < 6; i++) {
    char c = EEPROM.read(PASSWORD_ADDR + i);
    password[i] = isPrintable(c) ? c : '1';
  }
  password[6] = '\0';
}

void savePassword() {
  for (int i = 0; i < 6; i++) {
    EEPROM.update(PASSWORD_ADDR + i, password[i]);
  }
  EEPROM.update(PASSWORD_ADDR + 6, '\0');
}

void loadRFIDCards() {
  rfidCount = EEPROM.read(RFID_ADDR);
  if (rfidCount > 4) rfidCount = 0;

  for (int i = 0; i < rfidCount; i++) {
    String cardID = "";
    for (int j = 0; j < 8; j++) {
      char c = EEPROM.read(RFID_ADDR + 1 + i * 8 + j);
      cardID += c;
    }
    rfidCards[i] = cardID;
  }
}

void saveRFIDCards() {
  EEPROM.update(RFID_ADDR, rfidCount);
  for (int i = 0; i < rfidCount; i++) {
    for (int j = 0; j < 8; j++) {
      EEPROM.update(RFID_ADDR + 1 + i * 8 + j, rfidCards[i][j]);
    }
  }
}
