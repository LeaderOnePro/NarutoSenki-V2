/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

public class Cocos2dxHelper {
	// ===========================================================
	// Constants
	// ===========================================================
	private static final String PREFS_NAME = "Cocos2dxPrefsFile";

	// ===========================================================
	// Fields
	// ===========================================================

	private static Cocos2dxMusic sCocos2dMusic;
	private static Cocos2dxSound sCocos2dSound;
	private static AssetManager sAssetManager;
	private static Cocos2dxAccelerometer sCocos2dxAccelerometer;
	private static boolean sAccelerometerEnabled;
	private static String sPackageName;
	private static String sFileDirectory;
	private static Context sContext = null;
	private static Cocos2dxHelperListener sCocos2dxHelperListener;

	// ===========================================================
	// Constructors
	// ===========================================================

	public static void init(final Context pContext, final Cocos2dxHelperListener pCocos2dxHelperListener) {
		final ApplicationInfo applicationInfo = pContext.getApplicationInfo();
		
		Cocos2dxHelper.sContext = pContext;
		Cocos2dxHelper.sCocos2dxHelperListener = pCocos2dxHelperListener;

		Cocos2dxHelper.sPackageName = applicationInfo.packageName;
		Cocos2dxHelper.sFileDirectory = pContext.getFilesDir().getAbsolutePath();
		Cocos2dxHelper.nativeSetApkPath(applicationInfo.sourceDir);

		Cocos2dxHelper.sCocos2dxAccelerometer = new Cocos2dxAccelerometer(pContext);
		Cocos2dxHelper.sCocos2dMusic = new Cocos2dxMusic(pContext);
		int simultaneousStreams = Cocos2dxSound.MAX_SIMULTANEOUS_STREAMS_DEFAULT;
        if (Cocos2dxHelper.getDeviceModel().indexOf("GT-I9100") != -1) {
            simultaneousStreams = Cocos2dxSound.MAX_SIMULTANEOUS_STREAMS_I9100;
        }
        Cocos2dxHelper.sCocos2dSound = new Cocos2dxSound(pContext, simultaneousStreams);
		Cocos2dxHelper.sAssetManager = pContext.getAssets();
		Cocos2dxBitmap.setContext(pContext);
		Cocos2dxETCLoader.setContext(pContext);
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	// ===========================================================
	// Methods
	// ===========================================================

	private static native void nativeSetApkPath(final String pApkPath);

	private static native void nativeSetEditTextDialogResult(final byte[] pBytes);

	public static String getCocos2dxPackageName() {
		return Cocos2dxHelper.sPackageName;
	}

	public static String getCocos2dxWritablePath() {
		return Cocos2dxHelper.sFileDirectory;
	}

	public static String getCurrentLanguage() {
		return Locale.getDefault().getLanguage();
	}
	
	public static String getDeviceModel(){
		return Build.MODEL;
    }

	public static AssetManager getAssetManager() {
		return Cocos2dxHelper.sAssetManager;
	}

	public static void enableAccelerometer() {
		Cocos2dxHelper.sAccelerometerEnabled = true;
		Cocos2dxHelper.sCocos2dxAccelerometer.enable();
	}


	public static void setAccelerometerInterval(float interval) {
		Cocos2dxHelper.sCocos2dxAccelerometer.setInterval(interval);
	}

	public static void disableAccelerometer() {
		Cocos2dxHelper.sAccelerometerEnabled = false;
		Cocos2dxHelper.sCocos2dxAccelerometer.disable();
	}

	public static void preloadBackgroundMusic(final String pPath) {
		Cocos2dxHelper.sCocos2dMusic.preloadBackgroundMusic(pPath);
	}

	public static void playBackgroundMusic(final String pPath, final boolean isLoop) {
		Cocos2dxHelper.sCocos2dMusic.playBackgroundMusic(pPath, isLoop);
	}

	public static void resumeBackgroundMusic() {
		Cocos2dxHelper.sCocos2dMusic.resumeBackgroundMusic();
	}

	public static void pauseBackgroundMusic() {
		Cocos2dxHelper.sCocos2dMusic.pauseBackgroundMusic();
	}

	public static void stopBackgroundMusic() {
		Cocos2dxHelper.sCocos2dMusic.stopBackgroundMusic();
	}

	public static void rewindBackgroundMusic() {
		Cocos2dxHelper.sCocos2dMusic.rewindBackgroundMusic();
	}

	public static boolean isBackgroundMusicPlaying() {
		return Cocos2dxHelper.sCocos2dMusic.isBackgroundMusicPlaying();
	}

	public static float getBackgroundMusicVolume() {
		return Cocos2dxHelper.sCocos2dMusic.getBackgroundVolume();
	}

	public static void setBackgroundMusicVolume(final float volume) {
		Cocos2dxHelper.sCocos2dMusic.setBackgroundVolume(volume);
	}

	public static void preloadEffect(final String path) {
		Cocos2dxHelper.sCocos2dSound.preloadEffect(path);
	}

	public static int playEffect(final String path, final boolean isLoop) {
		return Cocos2dxHelper.sCocos2dSound.playEffect(path, isLoop);
	}

	public static void resumeEffect(final int soundId) {
		Cocos2dxHelper.sCocos2dSound.resumeEffect(soundId);
	}

	public static void pauseEffect(final int soundId) {
		Cocos2dxHelper.sCocos2dSound.pauseEffect(soundId);
	}

	public static void stopEffect(final int soundId) {
		Cocos2dxHelper.sCocos2dSound.stopEffect(soundId);
	}

	public static float getEffectsVolume() {
		return Cocos2dxHelper.sCocos2dSound.getEffectsVolume();
	}

	public static void setEffectsVolume(final float volume) {
		Cocos2dxHelper.sCocos2dSound.setEffectsVolume(volume);
	}

	public static void unloadEffect(final String path) {
		Cocos2dxHelper.sCocos2dSound.unloadEffect(path);
	}

	public static void pauseAllEffects() {
		Cocos2dxHelper.sCocos2dSound.pauseAllEffects();
	}

	public static void resumeAllEffects() {
		Cocos2dxHelper.sCocos2dSound.resumeAllEffects();
	}

	public static void stopAllEffects() {
		Cocos2dxHelper.sCocos2dSound.stopAllEffects();
	}

	public static void end() {
		Cocos2dxHelper.sCocos2dMusic.end();
		Cocos2dxHelper.sCocos2dSound.end();
	}

	public static void onResume() {
		if (Cocos2dxHelper.sAccelerometerEnabled) {
			Cocos2dxHelper.sCocos2dxAccelerometer.enable();
		}
	}

	public static void onPause() {
		if (Cocos2dxHelper.sAccelerometerEnabled) {
			Cocos2dxHelper.sCocos2dxAccelerometer.disable();
		}
	}

	public static void terminateProcess() {
		android.os.Process.killProcess(android.os.Process.myPid());
	}

	private static void showDialog(final String pTitle, final String pMessage) {
		Cocos2dxHelper.sCocos2dxHelperListener.showDialog(pTitle, pMessage);
	}

	private static void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) {
		Cocos2dxHelper.sCocos2dxHelperListener.showEditTextDialog(pTitle, pMessage, pInputMode, pInputFlag, pReturnType, pMaxLength);
	}

	public static void setEditTextDialogResult(final String pResult) {
		final byte[] bytesUTF8 = pResult.getBytes(StandardCharsets.UTF_8);

		Cocos2dxHelper.sCocos2dxHelperListener.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				Cocos2dxHelper.nativeSetEditTextDialogResult(bytesUTF8);
			}
		});
	}

    public static int getDPI()
    {
		if (sContext != null)
		{
			DisplayMetrics metrics = new DisplayMetrics();
			WindowManager wm = ((Activity)sContext).getWindowManager();
			if (wm != null)
			{
				Display d = wm.getDefaultDisplay();
				if (d != null)
				{
					d.getMetrics(metrics);
					return (int)(metrics.density*160.0f);
				}
			}
		}
		return -1;
    }
    
    // ===========================================================
 	// Functions for CCUserDefault
 	// ===========================================================
    
    public static boolean getBoolForKey(String key, boolean defaultValue) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	return settings.getBoolean(key, defaultValue);
    }
    
    public static int getIntegerForKey(String key, int defaultValue) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	return settings.getInt(key, defaultValue);
    }
    
    public static float getFloatForKey(String key, float defaultValue) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	return settings.getFloat(key, defaultValue);
    }
    
    public static double getDoubleForKey(String key, double defaultValue) {
    	// SharedPreferences doesn't support saving double value
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	return settings.getFloat(key, (float)defaultValue);
    }
    
    public static String getStringForKey(String key, String defaultValue) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	return settings.getString(key, defaultValue);
    }
    
    public static void setBoolForKey(String key, boolean value) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putBoolean(key, value);
    	editor.commit();
    }
    
    public static void setIntegerForKey(String key, int value) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putInt(key, value);
    	editor.commit();
    }
    
    public static void setFloatForKey(String key, float value) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putFloat(key, value);
    	editor.commit();
    }
    
    public static void setDoubleForKey(String key, double value) {
    	// SharedPreferences doesn't support recording double value
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putFloat(key, (float)value);
    	editor.commit();
    }
    
    public static void setStringForKey(String key, String value) {
    	SharedPreferences settings = sContext.getSharedPreferences(Cocos2dxHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putString(key, value);
    	editor.commit();
    }
	
	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

	public interface Cocos2dxHelperListener {
		void showDialog(final String pTitle, final String pMessage);
		void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength);

		void runOnGLThread(final Runnable pRunnable);
	}
}
