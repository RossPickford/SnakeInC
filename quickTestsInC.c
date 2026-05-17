#include <stdio.h>
#include <stdlib.h>

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

    month ids[] = {
        JAN,       /* first 4 */
        FEB,       /* second 7 */
        MAR, APR,  /* third 8, 9 */
        MAY,       /* fourth 10 */
        JUN,       /* fifth 11 */
        JUL,       /* sixth 12 */
        JAN, APR,  /* seventh 4, 9 */
        FEB, JUL}; /*eighth 7, 12 */

    size_t idSize[] = {1, 1, 2, 1, 1, 1, 2, 2};

    for (size_t i = 0, j = 0; i < 8; i++)
    {
        size_t k = 0;
        while (k < idSize[i])
        {
            printf("%d\n", ids[j + k++]);
        }
        j += k;
    }

    return 1;
}