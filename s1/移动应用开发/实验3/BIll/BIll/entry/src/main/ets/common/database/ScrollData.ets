import AccountData from './AccountData';
export class ScrollData {
  accountData: AccountData;
  scroller:Scroller;

  constructor(accountData: AccountData,scroller:Scroller) {
    this.accountData = accountData;
    this.scroller = scroller;
  }



}