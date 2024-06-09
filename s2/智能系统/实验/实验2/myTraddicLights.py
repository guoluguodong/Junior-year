import time
from pyknow import *
import random
from colorama import Fore, init
import math

init()
PERIOD = 10
# 每个周期需要通过 NS, WE 的车辆数目 模拟列表，第0个元素代表第0个周期
CARS = [[random.randint(10, 20), random.randint(0, 10)]
        for i in range(100)]
CARS.insert(0, [0, 0])


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
    @Rule(AS.oldFact << Fact(Ticks=MATCH.times))
    def ticks(self, times, oldFact):
        time.sleep(1)
        self.retract(oldFact)
        self.declare(Fact(Ticks=times + 1))
        print("====== 启动时间{}秒 ======".format(times))

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
            print(Fore.WHITE + '=========' + Fore.RED + "剩余时间: {} s".format(switchTime) + Fore.WHITE + '=========')
            print(Fore.GREEN + "剩余时间: {} s".format(
                switchTime) + Fore.WHITE + '=======' + Fore.GREEN + "剩余时间: {} s".format(switchTime))
            print(Fore.WHITE + '=========' + Fore.RED + "剩余时间: {} s".format(switchTime) + Fore.WHITE + '=========')
        if NScolor == "GREEN":
            print(
                Fore.WHITE + '=========' + Fore.GREEN + "剩余时间: {} s".format(
                    PERIOD - switchTime) + Fore.WHITE + '=========')
            print(Fore.RED + "剩余时间: {} s".format(
                PERIOD - switchTime) + Fore.WHITE + '=======' + Fore.RED + "剩余时间: {} s".format(PERIOD - switchTime))
            print(
                Fore.WHITE + '=========' + Fore.GREEN + "剩余时间: {} s".format(
                    PERIOD - switchTime) + Fore.WHITE + '=========')

    @Rule(
        Fact(Ticks=MATCH.times),
        AS.oldFact1 << Fact(switchTime=MATCH.switchTime),
        AS.oldFact2 << Fact(NeedChange=True),
        salience=2
    )
    def changeSwitchTime(self, oldFact1, oldFact2, times, switchTime):
        print("修改时间")
        self.retract(oldFact1)
        self.retract(oldFact2)
        cur_epoch = times // PERIOD
        newSwitchTime = switchTime
        # 修正红绿灯
        print("第 {} 个周期，南北方向通过{}辆车，东西方向通过{}辆车".format(cur_epoch, CARS[cur_epoch][0],
                                                                          CARS[cur_epoch][1]))
        if cur_epoch != 0:
            newSwitchTime = math.floor(PERIOD - PERIOD / (CARS[cur_epoch][0] / (CARS[cur_epoch][1] + 0.00001) + 1))
        self.declare(Fact(switchTime=newSwitchTime))
        self.declare(Fact(switch=True))
        self.declare(Fact(NeedShow=True))


engine = Greetings()
engine.reset()  # Prepare the engine for the execution.
engine.run()  # Run it!
