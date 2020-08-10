
#include <EEPROM.h>

byte matrix[10][10][2];
const byte actornum=6;
byte actorpos[][2]={{255,255},{255,255},{255,255},{255,255},{255,255},{255,255}};
byte actordir[actornum][2];
byte actorvar[actornum];
bool actoract[]={true,true,true,true,true,true};
byte actorord[actornum];
byte actorlink[actornum][3];
byte actorlay[actornum];
byte instructnum = 23;


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

bool playing = false;
bool debugactor[actornum];

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 16 bytes for the inputString:
  inputString.reserve(14);
  Serial.setTimeout(15);
}


void loop() {
  inputString=Serial.readStringUntil("\n");
  if (inputString.length()>0){
    Serial.println(inputString);
  }
  if (inputString.startsWith("m") and inputString.length()>0 ){ // Command:"m x,y,z,v"
        int x=int(inputString[2])-48;
        int y=int(inputString[4])-48;
        int z=int(inputString[6])-48;
        byte val=byte(inputString.substring(8).toInt());
        matrix[x][y][z]=val;

  }
  if (inputString.startsWith("a") and inputString.length()>0 ){ // Command: "a i,x,y,z,l"
        byte index = byte(inputString[2]-48);
        byte x = byte(inputString[4]-48);
        byte y = byte(inputString[6]-48);
        byte z = byte(inputString[8]-48);
        byte l = byte(inputString[10]-48);
        actorpos[index][0]=x;
        actorpos[index][1]=y;
        actorlay[index]=z;
        actorlink[index][0]=l;
  }
  if (inputString.startsWith("p") and inputString.length()>0){ // Command: "p"
    playing = not playing;
  }
  if (inputString.startsWith("s") and inputString.length()>0){ // Command: "s" it saves
    for(int z =0; z<2;z++){ 
      for(int x=0 ; x<10;x++){
        for(int y=0 ; y<10;y++){
          EEPROM.write(z*100 + x*10 + y, matrix[x][y][z]);
        }
      }
    }
    Serial.println("Saved matrix!");
    for(int i=0; i<6;i++){
      for(int j=0; j<2;j++){
        EEPROM.write(200+i*2+j,actorpos[i][j]);
      }
    }// 211 bytes
    for(int i=0; i<6;i++){
      for(int j=0; j<2;j++){
        EEPROM.write(212+i*2+j,actordir[i][j]);
      }
    }// 223 bytes
    for(int i=0; i<6;i++){
      EEPROM.write(223+i,actorvar[i]);
    }//229
    for(int i=0; i<6;i++){
      EEPROM.write(229+i,actoract[i]);
    }//236
    for(int i=0; i<6;i++){
      EEPROM.write(236+i,actorord[i]);
    }//243
    for(int i=0; i<6;i++){
      for(int j=0; j<3;j++){
        EEPROM.write(243+i*3+j,actordir[i][j]);
      }
    }//262
    for(int i=0; i<6;i++){
      EEPROM.write(262+i,actorlay[i]);
    }//269
    Serial.println("Saved!");
  }

  if (inputString.length()>0 and isDigit(inputString[0])){ // Command: "0/1/2/3/4 etc."
    debugactor[inputString[0]-48]= not debugactor[inputString[0]-48];
  }

  if (inputString.length()>0 and inputString.startsWith("t")){ // Command: "t"
    for(int z = 0;z<2;z++){
      for(int y = 0;y<10;y++){
        for(int x = 0;x<10;x++){
          Serial.print(matrix[x][9-y][1-z]);
          for (int i =0; i<2-String(matrix[x][9-y][1-z]).length();i++){
            Serial.print(" ");
          }
          Serial.print(" | ");
        }
       Serial.println("");
       for (int i=0;i<50;i++){
        Serial.print("=");
       }
       Serial.println("");
      }
     Serial.println("");
     Serial.println("");
     Serial.println("");
    }
  }

  
  if (inputString.startsWith("l") and inputString.length()>0){ // Command: "l" it loads
    for(int z=0; z<2;z++){ 
      for(int x=0; x<10;x++){
        for(int y=0; y<10;y++){
          matrix[x][y][z]=EEPROM.read(z*100 + x*10 + y);
        }
      }
    }
    Serial.println("loaded matrix!");
    for(int i=0; i<6;i++){
      for(int j=0; j<2;j++){
        actorpos[i][j]=EEPROM.read(200+i*2+j);
      }
    }// 211 bytes
    Serial.println("loaded positions");
    for(int i=0; i<6;i++){
      for(int j=0; j<2;j++){
        actordir[i][j]=EEPROM.read(212+i*2+j);
      }
    }// 223 bytes
    Serial.println("loaded directions");
    for(int i=0; i<6;i++){
      actorvar[i]=EEPROM.read(223+i);
    }//229
    for(int i=0; i<6;i++){
      actoract[i]=EEPROM.read(229+i);
    }//236
    for(int i=0; i<6;i++){
      actorord[i]=EEPROM.read(236+i);
    }//243
    for(int i=0; i<6;i++){
      for(int j; j<3;j++){
        actordir[i][j]=EEPROM.read(243+i*3+j);
      }
    }//262
    for(int i=0; i<6;i++){
      actorlay[i]=EEPROM.read(262+i);
    }//269
    Serial.println("Loaded!");
    playing=false;
  }
  
  if (inputString.startsWith("r") and inputString.length()>0){ // Read EEPROM
    for (int i = 0; i < 300; i++) {
      Serial.println(String(i)+" : "+String(EEPROM.read(i)));
    }
  }
  if (inputString.startsWith("c") and inputString.length()>0){ // Clear EEPROM
    for (int i = 0; i < 300; i++) {
      EEPROM.write(i, 255);
    }
    Serial.println("Cleared!");
  }

  if (playing){
    for (int i=0; i<actornum; i++){
        if (actorpos[i][0]!=255 and actorpos[i][1]!=255){
          if (actoract[i]){
            actorord[i]=matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]];
          }
          if (actorord[i]>instructnum and actorord[i]<=254){
            actorord[byte(actorlink[byte(actorord[i]/instructnum)])]=actorord[i]%instructnum;
            Serial.print("ACTOR "+String(i));
            Serial.print(" ON TILE "+String(actorord[i]));
            Serial.println(" TELLS "+String(byte(actorlink[byte(actorord[i]/instructnum)]))+" TO DO "+ String(actorord[i]%instructnum));
            Serial.println(matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]]);
            Serial.println("");
          }




    
          if (actorord[i]<=instructnum){
            switch(actorord[i]){
              
            case 2:
    
              actordir[i][1]=1;
              actordir[i][0]=0;
              break;
            case 1:
              actordir[i][0]=1;
              actordir[i][1]=0;
              break;
            case 4:
              actordir[i][1]=-1;
              actordir[i][0]=0;
              break;
            case 3:
              actordir[i][0]=-1;
              actordir[i][1]=0;
              break;
            case 5:
              actordir[i][0]=0; 
              actordir[i][1]=0;
              break;
            case 7:
              actordir[i][0]=0;
              actordir[i][1]=0;
              actorpos[i][1]+=1;
              break;
            case 6:
              actordir[i][0]=0;
              actordir[i][1]=0;
              actorpos[i][0]+=1;
              break;
            case 9:
              actordir[i][0]=0;
              actordir[i][1]=0;
              actorpos[i][1]-=1;
              break;
            case 8:
              actordir[i][0]=0;
              actordir[i][1]=0;
              actorpos[i][0]-=1;
              break;
            case 11:
              actorvar[i]=matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]];
              break;
            case 12:
              actorvar[i]=actorvar[actorlink[i][0]]+actorvar[actorlink[i][1]]+actorvar[actorlink[i][2]];

              break;
            case 13:
              matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]]++;
              break;
            case 14:
              matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]]--;
              break;
            case 15:
              matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]]+=actorvar[i];
              break;
            case 16:
              matrix[actorpos[i][0]][actorpos[i][1]][actorlay[i]]-=actorvar[i];
              break;
              
            case 10:
              for (int j; i>=actornum; j++){
                if(actorlay[j]==actorlay[i]){
                  if(abs(actorpos[i][0]-actorpos[j][0])<=1 and abs(actorpos[i][1]-actorpos[j][1])<=1){ // if statement. 
                    actorpos[i][0]+=2*(actorpos[i][0]-actorpos[j][0]);
                    actorpos[i][1]+=2*(actorpos[i][1]-actorpos[j][1]);
                    actordir[i][0]=0; 
                    actordir[i][1]=0;
                  }
                }
              }
            break;
            case 17: // toggle activity. 
              actoract[i]= !actoract[i];
            break;
            case 21:
              Serial.print(char(actorvar[i]+32));
            break;
            } 
          }
      
      if (debugactor[i]!=0){
        Serial.println("Actor "+ String(i));
        Serial.print("  Position ");
        Serial.print(actorpos[i][0]);
        Serial.print(" , ");
        Serial.print(actorpos[i][1]);
        Serial.print(" , ");
        Serial.println(actorlay[i]);

        Serial.print("  Direction ");
        Serial.print(actordir[i][0]);
        Serial.print(" , ");
        Serial.println(actordir[i][1]);

        Serial.print("  Var ");
        Serial.println(actorvar[i]);

        Serial.print("  Order ");
        Serial.println(actorord[i]);
        Serial.println();
      }

        
      actorpos[i][0]+=actordir[i][0];
      actorpos[i][1]+=actordir[i][1];
      if (actorpos[i][0]>9 or actorpos[i][0]<0 or actorpos[i][1]>9 or actorpos[i][1]<0) {
        actorpos[i][0]=255;
        actorpos[i][1]=255;
        Serial.println("ACTOR "+String(i)+" HAS FALLEN OFF THE MAP");
      }

      
     }
    }
  }

}  
