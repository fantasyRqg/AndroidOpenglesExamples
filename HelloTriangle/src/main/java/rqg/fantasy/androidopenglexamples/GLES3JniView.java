package rqg.fantasy.androidopenglexamples;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

import java.lang.ref.WeakReference;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * *Created by rqg on 3/8/17 5:55 PM.
 */

public class GLES3JniView extends GLSurfaceView {
    private static final String TAG = "GLES3JniView";
    /**
     * These fields are used by native code, do not access or modify.
     */
    private long mJniRenderer;

    public GLES3JniView(Context context) {
        this(context, null);
    }

    public GLES3JniView(Context context, AttributeSet attrs) {
        super(context, attrs);

        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);

        setRenderer(new Renderer(new WeakReference<GLES3JniView>(this)));

        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);


    }


    public void onDestroy() {
        destroy();
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        WeakReference<GLES3JniView> mThiz;

        public Renderer(WeakReference<GLES3JniView> thiz) {
            mThiz = thiz;
        }


        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES3JniView thiz = mThiz.get();
            if (thiz != null) thiz.init();

        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Log.d(TAG, "onSurfaceChanged() called with: gl = [" + gl + "], width = [" + width + "], height = [" + height + "]");

            GLES3JniView thiz = mThiz.get();
            if (thiz != null) thiz.resize(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            GLES3JniView thiz = mThiz.get();
            if (thiz != null) thiz.step();
        }
    }


    private native void init();

    private native void step();

    private native void resize(int width, int height);

    private native void destroy();

    private static native void nativeClassInit();

    static {
        System.loadLibrary("glesView");
        nativeClassInit();
    }


}
