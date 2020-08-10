
byte tapes[5][192];
byte codepos[5];
byte variables[256];
byte flags[5];
byte tape_delay[5];
String inputString;
bool playing = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  inputString.reserve(14);
  Serial.setTimeout(15);

}


void loop() {
  // put your main code here, to run repeatedly:
  inputString = Serial.readStringUntil("\n");

  if (inputString.startsWith("tape set")) { // command: "tape set i yy ttt aaa bbb" i-> tape num, y-> instruction pos, t-> instruction num, a-> arg 1, b-> arg 2
    tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3] = byte(inputString.substring(14, 17).toInt());
    tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3 + 1] = byte(inputString.substring(18, 21).toInt());
    tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3 + 2] = byte(inputString.substring(22, 25).toInt());

  }
  if (inputString.startsWith("tape get")) {
    Serial.print(tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3]);
    Serial.print(" ");
    Serial.print(tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3 + 1]);
    Serial.print(" ");
    Serial.println(tapes[inputString.substring(9, 10).toInt()][inputString.substring(11, 13).toInt() * 3 + 2]);
  }
  if (inputString.startsWith("print var")) { // Command: "print var nnn"
    Serial.println(variables[inputString.substring(10, 14).toInt()]);
  }
  if (inputString.startsWith("print vals")) { // Command: "print vars"
    for (byte i;i<255;i++){
      Serial.print(variables[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  if (inputString.startsWith("pause")) { // Command: "print var nnn"
    playing = false;
  }
  if (inputString.startsWith("play")) { // Command: "print var nnn"
    playing = true;
  }




  if (inputString.startsWith("show tapes")) {
    for (byte i = 0; i < 5; i++) {
      Serial.print("Tape ");
      Serial.print(i);
      for (byte j = 0; j < 30; j++) {
        Serial.print(" ");
      }
    }
    Serial.println();
    for (byte y = 0; y < 64; y++) {
      for (byte i = 0; i < 5; i++) {
        byte var1 = tapes[i][3 * y + 1]; // ??????x? NNNNNNNN ????????
        byte var2 = tapes[i][3 * y + 2]; // ???????x ???????? NNNNNNNN
        // NNNNNN?? ???????? ????????
        byte instruction = tapes[i][y * 3];

        byte space = 36;
        if (instruction == tapes[i][3 * (y - 1)] >> 2 == 7 ) {
          Serial.print("  ");
          space -= 2;
        }

        switch (instruction % 64) {
          case 1:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" = "));
              Serial.print(var2);
              space -= 7 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" = "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 11 + String(var1).length() + String(var2).length();
            }
            break;
          case 2:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" += "));
              Serial.print(var2);
              space -= 8 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" += "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 12 + String(var1).length() + String(var2).length();
            }
            break;
          case 3:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" -= "));
              Serial.print(var2);
              space -= 8 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" -= "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 12 + String(var1).length() + String(var2).length();
            }
            break;
          case 4:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" *= "));
              Serial.print(var2);
              space -= 8 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" *= "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 12 + String(var1).length() + String(var2).length();
            }
            break;
          case 5:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" /= "));
              Serial.print(var2);
              space -= 8 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" /= "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 12 + String(var1).length() + String(var2).length();
            }
            break;
          case 6:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" %= "));
              Serial.print(var2);
              space -= 8 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" %= "));
              Serial.print(F("var "));
              Serial.print(var2);
              space -= 12 + String(var1).length() + String(var2).length();
            }
            break;
          case 7:
            if (instruction / 128 < 1) {
              Serial.print(F("if "));
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(" == ");
              Serial.print(var2);
              Serial.print(":");
              space -= 12 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("if "));
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(" == ");
              Serial.print(F("var "));
              Serial.print(var2);
              Serial.print(":");
              space -= 16 + String(var1).length() + String(var2).length();
            }
            break;
          case 8:
            Serial.print(F("Wflag "));
            Serial.print(var1);
            Serial.print(F(" num "));
            Serial.print(var2);
            space -= 11 + String(var1).length() + String(var2).length();
            break;
          case 9:
            //Serial.print("null instruction");
            //space-=16;
            break;
          case 10:
            if (instruction / 128 < 1) {
              Serial.print(F("delay "));
              Serial.print(var1);
              space -= 6 + String(var1).length();
            }
            else {
              Serial.print(F("delay var "));
              Serial.print(var1);
              space -= 10 + String(var1).length();
            }
            break;
          case 11:
            if (instruction / 128 < 1) {
              Serial.print(F("print "));
              Serial.print(var1);
              space -= 6 + String(var1).length();
            }
            else {
              Serial.print(F("print var "));
              Serial.print(var1);
              space -= 10 + String(var1).length();
            }

            break;
          case 12:
            if (instruction / 128 < 1) {
              Serial.print(F("var "));
              Serial.print(var1);
              Serial.print(F(" = serial["));
              Serial.print(var2);
              Serial.print(F("]"));
              space -= 15 + String(var1).length() + String(var2).length();
            }
            else {
              Serial.print(F("var ( var "));
              Serial.print(var1);
              Serial.print(F(" ) = serial[ var "));
              Serial.print(var2);
              Serial.print(F(" ]"));
              space -= 29 + String(var1).length() + String(var2).length();
            }
            break;
        }
        for (byte j = 0; j < space; j++) {
          Serial.print(" ");
        }
      }
      Serial.println();
    }
  }


  if (playing) {
    for (byte i = 0; i < 5; i++) {
      byte var1 = tapes[i][3 * codepos[i] + 1]; // ??????x? NNNNNNNN ????????
      byte var2 = tapes[i][3 * codepos[i] + 2]; // ???????x ???????? NNNNNNNN
      byte instruction = tapes[i][3 * codepos[i]] ;
      //    Serial.println(instruction%64);
      if (tape_delay[i] <= 0) {

        switch (instruction % 64) {

          case 1:
            if (instruction / 128 < 1) {
              variables[var1] = var2;
            }
            else {
              variables[var1] = variables[var2];
            }
            break;
          case 2:
            if (instruction / 128 < 1) {
              variables[var1] += var2;
            }
            else {
              variables[var1] += variables[var2];
            }
            break;
          case 3:
            if (instruction / 128 < 1) {
              variables[var1] -= var2;
            }
            else {
              variables[var1] -= variables[var2];
            }
            break;
          case 4:
            if (instruction / 128 < 1) {
              variables[var1] *= var2;
            }
            else {
              variables[var1] *= variables[var2];
            }
            break;
          case 5:
            if (instruction / 128 < 1) {
              variables[var1] /= var2;
            }
            else {
              variables[var1] /= variables[var2];
            }
            break;
          case 6:
            if (instruction / 128 < 1) {
              variables[var1] %= var2;
            }
            else {
              variables[var1] %= variables[var2];
            }
            break;
          case 7:
            if (instruction / 128 < 1) { // If aa == b do next line, otherwise, skip it.
              if (not variables[var1] == var2) {
                codepos[i]++;
              }
            }
            else {
              if (not variables[var1] == variables[var2]) {
                codepos[i]++;
              }
            }
            break;
          case 8:  //wait until (var1) flag num (var2) has this many flags
            byte num_flags = 0;
            for (byte x = 0; x < 5; x++) {
              if (flags[i] == flags[x]) {
                num_flags++;
              }
            }
            if (num_flags < var2) {
              codepos[i]--;
            }
            break;
          case 9: //non-instruction

            break;
          case 10: // delay
            if (instruction / 128 < 1) {
              tape_delay[i] = var1;
            }
            else {
              tape_delay[i] = variables[var1];
            }
            break;
          case 11:
            if (instruction / 128 < 1) {
              Serial.print(var1);
            }
            else {
              Serial.print(variables[var1]);
            }
            break;
          case 12: // Input (var1) which variable to write the character to (var2) which character you should write from
            if (instruction / 128 < 1) {
              variables[var1] = inputString[var2];
            }
            else {
              variables[variables[var1]] = inputString[variables[var2]];
            }
            break;

        }
        codepos[i] = (codepos[i] + 1) % 64;

      }
      else {
        tape_delay[i]--;
      }
    }
  }

  //  for (byte i=0;i<255;i++){
  //    if (variables[i]!=0){
  //      Serial.print(String(i)+":"+String(variables[i])+" ");
  //    }
  //  }
  //  Serial.println("");

}
