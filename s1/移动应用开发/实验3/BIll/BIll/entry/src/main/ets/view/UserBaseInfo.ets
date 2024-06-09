@Component
export struct UserBaseInfo {
  @Prop nickname: string;
  @Prop signature: string;

  build() {
    Column() {
      // userIcon
      Image($r('app.media.ic_user'))
        .objectFit(ImageFit.Contain)
        .height($r('app.float.default_66'))
        .width($r('app.float.default_66'))
        .margin({ top: $r('app.float.default_71') })

      Column() {
        Text('LV.5')
          .fontSize($r('app.float.default_12'))
          .fontWeight(FontWeight.Bolder)
          .fontColor($r('app.color.leveColor'))
      }
      .width($r('app.float.default_44'))
      .height($r('app.float.default_16'))
      .margin({ top: $r('app.float.default_n8') })
      .border({ radius: $r('app.float.default_5') })
      .backgroundColor($r('app.color.leveBgColor'))
      .justifyContent(FlexAlign.Center)

      // nickname
      Text(this.nickname)
        .fontSize($r('app.float.default_20'))
        .fontFamily($r('app.string.Helvetica'))
        .margin({ bottom: $r('app.float.default_6') })
        .fontWeight(FontWeight.Normal)
        .fontColor($r('app.color.black'))

      // signature
      Text(this.signature).fontSize($r('app.float.default_16')).fontWeight(FontWeight.Normal)
        .fontFamily($r('app.string.PingFangSC_Regular'))
        .fontColor($r('app.color.signatureColor'))
    }
  }
}