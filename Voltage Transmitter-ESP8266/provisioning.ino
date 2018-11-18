void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

boolean WiFiConnection() {

  WiFiManager wifiManager;

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  //  // id/name placeholder/prompt default length
  WiFiManagerParameter customZipCodeKey("zipCode", "Zip Code", zipCode, 10);
  WiFiManagerParameter customApiKey("apiKey", "API Key", apiKey, 40);
  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&customZipCodeKey);
  wifiManager.addParameter(&customApiKey);

  wifiManager.setTimeout(120);//2min, if can't get connected just go back to sleep
  if (!wifiManager.autoConnect("clearSky")) {// SSID you connect to from browser
    Serial.println("failed to connect and hit timeout");
    digitalWrite(DONEpin, HIGH);
    return 0;
  }

  Serial.println("Connected");

  //read updated parameters
  strcpy(zipCode, customZipCodeKey.getValue());
  strcpy(apiKey, customApiKey.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["zipCode"] = zipCode;
    json["apiKey"] = apiKey;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("");// WiFi info
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
