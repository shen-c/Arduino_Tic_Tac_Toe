void reconnect() {
  while (!client.connected()) {
    dbprintln("Starting Connection");
    if (client.connect(DEVICE_MQTT_NAME, mqtt_username, mqtt_password)) {  // Attempt to connect
      dbprintln("Success!");
    } else {
      Serial.print("Failure, retrying...");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

#define MQTT_TOPIC "tictactoe"

//Send MQTT status update
void sendState(bool active) {
  StaticJsonBuffer<512> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  String status = active? "on" : "off";
  root["tictactoe"] = status;


  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

  client.publish(MQTT_TOPIC, buffer, true);
}