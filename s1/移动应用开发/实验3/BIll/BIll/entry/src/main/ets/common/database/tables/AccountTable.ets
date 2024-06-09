import relationalStore from '@ohos.data.relationalStore';
import AccountData from '../AccountData';
import { CommonConstants } from '../../constants/CommonConstants';
import Rdb from '../Rdb';

export default class AccountTable {
  private accountTable = new Rdb(CommonConstants.ACCOUNT_TABLE.tableName,
    CommonConstants.ACCOUNT_TABLE.sqlCreate,
    CommonConstants.ACCOUNT_TABLE.columns);

  constructor(callback: Function = () => {
  }) {
    this.accountTable.getRdbStore(callback);
  }

  getRdbStore(callback: Function = () => {
  }) {
    this.accountTable.getRdbStore(callback);
  }

  insertData(account: AccountData, callback: Function) {
    const valueBucket: relationalStore.ValuesBucket = generateBucket(account);
    this.accountTable.insertData(valueBucket, callback);
  }

  deleteData(account: AccountData, callback: Function) {
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
    predicates.equalTo('id', account.id);
    this.accountTable.deleteData(predicates, callback);
  }

  updateData(account: AccountData, callback: Function) {
    const valueBucket: relationalStore.ValuesBucket = generateBucket(account);
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
    predicates.equalTo('id', account.id);
    this.accountTable.updateData(predicates, valueBucket, callback);
  }

  query(amount: number, callback: Function, isAll: boolean = true) {
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
    if (!isAll) {
      predicates.equalTo('amount', amount);
    }
    this.accountTable.query(predicates, (resultSet: relationalStore.ResultSet) => {
      let count: number = resultSet.rowCount;
      if (count === 0 || typeof count === 'string') {
        console.log(`${CommonConstants.TABLE_TAG}` + 'Query no results!');
        callback([]);
      } else {
        resultSet.goToFirstRow();
        const result: AccountData[] = [];
        for (let i = 0; i < count; i++) {
          let tmp: AccountData = {
            id: 0,
            accountType: 0,
            typeText: '',
            amount: 0,
            year: 2000,
            month: 11,
            day: 11,
            note: ''
          };
          tmp.id = resultSet.getDouble(resultSet.getColumnIndex('id'));
          tmp.accountType = resultSet.getDouble(resultSet.getColumnIndex('accountType'));
          tmp.typeText = resultSet.getString(resultSet.getColumnIndex('typeText'));
          tmp.amount = resultSet.getDouble(resultSet.getColumnIndex('amount'));
          tmp.year = resultSet.getDouble(resultSet.getColumnIndex('year'));
          tmp.month = resultSet.getDouble(resultSet.getColumnIndex('month'));
          tmp.day = resultSet.getDouble(resultSet.getColumnIndex('day'));
          tmp.note =  resultSet.getString(resultSet.getColumnIndex('note'));
          result[i] = tmp;
          resultSet.goToNextRow();
        }
        callback(result);
      }
    });
  }
  queryByNote(note: String, callback: Function) {
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
      predicates.like('note', '%'+String(note)+'%');
    this.accountTable.query(predicates, (resultSet: relationalStore.ResultSet) => {
      let count: number = resultSet.rowCount;
      if (count === 0 || typeof count === 'string') {
        console.log(`${CommonConstants.TABLE_TAG}` + 'Query no results!');
        callback([]);
      } else {
        resultSet.goToFirstRow();
        const result: AccountData[] = [];
        for (let i = 0; i < count; i++) {
          let tmp: AccountData = {
            id: 0,
            accountType: 0,
            typeText: '',
            amount: 0,
            year: 2000,
            month: 11,
            day: 11,
            note: ''
          };
          tmp.id = resultSet.getDouble(resultSet.getColumnIndex('id'));
          tmp.accountType = resultSet.getDouble(resultSet.getColumnIndex('accountType'));
          tmp.typeText = resultSet.getString(resultSet.getColumnIndex('typeText'));
          tmp.amount = resultSet.getDouble(resultSet.getColumnIndex('amount'));
          tmp.year = resultSet.getDouble(resultSet.getColumnIndex('year'));
          tmp.month = resultSet.getDouble(resultSet.getColumnIndex('month'));
          tmp.day = resultSet.getDouble(resultSet.getColumnIndex('day'));
          tmp.note =  resultSet.getString(resultSet.getColumnIndex('note'));
          result[i] = tmp;
          resultSet.goToNextRow();
        }
        callback(result);
      }
    });
  }
  queryByType(type: String, callback: Function) {
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
    predicates.like('typeText', '%'+String(type)+'%');
    this.accountTable.query(predicates, (resultSet: relationalStore.ResultSet) => {
      let count: number = resultSet.rowCount;
      if (count === 0 || typeof count === 'string') {
        console.log(`${CommonConstants.TABLE_TAG}` + 'Query no results!');
        callback([]);
      } else {
        resultSet.goToFirstRow();
        const result: AccountData[] = [];
        for (let i = 0; i < count; i++) {
          let tmp: AccountData = {
            id: 0,
            accountType: 0,
            typeText: '',
            amount: 0,
            year: 2000,
            month: 11,
            day: 11,
            note: ''
          };
          tmp.id = resultSet.getDouble(resultSet.getColumnIndex('id'));
          tmp.accountType = resultSet.getDouble(resultSet.getColumnIndex('accountType'));
          tmp.typeText = resultSet.getString(resultSet.getColumnIndex('typeText'));
          tmp.amount = resultSet.getDouble(resultSet.getColumnIndex('amount'));
          tmp.year = resultSet.getDouble(resultSet.getColumnIndex('year'));
          tmp.month = resultSet.getDouble(resultSet.getColumnIndex('month'));
          tmp.day = resultSet.getDouble(resultSet.getColumnIndex('day'));
          tmp.note =  resultSet.getString(resultSet.getColumnIndex('note'));
          result[i] = tmp;
          resultSet.goToNextRow();
        }
        callback(result);
      }
    });
  }
  queryByTime(year: number,month:number,day:number,mode: number, callback: Function) {
    let predicates = new relationalStore.RdbPredicates(CommonConstants.ACCOUNT_TABLE.tableName);
    if (mode!=1) {
      if(mode == 4){
        predicates.equalTo('day', day);
        predicates.equalTo('month',month);
        predicates.equalTo('year',year)
      }
      else if(mode == 3){
        predicates.equalTo('month',month);
        predicates.equalTo('year',year);
      }
      else{
        predicates.equalTo('year',year);
      }
    }
    this.accountTable.query(predicates, (resultSet: relationalStore.ResultSet) => {
      let count: number = resultSet.rowCount;
      if (count === 0 || typeof count === 'string') {
        console.log(`${CommonConstants.TABLE_TAG}` + 'Query no results!');
        callback([]);
      } else {
        resultSet.goToFirstRow();
        const result: AccountData[] = [];
        for (let i = 0; i < count; i++) {
          let tmp: AccountData = {
            id: 0,
            accountType: 0,
            typeText: '',
            amount: 0,
            year: 2000,
            month: 11,
            day: 11,
            note: ''
          };
          tmp.id = resultSet.getDouble(resultSet.getColumnIndex('id'));
          tmp.accountType = resultSet.getDouble(resultSet.getColumnIndex('accountType'));
          tmp.typeText = resultSet.getString(resultSet.getColumnIndex('typeText'));
          tmp.amount = resultSet.getDouble(resultSet.getColumnIndex('amount'));
          tmp.year = resultSet.getDouble(resultSet.getColumnIndex('year'));
          tmp.month = resultSet.getDouble(resultSet.getColumnIndex('month'));
          tmp.day = resultSet.getDouble(resultSet.getColumnIndex('day'));
          tmp.note = resultSet.getString(resultSet.getColumnIndex('note'));
          result[i] = tmp;
          resultSet.goToNextRow();
        }
        callback(result);
      }
    });
  }
}

function generateBucket(account: AccountData): relationalStore.ValuesBucket {
  let obj: relationalStore.ValuesBucket = {};
  obj.accountType = account.accountType;
  obj.typeText = account.typeText;
  obj.amount = account.amount;
  obj.year = account.year;
  obj.month = account.month;
  obj.day = account.day;
  obj.note = account.note;

  return obj;
}