#include "LichessAPI.h"
String LichessAPI::lichessToken;
String LichessAPI::currentGame;

void LichessAPI::setLichessToken(String token){ // sets lichessAPI token
    LichessAPI::lichessToken = token;
}

String LichessAPI::getTopTen(){
    return LichessAPI::httpGet("https://lichess.org/api/player");
}

String LichessAPI::getOngoingGames(){
    return httpGet("https://lichess.org/api/account/playing");
}

String LichessAPI::getDailyPuzzle(){
    return httpGet("https://lichess.org/api/puzzle/daily");
}

String LichessAPI::getMyId(){
    return jsonParse(httpGet("https://lichess.org/api/account"), "id");
}

String LichessAPI::getMyProfile(){ // everything about the user, currently just json format
    return httpGet("https://lichess.org/api/account");
}

String LichessAPI::makeABoardMove(String gameId, String move){
    return httpPost("https://lichess.org/api/board/game/" + gameId + "/move/" + move);
}

String LichessAPI::makeABotMove(String gameId, String move){
    // return httpResponse("https://lichess.org/api/board/game/" + gameId + "/move/" + from + to);
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/move/" + move);
}

void LichessAPI::setCurrentGame(String gameId){
    LichessAPI::currentGame = gameId;
}

String LichessAPI::getCurrentGame(){
    return LichessAPI::currentGame;
}

String LichessAPI::abortGame(String gameId){
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/abort", "");
}

String LichessAPI::resignGame(String gameId){
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/resign", "");
}

String LichessAPI::httpGet(String url){
    if(WiFi.status()== WL_CONNECTED){ // if wifi is connected do the stuff
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(url.c_str());

        http.addHeader("Authorization", "Bearer " + LichessAPI::lichessToken); // header is needed to verify who is requsting with API token
    
        // Send HTTP GET request
        int httpResponseCode = http.GET();
        if (httpResponseCode>0) { // what we get back
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            return payload;
        } else{ // error
            String errorCode = "Error code: " + httpResponseCode;
            Serial.println(errorCode);
            return errorCode;
        }

        // Free resources
        http.end();

    } else{
        String wifiDisconnected = "WiFi Disconnected";
        Serial.println(wifiDisconnected);
        return wifiDisconnected;
    }
}

String LichessAPI::httpPost(String url){ // overloaded method that when a json data packet is not needed it calls the httpPost method with empty data
    return LichessAPI::httpPost(url, "");
}

String LichessAPI::httpPost(String url, String jsonBody){
    if(WiFi.status()== WL_CONNECTED){ // if wifi is connected do the stuff
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(url.c_str());

        http.addHeader("Authorization", "Bearer " + LichessAPI::lichessToken); // header is needed to verify who is requsting with API token
    
        int httpResponseCode = http.POST(jsonBody);
        if (httpResponseCode>0) { // what we get back
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            return payload;
        } else{ // error
            String errorCode = "Error code: " + httpResponseCode;
            Serial.println(errorCode);
            return errorCode;
        }

        // Free resources
        http.end();

    } else{
        String wifiDisconnected = "WiFi Disconnected";
        Serial.println(wifiDisconnected);
        return wifiDisconnected;
    }
}

String LichessAPI::jsonParse(String jsonStructure, String name){
    DynamicJsonDocument doc(1024); // dont know why 1024
    deserializeJson(doc, jsonStructure);
    String parsedName = doc[name];
    return parsedName;
}


