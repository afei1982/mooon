#include <sys/thread.h>
#include <sys/sys_util.h>

// ���з��̳߳��̶߳�Ӧ����CThread������
// Exam�߳�CDemoThread���к�ÿ��1������׼�����ӡһ�С�continue...��
class CExamThread: public sys::CThread
{
private:
	virtual void run();
	virtual bool before_start();
};

bool CExamThread::before_start()
{
    // start�ķ���ֵ��before_start����ͬ��һ�������߳�����ǰ�ĳ�ʼ��
	return true;
}

// �߳�ÿ��������׼�����ӡһ��continue...
void CExamThread::run()
{
    // stop��ʹ��is_stop����false
	while (!is_stop())
	{
	    // ˯��1����
        // do_millisleep����CThread�ṩ������˯��ʹ�õģ�
        // ��ͨ������wakeup��˯���ж�
	    do_millisleep(1000);
	    printf("continue ...\n");
	}
}

int main()
{
	// �����������߳�
	CExamThread* thread = new CExamThread;
	thread->inc_refcount();
	try
	{
	    if (!thread->start())
	    {
	        // ֻ�е�before_start����false��thread->start�Ż᷵��false���������Ƿ���true
	        thread->dec_refcount();
	        exit(1);
	    }
	}
	catch (sys::CSyscallException& ex)
	{
	    printf("Start thread error: %s\n"
	         , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
	    thread->dec_refcount();
	    exit(1);
	}
	
	// ���߳�˯��10����
	sys::CSysUtil::millisleep(10000);
	
	thread->stop(); // ֹͣ�����߳��˳�
	thread->dec_refcount(); // �ǵ����������ü���������Ҫ��ʹ�������Ӧ�ļ����ü���
	
	return 0;
}
