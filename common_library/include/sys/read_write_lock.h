#ifndef READ_WRITE_LOCK
#define READ_WRITE_LOCK
#include <pthread.h>
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/***
  * ��д����
  * ��ע��: ˭����˭����ԭ�򣬼�һ���̼߳ӵ�������������һ�߳�������
  * ���Ҽ����ͽ�������ɶԵ��ã�������������
  */
class CReadWriteLock
{
public:
	CReadWriteLock();
	~CReadWriteLock();

    /***
      * �ͷŶ���д��
      * @exception: �����׳�CSyscallException�쳣
      */
	void unlock();

    /***
      * ��ȡ���������д���������У���һֱ�ȴ���ֱ�ӿɻ�ȡ������
      * @exception: �����׳�CSyscallException�쳣
      */
	void lock_read();

    /***
      * ��ȡд�������д���������У���һֱ�ȴ���ֱ�ӿɻ�ȡ��д��
      * @exception: �����׳�CSyscallException�쳣
      */
	void lock_write();		

    /***
      * ����ȥ��ȡ���������д���������У�����������
      * @return: ����ɹ���ȡ�˶������򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
	bool try_lock_read();

    /***
      * ����ȥ��ȡд�������д���������У�����������
      * @return: ����ɹ���ȡ��д�����򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
	bool try_lock_write();

    /***
      * �Գ�ʱ��ʽ��ȡ���������д���������У���ȴ�ָ���ĺ�������
      * �����ָ���ĺ���ʱ���ڣ��Բ��ܵõ�����������������
      * @millisecond: �ȴ���ȡ�����ĺ�����
      * @return: �����ָ���ĺ���ʱ���ڻ�ȡ���˶������򷵻�true�����������ʱ�򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
	bool timed_lock_read(uint32_t millisecond);

    /***
      * �Գ�ʱ��ʽ��ȡд�������д���������У���ȴ�ָ���ĺ�������
      * �����ָ���ĺ���ʱ���ڣ��Բ��ܵõ�д��������������
      * @millisecond: �ȴ���ȡд���ĺ�����
      * @return: �����ָ���ĺ���ʱ���ڻ�ȡ����д�����򷵻�true�����������ʱ�򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
	bool timed_lock_write(uint32_t millisecond);
	
private:
	pthread_rwlock_t _rwlock;
};

/***
  * ���������࣬�����Զ��ͷŶ���
  */
class CReadLockHelper
{
public:
    CReadLockHelper(CReadWriteLock& lock)
      :_read_lock(lock)
    {
        _read_lock.lock_read();
    }    
    
    /** ����������������������Զ��ͷ��� */
    ~CReadLockHelper()
    {
        _read_lock.unlock();
    }
    
private:
    CReadWriteLock& _read_lock;
};

/***
  * ���������࣬�����Զ��ͷ�д��
  */
class CWriteLockHelper
{
public:
    CWriteLockHelper(CReadWriteLock& lock)
      :_write_lock(lock)
    {
        _write_lock.lock_write();
    }
    
    /** ����������������������Զ��ͷ��� */
    ~CWriteLockHelper()
    {
        _write_lock.unlock();
    }
    
private:
    CReadWriteLock& _write_lock;
};

SYS_NAMESPACE_END
#endif // READ_WRITE_LOCK
