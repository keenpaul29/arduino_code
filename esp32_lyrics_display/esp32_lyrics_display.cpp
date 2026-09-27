#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <SpotifyArduino.h>
#include <WiFiClientSecure.h>

// I2C Setup: Address 0x27, 16 Columns, 2 Rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure client;
SpotifyArduino spotify(client, "YOUR_SPOTIFY_CLIENT_ID", "YOUR_SPOTIFY_CLIENT_SECRET");

struct LyricLine {
  long timeMs;
  String text;
};

LyricLine lyrics[120];
int totalLines = 0;
String currentTrack = "";
String currentArtist = "";
long lastProgressMs = 0;
unsigned long lastUpdateMillis = 0;
bool currentlyPlayingValid = false;
bool spotifyIsPlaying = false;
long spotifyProgressMs = 0;
String spotifyTrackName = "";
String spotifyArtistName = "";

void processCurrentlyPlayingData(CurrentlyPlaying currentlyPlaying) {
  currentlyPlayingValid = true;
  spotifyIsPlaying = currentlyPlaying.isPlaying;
  spotifyProgressMs = currentlyPlaying.progressMs;
  spotifyTrackName = currentlyPlaying.trackName ? currentlyPlaying.trackName : "";
  spotifyArtistName = currentlyPlaying.numArtists > 0 && currentlyPlaying.artists[0].artistName
    ? currentlyPlaying.artists[0].artistName
    : "";
}

void parseLrc(String lrcText) {
  totalLines = 0;
  int lineStart = 0;
  while (lineStart < lrcText.length() && totalLines < 120) {
    int lineEnd = lrcText.indexOf('\n', lineStart);
    if (lineEnd == -1) lineEnd = lrcText.length();

    String line = lrcText.substring(lineStart, lineEnd);
    line.trim();

    if (line.startsWith("[")) {
      int closeBracket = line.indexOf(']');
      if (closeBracket > 0) {
        String timeStr = line.substring(1, closeBracket);
        String text = line.substring(closeBracket + 1);

        int minutes = timeStr.substring(0, 2).toInt();
        float seconds = timeStr.substring(3).toFloat();
        long timeMs = (minutes * 60 + seconds) * 1000;

        lyrics[totalLines].timeMs = timeMs;
        lyrics[totalLines].text = text;
        totalLines++;
      }
    }
    lineStart = lineEnd + 1;
  }
}

void fetchLyrics(String trackName, String artistName) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  
  String encodedTrack = trackName;
  String encodedArtist = artistName;
  encodedTrack.replace(" ", "%20");
  encodedArtist.replace(" ", "%20");

  String url = "https://lrclib.net/api/get?track_name=" + encodedTrack + "&artist_name=" + encodedArtist;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(8192);
    deserializeJson(doc, payload);
    String syncedLyrics = doc["syncedLyrics"].as<String>();
    parseLrc(syncedLyrics);
  } else {
    totalLines = 0;
    lcd.clear();
    lcd.print("No Lyrics Found");
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9); // SDA=8, SCL=9
  lcd.init();
  lcd.backlight();
  lcd.print("Connecting WiFi...");

  WiFiManager wm;
  wm.autoConnect("Spotify-Lyrics-AP");

  lcd.clear();
  lcd.print("WiFi Connected!");
  client.setInsecure();
}

void renderLyrics(long currentProgressMs) {
  if (totalLines == 0) return;

  String currentLineText = "...";
  for (int i = 0; i < totalLines; i++) {
    if (currentProgressMs >= lyrics[i].timeMs) {
      currentLineText = lyrics[i].text;
    } else {
      break;
    }
  }

  static String lastDisplayed = "";
  if (currentLineText != lastDisplayed) {
    lastDisplayed = currentLineText;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(currentLineText.substring(0, 16));
    if (currentLineText.length() > 16) {
      lcd.setCursor(0, 1);
      lcd.print(currentLineText.substring(16, 32));
    }
  }
}

void loop() {
  // Poll Spotify API every 2 seconds
  if (millis() - lastUpdateMillis > 2000) {
    currentlyPlayingValid = false;
    int statusCode = spotify.getCurrentlyPlaying(processCurrentlyPlayingData);
    if (statusCode != 200 || !currentlyPlayingValid) {
      lastUpdateMillis = millis();
      return;
    }

    if (spotifyIsPlaying) {
      String newTrack = spotifyTrackName;
      String newArtist = spotifyArtistName;

      if (newTrack != currentTrack || newArtist != currentArtist) {
        currentTrack = newTrack;
        currentArtist = newArtist;
        lcd.clear();
        lcd.print("Fetching Lyrics");
        fetchLyrics(currentTrack, currentArtist);
      }

      lastProgressMs = spotifyProgressMs;
      lastUpdateMillis = millis();
    }
  }

  // Smooth local time calculation between polls
  long estimatedProgress = lastProgressMs + (millis() - lastUpdateMillis);
  renderLyrics(estimatedProgress);
  delay(100);
}