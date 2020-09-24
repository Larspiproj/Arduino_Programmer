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

void storeMemoryAddressInShiftRegister(int data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(SER, data & 0x80);
    data <<= 1;
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
  }
}

void storeOpcodesInShiftRegister(int data) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(SER, data & 0x80);
    data <<= 1;
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
  }
}

void storeShiftRegisterInStorageRegister() {
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}

void clockToStoreInRam() {
  digitalWrite(ACLK, HIGH);
  digitalWrite(ACLK, LOW);
}

// Reset the computer
void reset() {
  digitalWrite(RESET, HIGH);
  delay(1000);
  digitalWrite(RESET, LOW);
}

// Defining opcodes 
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

// Program fibonacci 
static int program[16] = {
  /*  0 */ LDI(0),
  /*  1 */ STA(13),
  /*  2 */ OUT,
  /*  3 */ LDI(1),
  /*  4 */ STA(14), 
  /*  5 */ OUT,
  /*  6 */ ADD(13),
  /*  7 */ JC(0),
  /*  8 */ STA(15), 
  /*  9 */ LDA(14), 
  /* 10 */ STA(13),
  /* 11 */ LDA(15), 
  /* 12 */ JMP(4), 
  /* 13 */ 0, 
  /* 14 */ 0,
  /* 15 */ 0,
};

void setup() {
  initialize();
  digitalWrite(PROG, LOW); // select programming mode and "disconnect" bus

  for (int i = 0; i < 16; i++) {
    storeMemoryAddressInShiftRegister(i);
    storeOpcodesInShiftRegister(program[i]);
    storeShiftRegisterInStorageRegister();
    clockToStoreInRam();
  }
  
  digitalWrite(PROG, HIGH); // quit programming mode and "connect" bus again
  reset(); // resets computer
  cleanUp();

}

void loop() {
}

