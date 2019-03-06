package com.cylan.compress;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("compress-lib");
    }

    static final String targetFIle = Environment.getExternalStorageDirectory().getAbsolutePath() + "/target.jpg";
    static final String generateFIle = Environment.getExternalStorageDirectory().getAbsolutePath() + "/result.jpg";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    public void compressbegin(View view) {
        Log.e("JNI_LOG",targetFIle);
        Log.e("JNI_LOG",generateFIle);
        compress(BitmapFactory.decodeFile(targetFIle), generateFIle, 60);
    }


    public native boolean compress(Bitmap bitmap, String fileName, int qulit);
}
