#ifndef LichessAPI_h
#define LichessAPI_h
    #include "Arduino.h"    // general library
    #include <HTTPClient.h> // HTTP requests
    #include <ArduinoJson.h>             //For parcing data to upload to website

    class LichessAPI{
        private: 
            static String lichessToken;
            static String httpGet(String url);
            static String httpPost(String url);
            static String httpPost(String url, String jsonBody);
            static String httpResponse(int httpResponseCode, HTTPClient aaaa);
            static String jsonParse(String jsonStructure, String name);
            static String currentGameId;
        public:
            static String getAllTop10(); // get top ten players
            static String getMyOngoingGames();
            static String getMyProfile();
            static String getMyId();
            static void setLichessToken(String token);
            static String makeABoardMove(String gameId, String move);
            static String makeABotMove(String gameId, String move);
            static String getTheDailyPuzzle();
            static String getCurrentGameId();
            static void setCurrentGameId(String gameId);
            static String abortGame(String gameId);
            static String resignGame(String gameId);
            static String getCurrentGameFEN();
    };

#endif