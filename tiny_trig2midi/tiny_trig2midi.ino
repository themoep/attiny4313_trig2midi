// alternative firmware for ladik m-216
// @themoep

#define LEDPIN 5
#define BTNPIN 4

#define NUMTRIGS 12
// ----------------------- ki  sn  ch  oh  cl  p2  t1  t2  t3  rd  cr  p1
const int mdMidiNotes[] = {36, 38, 42, 46, 39, 67, 48, 45, 41, 51, 49, 65};
const int triggerPins[] = {16,  0,  6,  7, 15, 14, 13, 12, 11, 10,  9,  8}; // arduino pins
// --= trigger in on plate  1   2   3   4   5   6   7   8   9  10  11  12   // trigger on faceplate
// ------------ pin on avr 19   2   8   9  18  17  16  15  14  13  12  11   // avr pins


boolean triggerStates[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int trigCount = 0;


void setup()
{
  pinMode(LEDPIN, OUTPUT);

  // initialize all trigger pins as inputs
  int i;
  for (i = 0; i < NUMTRIGS; i++) {
    pinMode(triggerPins[i], INPUT);
    digitalWrite(triggerPins[i], LOW);
  }
  Serial.begin(31250);

  for (i = 0; i < NUMTRIGS; i++) {
    digitalWrite(LEDPIN, 1);
    delay(20);
    digitalWrite(LEDPIN, 0);
    delay(20);
  }
}


void loop()
{
  int i;
  for (i = 0; i < NUMTRIGS; i++) {
    boolean state = digitalRead(triggerPins[i]);
    if (state == HIGH && triggerStates[i] == LOW)
    {
      noteOn(0x90, mdMidiNotes[i], 120);
      trigCount++;
      triggerStates[i] = state;
      continue;
    }

    if (state == LOW && triggerStates[i] == HIGH) {
      noteOn(0x90, mdMidiNotes[i], 0);
      trigCount--;
      triggerStates[i] = state;
      continue;
    }
  }
  if (trigCount > 0) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }
}


void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
