package rqg.fantasy.androidopenglexamples;

import android.app.Activity;
import android.databinding.DataBindingUtil;
import android.os.Bundle;

import rqg.fantasy.androidopenglexamples.databinding.ActivityMainBinding;

public class MainActivity extends Activity {

    ActivityMainBinding mActivityMainBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mActivityMainBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);

    }


    @Override
    protected void onResume() {
        super.onResume();

        mActivityMainBinding.gles1.onResume();
        mActivityMainBinding.gles2.onResume();
    }


    @Override
    protected void onPause() {
        super.onPause();
        mActivityMainBinding.gles1.onPause();

        mActivityMainBinding.gles2.onPause();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mActivityMainBinding.gles1.onDestroy();
        mActivityMainBinding.gles2.onDestroy();
    }
}
