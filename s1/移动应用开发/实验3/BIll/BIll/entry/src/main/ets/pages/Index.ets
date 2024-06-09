
import common from '@ohos.app.ability.common';
import router from '@ohos.router';
import data_preferences from '@ohos.data.preferences';
// import Logger from '../common/utils/Logger';
import { CommonConstants as Const } from '../common/constants/CommonConstants';
import UserPrivacyDialog from '../view/dialog/UserPrivacyDialog';

// app preferences name
const H_STORE: string = 'healthAppStore';
const IS_PRIVACY: string = 'isPrivacy';

@Entry
@Component
struct SplashIndex {
  context: common.UIAbilityContext = getContext(this) as common.UIAbilityContext;
  dialogController: CustomDialogController = new CustomDialogController({
    builder: UserPrivacyDialog(
      {
        cancel: () => {
          this.exitApp()
        },
        confirm: () => {
          this.onConfirm();
          console.log("aaaaaaa1");
        }
      }),
    cancel: () => {
      this.exitApp()
    },
    autoCancel: false,
    alignment: DialogAlignment.Bottom,
    offset: { dx: 0, dy: Const.OFFSET_24 }
  });

  onConfirm() {

    this.jumpAdPage();
  }

  exitApp() {
    this.context.terminateSelf();
  }

  jumpAdPage() {
    console.log("aaaaaaa");
    setTimeout(() => {
      router.replaceUrl({ url: 'pages/AdPages' },router.RouterMode.Standard,
        (err) => {
          if (err) {
            console.error(`Invoke replaceUrl failed, code is ${err.code}, message is ${err.message}`);
            return;
          }
          console.info('Invoke replaceUrl succeeded.');
        });
    }, Const.LAUNCHER_DELAY_TIME);

    console.log("aaaaaaa3");
  }

  aboutToAppear() {
    let preferences = data_preferences.getPreferences(this.context, H_STORE);
    preferences.then((res) => {
      res.get(IS_PRIVACY, false).then((isPrivate) => {
        if (isPrivate === true) {
          this.jumpAdPage();
        } else {
          this.dialogController.open();
        }
      });
    });
  }

  aboutToDisappear() {
    clearTimeout();
  }

  build() {
    Column() {

    }
    .width('100%')
    .height('100%')
    .backgroundImagePosition({ x: 0, y: 0 })
    .backgroundImage($r('app.media.ic_splash_bg'))
    .backgroundImageSize({ width: '100%', height: '100%' })
  }
}