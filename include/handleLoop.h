#include <WebServer.h>

void handleConnectedLoop();
void handleStandaloneLoop();

int loopCounter = 0;
int globalCounter = 0;

void handleConnectedLoop(int led)
{
  // every loop
  server.handleClient();
  if (loopCounter > 100)
  {
    if (globalCounter > 600)
    {
      globalCounter = 0;
      // Max Loop time == 10 min
    }
    globalCounter++;
    loopCounter = 0;
    Serial.print("Reset loop Counter");
    Serial.println(globalCounter);
  }
  if (globalCounter % 2)
  {
    digitalWrite(led, 0);
  }
  else
  {
    digitalWrite(led, 1);
  }
  loopCounter++;
  delay(10);
}


void handleStandaloneLoop(int led)
{
  // Standalone AP
  // every loop
  server.handleClient();
  if (loopCounter > 100)
  {
    loopCounter = 0;
  }
  if (loopCounter % 10)
  {
    digitalWrite(led, 0);
  }
  else
  {
    digitalWrite(led, 1);
  }
  loopCounter++;
  delay(10);
}
