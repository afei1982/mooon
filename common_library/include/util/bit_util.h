#ifndef _BIT_UTIL_H
#define _BIT_UTIL_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/***
  * λ����������
  */
class CBitUtil
{
public:
    /***
      * ��ָ��λȡ��������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static void flip(char* bitmap, uint32_t position);

    /***
      * ����ָ��λ�Ƿ�Ϊ1������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static bool test(char* bitmap, uint32_t position);

    /***
      * �õ�ָ��λ��ֵ������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static uint8_t get_bit(char* bitmap, uint32_t position);

    /***
      * ����ָ��λ��ֵ������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      * @zero: ��position����λ����Ϊ0��1�����Ϊtrue������Ϊ0����������Ϊ1
      */
    static void set_bit(char* bitmap, uint32_t position, bool zero);
};

UTIL_NAMESPACE_END
#endif // _BIT_UTIL_H
