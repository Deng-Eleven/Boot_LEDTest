#Boot_LEDTest-
/**
  @brief LED矩阵驱动头文件
  使用说明：
  01 - 程序总共通过使用MCU控制11个LED灯的点亮和熄灭，根据产品实际工作情况，最多情况下同时点亮5个LED，考虑到MCU的灌电流因素，对LED点亮的数量进行了函数限制
  02 - 提供了一个简单的测试函数，首先通过流水的形式点亮LED，之后通过局部点亮LED来进行观察亮度一致性
  03 - 程序中对NRST的引脚功能进行了重新配置，NRST功能失效，此时它可以被配置成普通的输入输出IO
  @copyright 风翎科技
  @author 邓思忆
  @date 2024/07/17
  @version 1.0
*/
