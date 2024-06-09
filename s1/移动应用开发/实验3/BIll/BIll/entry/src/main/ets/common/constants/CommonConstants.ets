/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License,Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// import CommonConstantsInfo from '../../viewmodel/CommonConstantsInfo';
import relationalStore from '@ohos.data.relationalStore';
import { AccountTable } from '../database/ConstantInterface';

export class CommonConstants {
  // static readonly RDB_NAME = { dbName: 'taskInfo.db' } as CommonConstantsInfo; // db name

  /** day info table */
  // static readonly DAY_INFO = {
  //   tableName: 'dayInfo',
  //   columns: ['date', 'targetTaskNum', 'finTaskNum']
  // } as CommonConstantsInfo
  //
  // /** global info table */
  // static readonly GLOBAL_INFO = {
  //   tableName: 'globalInfo',
  //   columns: ['id', 'firstDate', 'lastDate', 'checkInDays', 'achievements']
  // } as CommonConstantsInfo
  //
  // /** task info table */
  // static readonly TASK_INFO = {
  //   tableName: 'taskInfo',
  //   columns: [
  //     'id',
  //     'date',
  //     'taskID',
  //     'targetValue',
  //     'isAlarm',
  //     'startTime',
  //     'endTime',
  //     'frequency',
  //     'isDone',
  //     'finValue',
  //     'isOpen'
  //   ]
  // } as CommonConstantsInfo
  //
  // /** form info table */
  // static readonly FORM_INFO = {
  //   tableName: 'formInfo',
  //   columns: ['id', 'formId', 'formName', 'formDimension']
  // } as CommonConstantsInfo

  // TaskNum
  static readonly TASK_NUM = 6;

  // THOUSANDTH
  static readonly THOUSANDTH_15: string = '1.5%'; // ‘1.5%’
  static readonly THOUSANDTH_12: string = '2.2%'; // ‘2.2%’
  static readonly THOUSANDTH_33: string = '3.3%'; // ‘3.3%’
  static readonly THOUSANDTH_50: string = '5%'; // ‘5%’
  static readonly THOUSANDTH_66: string = '6.6%'; // ‘6.6%’
  static readonly THOUSANDTH_80: string = '8%'; // ‘8%’
  static readonly THOUSANDTH_100: string = '10%'; // ‘10%’
  static readonly THOUSANDTH_120: string = '12%'; // ‘12%’
  static readonly THOUSANDTH_160: string = '16%'; // ‘16%’
  static readonly THOUSANDTH_400: string = '40%'; // ‘40%’
  static readonly THOUSANDTH_420: string = '42%'; // ‘42%’
  static readonly THOUSANDTH_500: string = '50%'; // ‘50%’
  static readonly THOUSANDTH_560: string = '56%'; // ‘56%’
  static readonly THOUSANDTH_800: string = '80%'; // ‘80%’
  static readonly THOUSANDTH_830: string = '83%'; // ‘83%’
  static readonly THOUSANDTH_880: string = '88%'; // ‘88%’
  static readonly THOUSANDTH_900: string = '90%'; // ‘90%’
  static readonly THOUSANDTH_940: string = '94%'; // ‘90%’
  static readonly THOUSANDTH_1000: string = '100%'; // ‘100%’

  static readonly DEFAULT_2: number = 2;

  static readonly DEFAULT_6: number = 6;

  static readonly DEFAULT_8: number = 8;

  static readonly DEFAULT_12: number = 12;

  static readonly DEFAULT_10: number = 10;

  static readonly DEFAULT_16: number = 16;

  static readonly DEFAULT_18: number = 18;

  static readonly DEFAULT_20: number = 20;

  static readonly DEFAULT_24: number = 24;

  static readonly DEFAULT_28: number = 28;

  static readonly DEFAULT_32: number = 32;

  static readonly DEFAULT_48: number = 48;

  static readonly DEFAULT_56: number = 56;

  static readonly DEFAULT_60: number = 60;

  static readonly DEFAULT_100: number = 100;

  static readonly DEFAULT_180: number = 180;

  // fontWeight
  static readonly FONT_WEIGHT_400: number = 400;

  static readonly FONT_WEIGHT_500: number = 500;

  static readonly FONT_WEIGHT_700: number = 700;

  static readonly FONT_WEIGHT_900: number = 900;


  // opacity
  static readonly OPACITY_4: number = 0.4;

  static readonly OPACITY_6: number = 0.6;

  // radius
  static readonly BORDER_RADIUS_PERCENT_50: string = '50%';

  // duration
  static readonly AD_DURATION: number = 5; // 5s
  static readonly LAUNCHER_DELAY_TIME: number = 2000; // 2000ms
  static readonly DURATION_1000: number = 1000; // 1000ms
  static readonly DURATION_800: number = 800; // 700ms
  static readonly DURATION_100: number = 100; // 100ms


  // list space
  static readonly LIST_ITEM_SPACE: number = 2;

  static readonly SPACE_4: number = 4;

  // navigation title
  static readonly ADD_TASK_TITLE: string = '添加任务';

  static readonly EDIT_TASK_TITLE: string = '编辑任务';

  // prompt message
  static readonly SETTING_FINISHED_MESSAGE = '设置完成！！！';

  static readonly SETTING_FINISH_FAILED_MESSAGE = '网络连接错误';

  static readonly CHOOSE_TIME_OUT_RANGE: string = '选择时间超出范围';

  static readonly NICK_NAME = '李宽宇';

  static readonly SIGNATURE = '移动应用开发实验三';

  static readonly HOME_BTN_Z = 2;

  // time range
  static readonly DEFAULT_TIME: string = '08:00';

  static readonly H_STORE: string = 'BillAPPStore';

  static readonly REMINDER_AGENT_TAG: string = 'reminderAgent';

  static readonly PACKAGE_NAME: string = 'com.example.Bill';

  static readonly ENTRY_ABILITY: string = 'EntryAbility';

  // offset
  static readonly ZERO: number = 0;

  static readonly MINUS_20: number = -20;

  static readonly HAS_NO_INDEX: number = -1;

  static readonly OFFSET_24: number = -24;

  // targetSetting Range
  static readonly DRINK_STEP: number = 25;

  static readonly DRINK_MAX_RANGE: number = 500;

  static readonly TIMES_50: number = 50;

  static readonly TIMES_100: number = 100;

  static readonly EAT_APPLE_RANGE: number = 100;


  // letter spacing
  static readonly LETTER_1: number = 0.1;

  static readonly LETTER_34: number = 3.4;

  // achievement
  static readonly ACHIEVE_CARD_IMG_HEIGHT: number = 88;

  static readonly ACHIEVE_CARD_TOP: number = 38;

  static readonly ACHIEVE_CARD_BOTTOM: number = 10;

  static readonly ACHIEVE_SPLIT_RATIO: number = 1 / 3;

  static readonly ACHIEVE_TITLE_BAR_LEFT: number = 20;

  static readonly ACHIEVE_TITLE_BAR_TOP: number = 15;

  static readonly FULL_WIDTH: string = '100%';

  static readonly FULL_HEIGHT: string = '100%';

  static readonly WEEK_DAY_NUM: number = 7; // number days of one week

  static readonly WEEK_DAY_TIME: number = 7 * 24 * 60 * 60 * 1000;

  // Card Constants
  static readonly TAG = "UpdateFormUtils";
  static readonly FORM_PARAM_IDENTITY_KEY = "ohos.extra.param.key.form_identity";
  static readonly FORM_PARAM_DIMENSION_KEY = "ohos.extra.param.key.form_dimension";
  static readonly FORM_PARAM_NAME_KEY = "ohos.extra.param.key.form_name";
  static readonly DEFAULT_DIMENSION_2X2 = 2;
  static readonly DEFAULT_DIMENSION_2X4 = 3;
  static readonly WIDGET_NAME_AGENCY = "agency";
  static readonly WIDGET_NAME_PROGRESS = "progress";

  // rdb const
  static readonly STORE_CONFIG: relationalStore.StoreConfig = {
    name: 'database.db',
    securityLevel: relationalStore.SecurityLevel.S1
  };
  /**
   * Account table config.
   */
  static readonly ACCOUNT_TABLE: AccountTable = {
    tableName: 'accountTable',
    sqlCreate: 'CREATE TABLE IF NOT EXISTS accountTable(id INTEGER PRIMARY KEY AUTOINCREMENT, accountType INTEGER, ' +
    'typeText TEXT, amount INTEGER, year INTEGER,month INTEGER,day INTEGER,note TEXT)',
    columns: ['id', 'accountType', 'typeText', 'amount','year','month','day','note']
  };
  /**
   * Search text of Search component.
   */
  static readonly SEARCH_TEXT = '搜索';
  /**
   * toast text of prompt component.
   */
  static readonly TOAST_TEXT_1 = '账目类型不能为空';
  static readonly TOAST_TEXT_2 = '账目金额不为正整数';
  /**
   * Component size.
   */
  static readonly DIALOG_HEIGHT = '70%';
  static readonly TABS_HEIGHT = '45%';
  static readonly MINIMUM_SIZE = 0;
  static readonly FULL_SIZE = 1;
  static readonly PROMPT_BOTTOM = '70vp';
  /**
   * Component location.
   */
  static readonly EDIT_POSITION_X = '80%';
  static readonly EDIT_POSITION_Y = '90%';
  static readonly DELETE_POSITION_X = '50%';
  static readonly DELETE_POSITION_Y = '90%';
  static readonly FILTRATE_POSITION_X = '60%';
  static readonly FILTRATE_POSITION_Y = '90%';
  /**
   * Log tag.
   */
  static readonly RDB_TAG = '[Debug.Rdb]';
  static readonly TABLE_TAG = '[Debug.AccountTable]';
  static readonly INDEX_TAG = '[Debug.Index]';
}

export enum TaskType {
  Getup = 'getup',
  Drink= 'drink',
  Apple = 'apple',
  Smile = 'smile',
  Clean = 'clean',
  Sleep = 'sleep'
}

export enum Unit {
  Liter = 'L',
  Pcs = '个',
  Times = '次',
  Empty = ''
}
