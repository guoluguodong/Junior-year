import {ScrollData} from './ScrollData';
export class GroupScroll {
  title:String='';
  scrollData = [];
  statisticData:number[]=[];
  constructor(title:String, scrollData:ScrollData[]) {
    this.title=title;
    this.scrollData=scrollData;
    this.StatisticalIncomeExpenditurePart();
  }
  StatisticalIncomeExpenditurePart(scroll_acc:ScrollData[]=this.scrollData) {
    let income = 0;
    let outcome = 0;
    for (let i = 0;i < scroll_acc.length; ++i) {
      if (scroll_acc[i].accountData.accountType == 0) {
        outcome = outcome - scroll_acc[i].accountData.amount;
      }
      else {
        income = income + scroll_acc[i].accountData.amount;
      }
    }
    let total = income + outcome;
    this.statisticData =  [income,outcome,total];
  }
}