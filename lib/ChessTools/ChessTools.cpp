#include "ChessTools.h"

String ChessTools::parseFEN(String FEN){
    int i = 0;
    String parsedFEN;
    while(FEN.indexOf("/", i) != -1){

        // str = str.substring(str.indexOf(":") + 1);
        int indexFEN = FEN.indexOf("/", i);
        i = indexFEN;
        
    }

    return "";
}
