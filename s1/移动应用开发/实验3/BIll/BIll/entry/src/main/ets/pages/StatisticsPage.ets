import AccountData from '../common/database/AccountData';
import router from '@ohos.router';
import { CustomPieChart, PicChartElement } from '../view/CustomPieChart'

@Entry
@Component
struct StatisticsPage {
  @State isIncome: boolean = false;
  // @State count: number = 0;
  public outcomeValueArr: number[] = [10, 10, 10, 10, 10, 10];
  @State TrueOutcomeValueArr: number[] = [0, 0, 0, 0, 0, 0];
  private outcomeArr: String[] = ['吃饭', '零食', '书籍', '旅游', '娱乐', '宠物'];
  public incomeValueArr: number[] = [10, 10, 10, 10];
  @State TrueIncomeValueArr: number[] = [0, 0, 0, 0];
  params = router.getParams(); // 获取传递过来的参数对象
  accountArr: AccountData[] = this.params['account'];
  curYear: number = this.params['curYear'];
  curMonth: number = this.params['curMonth'];
  curDay: number = this.params['curDay'];
  mode: number = this.params['mode'];
  private incomeArr: String[] = ['工作收入', '投资', '生活费', '奖学金'];
  @State picChartElements: PicChartElement[] = [];
  @State picChartElements2: PicChartElement[] = [];

  aboutToAppear() {
    this.StatisticByCate();

  }
  // 按照类别进行统计
  StatisticByCate() {
    this.TrueOutcomeValueArr = [0, 0, 0, 0, 0, 0];
    this.TrueIncomeValueArr = [0, 0, 0, 0];
    for (let i = 0;i < this.accountArr.length; i = i + 1) {
      if (this.accountArr[i].accountType == 0) {
        for (let k = 0;k < this.outcomeArr.length; k = k + 1) {
          if (this.accountArr[i].typeText == this.outcomeArr[k]) {
            this.TrueOutcomeValueArr[k] = this.TrueOutcomeValueArr[k] + this.accountArr[i].amount;
            break;
          }
        }
      }
      if (this.accountArr[i].accountType == 1) {
        for (let k = 0;k < this.outcomeArr.length; k = k + 1) {
          if (this.accountArr[i].typeText == this.incomeArr[k]) {
            this.TrueIncomeValueArr[k] = this.TrueIncomeValueArr[k] + this.accountArr[i].amount;
            break;
          }
        }
      }
    }
    this.picChartElements = [];
    this.picChartElements2 = [];
    this.picChartElements = [
      new PicChartElement('吃饭', this.TrueOutcomeValueArr[0], '#ff9421'),
      new PicChartElement('零食', this.TrueOutcomeValueArr[1], '#ffd100'),
      new PicChartElement('书籍', this.TrueOutcomeValueArr[2], '#4cd041'),
      new PicChartElement('旅游', this.TrueOutcomeValueArr[3], '#4cd0ee'),
      new PicChartElement('娱乐', this.TrueOutcomeValueArr[4], '#999999'),
      new PicChartElement('宠物', this.TrueOutcomeValueArr[5], '#ffb75858')
    ];

    // 饼图2数据
    this.picChartElements2 = [
      new PicChartElement('工作收入', this.TrueIncomeValueArr[0], '#ff9421'),
      new PicChartElement('投资', this.TrueIncomeValueArr[1], '#ffd100'),
      new PicChartElement('生活费', this.TrueIncomeValueArr[2], '#4cd041'),
      new PicChartElement('奖学金', this.TrueIncomeValueArr[3], '#4cd0ee')
    ]
  }

  ArraySum(a: number[]) {
    let sum = 0;
    for (let i = 0;i < a.length; i++) {
      sum = a[i] + sum;
    }
    return sum;
  }

  build() {
    Column({ space: 5 }) {
      if (this.mode == 1) {
        Text('所有账单')
          .fontSize($r('app.float.font_size_L'))
          .fontColor($r('app.color.black'))
          .margin({ top: $r('app.float.edge_size_L') })
      }
      else if (this.mode == 2) {
        Text(String(this.curYear) + '年')
          .fontSize($r('app.float.font_size_L'))
          .fontColor($r('app.color.black'))
          .margin({ top: $r('app.float.edge_size_L') })
      }
      else if (this.mode == 3) {
        Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月")
          .fontSize($r('app.float.font_size_L'))
          .fontColor($r('app.color.black'))
          .margin({ top: $r('app.float.edge_size_L') })
      }
      else if (this.mode == 4) {
        Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月" + String(this.curDay) + "日")
          .fontSize($r('app.float.font_size_L'))
          .fontColor($r('app.color.black'))
          .margin({ top: $r('app.float.edge_size_L') })
      }
      if (this.isIncome == false) {
        Row() {
          Button() {
            Row() {
              Text("收入:")
                .fontSize($r('app.float.font_size_M'))
                .margin(7)
              Text("￥" + String(this.ArraySum(this.TrueIncomeValueArr)))
                .fontSize($r('app.float.font_size_L'))
                .margin(7)
            }
          }
          .onClick(() => {
            this.isIncome = true;
            this.StatisticByCate();
          })
          .backgroundColor($r('app.color.white'))

          Divider()
            .vertical(true)
            .height(22)
            .color('#182431')
            .opacity(0.6)
            .margin({ left: 8, right: 8 })
          Button() {
            Row() {
              Text("支出")
                .fontSize($r('app.float.font_size_M'))
                .margin(7)
                .fontColor($r('app.color.primaryRed'))
              Text("￥" + String(this.ArraySum(this.TrueOutcomeValueArr)))
                .fontSize($r('app.float.font_size_L'))
                .margin(7)

                .fontColor($r('app.color.primaryRed'))
            }
          }
          .onClick(() => {
            this.isIncome = false;

            this.StatisticByCate();
          })
          .backgroundColor($r('app.color.white'))
        }
        .margin({ top: 15 })

        DataPanel({
          values: this.TrueOutcomeValueArr,
          max: this.ArraySum(this.TrueOutcomeValueArr),
          type: DataPanelType.Circle
        }).width(168).height(168)
        DataPanel({ values: this.outcomeValueArr, max: 60, type: DataPanelType.Line })
          .width(312).height(10)
        Row() {
          ForEach(this.outcomeArr, (item: String, index) => {
            Text(item.toString() + String(this.TrueOutcomeValueArr[index]))
              .fontSize($r('app.float.font_size_M'))
              .width(52)
              .textAlign(TextAlign.Center)
          })
        }
        .width('100%')
        .justifyContent(FlexAlign.Center)
        .margin({ top: 5 })

        Column({ space: 10 }) {
          // 饼图1 (半径，单位使用默认值)
          CustomPieChart({ picChartElements: this.picChartElements })
        }
        .width('100%')
        .padding(30)
      }
      else {
        Row() {
          Button() {
            Row() {
              Text("收入:")
                .fontSize($r('app.float.font_size_M'))
                .fontColor($r('app.color.primaryRed'))
                .margin(7)
              Text("￥" + String(this.ArraySum(this.TrueIncomeValueArr)))
                .fontSize($r('app.float.font_size_L'))
                .margin(7)
                .fontColor($r('app.color.primaryRed'))
            }
          }
          .onClick(() => {
            this.isIncome = true;
            this.StatisticByCate();
          })
          .backgroundColor($r('app.color.white'))

          Divider()
            .vertical(true)
            .height(22)
            .color('#182431')
            .opacity(0.6)
            .margin({ left: 8, right: 8 })
          Button() {
            Row() {
              Text("支出")
                .fontSize($r('app.float.font_size_M'))
                .margin(7)
              Text("￥" + String(this.ArraySum(this.TrueOutcomeValueArr)))
                .fontSize($r('app.float.font_size_L'))
                .margin(7)
            }
          }
          .onClick(() => {
            this.isIncome = false;
            this.StatisticByCate();
          })
          .backgroundColor($r('app.color.white'))
        }
        .margin({ top: 15 })

        DataPanel({
          values: this.TrueIncomeValueArr,
          max: this.ArraySum(this.TrueIncomeValueArr),
          type: DataPanelType.Circle
        }).width(168).height(168)

        DataPanel({ values: this.incomeValueArr, max: 40, type: DataPanelType.Line })
          .width(312).height(10)
        Row() {
          ForEach(this.incomeArr, (item: String, index) => {
            Text(item.toString() + String(this.TrueIncomeValueArr[index]))
              .fontSize($r('app.float.font_size_M'))
              .width(78)
              .textAlign(TextAlign.Center)
          })
        }
        .width('100%')
        .justifyContent(FlexAlign.Center)
        .margin({ top: 5 })

        Column({ space: 10 }) {
          // 饼图2 (半径，单位使用默认值)
          CustomPieChart({ picChartElements: this.picChartElements2 })
        }
        .width('100%')
        .padding(30)
      }
    }
    //   Column({ space: 10 }) {
    //     // 在 100 * 100 的矩形框中绘制一段折线，起点(0, 0)，经过(20,60)，到达终点(100, 100)
    //     // Polyline({ width: 100, height: 100 })
    //     //   .points([[0, 0], [20, 60], [100, 100]])
    //     //   .fillOpacity(0)
    //     //   .stroke(Color.Blue)
    //     //   .strokeWidth(3)
    //     // 在 100 * 100 的矩形框中绘制一段折线，起点(20, 0)，经过(0,100)，到达终点(100, 90)
    //     Polyline()
    //       .width(250)
    //       .height(100)
    //       .fillOpacity(0)
    //       .stroke(Color.Black)
    //       .strokeWidth(4)
    //       .points([[0, 0], [0, 90], [100, 90]])
    //         // 设置折线拐角处为圆弧
    //       .strokeLineJoin(LineJoinStyle.Round)
    //         // 设置折线两端为半圆
    //       .strokeLineCap(LineCapStyle.Round)
    //   }.width('100%')
    // }.justifyContent(FlexAlign.Center)
  }
}