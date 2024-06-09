############################  导库  ############################

# 显示窗口所需要的库
from prompt_toolkit import Application
from prompt_toolkit.buffer import Buffer
from prompt_toolkit.layout.containers import HSplit, Window
from prompt_toolkit.layout.controls import BufferControl
from prompt_toolkit.layout.layout import Layout
from prompt_toolkit.key_binding import KeyBindings

import time
from pyknow import *
import random
from colorama import Fore, init
import math

init()
PERIOD = 10
# 每个周期需要通过 NS, WE 的车辆数目 模拟列表，第0个元素代表第0个周期
# CARS = [[random.randint(10, 20), random.randint(0, 10)]
#         for i in range(100)]
CARS=[]
NSCARS=[0]
WECARS=[0]

class Greetings(KnowledgeEngine):
    @DefFacts()
    def _initial_action(self):
        yield Fact(Ticks=0)
        yield Fact(NSsign='RED')
        yield Fact(WEsign='GREEN')
        yield Fact(switch=False)
        yield Fact(switchTime=5)
        yield Fact(NeedChange=False)
        yield Fact(NeedShow=False)

    """
    计时，每秒都会输出时间
    """
    @Rule(AS.oldFact << Fact(Ticks=MATCH.times),
          Fact(NSsign=MATCH.NScolor),
    )
    def ticks(self, times, oldFact,NScolor):
        time.sleep(1)
        self.retract(oldFact)
        self.declare(Fact(Ticks=times + 1))
        print("====== 启动时间{}秒{} ======".format(times,NScolor))
        if NScolor == 'GREEN':
            output_queue.put(b"1")
        else:
            output_queue.put(b"0")
    """
      计时到半周期，需要交换红绿灯并显示
    """
    @Rule(
        Fact(Ticks=MATCH.times),
        Fact(switchTime=MATCH.switchTime),
        TEST(lambda switchTime, times: times % 10 == switchTime),
        salience=3
    )
    def needSwitch(self):
        self.declare(Fact(switch=True))
        self.declare(Fact(NeedShow=True))

    """
      一个周期后修改比例，修改完成交换红绿灯并显示
    """
    @Rule(
        Fact(Ticks=MATCH.times),
        TEST(lambda switchTime, times: times % PERIOD == 0),
        salience=2
    )
    def needChange(self):
        self.declare(Fact(NeedChange=True))

    """
    换灯，南北RED->GREEN,东西Green->RED
    """

    @Rule(
        AS.oldSwtich << Fact(switch=True),
        AS.oldNS << Fact(NSsign='RED'),
        AS.oldWE << Fact(WEsign='GREEN'),
        salience=2
    )
    def switch1(self, oldSwtich, oldNS, oldWE):
        self.declare(Fact(NSsign='GREEN'))
        self.declare(Fact(WEsign='RED'))
        self.retract(oldSwtich)
        self.retract(oldWE)
        self.retract(oldNS)

    """
       换灯，南北Green->RED,东西RED->GREEN
    """

    @Rule(
        AS.oldSwtich << Fact(switch=True),
        AS.oldNS << Fact(NSsign='GREEN'),
        AS.oldWE << Fact(WEsign='RED'),
        salience=2
    )
    def switch2(self, oldSwtich, oldNS, oldWE):
        self.declare(Fact(NSsign='RED'))
        self.declare(Fact(WEsign='GREEN'))
        self.retract(oldSwtich)
        self.retract(oldWE)
        self.retract(oldNS)

    """
    显示当前的指示灯情况
    """

    @Rule(
        Fact(NSsign=MATCH.NScolor),
        Fact(WEsign=MATCH.WEcolor),
        Fact(switchTime=MATCH.switchTime),
        AS.oldFact << Fact(NeedShow=True),
        salience=4
    )
    def show(self, NScolor, switchTime, oldFact):
        self.retract(oldFact)
        print('当前路况')
        if NScolor == "RED":
            print(Fore.WHITE + '=========' + Fore.GREEN + "剩余时间: {} s".format(switchTime) + Fore.WHITE + '=========')
            print(Fore.RED + "剩余时间: {} s".format(
                switchTime) + Fore.WHITE + '=======' + Fore.RED + "剩余时间: {} s".format(switchTime))
            print(Fore.WHITE + '=========' + Fore.GREEN + "剩余时间: {} s".format(switchTime) + Fore.WHITE + '=========')
        if NScolor == "GREEN":
            print(
                Fore.WHITE + '=========' + Fore.RED + "剩余时间: {} s".format(
                    PERIOD - switchTime) + Fore.WHITE + '=========')
            print(Fore.GREEN + "剩余时间: {} s".format(
                PERIOD - switchTime) + Fore.WHITE + '=======' + Fore.GREEN + "剩余时间: {} s".format(PERIOD - switchTime))
            print(
                Fore.WHITE + '=========' + Fore.RED + "剩余时间: {} s".format(
                    PERIOD - switchTime) + Fore.WHITE + '=========')

    @Rule(
        Fact(Ticks=MATCH.times),
        AS.oldFact1 << Fact(switchTime=MATCH.switchTime),
        AS.oldFact2 << Fact(NeedChange=True),
        salience=2
    )
    def changeSwitchTime(self, oldFact1, oldFact2, times, switchTime):
        # print("修改时间")
        self.retract(oldFact1)
        self.retract(oldFact2)
        temp = []
        temp.append(NSCARS[0])
        temp.append(WECARS[0])
        CARS.append(temp)
        NSCARS[0] = 0
        WECARS[0] = 0
        cur_epoch = times // PERIOD
        newSwitchTime = switchTime
        # 修正红绿灯
        print("第 {} 个周期，南北方向通过{}辆车，东西方向通过{}辆车".format(cur_epoch, CARS[cur_epoch][1], CARS[cur_epoch][0]))
        if CARS[cur_epoch][0]==0 and CARS[cur_epoch][1]==0:
            newSwitchTime = 5
        else:
            newSwitchTime = math.floor(PERIOD / (CARS[cur_epoch][0] / (CARS[cur_epoch][1] + 0.00001) + 1))
        if newSwitchTime < 3:
            newSwitchTime = 3
        if newSwitchTime > 7:
            newSwitchTime =7
        self.declare(Fact(switchTime=newSwitchTime))
        self.declare(Fact(switch=True))
        self.declare(Fact(NeedShow=True))


# 串口通信所需要的库
import serial
import threading
from queue import Queue

############################  串口  ############################

# 创建三个缓冲区
buffer_display = Buffer()   # 用于显示上方提示信息
buffer_output = Buffer()    # 用于显示从串口读入的数据
buffer_input = Buffer()     # 用于从用户读入要向串口写入的数据

# 串口参数设置
serialPort = "COM3" # 串口
baudRate = 9600     # 波特率
buffer_display.text = "输入`exit`退出程序，输入`1`或`0`控制交通灯亮灭\n"
buffer_display.text += "参数设置：串口=%s ，波特率=%d" % (serialPort, baudRate)

# 连接串口
try:
    ser = serial.Serial(serialPort, baudRate, timeout=0.5)
except:
    raise Exception("无法打开串口！请确保串口号正确，且设备已连接！")

# 创建输出队列，存储要向串口写入的数据
output_queue = Queue()

# 创建终止标志变量，用于Control-C退出时终止线程
stop_event = threading.Event()

# 读串口线程
def read_serial(event):
    while not event.is_set():
        try:
            # 读取串口数据
            data = ser.readline().decode('utf-8').rstrip()
            # 将读取的数据写入显示缓冲区
            if data:
                print(data)
                if data == "NSYES":
                    WECARS[0] = WECARS[0] + 1
                if data == "WEYES":
                    NSCARS[0] = NSCARS[0] + 1
        except Exception as e:
            pass
    print("Read Serial Exit!")

# 写串口线程
def write_serial(event):
    while not event.is_set():
        try:
            # 从输出队列中读取数据
            # [Note] 如果队列为空，会阻塞，直到有数据写入队列。因此要设置一个超时时间
            data = output_queue.get(timeout=0.5)
            # 将数据写入串口
            if data:
                ser.write(data)
        except Exception as e:
            pass
    print("Write Serial Exit!")
def traffic_serial(event):

    engine = Greetings()
    engine.reset()  # Prepare the engine for the execution.
    engine.run()  # Run it!

############################  显示  ############################

# 创建一个显示容器
root_container = HSplit([
    Window(height=2, content=BufferControl(buffer=buffer_display, focusable=False)),  # 上方提示信息
    Window(height=1, char='-'),  # 分割线
    Window(height=5, content=BufferControl(buffer=buffer_output, focusable=False)),  # 上方显示
    Window(height=1, char='-'),  # 分割线
    Window(height=1, content=BufferControl(buffer=buffer_input, focusable=True)),  # 下方指令输入
])
# 创建一个布局
layout = Layout(root_container)

# 绑定快捷键
kb = KeyBindings()
# 退出快捷键（Ctrl + C）
@kb.add('c-c')
def exit_(event):
    event.app.exit()  # 退出应用

# 回车快捷键（Enter）
@kb.add('enter')
def enter_(event):
    text = buffer_input.text.strip()
    if text == 'exit':
        exit_(event)
    # 将输入的数据写入缓冲区
    elif text == '1':
        output_queue.put(b"1")
        buffer_output.text += "Sent: {text}\n"
    elif text == '0':
        output_queue.put(b"0")
        buffer_output.text += "Sent: {text}\n"
    else:
        buffer_output.text += "Error: Invalid input: {text}\n"
    # 清空输入缓冲区
    buffer_input.text = ""

# 创建终端应用
app = Application(layout=layout, key_bindings=kb, full_screen=True)


if __name__ == '__main__':
    # 启动线程
    t1 = threading.Thread(target=read_serial, args=(stop_event,))
    t2 = threading.Thread(target=write_serial, args=(stop_event,))
    t3 = threading.Thread(target=traffic_serial, args=(stop_event,))
    t1.start()
    t2.start()
    t3.start()
    app.run() # 阻塞
    # 退出后终止线程
    stop_event.set()
    # 关闭串口
    ser.close()
