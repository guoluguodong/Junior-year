
import { CommonConstants } from '../common/constants/CommonConstants';
import prompt from '@ohos.promptAction';
@Preview
@CustomDialog
export struct ColorDialogComponent {
  controller?: CustomDialogController;
  @Link bGC: Color;
  confirm?: (bGC: Color) => void;

  build() {
    Row(){
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Black)
        .width(30)
        .height(30)
        .margin({left:20})
        .onClick(() => {
              this.bGC= Color.Black
              this.confirm && this.confirm(this.bGC);
              this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Gray)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Gray
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Red)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Red
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Yellow)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Yellow
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Pink)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Pink
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Orange)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Pink
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
      Button({ type: ButtonType.Circle, stateEffect: false })
        .backgroundColor(Color.Green)
        .width(30)
        .height(30)        .margin({left:20})
        .onClick(() => {
          this.bGC= Color.Green
          this.confirm && this.confirm(this.bGC);
          this.controller?.close();
        })
    }
    .alignItems(VerticalAlign.Center)
    .width(CommonConstants.FULL_WIDTH)
    .height(100)
    .backgroundColor(Color.White)
    .align(Alignment.BottomEnd)
  }
}