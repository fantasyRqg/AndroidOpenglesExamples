package rqg.fantasy.nativeview;

import android.app.Activity;
import android.content.res.Configuration;
import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.util.Log;

import rqg.fantasy.nativeview.databinding.ActivityMainBinding;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";

    ActivityMainBinding mBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.d(TAG, "onConfigurationChanged() called with: newConfig = [" + newConfig + "]");
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume() called " + mBinding.glesView);
        super.onResume();
        mBinding.glesView.onResume();
    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause() called");
        super.onPause();

        mBinding.glesView.onPause();
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy() called");
        super.onDestroy();

    }
}
