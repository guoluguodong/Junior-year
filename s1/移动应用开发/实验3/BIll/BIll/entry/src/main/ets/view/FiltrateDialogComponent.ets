import picker from '@ohos.file.picker';
import { CommonConstants } from '../common/constants/CommonConstants';

@Preview
@CustomDialog
export struct FiltrateDialogComponent {
  @Link curYear: number;
  @Link curMonth: number;
  @Link curDay: number;
  @Link mode: number;
  controller: CustomDialogController;
  confirm?: (curYear: number, curMonth: number, curDay: number, mode: number) => void;
  selectedDate: Date = new Date('2001-1-1')
  private years: string[] = ['2000', '2001', '2002', '2003', '2004', '2005', '2006', '2007', '2008', '2009', '2010', '2011', '2012', '2013', '2014', '2015', '2016', '2017', '2018', '2019', '2020', '2021', '2022', '2023', '2024', '2025', '2026', '2027', '2028', '2029', '2030', '2031', '2032', '2033', '2034', '2035', '2036', '2037', '2038', '2039', '2040', '2041', '2042', '2043', '2044', '2045', '2046', '2047', '2048', '2049', '2050', '2051', '2052', '2053', '2054', '2055', '2056', '2057', '2058', '2059', '2060', '2061', '2062', '2063', '2064', '2065', '2066', '2067', '2068', '2069', '2070', '2071', '2072', '2073', '2074', '2075', '2076', '2077', '2078', '2079', '2080', '2081', '2082', '2083', '2084', '2085', '2086', '2087', '2088', '2089', '2090', '2091', '2092', '2093', '2094', '2095', '2096', '2097', '2098'];
  private month: string[] = ['Jan', 'Feb', 'Mar', 'Ari', 'May', 'Jun', 'Jul', 'Aut', 'Sep', 'Oct', 'Nov', 'Dec'];

  build() {
    Column() {
      Button('全部账单')
        .backgroundColor(Color.White)
        .fontSize(20)
        .fontColor(Color.Black)
        .margin({ top: 10, bottom: 10 })
        .onClick(() => {
          this.curYear = 2099
          this.curMonth = 13
          this.curDay = 32
          this.mode = 1;
          this.confirm && this.confirm(this.curYear, this.curMonth, this.curDay, this.mode);
          this.controller?.close();
        })

      Button('年度账单')
        .backgroundColor(Color.White)
        .fontSize(20)
        .fontColor(Color.Black)
        .margin({ top: 10, bottom: 10 })
        .onClick(() => {
          TextPickerDialog.show({
            range: this.years,
            selected: 1,
            onAccept: (value: TextPickerResult) => {
              console.info("TextPickerDialog:onAccept()" + JSON.stringify(value));
              this.curMonth = 0;
              this.curDay = 0;
              this.curYear = Number(value.value);
              this.mode = 2;
              console.info("DatePickerDialog:onAccept()" + JSON.stringify(value));
              this.confirm && this.confirm(this.curYear, this.curMonth, this.curDay, this.mode);
              this.controller?.close();
            },
            onCancel: () => {
              console.info("DatePickerDialog:onCancel()")
            },
            onChange: (value: TextPickerResult) => {
              console.info("DatePickerDialog:onChange()" + JSON.stringify(value))
            }
          })
        })

      Button('月度账单')
        .backgroundColor(Color.White)
        .fontSize(20)
        .fontColor(Color.Black)
        .margin({ top: 10, bottom: 10 })
        .onClick(() => {
          TextPickerDialog.show({
            range: this.years,
            selected: 1,
            onAccept: (value: TextPickerResult) => {
              console.info("TextPickerDialog:onAccept()" + JSON.stringify(value));
              this.curMonth = 0;
              this.curDay = 0;
              this.curYear =Number(value.value);
              TextPickerDialog.show({
                range: this.month,
                selected: 1,
                onAccept: (value: TextPickerResult) => {
                  console.info("TextPickerDialog:onAccept()" + JSON.stringify(value));
                  let i=0;
                  for(i=0;i<12;i++){
                  if(this.month[i]==value.value)
                    break
                  }
                  this.curMonth = i;
                  this.curDay = 0;
                  this.mode = 3;
                  console.info("DatePickerDialog:onAccept()" + JSON.stringify(value));
                  this.confirm && this.confirm(this.curYear, this.curMonth, this.curDay, this.mode);
                  this.controller?.close();
                },
                onCancel: () => {
                  console.info("DatePickerDialog:onCancel()")
                },
                onChange: (value: TextPickerResult) => {
                  console.info("DatePickerDialog:onChange()" + JSON.stringify(value))
                }
              })
            },
            onCancel: () => {
              console.info("DatePickerDialog:onCancel()")
            },
            onChange: (value: TextPickerResult) => {
              console.info("DatePickerDialog:onChange()" + JSON.stringify(value))
            }
          })

        })
      Button('日账单')
        .backgroundColor(Color.White)
        .fontSize(20)
        .fontColor(Color.Black)
        .margin({ top: 10, bottom: 10 })
        .onClick(() => {
          DatePickerDialog.show({
            start: new Date("2000-1-1"),
            end: new Date("2100-12-31"),
            selected: this.selectedDate,
            onAccept: (value: DatePickerResult) => {
              // 通过Date的setFullYear方法设置按下确定按钮时的日期，这样当弹窗再次弹出时显示选中的是上一次确定的日期
              this.selectedDate.setFullYear(value.year, value.month, value.day)
              this.curYear = value.year
              this.curMonth = value.month
              this.curDay = value.day
              this.mode = 4;
              console.info("DatePickerDialog:onAccept()" + JSON.stringify(value))
              this.confirm && this.confirm(this.curYear, this.curMonth, this.curDay, this.mode);
              this.controller?.close();
            },
            onCancel: () => {
              console.info("DatePickerDialog:onCancel()")
            },
            onChange: (value: DatePickerResult) => {
              console.info("DatePickerDialog:onChange()" + JSON.stringify(value))
            }
          })
        })
    }
    .width(CommonConstants.FULL_WIDTH)
    .borderRadius({ topLeft: $r('app.float.font_size_L'), topRight: $r('app.float.font_size_L') })
    .backgroundColor(Color.White)
    .align(Alignment.BottomEnd)
  }
}