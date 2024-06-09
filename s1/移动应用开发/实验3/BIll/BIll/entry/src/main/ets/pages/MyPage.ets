import { ListInfo } from '../view/ListInfo';
import { UserBaseInfo } from '../view/UserBaseInfo';
import { CommonConstants as Const } from '../common/constants/CommonConstants';

@Component
export struct MineIndex {
  @State nickname: string = Const.NICK_NAME;
  @State signature: string = Const.SIGNATURE;

  build() {
    Column() {
      UserBaseInfo({
        nickname: this.nickname,
        signature: this.signature
      });

      ListInfo();
    }
    .height(Const.FULL_HEIGHT)
    .backgroundColor($r('app.color.mineBgColor'))
  }
}