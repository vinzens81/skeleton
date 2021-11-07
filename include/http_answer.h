void standaloneAP();
void handleRoot();
void getParams();
void resetWifi();
void handleNotFound();
void getStatus();

void handleRoot()
{
  server.send(200, "text/plain", "hello, Nothing to do!");
}

void setParams()
{
  String message = "";

  if (server.arg("name").length() > 0)
  {
    if (server.arg("content").length() > 0)
    {
      if (writeFile(server.arg("name"), server.arg("content")))
      {
        message = "Saved!";
      }
      else
      {
        message = "Error during write to disk";
      }
    }
    else
    {
      message = "No Content Supplied";
    }
  }
  else
  {
    message = "Query Params error!";
  }
  server.send(200, "text/plain", message);
}

void getParams()
{
  String message = "";
  if (server.arg("name").length() > 0)
  {
    message = readFile(server.arg("name"));
    Serial.print("Got Param from file: ");
    Serial.println(message);
  }
  else
  {
    Serial.println("Parameter not given");
    message = "Query Params error!";
  }
  server.send(200, "text/plain", message);
}

void resetWifi()
{
  digitalWrite(led, 1);
  boolean ssidDelete = deleteFile("ssid");
  boolean passDelete = deleteFile("password");
  if (ssidDelete and passDelete)
  {
    server.send(200, "text/plain", "Deleted!");
    ESP.restart();
  }
  else
  {
    server.send(500, "text/plain", "something went wrong!");
  }
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void getStatus()
{
  String list = getListOfAllFiles();
  String message = "";
  message += "<html><body><h1>Welcom to ESP</h1><br>";
  message += "ESP connected to: ";
  message += ssid;
  message += "<br>";
  message += "Local IP Adress is: ";
  message += localIP;
  message += "<br><br><br><br><br>";
  message += "<br><p style='color:red;'>Danger Area!!</p><br>";
  message += "<form action='/getParams'>Get internal stored variable: <input type='text' name='name'><input type='submit' value='Submit'></form>";
  message += "<form action='/setParams'>Set internal stored variable: <input type='text' name='name'><input type='text' name='content'><input type='submit' value='Submit'></form><br>";
  message += "<form action='/resetWifi'>reset Wifi settings <input type='submit' value='reset'></form>";
  message += "<br><br><br><br><br>";
  message += list;

  message += "</body></html>";
  server.send(200, "text/html", message);
}

void standaloneAP()
{
  String message = "";
  message += "<html><body><h1>Welcom to ESP</h1><br>";
  message += "The ESP is in standalone Mode. Pease set Wifi Name and Password";
  message += "<br>";
  message += "<form action='/setParams'>Set SSID: <input type='hidden' name='name' value='ssid'/><input type='text' name='content'><input type='submit' value='Submit'></form><br>";
  message += "<form action='/setParams'>Set Pass: <input type='hidden' name='name' value='password'/><input type='text' name='content'><input type='submit' value='Submit'></form><br>";
  message += "<br>After setting both, you need to reset the ESP";
  message += "<br><br><br><p style='color:red;'>Danger Area!!</p><br>";
  message += "<form action='/getParams'>Get internal stored variable: <input type='text' name='name'><input type='submit' value='Submit'></form>";
  message += "<form action='/setParams'>Set internal stored variable: <input type='text' name='name'><input type='text' name='content'><input type='submit' value='Submit'></form><br>";
  message += "<form action='/resetWifi'>reset Wifi settings <input type='submit' value='reset'></form>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}
