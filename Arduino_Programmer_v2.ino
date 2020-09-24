#define PROG 2
#define ACLK 3

#define SER 5
#define RCLK 6
#define SRCLK 7

#define RESET 11

void initialize() {
  digitalWrite(PROG, LOW);
  digitalWrite(ACLK, LOW);
  digitalWrite(SER, LOW);
  digitalWrite(RCLK, LOW);
  digitalWrite(SRCLK, LOW);
  digitalWrite(RESET, LOW);

  pinMode(PROG, OUTPUT);
  pinMode(ACLK, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RESET, OUTPUT);
}

void cleanUp() {
  pinMode(PROG, INPUT);
  pinMode(ACLK, INPUT);
  pinMode(SER, INPUT);
  pinMode(RCLK, INPUT);
  pinMode(SRCLK, INPUT);
  pinMode(RESET, INPUT);
}

void storeOpcodesInShiftRegister(int data) {
  //Serial.print("Opcode: ");
  //Serial.print(data, BIN);
  //Serial.print("\n\n");
  for (int i = 0; i < 8; i++) {
    digitalWrite(SER, data & 0x80);
    //Serial.print("SER: ");
    //Serial.print(digitalRead(SER), BIN);
    //Serial.print("\n\n");
    data <<= 1;
    digitalWrite(SRCLK, HIGH);
    //delay(100);
    digitalWrite(SRCLK, LOW);
    //delay(100);
  }
}

void storeMemoryAddressInShiftRegister(int data) {
  //Serial.print("Address: ");
  //Serial.print(data, BIN);
  //Serial.print("\n\n");
  for (int i = 0; i < 8; i++) {
    digitalWrite(SER, data & 0x80);
    data <<= 1;
    digitalWrite(SRCLK, HIGH);
    //delay(100);
    digitalWrite(SRCLK, LOW);
    //delay(100);
  }
}

void storeShiftRegisterInStorageRegister() {
  digitalWrite(RCLK, HIGH);
  //delay(100);
  digitalWrite(RCLK, LOW);
}

void clockToStoreInRam() {
  digitalWrite(ACLK, HIGH);
  //delay(1000);
  digitalWrite(ACLK, LOW);
}

// Reset the computer
//
void reset() {
  digitalWrite(RESET, HIGH);
  delay(1000);
  digitalWrite(RESET, LOW);
}

// Macros to help w/ writing computer assembly programs
//
#define NOP          (0b00000000)
#define LDA(addr)    (0b00010000 | (addr))
#define ADD(addr)    (0b00100000 | (addr))
#define SUB(addr)    (0b00110000 | (addr))
#define STA(addr)    (0b01000000 | (addr))
#define LDI(imm)     (0b01010000 | (imm)) 
#define JMP(addr)    (0b01100000 | (addr))
#define JC(addr)     (0b01110000 | (addr))
#define JZ(addr)     (0b10000000 | (addr))
#define OUT          (0b11100000)
#define HALT         (0b11110000)

// Test program: count up by powers of 2
//
static int program[16] = {
  /*  0 */ LDI(1),            // start:  LDI 1     Start at 1
  /*  1 */ STA(15),           //         STA 15    Save A 
  /*  2 */ OUT,               // again:  OUT       Display a
  /*  3 */ ADD(15),           //         ADD 15    Double A
  /*  4 */ JC(0),             //         JC start  Reset when we overflow
  /*  5 */ STA(15),           //         STA 15    Save doubled value
  /*  6 */ JMP(2),            //         JMP again Keep doubling
  /*  7 */ NOP,
  /*  8 */ NOP, 
  /*  9 */ NOP, 
  /* 10 */ NOP, 
  /* 11 */ NOP, 
  /* 12 */ NOP, 
  /* 13 */ NOP, 
  /* 14 */ NOP, 
  /* 15 */ 0,                 // temporary storage
};

void setup() {
  Serial.begin(57600);
  initialize();
  digitalWrite(PROG, LOW);

  for (int i = 0; i < 16; i++) {
    storeOpcodesInShiftRegister(program[i]);
    storeMemoryAddressInShiftRegister(i);
    storeShiftRegisterInStorageRegister();
    //delay(2000);
    clockToStoreInRam();
    //delay(2000);
  }
  digitalWrite(PROG, HIGH);
  reset();
  cleanUp();
}

void loop() {
}

