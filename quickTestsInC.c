#include <stdio.h>
#include <stdlib.h>
#include "arena.h"
#include "ui_types.h"

typedef enum month
{
    JAN = 4,
    FEB = 7,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
} month;

typedef struct info
{
    month mon;
    int number;
} info;

typedef struct moreInfo
{
    month mon;
    int number;
    float more;
    double wow;
} moreInfo;

size_t getTypeFromID(displayType_ID id)
{
    switch (id)
    {
    case DI_BOX:
        return sizeof(Box);
    case DI_IMAGE:
        return sizeof(Image);
    case DI_TEXT:
        return sizeof(Text);
    default:
        return 0;
    }
}

void LinkAllocateDisplayToElement(Arena *arena, UI_Element *elmnt, displayType_ID *id, size_t displayCount)
{
    elmnt->displayCount = displayCount;
    elmnt->displayData = (displayType *)arenaAlloc(arena, sizeof(displayType) * displayCount);

    for (size_t i = 0; i < displayCount; i++)
    {
        size_t size = getTypeFromID(*(id + i));
        (elmnt->displayData + i)->id = *(id + i);
        (elmnt->displayData + i)->displayData = arenaAlloc(arena, size);
    }
}

int main()
{

    // Testing casting void pointers with known byte sizes =======================
    /*
    info in = {.mon = JAN, .number = 25};

    void *vPtr = &in;

    month *mon = (month *)vPtr;

    // char* conversion = ((char *)vPtr + sizeof(month))
    int *numPtr = (int *)((char *)vPtr + sizeof(month));

    printf("The month is: %llu, The number is: %llu\n", *mon, *numPtr);

    moreInfo mInfo = {.mon = FEB, .number = 500, .more = 3.4f, .wow = 1.239999999};

    void *mInfPtr = &mInfo;

    info *inf = (info *)&mInfo;

    printf("The month is: %llu, The number is: %llu\n", inf->mon, inf->number);
    */

    //=============================================================================

    // Very basic arena play testing ==============================================

    /* void *arena = malloc(1024);
    size_t offset = 0;

    int nums[] = {1, 2, 3};
    float fNums[] = {1.2f, 2.3f, 3.4f};

    int *numsPtr = (int *)arena;
    float *fNumsPtr = NULL;

    for (int i = 0; i < 3; i++)
    {
        *(int *)(arena + offset) = nums[i];
        offset += sizeof(int);
    }

    fNumsPtr = (arena + offset);

    for (int i = 0; i < 3; i++)
    {
        *(float *)(arena + offset) = fNums[i];
        offset += sizeof(float);
    }

    printf("the int numbers are: %d, %d, %d\n", *numsPtr, *(numsPtr + 1), *(numsPtr + 2));
    printf("the int numbers are: %f, %f, %f\n", *fNumsPtr, *(fNumsPtr + 1), *(fNumsPtr + 2)); */

    //=============================================================================

    // Trying to process a pseudo jagged array ====================================

    /* month ids[] = {
        JAN,       // first 4
        FEB,       // second 7
        MAR, APR,  // third 8, 9
        MAY,       // fourth 10
        JUN,       // fifth 11
        JUL,       // sixth 12
        JAN, APR,  // seventh 4, 9
        FEB, JUL}; //eighth 7, 12

    size_t idSize[] = {1, 1, 2, 1, 1, 1, 2, 2};

    for (size_t i = 0, j = 0; i < 8; i++)
    {
        size_t k = 0;
        while (k < idSize[i])
        {
            printf("%d\n", ids[j + k++]);
        }
        j += k;
    } */

    //=============================================================================

    // allocating different structs into the same arena ===========================

    /* void *arena = malloc(100);
    size_t offset = 0;

    info *infPtr = (info *)((char *)arena + offset);
    offset += sizeof(info);

    moreInfo *morInfPtr = (moreInfo *)((char *)arena + offset);

    infPtr->mon = JAN;

    morInfPtr->number = 435;

    printf("%d, %d\n", infPtr->mon, morInfPtr->number);

    free(arena); */

    //==========================================================================

    // Testing out arena.h to see it if works ==================================

    /*  UI_Element *elementPtr = NULL;
     Arena arena;

     createArena(&arena, 1024);

     elementPtr = (UI_Element *)arenaAlloc(&arena, sizeof(UI_Element) * 3);

     elementPtr->btnData = (ButtonData *)arenaAlloc(&arena, sizeof(ButtonData));

     elementPtr->btnData->currentState = BSTATE_NORMAL;

     elementPtr->rect.x = 45.0f;

     SDL_Log("%f, %d\n", elementPtr->rect.x, elementPtr->btnData->currentState);

     destroyArena(&arena); */

    //==========================================================================

    // Checking allocating for display types ===================================

    Arena arena;

    createArena(&arena, 1000);

    displayType_ID ids[] = {
        DI_TEXT, DI_BOX,
        DI_TEXT};

    size_t idSize[] = {
        2,
        1};

    char *test = "testing";

    UI_Element *elmnts = (UI_Element *)arenaAlloc(&arena, sizeof(UI_Element) * 2);

    for (size_t i = 0, j = 0; i < 2; i++)
    {
        size_t k = 0;
        while (k < idSize[i])
        {
            LinkAllocateDisplayToElement(&arena, (elmnts + i), (ids + j + k++), idSize[i]);
        }
        j += k;
    }

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < (elmnts + i)->displayCount; j++)
        {
            if (((elmnts + i)->displayData + j)->id == DI_TEXT)
            {
                ((Text *)((elmnts + i)->displayData + j)->displayData)->text = test;
            }
        }
    }

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < (elmnts + i)->displayCount; j++)
        {
            if (((elmnts + i)->displayData + j)->id == DI_TEXT)
                SDL_Log("Test: %s\n", ((Text *)((elmnts + i)->displayData + j)->displayData)->text);
        }
    }

    destroyArena(&arena);

    return 0;
}