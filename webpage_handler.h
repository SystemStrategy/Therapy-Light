
void general_prog() {
  String s = General_page; //Read HTML contents 
  if(Wrong_Password) s.replace("@@error_msg@@", "Incorrect Password");
  else s.replace("@@error_msg@@", "");
  s.replace("@@Webpage_Duration@@", String(Duration));
  s.replace("@@Webpage_Hex_Color1@@", Hex_Color1);
  s.replace("@@Webpage_Hex_Color2@@", Hex_Color2);
  s.replace("@@Webpage_Hex_Color3@@", Hex_Color3);
  s.replace("@@Webpage_Hex_Color4@@", Hex_Color4);
  Wrong_Password = false;
  server.send(200, "text/html", s); //Send web page
}

void Webpage_Entry() {
  if(Password == server.arg("Login_Pass")){
    Duration = server.arg("Webpage_TX_Duration").toInt();
    Hex_Color1  = server.arg("Webpage_TX_Hex_Color1");
    Hex_Color2  = server.arg("Webpage_TX_Hex_Color2");
    Hex_Color3  = server.arg("Webpage_TX_Hex_Color3");
    Hex_Color4  = server.arg("Webpage_TX_Hex_Color4");
    Save_Parameters_File();
    Wrong_Password = false;
  }
  else Wrong_Password = true;
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Updated-- Press Back Button");
}


void Start_Web_Services(){

  WiFi.disconnect();   //added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); //added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Therapy Light Config");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);  

  server.on("/", []() {
      general_prog();   //form action is handled here
    });
  server.on("/hotspot-detect.html", []() {
      general_prog();   //form action is handled here
    });
  server.on("/generate_204", []() {
      general_prog();   //form action is handled here
    });
  server.on("/gen_204", []() {
      general_prog();   //form action is handled here
    });
  server.on("/params", []() {
      Webpage_Entry();   //form action is handled here
    });
  server.on("/reset", []() { 
      deleteFile(SPIFFS, "/Parameters.txt");
      server.sendHeader("Location", "/");
      server.send(302, "text/plain", "Updated-- Press Back Button");
      delay(5000);
      ESP.deepSleep(100000);    
    });

  server.begin();                  //Start server
}

