/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef UTIL_SYS_INFO_H
#define UTIL_SYS_INFO_H
#include <vector>
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

/***
  * ������ȡϵͳ���ں˺ͽ��̵ĸ���ʵʱ��Ϣ����CPU���ڴ�����
  */
class CSysInfo
{
public:
    /***
      * ϵͳ��ǰʵʱ��Ϣ
      */
    typedef struct
    {
        long uptime_second;             /* Seconds since boot */
        unsigned long average_load[3];  /* 1, 5, and 15 minute load averages */
        unsigned long ram_total;        /* Total usable main memory size */
        unsigned long ram_free;         /* Available memory size */
        unsigned long ram_shared;       /* Amount of shared memory */
        unsigned long ram_buffer;       /* Memory used by buffers */
        unsigned long swap_total;       /* Total swap space size */
        unsigned long swap_free;        /* swap space still available */
        unsigned short process_number;  /* Number of current processes */
    }sys_info_t;

    /***
      * ��ǰ����ʱ����Ϣ
      */
    typedef struct
    {
        long user_time;             /* user time */
        long system_time;          /* system time */
        long user_time_children;    /* user time of children */
        long system_time_children; /* system time of children */
    }process_time_t;

    /***
      * ��ǰϵͳCPU��Ϣ
      */
    typedef struct
    {
        // ��λ: jiffies, 1jiffies=0.01��
        uint32_t total;
        uint32_t user;    /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣������û�̬������ʱ�䣬������ niceֵΪ������ */
        uint32_t nice;    /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�niceֵΪ���Ľ�����ռ�õ�CPUʱ�� */
        uint32_t system;  /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣����ں���̬������ʱ�� */
        uint32_t idle;    /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣���IO�ȴ�ʱ������������ȴ�ʱ�� */
        uint32_t iowait;  /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�IO�ȴ�ʱ��(2.5.41) */
        uint32_t irq;     /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�Ӳ�ж�ʱ��(2.6.0) */
        uint32_t softirq; /** ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣����ж�ʱ��(2.6.0) */
        //uint32_t stealstolen; /** which is the time spent in other operating systems when running in a virtualized environment(2.6.11) */
        //uint32_t guest;       /** which is the time spent running a virtual  CPU  for  guest operating systems under the control of the Linux kernel(2.6.24) */
    }cpu_info_t;

    /***
      * ��ǰϵͳ�ڴ���Ϣ
      */
    typedef struct
    {
        uint32_t mem_total;
        uint32_t mem_free;
        uint32_t buffers;
        uint32_t cached;
        uint32_t swap_cached;
        uint32_t swap_total;
        uint32_t swap_free;
    }mem_info_t;

    /***
      * �ں˰汾��
      */
    typedef struct
    {
        int16_t major;    /** ���汾�� */
        int16_t minor;    /** �ΰ汾��(����ΰ汾����ż������ô�ں����ȶ��棻�����������ǿ�����) */
        int16_t revision; /** �޶��汾�� */
    }kernel_version_t;

    /***
      * ��ʱ����״̬��Ϣ
      *
      * ���̵�״ֵ̬:
        D    Uninterruptible sleep (usually IO)
        R    Running or runnable (on run queue)
        S    Interruptible sleep (waiting for an event to complete)
        T    Stopped, either by a job control signal or because it is being traced.
        W    paging (not valid since the 2.6.xx kernel)
        X    dead (should never be seen)
        Z    Defunct ("zombie") process, terminated but not reaped by its parent.
      */
    typedef struct
    {
        /** 01 */ pid_t pid;                     /** ���̺ţ�����������ֵ����鿴/proc/sys/kernel/pid_max */
        /** 02 */ char comm[FILENAME_MAX];       /** ���̵����֣�������·�� */
        /** 03 */ char state;                    /** ���̵�״̬ */
        /** 04 */ pid_t ppid;                    /** �����̺� */
        /** 05 */ pid_t pgrp;                    /** ������� */
        /** 06 */ pid_t session;                 /** ���̻Ự�� */
        /** 07 */ int tty_nr;                    /** The tty the process uses */
        /** 08 */ pid_t tpgid;                   /** The tty the process uses */
        /** 09 */ unsigned int flags;            /** The kernel flags word of the process (%lu before Linux 2.6.22) */
        /** 10 */ unsigned long minflt;          /** The number of minor faults the process has made which have not required loading a memory page from disk */
        /** 11 */ unsigned long cminflt;         /** The number of minor faults that the process's waited-for children have made */
        /** 12 */ unsigned long majflt;          /** The number of major faults the process has made which have required loading a memory page from disk */
        /** 13 */ unsigned long cmajflt;         /** The number of major faults that the process's waited-for children have made */
        /** 14 */ unsigned long utime;           /** The number of jiffies that this process has been scheduled in user mode */
        /** 15 */ unsigned long stime;           /** The number of jiffies that this process has been scheduled in kernel mode */
        /** 16 */ long cutime;                   /** The  number  of  jiffies that this process's waited-for children have been scheduled in user mode */
        /** 17 */ long cstime;                   /** The number of jiffies that this process's waited-for children have been scheduled in kernel mode */
        /** 18 */ long priority;                 /** The standard nice value, plus fifteen.  The value is never negative in the kernel */
        /** 19 */ long nice;                     /** The nice value ranges from 19 (nicest) to -19 (not nice to others) */
        /** 20 */ long num_threads;              /** Number of threads in this process (since Linux 2.6).  Before kernel 2.6, this field was hard coded to 0 as a placeholder */
        /** 21 */ long itrealvalue;              /** The  time  in  jiffies before the next SIGALRM is sent to the process due to an interval timer.2.6.17, this field is no longer maintained, and is hard coded as 0 */
        /** 22 */ long long starttime;           /** The time in jiffies the process started after system boot */
        /** 23 */ unsigned long vsize;           /** Virtual memory size in bytes */
        /** 24 */ long rss;                      /** Resident Set Size: number of pages the process has in real memory, minus 3 for administrative purposes */
        /** 25 */ unsigned long rlim;            /** Current limit in bytes on the rss of the process (usually 4294967295 on i386) */
        /** 26 */ unsigned long startcode;       /** The address above which program text can run */
        /** 27 */ unsigned long endcode;         /** The address below which program text can run */
        /** 28 */ unsigned long startstack;      /** The address of the start of the stack */
        /** 29 */ unsigned long kstkesp;         /** The current value of esp (stack pointer), as found in the kernel stack page for the process */
        /** 30 */ unsigned long kstkeip;         /** The current EIP (instruction pointer) */
        /** 31 */ unsigned long signal;          /** The bitmap of pending signals */
        /** 32 */ unsigned long blocked;         /** The bitmap of blocked signals */
        /** 33 */ unsigned long sigignore;       /** The bitmap of ignored signals */
        /** 34 */ unsigned long sigcatch;        /** The bitmap of caught signals */
        /** 35 */ unsigned long nswap;           /** Number of pages swapped (not maintained). */
        /** 36 */ unsigned long cnswap;          /** Cumulative nswap for child processes (not maintained) */
        /** 37 */ int exit_signal;               /** Signal to be sent to parent when we die (since Linux 2.1.22) */
        /** 38 */ int processor;                 /** CPU number last executed on (since Linux 2.2.8) */
    }process_info_t;

    /***
      * �����������ݽṹ
      */
    typedef struct
    {
        /** 01 */ char interface_name[INTERFACE_NAME_MAX]; /** ����������eth0 */

                  /** �������� */
        /** 02 */ unsigned long receive_bytes;             /** ���������յ����ֽ��� */
        /** 03 */ unsigned long receive_packets;
        /** 04 */ unsigned long receive_errors;
        /** 05 */ unsigned long receive_dropped;
        /** 06 */ unsigned long receive_fifo_errors;
        /** 07 */ unsigned long receive_frame;
        /** 08 */ unsigned long receive_compressed;
        /** 09 */ unsigned long receive_multicast;

                  /** �������� */
        /** 10 */ unsigned long transmit_bytes;             /** �������ѷ��͵��ֽ��� */
        /** 11 */ unsigned long transmit_packets;
        /** 12 */ unsigned long transmit_errors;
        /** 13 */ unsigned long transmit_dropped;
        /** 14 */ unsigned long transmit_fifo_errors;
        /** 15 */ unsigned long transmit_collisions;
        /** 16 */ unsigned long transmit_carrier;
        /** 17 */ unsigned long transmit_compressed;        
    }net_info_t;

    /***
      * ����ҳ��Ϣ�ṹ
      */
    typedef struct
    {
        long size;     /** �����С */
        long resident; /** ��פ�ڴ�ռ��С */
        long share;    /** �����ڴ�ҳ�� */
        long text;     /** �����ռ���ڴ�ҳ�� */
        long lib;      /** ����/��ջ��ռ���ڴ�ҳ�� */
        long data;     /** ���ÿ�ռ���ڴ�ҳ�� */
    }process_page_info_t;

public:
    /** ��ȡϵͳ��Ϣ��������ο�sys_info_t������ */
    static bool get_sys_info(sys_info_t& sys_info);

    /** ��ȡ�ڴ���Ϣ��������ο�mem_info_t������ */
    static bool get_mem_info(mem_info_t& mem_info);

    /** ��ȡ��CPU��Ϣ��������ο�cpu_info_t������ */
    static bool get_cpu_info(cpu_info_t& cpu_info);

    /** ��ȡ����CPU��Ϣ��������ο�cpu_info_t������ */
	static int get_cpu_info_array(std::vector<cpu_info_t>& cpu_info_array);

    /** �õ��ں˰汾�� */
    static bool get_kernel_version(kernel_version_t& kernel_version);

    /** ��ȡ������Ϣ��������ο�process_info_t������ */
    static bool get_process_info(process_info_t& process_info);

    /** ��ȡ����ҳ��Ϣ��������ο�process_page_info_t������ */
    static bool get_process_page_info(process_page_info_t& process_page_info);

    /** ��ȡ��������ʱ�����ݣ�������ο�process_time_t������ */
    static bool get_process_times(process_time_t& process_time);
        
    /***
      * ��ȡ������������Ϣ
      * ���� = (��ǰ��ȡ��ֵ - ��һʱ���ȡ��ֵ) / ���μ����ʱ��
      * @interface_name: ����������eth0��
      * @net_info: �洢��������������
      */
    static bool get_net_info(const char* interface_name, net_info_t& net_info);
    static bool get_net_info_array(std::vector<net_info_t>& net_info_array);    

private:
    static bool do_get_net_info_array(const char* interface_name, std::vector<net_info_t>& net_info_array);
};

#endif // UTIL_SYS_INFO_H
SYS_NAMESPACE_END
