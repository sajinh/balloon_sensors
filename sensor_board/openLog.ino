void initialize_openLog(void){
  pinMode(resetOpenLog, OUTPUT);
  OpenLog.begin(9600);

  //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);
  //Serial.println("into OpenLog ");

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  }
  //Serial.println("OpenLog ok");
  //Send three control z to enter OpenLog command mode
  //This is how Arduino v0022 used to do it. Doesn't work with v1.0
  //Serial.print(byte(26));
  //Serial.print(byte(26));
  //Serial.print(byte(26));

  //Works with Arduino v1.0
  OpenLog.write(26);
  OpenLog.write(26);
  OpenLog.write(26);
  
  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '>') break;
  }
    
  //Send new (random from 0 to 999) file name
  
  //Old way
  sprintf(buff, "new log%03d.txt\r", fileNumber);
  OpenLog.print(buff); //\r in string + regular print works with older v2.5 Openlogs

  //New way
  //sprintf(buff, "new nate%03d.txt", fileNumber);
  //Serial.println(buff); //regular println works with v2.51 and above
  
  //Wait for OpenLog to return to waiting for a command
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '>') break;
  }
  
  sprintf(buff, "append log%03d.txt\r", fileNumber);
  OpenLog.print(buff);
  
  //Wait for OpenLog to indicate file is open and ready for writing
  while(1) {
    if(OpenLog.available())
      if(OpenLog.read() == '<') break;
  } 
}

