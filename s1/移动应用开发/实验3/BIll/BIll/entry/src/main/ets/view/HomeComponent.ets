import AccountTable from '../common/database/tables/AccountTable';
import AccountData from '../common/database/AccountData';
import { CommonConstants } from '../common/constants/CommonConstants';
import { DialogComponent } from '../view/DialogComponent';
import { FiltrateDialogComponent } from '../view/FiltrateDialogComponent';
import { ImageList } from '../common/database/AccountList';
import Logger from '../common/utils/Logger';
import { ScrollData } from '../common/database/ScrollData'
import router from '@ohos.router';
import { GroupScroll } from '../common/database/GroupScroll'
import HashMap from '@ohos.util.HashMap';
import { ColorDialogComponent } from '../view/ColorDialogComponent';
@Component
export default struct HomeIndex {
  @State accounts: Array<AccountData> = [];
  @State scroll_accounts: Array<ScrollData> = [];
  @State group_account: HashMap<String, GroupScroll> = new HashMap();
  @State group_account_arr: Array<GroupScroll> = [];
  @State searchText: string = '';
  @State isEdit: boolean = false;
  @State isInsert: boolean = false;
  @State bGC: Color = Color.Black;
  @State newAccount: AccountData = {
    id: 0,
    accountType: 0,
    typeText: '',
    amount: 0,
    year: 2000,
    month: 11,
    day: 11,
    note: ''
  };
  @State index: number = -1;
  scroller: Scroller = new Scroller()
  @State income: number = 0.00;
  @State outcome: number = 0.00;
  @State total: number = 0.00;
  @State isIncome: number = 0;
  @State curYear: number = 2000;
  @State curMonth: number = 1;
  @State curDay: number = 1;
  @State mode: number = 1;
  @State selectedDate: Date = new Date();
  private AccountTable = new AccountTable(() => {
  });
  private deleteList: Array<AccountData> = [];
  searchController: SearchController = new SearchController();
  private scroll2DeleteData: ScrollData = null;
  private downX = 0;
  private deleteWidth = 65;
  dialogController: CustomDialogController = new CustomDialogController({
    builder: DialogComponent({
      isInsert: $isInsert,
      newAccount: $newAccount,
      confirm: (isInsert: boolean, newAccount: AccountData) => this.accept(isInsert, newAccount)
    }),
    customStyle: true,
    alignment: DialogAlignment.Bottom
  });
  filtrateDialogController: CustomDialogController = new CustomDialogController({
    builder: FiltrateDialogComponent({
      curYear: $curYear,
      curMonth: $curMonth,
      curDay: $curDay,
      mode: $mode,
      confirm: (curYear, curMonth, curDay, mode) => this.filtrateAccept(curYear, curMonth, curDay, mode)
    }),
    alignment: DialogAlignment.Default, // 可设置dialog的对齐方式，设定显示在底部或中间等，默认为底部显示
  })
  colorDialogController: CustomDialogController = new CustomDialogController({
    builder: ColorDialogComponent({
      bGC: $bGC,
      confirm: (bGC) => this.ColorfiltrateAccept(bGC)
    }),
    alignment: DialogAlignment.Default, // 可设置dialog的对齐方式，设定显示在底部或中间等，默认为底部显示
  })
  ColorfiltrateAccept(bGC){
      this.bGC=bGC;
  }
  filtrateAccept(curYear, curMonth, curDay, mode) {
    console.info('Callback when the second button is clicked: ' + String(curYear) + '-' + String(curMonth + 1) + '-' + String(curDay) + 'mode' + String(mode))
    this.AccountTable.getRdbStore(() => {
      this.AccountTable.queryByTime(curYear, curMonth, curDay, mode, (result: AccountData[]) => {
        this.accounts = result;
        this.accountData2ScrollData(this.accounts);
        this.StatisticalIncomeExpenditure();
      });
    });

  }

  accept(isInsert: boolean, newAccount: AccountData): void {
    if (isInsert) {
      Logger.info(`${CommonConstants.INDEX_TAG}`, `The account inserted is:  ${JSON.stringify(newAccount)}`);
      this.AccountTable.insertData(newAccount, (id: number) => {
        newAccount.id = id;
        this.accounts.push(newAccount);
        this.accountData2ScrollData(this.accounts);
        this.StatisticalIncomeExpenditure();
      });
    } else {
      this.AccountTable.updateData(newAccount, () => {
      });
      let list = this.accounts;
      this.accounts = [];
      list[this.index] = newAccount;
      this.accounts = list;
      this.index = -1;
      this.accountData2ScrollData(this.accounts);
      this.StatisticalIncomeExpenditure();
    }

  }

  aboutToAppear() {
    this.AccountTable.getRdbStore(() => {
      this.AccountTable.query(0, (result: AccountData[]) => {
        this.accounts = result;
        this.accountData2ScrollData(this.accounts);
        this.StatisticalIncomeExpenditure();
      }, true);
    });
  }

  selectListItem(item: AccountData) {
    this.isInsert = false;
    this.index = this.accounts.indexOf(item);
    this.newAccount = {
      id: item.id,
      accountType: item.accountType,
      typeText: item.typeText,
      amount: item.amount,
      year: item.year,
      month: item.month,
      day: item.day,
      note: item.note
    };
  }

  deleteListItem() {
    for (let i = 0; i < this.deleteList.length; i++) {
      let index = this.accounts.indexOf(this.deleteList[i]);
      this.accounts.splice(index, 1);
      this.AccountTable.deleteData(this.deleteList[i], () => {
      });
    }
    this.deleteList = [];
    this.isEdit = false;
    this.accountData2ScrollData(this.accounts);
    this.StatisticalIncomeExpenditure();
  }

  accountData2ScrollData(accounts: AccountData[]) {
    this.scroll_accounts = [];
    for (let i = 0;i < accounts.length; ++i) {
      this.scroll_accounts.push({ accountData: accounts[i], scroller: new Scroller() })
    }
    this.sortByYearAndMonth();
    this.divideByYearAndMonth();
  }

  CompareCustom(a: ScrollData, b: ScrollData) {
    if (a.accountData.year > b.accountData.year) {
      return false;
    }
    else if (a.accountData.year < b.accountData.year) {
      return true;
    }
    else {
      if (a.accountData.month > b.accountData.month) {
        return false;
      }
      else if (a.accountData.month < b.accountData.month) {
        return true;
      }
      else {
        if (a.accountData.day > b.accountData.day) {
          return false;
        }
        else if (a.accountData.day <= b.accountData.day) {
          return true;
        }
      }
    }
  }

  sortByYearAndMonth() {
    for (let i = 0;i < this.scroll_accounts.length; i++) {
      let max: ScrollData = this.scroll_accounts[i];
      let maxIndex = i;
      for (let j = i + 1;j < this.scroll_accounts.length; j++)
        if (this.CompareCustom(max, this.scroll_accounts[j])) {
          max = this.scroll_accounts[j];
          maxIndex = j;
        }
      let tmp: ScrollData = this.scroll_accounts[i];
      this.scroll_accounts[i] = this.scroll_accounts[maxIndex];
      this.scroll_accounts[maxIndex] = tmp;
    }
    Logger.info("sort")
  }

  divideByYearAndMonth() {
    this.group_account.clear();
    let titleList = [];
    for (let i = 0;i < this.scroll_accounts.length; i++) {
      let yearMonth: String =
        String(this.scroll_accounts[i].accountData.year) + '年' +
        String(this.scroll_accounts[i].accountData.month + 1) + '月'
      if (!this.group_account.hasKey(yearMonth)) {
        let tmp: Array<ScrollData> = [];
        tmp.push(this.scroll_accounts[i]);
        let gstmp: GroupScroll = new GroupScroll(yearMonth, tmp);
        this.group_account.set(yearMonth, gstmp);
        titleList.push(yearMonth);
      }
      else {
        let gstmp: GroupScroll = this.group_account.get(yearMonth);
        gstmp.scrollData.push(this.scroll_accounts[i]);
        gstmp.StatisticalIncomeExpenditurePart();
        this.group_account.set(yearMonth, gstmp);
      }
    }
    this.group_account_arr = [];
    for (let i = 0;i < titleList.length; i++) {
      this.group_account_arr.push(this.group_account.get(titleList[i]));
    }
  }

  StatisticalIncomeExpenditure() {
    this.income = 0;
    this.outcome = 0;
    for (let i = 0;i < this.accounts.length; ++i) {
      if (this.accounts[i].accountType == 0) {
        this.outcome = this.outcome - this.accounts[i].amount;
      }
      else {
        this.income = this.income + this.accounts[i].amount;
      }
    }
    this.total = this.income + this.outcome;
  }

  @Builder itemHead(text: string, income: string, outcome: string, total: string) {
    Column() {
      Row() {
        Column() {
          Text(text)
            .fontSize(20)
          Row() {
            Text('收入￥' + income)
              .fontSize(12)
            Text('支出￥' + outcome)
              .fontSize(12)
          }
        }
        .width(200)
        .alignItems(HorizontalAlign.Start)
        .padding({ left: 30, right: 30 })

        Blank()
          .layoutWeight(1)
        Text('合计￥' + total)
          .fontSize(20)
      }
    }
    .backgroundImage($r('app.media.itemBackground'))
    .width(CommonConstants.FULL_WIDTH)
    .alignItems(HorizontalAlign.Start)
  }

  @Builder CustomItem() {

      List({ space: CommonConstants.FULL_SIZE }) {
        // // 循环渲染ListItemGroup，contactsGroups为多个分组联系人contacts和标题title的数据集合
        ForEach(this.group_account_arr, item_out => {
          ListItemGroup({
            header: this.itemHead(item_out.title, String(item_out.statisticData[0]), String(item_out.statisticData[1]), String(item_out.statisticData[2]))
          }) {

            // 循环渲染ListItem
            ForEach(item_out.scrollData, item => {
              ListItem() {
                Scroll(item.scroller) {
                  Row() {
                    Row() {
                      Image(ImageList[item.accountData.typeText])
                        .width($r('app.float.component_size_M'))
                        .aspectRatio(CommonConstants.FULL_SIZE)
                        .margin({ right: $r('app.float.edge_size_MP') })

                      Text(item.accountData.typeText)
                        .height($r('app.float.component_size_SM'))
                        .fontSize($r('app.float.font_size_M'))
                        .width(70)
                      Divider()
                        .vertical(true)
                        .height(22)
                        .color('#182431')
                        .opacity(0.6)
                        .margin({ left: 8, right: 8 })
                      Column() {
                        Text("备注：" + item.accountData.note)
                          .height($r('app.float.component_size_SM'))
                          .fontSize($r('app.float.font_size_S'))
                          .fontColor(Color.Gray)
                        Text(String(item.accountData.year) + '-' + String(item.accountData.month + 1) + '-' + String(item.accountData.day))
                          .height($r('app.float.component_size_SM'))
                          .fontSize($r('app.float.font_size_S'))
                          .fontColor(Color.Gray)
                      }

                      Blank()
                        .layoutWeight(1)

                      if (!this.isEdit) {
                        Text(item.accountData.accountType === 0 ? '-' + item.accountData.amount.toString() : '+' + item.accountData.amount.toString())
                          .fontSize($r('app.float.font_size_M'))
                          .fontColor(item.accountData.accountType === 0 ? $r('app.color.pay_color') : $r('app.color.main_color'))
                          .align(Alignment.End)
                          .flexGrow(CommonConstants.FULL_SIZE)
                      } else {
                        Row() {
                          Toggle({ type: ToggleType.Checkbox })
                            .onChange((isOn) => {
                              if (isOn) {
                                this.deleteList.push(item.accountData);
                              } else {
                                let index = this.deleteList.indexOf(item.accountData);
                                this.deleteList.splice(index, 1);
                              }
                            })
                        }
                        .align(Alignment.End)
                        .flexGrow(CommonConstants.FULL_SIZE)
                        .justifyContent(FlexAlign.End)
                      }

                    }
                    .width(CommonConstants.FULL_WIDTH)
                    .padding({ left: $r('app.float.edge_size_M'), right: $r('app.float.edge_size_M') })

                    Button() {
                      Text('删除')
                        .height($r('app.float.component_size_SM'))
                        .fontSize($r('app.float.font_size_M'))
                    }
                    .type(ButtonType.Normal)
                    .width(65)
                    .backgroundColor($r('app.color.primaryRed'))
                    .onClick(() => {
                      // this.scroll_accounts.splice(this.scroll_accounts.indexOf(item),1)
                      // this.accounts.splice(this.accounts.indexOf(item.accountData),1)
                      this.deleteList.push(item.accountData)
                      this.deleteListItem()
                    })
                  }
                }
                .scrollable(ScrollDirection.Horizontal)
                .scrollBar(BarState.Off)
                .onTouch((event: TouchEvent) => {
                  if (this.scroll2DeleteData != null && this.scroll2DeleteData != null) {
                    this.scroll2DeleteData.scroller.scrollTo({ xOffset: 0, yOffset: 0,
                      animation: { duration: 100, curve: Curve.Linear } })
                  }
                  switch (event.type) {
                    case TouchType.Down:
                      this.downX = event.touches[0].x
                      break;
                    case TouchType.Up:
                      let xOffset = event.touches[0].x - this.downX
                      if (xOffset == 0) {
                        return
                      }
                      var toxOffset = 0
                      this.scroll2DeleteData = null;
                      if (Math.abs(xOffset) > vp2px(this.deleteWidth) / 2 && xOffset < 0) {
                        toxOffset = vp2px(this.deleteWidth)
                        this.scroll2DeleteData = item
                      }
                      item.scroller.scrollTo({ xOffset: toxOffset, yOffset: 0,
                        animation: { duration: 300, curve: Curve.Linear } })
                      this.downX = 0;
                      break;

                  }
                })
              }
              .width(CommonConstants.FULL_WIDTH)
              .height($r('app.float.component_size_LM'))
              .onClick(() => {
                this.selectListItem(item.accountData);
                this.dialogController.open();
              })
            })
          }

        })
      }

    .scrollBar(BarState.Auto)
    .width(CommonConstants.FULL_WIDTH)
    .height(480)
    .borderRadius($r('app.float.radius_size_L'))
    .backgroundColor(Color.White)
    .divider({
      strokeWidth: 1,
      startMargin: 60,
      endMargin: 10,
      color: '#ffe9f0f0'
    })

    if (!this.isEdit) {
      Button() {
        Image($r('app.media.ic_home_add'))
      }
      .width($r('app.float.component_size_MP'))
      .height($r('app.float.component_size_MP'))
      .position({ x: CommonConstants.FILTRATE_POSITION_X, y: CommonConstants.FILTRATE_POSITION_Y })
      .onClick(() => {
        this.isInsert = true;
        this.newAccount = {
          id: 0,
          accountType: 0,
          typeText: '',
          amount: 0,
          year: 2000,
          month: 12,
          day: 1,
          note: ''
        };
        this.dialogController.open();
      })

      Button() {
        Image($r('app.media.filtrate'))
      }
      .width($r('app.float.component_size_MP'))
      .height($r('app.float.component_size_MP'))
      .position({ x: CommonConstants.EDIT_POSITION_X, y: CommonConstants.EDIT_POSITION_Y })
      .onClick(() => {
        this.isInsert = true;
        this.newAccount = {
          id: 0,
          accountType: 0,
          typeText: '',
          amount: 0,
          year: 2000,
          month: 12,
          day: 1,
          note: ''
        };
        this.filtrateDialogController.open();
      })
    }

    if (this.isEdit) {
      Button() {
        Image($r('app.media.delete'))
      }
      .width($r('app.float.component_size_MP'))
      .height($r('app.float.component_size_MP'))
      .backgroundColor($r('app.color.background_color'))
      .markAnchor({ x: $r('app.float.mark_anchor'), y: CommonConstants.MINIMUM_SIZE })
      .position({ x: CommonConstants.DELETE_POSITION_X, y: CommonConstants.DELETE_POSITION_Y })
      .onClick(() => {

        this.deleteListItem();
      })
    }

  }

  build() {
    Stack() {
      Column() {
        Row() {
          Text($r('app.string.no_task'))
            .height($r('app.float.component_size_SP'))
            .fontSize($r('app.float.font_size_L'))
            .fontColor(Color.White)
            .margin({ left: $r('app.float.font_size_L') })

          Image($r('app.media.ic_public_edit'))
            .width($r('app.float.component_size_S'))
            .aspectRatio(CommonConstants.FULL_SIZE)
            .margin({ right: $r('app.float.font_size_L') })
            .onClick(() => {
              this.isEdit = true;
            })
        }
        .width(CommonConstants.FULL_WIDTH)
        .justifyContent(FlexAlign.SpaceBetween)
        .margin({ top: $r('app.float.edge_size_M'), bottom: $r('app.float.edge_size_MM') })

        Row() {
          Search({
            value: this.searchText,
            placeholder: CommonConstants.SEARCH_TEXT,
            controller: this.searchController
          })
            .width(CommonConstants.FULL_WIDTH)
            .borderRadius($r('app.float.radius_size_M'))
            .borderWidth($r('app.float.border_size_S'))
            .borderColor($r('app.color.border_color'))
            .placeholderFont({ size: $r('app.float.font_size_M') })
            .textFont({ size: $r('app.float.font_size_M') })
            .backgroundColor(Color.White)
            .onChange((searchValue: string) => {
              this.searchText = searchValue;
            })
            .onSubmit((searchValue: string) => {
              if (searchValue === '') {
                this.AccountTable.query(0, (result: AccountData[]) => {
                  this.accounts = result;
                  this.accountData2ScrollData(this.accounts);
                }, true);
              } else {
                if (String(Number(searchValue)) != 'NaN') {
                  this.AccountTable.query(Number(searchValue), (result: AccountData[]) => {
                    this.accounts = result;
                    this.accountData2ScrollData(this.accounts);
                  }, false);
                }

                else {
                  this.AccountTable.queryByNote(String(searchValue), (result: AccountData[]) => {
                    this.accounts = result;
                    this.accountData2ScrollData(this.accounts);
                  });
                }

              }
            })
        }
        .width(CommonConstants.FULL_WIDTH)
        .padding({ left: $r('app.float.edge_size_M'), right: $r('app.float.edge_size_M') })
        .margin({ top: $r('app.float.edge_size_S'), bottom: $r('app.float.edge_size_S') })

        Column() {

          Row() {
            Image($r('app.media.icons_paint'))
              .width(30)
              .height(30)
              .margin({right:5})
              .onClick(() => {
                this.colorDialogController.open();
              })
            Image($r('app.media.Pie'))
              .width(30)
              .height(30)
              .onClick(() => {
                router.pushUrl({
                  url: 'pages/StatisticsPage', // 目标url
                  params: {
                    "account": this.accounts,
                    "curYear": this.curYear,
                    "curMonth": this.curMonth,
                    "curDay": this.curDay,
                    "mode": this.mode
                  } // 添加params属性，传递自定义参数
                }, (err) => {
                  if (err) {
                    console.error(`Invoke pushUrl failed, code is ${err.code}, message is ${err.message}`);
                    return;
                  }
                  console.info('Invoke pushUrl succeeded.');
                })
              })
          }
          .width(300)
          .margin({bottom:10})
          //
          Text(String(this.total) + '元')
            .fontSize($r('app.float.font_size_L'))
            .fontColor(Color.White)

          if (this.mode == 1) {
            Text('全部收支结余')
              .fontSize(12)
              .fontColor($r('app.color.white'))
              .margin({ top: 3 })
          }
          else if (this.mode == 2) {
            Text(String(this.curYear) + '年收支结余')
              .fontSize(12)
              .fontColor($r('app.color.white'))
              .margin({ top: 3 })
          }
          else if (this.mode == 3) {
            Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月收支结余")
              .fontSize(12)
              .fontColor($r('app.color.white'))
              .margin({ top: 3 })
          }
          else if (this.mode == 4) {
            Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月" + String(this.curDay) + "日收支结余")
              .fontSize(12)
              .fontColor($r('app.color.white'))
              .margin({ top: 3 })
          }
          Row() {
            Column() {
              Text(String(this.income) + '元')
                .fontSize(20)
                .fontColor(Color.White)

              if (this.mode == 1) {
                Text('全部收入')
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 2) {
                Text(String(this.curYear) + '年收入')
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 3) {
                Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月收入")
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 4) {
                Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月" + String(this.curDay) + "日收入")
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
            }
            .width(200)
            .margin({ top: $r('app.float.edge_size_S'), bottom: $r('app.float.edge_size_S') })
            .alignItems(HorizontalAlign.Center)

            Blank()
              .layoutWeight(1)
            Column() {
              Text(String(this.outcome) + '元')
                .fontSize(20)
                .fontColor(Color.White)

              if (this.mode == 1) {
                Text('全部支出')
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 2) {
                Text(String(this.curYear) + '年支出')
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 3) {
                Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月支出")
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
              else if (this.mode == 4) {
                Text(String(this.curYear) + '年' + String(this.curMonth + 1) + "月" + String(this.curDay) + "日支出")
                  .fontSize(12)
                  .fontColor($r('app.color.white'))
                  .margin({ top: 3 })
              }
            }
            .width(200)
            .margin({ top: $r('app.float.edge_size_S'), bottom: $r('app.float.edge_size_S') })
            .alignItems(HorizontalAlign.Center)
          }
          .margin({ top: 10 })
        }

        this.CustomItem()
      }
      .backgroundColor(this.bGC)
      .width(CommonConstants.FULL_WIDTH)
      .margin({ top: $r('app.float.edge_size_S'), bottom: $r('app.float.edge_size_S') })
      .alignItems(HorizontalAlign.Center)

    }

    // .width(CommonConstants.FULL_WIDTH)
    // .height(CommonConstants.FULL_HEIGHT)
    // .backgroundColor($r('app.color.background_color'))
  }
}
