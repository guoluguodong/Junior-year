import AccountItem from './AccountItem';

export const PayList: Array<AccountItem> = [
  {
    icon: $r('app.media.foods'),
    iconSelected: $r('app.media.foods_selected'),
    accountType: 0,
    typeText: '吃饭'
  },
  {
    icon: $r('app.media.snacks'),
    iconSelected: $r('app.media.snacks_selected'),
    accountType: 0,
    typeText: '零食'
  },
  {
    icon: $r('app.media.fuel'),
    iconSelected: $r('app.media.fuel_selected'),
    accountType: 0,
    typeText: '书籍'
  },
  {
    icon: $r('app.media.travel'),
    iconSelected: $r('app.media.travel_selected'),
    accountType: 0,
    typeText: '旅游'
  },
  {
    icon: $r('app.media.games'),
    iconSelected: $r('app.media.games_selected'),
    accountType: 0,
    typeText: '娱乐'
  },
  {
    icon: $r('app.media.pets'),
    iconSelected: $r('app.media.pets_selected'),
    accountType: 0,
    typeText: '宠物'
  }
]

export const EarnList: Array<AccountItem> = [
  {
    icon: $r('app.media.income'),
    iconSelected: $r('app.media.income_selected'),
    accountType: 1,
    typeText: '工作收入'
  },
  {
    icon: $r('app.media.invest'),
    iconSelected: $r('app.media.invest_selected'),
    accountType: 1,
    typeText: '投资'
  },
  {
    icon: $r('app.media.income'),
    iconSelected: $r('app.media.invest_selected'),
    accountType: 1,
    typeText: '生活费'
  },
  {
    icon: $r('app.media.invest'),
    iconSelected: $r('app.media.invest_selected'),
    accountType: 1,
    typeText: '奖学金'
  }
]

export const ImageList: Record<string,Resource> = {
  '吃饭': $r('app.media.foods'),
  '零食': $r('app.media.snacks'),
  '书籍': $r('app.media.fuel'),
  '旅游': $r('app.media.travel'),
  '娱乐': $r('app.media.games'),
  '宠物': $r('app.media.pets'),
  '工作收入': $r('app.media.income'),
  '投资': $r('app.media.invest'),
  '生活费': $r('app.media.income'),
  '奖学金': $r('app.media.invest')
}