package com.avalon.utils;

import android.content.Intent;
import android.net.Uri;
import org.cocos2dx.lib.Cocos2dxHelper;

abstract class Url
{
    static void open(String url)
    {
        try {
            Intent i = new Intent(Intent.ACTION_VIEW);
            i.setData(Uri.parse(url));
            Cocos2dxHelper.getActivity().startActivity(i);
        } catch (Exception e) {
        }
    }
}
