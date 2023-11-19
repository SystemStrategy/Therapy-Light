

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message, size_t len) {
  Serial.println("Attempting to write File");
  delay(1000);
  static uint8_t buf[512];
  int i = 0;
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  while (len) {
    size_t toWrite = len;
    if (toWrite > 512) {
      toWrite = 512;
    }
    memcpy(buf, message + (i * 512), toWrite);
    file.write(buf, toWrite);
    len -= toWrite;
    i++;
  }
  Serial.println("- file written");
  file.close();
}

void Save_Parameters_File() {
  memset(Clear_Text_Data, 0, 512);
  DynamicJsonBuffer JsonBuffer;
  JsonObject& root = JsonBuffer.createObject();
  
   //Notification Email Info
  JsonArray& Notification = root.createNestedArray("Duration");
  Notification.add(Duration);
  
  
  JsonArray& Final = root.createNestedArray("Colors");
  Final.add(Hex_Color1);
  Final.add(Hex_Color2);
  Final.add(Hex_Color3);
  Final.add(Hex_Color4);

  root.printTo(Clear_Text_Data);
  int len = strlen(Clear_Text_Data); 
  deleteFile(SPIFFS, "/Parameters.txt");
  writeFile(SPIFFS, "/Parameters.txt", Clear_Text_Data, len);
  Serial.println(Clear_Text_Data);
}

void Read_Parameters_File(fs::FS &fs, const char * path) {
  memset(Clear_Text_Data, 0, 512);
  bool Got_File = false;
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    return;
  }

  if (file && !file.isDirectory()) {
    //Serial.println("- read from file:");
    int j = 0;
    while (file.available()) {
      char c = (file.read());
      Clear_Text_Data[j] = c;
      Got_File = true;
      j++;
    }
    Clear_Text_Data[j] = '\0';
  }

  file.close();

  if (Got_File) {
    Serial.println(Clear_Text_Data);
    DynamicJsonBuffer JsonBuffer;
    JsonObject& root = JsonBuffer.parseObject(Clear_Text_Data);

    Duration  = root["Duration"][0].as<int>();

    Hex_Color1  = root["Colors"][0].as<String>();
    Hex_Color2  = root["Colors"][1].as<String>();
    Hex_Color3  = root["Colors"][2].as<String>();
    Hex_Color4  = root["Colors"][3].as<String>();
  }
}

void SetupFile(){
  if(!SPIFFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LITTLEFS Mount Failed");
    File_Avaliable = false;
    return;
  }
  else File_Avaliable = true;
  Read_Parameters_File(SPIFFS, "/Parameters.txt");
}