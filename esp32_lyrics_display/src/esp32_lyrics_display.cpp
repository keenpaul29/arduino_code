#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// WiFi Credentials
const char* WIFI_SSID = "";
const char* WIFI_PASS = "";

// Spotify Credentials
const char* CLIENT_ID     = "58cea4ed139d43e99b417e8d2075473f";
const char* CLIENT_SECRET = "e7f5de011c3e4644b0522394abd38170";
const char* REFRESH_TOKEN = "AQB147RpB9DMCB0o-h658LlINcFJjBxnqBrOnM2c06LwTuTTNSnZP63-75z_Fx4raPJjAriu6wRh4wWGTeNBY4oCQdTFHbxoR0xeLswzPAg68l7G_RyqfUR7ge7iKoXq-tk";

// Color Definitions (565 RGB)
#define SPOTIFY_GREEN 0x1DCB
#define DARK_BG       0x10A2
#define TEXT_WHITE    0xFFFF
#define TEXT_GRAY     0x8410

TFT_eSPI tft = TFT_eSPI();

String accessToken = "";
unsigned long tokenExpiresAt = 0;
String currentTrack = "";
String currentArtist = "";

bool getAccessToken() {
  if (millis() < tokenExpiresAt && accessToken != "") return true;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, "https://accounts.spotify.com/api/token");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String body = "grant_type=refresh_token&refresh_token=" + String(REFRESH_TOKEN) +
                "&client_id=" + String(CLIENT_ID) +
                "&client_secret=" + String(CLIENT_SECRET);

  int httpCode = http.POST(body);
  if (httpCode == 200) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());
    accessToken = doc["access_token"].as<String>();
    int expiresIn = doc["expires_in"];
    tokenExpiresAt = millis() + ((expiresIn - 60) * 1000);
    http.end();
    return true;
  }
  http.end();
  return false;
}

void displayTrackInfo(String track, String artist) {
  tft.fillRect(0, 0, 240, 80, DARK_BG);
  
  // Track Name
  tft.setTextColor(SPOTIFY_GREEN, DARK_BG);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(track.substring(0, 20), 10, 15, 4);

  // Artist Name
  tft.setTextColor(TEXT_GRAY, DARK_BG);
  tft.drawString(artist.substring(0, 24), 10, 45, 2);
  
  tft.drawFastHLine(10, 75, 220, TEXT_GRAY);
}

void fetchLyrics(String track, String artist) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = "https://lrclib.net/api/get?artist_name=" + artist + "&track_name=" + track;
  url.replace(" ", "%20");

  http.begin(client, url);
  int httpCode = http.GET();

  tft.fillRect(0, 85, 240, 235, DARK_BG);
  tft.setTextColor(TEXT_WHITE, DARK_BG);

  if (httpCode == 200) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());
    String syncedLyrics = doc["syncedLyrics"].as<String>();

    if (syncedLyrics.length() > 0) {
      tft.drawString("Synced Lyrics Loaded", 10, 100, 2);
    } else {
      tft.drawString("No Synced Lyrics Available", 10, 100, 2);
    }
  } else {
    tft.drawString("Lyrics Not Found", 10, 100, 2);
  }
  http.end();
}

void getCurrentlyPlaying() {
  if (!getAccessToken()) return;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  http.begin(client, "https://api.spotify.com/v1/me/player/currently-playing");
  http.addHeader("Authorization", "Bearer " + accessToken);

  int httpCode = http.GET();
  if (httpCode == 200) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());

    bool isPlaying = doc["is_playing"];
    if (!isPlaying) {
      tft.fillScreen(DARK_BG);
      tft.setTextColor(TEXT_WHITE, DARK_BG);
      tft.drawString("Playback Paused", 20, 140, 4);
      http.end();
      return;
    }

    String trackName = doc["item"]["name"].as<String>();
    String artistName = doc["item"]["artists"][0]["name"].as<String>();

    if (trackName != currentTrack) {
      currentTrack = trackName;
      currentArtist = artistName;
      displayTrackInfo(trackName, artistName);
      fetchLyrics(trackName, artistName);
    }
  } else if (httpCode == 204) {
    tft.fillScreen(DARK_BG);
    tft.setTextColor(TEXT_WHITE, DARK_BG);
    tft.drawString("Nothing Playing", 20, 140, 4);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0); // 0 or 2 for Portrait, 1 or 3 for Landscape
  tft.fillScreen(DARK_BG);
  
  tft.setTextColor(SPOTIFY_GREEN, DARK_BG);
  tft.drawString("Connecting WiFi...", 10, 20, 4);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  tft.fillScreen(DARK_BG);
  tft.drawString("Spotify Ready!", 10, 20, 4);
  delay(1000);
}

void loop() {
  getCurrentlyPlaying();
  delay(3000);
}