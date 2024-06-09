package com.example.myapplication.slice;// 请根据实际工程/包名引入
import com.example.myapplication.ResourceTable;
import ohos.aafwk.ability.AbilitySlice;
import ohos.agp.components.*;
import java.util.List;
public class SampleItemProvider extends BaseItemProvider {
    private List<SampleItem> list;
    private AbilitySlice slice;
    public SampleItemProvider(List<SampleItem> list, AbilitySlice slice) {
        this.list = list;
        this.slice = slice;
    }
    @Override
    public int getCount() {
        return list == null ? 0 : list.size();
    }
    @Override
    public Object getItem(int position) {
        if (list != null && position >= 0 && position < list.size()){
            return list.get(position);
        }
        return null;
    }
    @Override
    public long getItemId(int position) {
        return position;
    }
    @Override
    public Component getComponent(int position, Component convertComponent, ComponentContainer componentContainer) {
        final Component cpt;
        if (convertComponent == null) {
            cpt = LayoutScatter.getInstance(slice).parse(ResourceTable.Layout_item_sample, null, false);
        } else {
            cpt = convertComponent;
        }
        SampleItem sampleItem = list.get(position);
        Text text = (Text) cpt.findComponentById(ResourceTable.Id_item_index);
        text.setText(sampleItem.getName());
        return cpt;
    }
}