package net.linuxoids.ar;

public class MyNativeActivity extends android.app.NativeActivity {
  static {
    System.loadLibrary ("gnustl_shared");
    System.loadLibrary ("png");
  }
}
