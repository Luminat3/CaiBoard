#include <Key.h>
#include <Keypad.h>

#include <BleKeyboard.h>




const byte rows = 5; //set 5 karena baris dan kolom keypad ada 5
const byte cols = 5;

//ini pake untuk dari lib agek return value dari array yang ado di bawah pas button dipencet
char keys[rows][cols] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', '0'},
  {'A', 'B', 'C', 'D', 'E'},
  {'F', 'G', 'H', 'I', 'J'},
  {'K', 'L', 'M', 'N', 'O'}
};


// nanti set pin yang bakal dipakek oleh keypeypad, harus berurutan
byte rowPin[rows] = {13, 12, 14, 27, 26};
byte colPin[cols] = {16, 17, 18, 19, 21};
Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, rows, cols);


BleKeyboard bleKeyboard("CaiPad25", "JAN", 100);

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();

  for (int i = 0; i < rows; i++) {
    pinMode(rowPin[i], OUTPUT);
    digitalWrite(rowPin[i], HIGH); // Set default baris ke HIGH
  }

  for (int i = 0; i < cols; i++) {
    pinMode(colPin[i], INPUT_PULLUP);
  }

}

void loop() {
  char key = keypad.getKey();
  if(bleKeyboard.isConnected() && key){
    switch(key){
      case '1' :
        bleKeyboard.press('1');
        bleKeyboard.releaseAll();
        break;
      case '2' :
        bleKeyboard.press('2');
        bleKeyboard.releaseAll();
        break;
      case '3' :
        bleKeyboard.write('3');
        break;
      case '4' :
        bleKeyboard.write('4');
        break;
      case '5' :
        bleKeyboard.write('5');
        break;
      case '6' :
        bleKeyboard.write('6');
        break;
      case '7' :
        bleKeyboard.write('7');
        break;
      case '8' :
        bleKeyboard.write('8');
        break;
      case '9' :
        bleKeyboard.write('9');
        break;
      case '0' :
        bleKeyboard.write('10');
        break;
      case 'A' :
        bleKeyboard.write('A');
        break;
      case 'B' :
        bleKeyboard.write('B');
        break;
      case 'C' :
        bleKeyboard.write('C');
        break;
      case 'D' :
        bleKeyboard.write('D');
        break;
      case 'E' :
        bleKeyboard.write('E');
        break;
      case 'F' :
        bleKeyboard.write('F');
        break;
      case 'G' :
        bleKeyboard.write('G');
        break;
      case 'H' :
        bleKeyboard.write('H');
        break;
      case 'I' :
        bleKeyboard.write('I');
        break;
      case 'J' :
        bleKeyboard.write('J');
        break;
      case 'K' :
        bleKeyboard.write('K');
        break;
      case 'L' :
        bleKeyboard.write('L');
        break;
      case 'M' :
        bleKeyboard.write('M');
        break;
      case 'N' :
        bleKeyboard.write('N');
        break;
      case 'O' :
        bleKeyboard.write('O');
        break;
    }
 //ngerelease semua button
  }
}
