package rqg.fantasy.androidopenglexamples;

import android.app.Activity;
import android.content.res.Configuration;
import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.util.Log;

import rqg.fantasy.androidopenglexamples.databinding.ActivityMainBinding;


public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";

    ActivityMainBinding mActivityMainBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mActivityMainBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume: " + mActivityMainBinding.glViewA.hashCode() + " , " + mActivityMainBinding.glViewB.hashCode());
        super.onResume();

    }


    @Override
    protected void onPause() {
        super.onPause();


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }
}
