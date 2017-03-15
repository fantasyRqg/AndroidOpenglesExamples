package rqg.fantasy.nativeview;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * *Created by rqg on 3/9/17 6:21 PM.
 */

public class NativeGLESView extends SurfaceView implements SurfaceHolder.Callback2 {

    private static final String TAG = "NativeGLESView";

    /**
     * These fields are used by native code, do not access or modify.
     */
    private long mGlThreadPointer;
    private long mEglWrapperPointer;


    //in case of garbage collection cause native AssetManager crash
    private AssetManager mAMgr;

    public NativeGLESView(Context context) {
        super(context);
        init();
    }

    public NativeGLESView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();

    }

    public NativeGLESView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        getHolder().addCallback(this);

        mAMgr = getContext().getAssets();
    }

    public void onResume() {
        nativeOnResume();
    }

    public void onPause() {
        nativeOnPause();
    }


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        nativeSurfaceCreated(holder.getSurface(), mAMgr);

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        nativeSurfaceChanged(format, width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        nativeSurfaceDestroyed();
    }

    @Override
    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        nativeSurfaceRedrawNeeded();
    }

    @SuppressWarnings("all")
    private native void nativeOnResume();

    @SuppressWarnings("all")
    private native void nativeOnPause();

    @SuppressWarnings("all")
    private native void nativeSurfaceCreated(Surface surface, AssetManager assetManager);

    @SuppressWarnings("all")
    private native void nativeSurfaceChanged(int format, int width, int height);

    @SuppressWarnings("all")
    private native void nativeSurfaceDestroyed();

    @SuppressWarnings("all")
    private native void nativeSurfaceRedrawNeeded();


    @SuppressWarnings("all")
    private static native void nativeClassInit();

    static {
        System.loadLibrary("nativeGlesView");
        nativeClassInit();
    }
}
