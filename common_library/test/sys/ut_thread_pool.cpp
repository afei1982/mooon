#include <assert.h>
#include <sys/sys_util.h>
#include <sys/pool_thread.h>
#include <sys/thread_pool.h>

// �����̣߳���ע���Ǽ̳г��߳�CPoolThread���������߳�CThread��
// ����CPoolThread������CThread������
class CTestThread: public sys::CPoolThread
{
public:
	CTestThread()
		:_number_printed(0)
	{
	}
	
private:
	virtual void run()
	{
		// �����������ʵ����Ҫ���̸߳ɵ���
		if (_number_printed++ < 3)
		{
			// ֻ��ӡ���Σ��Ա�۲�Ч��
			printf("thread %u say hello.\n", get_thread_id());
		}

        // do_millisleep����CPoolThread�ṩ������˯���õģ�
        // ����ͨ������wakeup�ж�˯��
		do_millisleep(1000);
	}
	
	virtual bool before_start()
	{
		// ���������������߳�����ǰ��׼������
		return true;
	}
	
private:
	int _number_printed; // ��ӡ����
};

int main()
{
	sys::CThreadPool<CTestThread> thread_pool; // ���ﶨ���̳߳�ʵ��
	
	try
	{
		// create�����׳��쳣��������Ҫ����
		thread_pool.create(10); // ����10���̵߳��̳߳�
		
		// ���̴߳����ɹ��󣬲�����������������״̬�����Ǵ��ڵȴ�״̬��
		// ������Ҫ�������ǣ��������£�
		uint16_t thread_count = thread_pool.get_thread_count();
		CTestThread** test_thread_array = thread_pool.get_thread_array();
		for (uint16_t i=0; i<thread_count; ++i)
		{
			// ���ѳ��̣߳����������wakeup����CTestThreadһֱ��ȴ�������
			test_thread_array[i]->wakeup();
		}
		
		// ��CTestThread���㹻��ʱ���������
		sys::CSysUtil::millisleep(5000);
		// �ȴ������߳��˳���Ȼ�������̳߳�		
		thread_pool.destroy();
	}
	catch (sys::CSyscallException& ex)
	{
		// ���쳣��Ϣ��ӡ���������㶨λԭ��
		printf("Create thread pool exception: %s.\n"
			,sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
	}
	
	return 0;
}
