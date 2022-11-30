#include "LichessAPI.h"
String LichessAPI::lichessToken;
String LichessAPI::currentGameId;

void LichessAPI::setLichessToken(String token){ // sets lichessAPI token
    LichessAPI::lichessToken = token;
}

String LichessAPI::getAllTop10(){
    return httpGet("https://lichess.org/api/player");
}

String LichessAPI::getMyOngoingGames(){
    return httpGet("https://lichess.org/api/account/playing");
}

String LichessAPI::getTheDailyPuzzle(){
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
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/move/" + move);
}

void LichessAPI::setCurrentGameId(String gameId){
    LichessAPI::currentGameId = gameId;
}

String LichessAPI::getCurrentGameId(){
    return LichessAPI::currentGameId;
}

String LichessAPI::getCurrentGameFEN(){
    String jsonRecieved = httpGet("https://lichess.org/api/account/playing"); // get json of all games played
    int indexOf = jsonRecieved.indexOf(LichessAPI::getCurrentGameId());
    String fenString = "\"fen\":\"";
    int fenPosition = jsonRecieved.indexOf(fenString, indexOf) + fenString.length();
    int endQuotePosition = jsonRecieved.indexOf("\"", fenPosition);

    return jsonRecieved.substring(fenPosition,endQuotePosition);
}


String LichessAPI::abortGame(String gameId){
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/abort");
}

String LichessAPI::resignGame(String gameId){
    return httpPost("https://lichess.org/api/bot/game/" + gameId + "/resign");
}

String LichessAPI::httpGet(String url){
    Serial.print("Url being used: ");
    Serial.println(url);

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
            Serial.print("HTTP payload: ");
            Serial.println(payload);
            Serial.println();
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
            Serial.print("HTTP payload: ");
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


