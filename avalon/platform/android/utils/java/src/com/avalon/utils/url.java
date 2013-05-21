package com.avalon.utils;

import android.content.Intent;
import android.net.Uri;
import org.cocos2dx.lib.Cocos2dxActivity;

abstract class Url
{
    static void open(String url)
    {
        try {
            Intent i = new Intent(Intent.ACTION_VIEW);
            i.setData(Uri.parse(url));
            Cocos2dxActivity.getContext().startActivity(i);
        } catch (Exception e) {
        }
    }
}
