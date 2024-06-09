export interface NavItem {
  icon: Resource;
  icon_selected: Resource;
  text: Resource;
  id: number;
}

// tabId
export enum TabId {
  HOME,
  MINE
}

export const NavList: NavItem[] = [
  {
    icon: $r('app.media.ic_tabs_home_normal'),
    icon_selected: $r('app.media.ic_tabs_home_sel'),
    text: $r('app.string.tab_home'),
    id: TabId.HOME
  },
  {
    icon: $r('app.media.ic_tabs_mine_normal'),
    icon_selected: $r('app.media.ic_tabs_mine_sel'),
    text: $r('app.string.tab_mine'),
    id: TabId.MINE
  }
]