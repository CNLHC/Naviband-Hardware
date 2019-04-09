#include "NBUtil/streamDetector.h"
#include "NBTask/NBTaskBTHDaemon.h"
#include <stdint.h>
#include "cmsis_os.h"

extern osMessageQId qBTHSerialReadHandle;


void NBBTHDaemonEntry(void const * argument){
    streamPatternDetector tFrameHeadDetector;
    streamPatternDetector tFrameEndDetector;
    sPDConstructor(&tFrameEndDetector,"NB__end__\r\n");
    sPDConstructor(&tFrameHeadDetector,"NB__head__\r\n");
    uint32_t  tInnerFSM = 0;
    uint8_t tCHBuffer;
    uint8_t tCommandBuffer[100];
    uint8_t tBufferIndex=0;
    osEvent evt;
    while(1){
        evt=osMessageGet(qBTHSerialReadHandle,osWaitForever);
        if(evt.status!=osEventMessage)
            continue;
        tCHBuffer = (uint8_t )(evt.value.v);
        sPDfeed(&tFrameEndDetector,tCHBuffer);
        sPDfeed(&tFrameHeadDetector,tCHBuffer);
        switch(tInnerFSM){
            case 0:
                if(sPDGetHitTimes(&tFrameHeadDetector)>0)
                    tInnerFSM=1;
                break;
            case 1:
                if(sPDGetHitTimes(&tFrameEndDetector)==0&& tBufferIndex<=100)
                   tCommandBuffer[tBufferIndex++]=tCHBuffer;
                else{
                    tCommandBuffer[tBufferIndex++]='\0';
                    tInnerFSM=2;
                }
                break;
            case 2:
                sPDreset(&tFrameEndDetector);
                sPDreset(&tFrameHeadDetector);
                tBufferIndex=0;
                tInnerFSM=0;
                break;
        }
    }
   // should not reach here
   // sPDDestructor(&tFrameEndDetector);
   // sPDDestructor(&tFrameHeadDetector);
}


