#include "util/string_util.h"
using namespace util;

int main()
{
    // ����string2int32����
    printf("\n>>>>>>>>>>TEST string2int32<<<<<<<<<<\n");
    
    // ����1
    int result1;
    const char* str1 = "123456";    
    if (CStringUtil::string2int32(str1, result1))
        printf("%s ===> %d\n", str1, result1);
    else
        printf("ERROR string2int32: %s\n", str1);

    // ����2
    int result2;
    const char* str2 = "123a456";    
    if (CStringUtil::string2int32(str2, result2))
        printf("%s ===> %d\n", str2, result2);
    else
        printf("ERROR string2int32: %s\n", str2);

    // ����3
    int result3;
    const char* str3 = "123a456";    
    if (CStringUtil::string2int32(str3, result3, 3))
        printf("%s ===> %d\n", str3, result3);
    else
        printf("ERROR string2int32: %s\n", str3);


    // ����4
    int result4;
    const char* str4 = "-123456";    
    if (CStringUtil::string2int32(str4, result4))
        printf("%s ===> %d\n", str4, result4);
    else
        printf("ERROR string2int32: %s\n", str4);

    // ����5
    int result5;
    const char* str5 = "0123456";    
    if (CStringUtil::string2int32(str5, result5))
        printf("%s ===> %d\n", str5, result5);
    else
        printf("ERROR string2int32: %s\n", str5);

    // ����6
    int result6;
    const char* str6 = "-0";    
    if (CStringUtil::string2int32(str6, result6))
        printf("%s ===> %d\n", str6, result6);
    else
        printf("ERROR string2int32: %s\n", str6);

    // ����7
    int result7;
    const char* str7 = "-023";    
    if (CStringUtil::string2int32(str7, result7))
        printf("%s ===> %d\n", str7, result7);
    else
        printf("ERROR string2int32: %s\n", str7);

    return 0;    
}
