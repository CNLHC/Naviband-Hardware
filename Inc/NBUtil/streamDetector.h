#include <string.h>
#include <stdlib.h>
#include <stdint.h>
/*!
to dectect specific string pattern from char stream with O(n) space.
*/

typedef struct streamPatternDetector{
        char *mPatternBuffer;
        int mPatternLength;
        int mInnerFSM;
        int mHitTimes;
} streamPatternDetector;

void sPDConstructor(streamPatternDetector *self,const char* pattern)
{
    int i;
    int len =  strlen(pattern);
    self->mPatternBuffer = (char *)malloc(sizeof(uint8_t)*len);
    self->mPatternLength = len;
    self->mHitTimes=0;
    self->mInnerFSM=0;

    
    for (i=0;i<len;i++)
        self->mPatternBuffer[i] = pattern[i];
}

void sPDDestructor(streamPatternDetector * self){
    free(self->mPatternBuffer);
}

void sPDreset(streamPatternDetector * self){
    self->mInnerFSM = 0 ;
    self->mHitTimes = 0 ;
}
void sPDfeed(streamPatternDetector *self,char ch){
    if(ch == self->mPatternBuffer[self->mInnerFSM])
        self->mInnerFSM++;

    if (self->mInnerFSM == self->mPatternLength){
        self->mInnerFSM=0;
        self->mHitTimes++;
    }
}
int sPDGetHitTimes(streamPatternDetector *self){
    return self->mHitTimes;
}

