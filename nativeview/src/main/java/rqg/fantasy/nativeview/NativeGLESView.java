package rqg.fantasy.nativeview;

import android.content.Context;
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
        nativeSurfaceCreated(holder.getSurface());
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


    private native void nativeOnResume();

    private native void nativeOnPause();

    private native void nativeSurfaceCreated(Surface surface);

    private native void nativeSurfaceChanged(int format, int width, int height);

    private native void nativeSurfaceDestroyed();

    private native void nativeSurfaceRedrawNeeded();

    private static native void nativeClassInit();

    static {
        System.loadLibrary("nativeGlesView");
        nativeClassInit();
    }
}
