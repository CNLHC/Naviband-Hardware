
#include "NBBTHParser.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum NBBTHCommand NBParseFrameContent(const char *content){
    uint8_t  tBuffer[50];
    memcpy(tBuffer,content,50);
    if(strstr((const char *)tBuffer,"R1")!=NULL)
        return BTHCVibrateR;
    else if (strstr((const char *)tBuffer,"L1")!=NULL)
        return BTHCVibrateL;
    else
        return BTHCNULL;
}

