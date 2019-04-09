#include "NBUtil/streamDetector.h"
#include "NBTask/NBTaskBTHDaemon.h"
#include <stdint.h>
void NBBTHDaemonEntry(void const * argument){
    streamPatternDetector tFrameHeadDetector;
    streamPatternDetector tFrameEndDetector;
    sPDConstructor(&tFrameEndDetector,"NB__fend__\r\n");
    sPDConstructor(&tFrameHeadDetector,"NB__fhead__\r\n");
    uint32_t  tInnerFSM = 0;
    uint8_t tCHBuffer;
    uint8_t tCommandBuffer[100];
    uint8_t tBufferIndex=0;
    while(1){
        sPDfeed(&tFrameEndDetector,tCHBuffer);
        sPDfeed(&tFrameHeadDetector,tCHBuffer);
        switch(tInnerFSM){
            case 0:
                if(sPDGetHitTimes(&tFrameHeadDetector)>0)
                    tInnerFSM=1;
                break;
            case 1:
                if(sPDGetHitTimes(&tFrameEndDetector)==0|| tBufferIndex>=100)
                   tCommandBuffer[tBufferIndex++]=tCHBuffer;
                else
                    //TODO parser logic and queueHandle
                    tInnerFSM=2;
                break;
            case 2:
                sPDreset(&tFrameEndDetector);
                sPDreset(&tFrameHeadDetector);
                tBufferIndex=0;
        }
    }
   // should not reach here
   // sPDDestructor(&tFrameEndDetector);
   // sPDDestructor(&tFrameHeadDetector);
}


