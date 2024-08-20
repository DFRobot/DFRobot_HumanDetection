DFRobot_HumanDetection
===========================

* [English Version](./README.md)

这是一个人体毫米波的驱动库，可以实现呼吸睡眠、跌倒检测功能的检测。
  呼吸睡眠功能基于毫米波雷达体制实现人体生物存在感知及人体运动感知，持续记录人体存在情况，根据睡眠过程中的身体运动幅度变化和呼吸心率变化，对目标的睡眠状态、呼吸心跳频率进行实时判断，在一段睡眠过程结束后输出睡眠评分，根据相关睡眠参数的输出结合到健康康养的应用上。
  跌倒检测功能是基于人员姿态参数特征做算法判断，通过速度，距离，姿态等运动变化，测目标人员的跌倒状态，持续记录目标是否发生跌倒危险，同时针对长时间异常停留响应静止驻留报警。

![产品效果图片](../../resources/images/SEN0623.png)

## 产品链接（https://www.dfrobot.com.cn/goods-3965.html）

    SKU: SEN0623
  
## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

这是一个人体毫米波的驱动库，可以实现呼吸睡眠、跌倒检测功能的检测。


## 库安装

要使用这个库，首先将库下载到Raspberry Pi，然后打开例程文件夹。要执行一个例程demox.py，请在命令行中输入python demox.py。例如，要执行basics.py例程，你需要输入:

```python
python basics.py
```

## 方法

```python
    def begin(self):
        '''!
            @fn begin
            @brief 初始化传感器
            @return 返回初始化状态
            @retval 0 初始化成功
            @retval 1 初始化失败
        '''
    
    def config_work_mode(self, mode):
        '''!
            @fn config_work_mode
            @brief 初始化模式
            @param mode 模式选择
            @return 返回初始化状态
            @retval 0 模式配置成功
            @retval 1 模式配置失败 
        '''
    
    def get_workmode(self):
        '''!
            @fn get_workmode
            @brief 获取工作模式
            @return 返回工作模式
        '''
    
    def config_LED_light(self,led,sta):
        '''!
            @fn config_LED_light
            @brief 配置LED灯
            @param led 选择打开那个LED灯
            @param sta 0:打开，1:关闭
            @return 返回控制啊状态
            @retval 0 配置成功
            @retval 1 配置失败
        '''
    
    def get_LED_light_state(self,led):
        '''!
            @fn get_LED_light_state
            @brief 获取LED灯的状态
            @param led LED灯选择
            @return 返回灯状态
            @retval 0 灯关闭
            @retval 1 灯打开
        '''
    
    def sensor_ret(self):
        '''!
            @fn sensor_ret
            @brief 复位传感器
            @return 返回复位状态
            @retval 0 复位成功
            @retval 1 复位失败
        '''

    def sm_human_data(self, hm):
        '''!
            @fn sm_human_data
            @brief 查询在睡眠模式下人体相关的内容
            @param hm 获取数据内容选择
        '''

    def get_heart_rate(self):
        '''!
            @fn get_heart_rate
            @brief 获取心率
            @return 返回心率
        '''

    def get_breathe_state(self):
        '''!
            @fn getBreatheState
            @brief 获取呼吸检测信息
            @return 返回呼吸信息
            @retval 1 正常
            @retval 2 获取过快
            @retval 3 获取过慢
            @retval 4 无
        '''

    def get_breathe_value(self):
        '''!
            @fn getBreatheValue
            @brief 获取呼吸数值
            @return 返回呼吸数值
        '''

    def sm_sleep_data(self, sl):
        '''!
            @fn sm_sleep_data
            @brief 获取睡眠相关数据
            @param sl 需要获取的数据
            @return 返回获取的数据
        '''

    def config_sleep(self,sl,data):
        '''!
            @fn config_sleep
            @brief 配置睡眠模式功能
            @param sl 功能选择
            @param data 配置数据
            @return 返回设置状态
        '''
    
    def dm_install_angle(self,x,y,z):
        '''!
            @fn dm_install_angle
            @brief 用于跌倒模式中雷达角度安装设置
            @param x x 角度
            @param y y 角度
            @param z z 角度
        '''

    def dm_get_install_angle(self):
        '''!
            @fn dm_get_install_angle
            @brief 获取雷达安装角度
            @param x x 角度
            @param y y 角度
            @param z z 角度
        '''
    
    def dm_install_height(self,he):
        '''!
            @fn dm_install_height
            @brief 设置雷达安装高度
            @param he 安装高度
        '''

    def dm_getInstall_height(self):
        '''!
            @fn dm_getInstall_height
            @brief 获取安装高度
            @return 返回获取的安装高度
        '''
    
    def dm_auto_measure_height(self):
        '''!
            @fn autoMeasureHeight
            @brief 获取自动测高数据
            @return 返回自动测高数据
        '''
    
    def dm_humanData(self, dh):
        '''!
            @fn dmHumanData
            @brief 在跌倒模式下获取人体相关数据
            @return 返回获取数据
        '''

    def track(self):
        '''!
            @fn track
            @param x x 坐标
            @param y y 坐标
        '''
    
    def track_frequency(self):
        '''!
            @fn trackFrequency
            @brief 获取轨迹点上报频率
            @return 返回轨迹点上报频率
        '''
    def unmannedTime(self):
        '''!
            @fn unmannedTime
            @brief 无人时间查询
        '''

    def getFall_data(self,dm):
        '''!
            @fn getFallData
            @brief 获取跌倒检测功能数据
            @param dm 数据选择
            @return 返回获取的数据
        '''
    
    def get_fall_time(self):
        '''!
            @fn getFallTime
            @brief 获取跌倒时长
            @return 返回跌倒时长
        '''
    
    def static_residency_time(self):
        '''!
            @fn staticResidencyTime
            @brief 驻留时长查询
            @return 返回驻留时长
        '''
        
    def accumulated_height_duration(self):
        '''!
            @fn accumulatedHeightDuration
            @brief 高度累计时间查询
            @return 返回高度累计时间数据
        '''
    
    def dm_human_config(self,con,data):
        '''!
            @fn dmHumanConfig
            @brief 配置跌倒模式下人体配置
            @param con 配置选择
            @param da 配置数据
            @return 返回配置状态
            @retval 1 配置失败
            @retval 0 配置成功
        '''

    def unattended_time_config(self,time):
        '''!
            @fn unattendedTimeConfig
            @brief 设置在跌倒模式下无人时间查询
            @brief time 设置时间
            @return 返回设置状态
            @retval 0 成功
            @retval 1 失败
        '''
    
    def dm_fall_config(self,con,data):
        '''!
            @fn dmFallConfig
            @brief 跌倒模式配置
            @param con 配置选项
            @param data 配置数据
            @return 返回配置状态
            @retval 0 成功
            @retval 1 失败
        '''
```

## 兼容性

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |     √     |            |          |         |
| RaspberryPi4 |           |            |     √    |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## 版本

- 2024-06-3 - 1.0.0 版本

## 创作者

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))