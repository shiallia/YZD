# YZD

## shell的启动流程

1. 隐藏mfc窗口
2. 注册如下快捷键，并实现了相应的功能
    ```c++
    ::RegisterHotKey(m_hWnd, 001, MOD_CONTROL | MOD_SHIFT, 0X31);     //1   explorer
    ::RegisterHotKey(m_hWnd, 002, MOD_CONTROL | MOD_SHIFT, 0X32);     //2   cmd
    ::RegisterHotKey(m_hWnd, 003, MOD_CONTROL | MOD_SHIFT, 0X33);     //3   任务管理器
    ::RegisterHotKey(m_hWnd, 004, MOD_CONTROL | MOD_SHIFT, 0X34);     //4   显示cpu，内存  
    ::RegisterHotKey(m_hWnd, 005, MOD_CONTROL | MOD_SHIFT, 0X35);     //5   ewf，commit  
    ::RegisterHotKey(m_hWnd, 006, MOD_CONTROL | MOD_SHIFT, 0X36);     //6   重置版本号     
    ::RegisterHotKey(m_hWnd, 111, MOD_ALT, VK_F4);                    //alt+f4
    ::RegisterHotKey(m_hWnd, 017, MOD_CONTROL | MOD_SHIFT, 0X38);     //8    从u盘还原     
    ::RegisterHotKey(m_hWnd, 020, MOD_CONTROL | MOD_SHIFT, 0X39);     //9    从c盘还原
    ::RegisterHotKey(m_hWnd, 007, MOD_CONTROL, 0x4F);         //unlock1,control+o
    ::RegisterHotKey(m_hWnd, 011, MOD_ALT, 0x50);             //unlock2,alt+p
    ::RegisterHotKey(m_hWnd, 012, MOD_CONTROL | MOD_ALT, 0x4D); //lock,control+alt+m
    ```
3. 读取配置文件，获得启动程序和任务管理程序的路径，以及ewf状态
4. 根据ewf状态，自动配置机器
5. 启动主程序

## monitor主要功能

* 监控设备当前的cpu，内存占用
* 显示设备当前ip
