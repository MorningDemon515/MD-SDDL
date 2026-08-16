package com.snow;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;

import java.lang.reflect.Method;
import java.util.Random;

public class SnowflakeApp {
    
    public static void main(String[] args) {
        Looper.prepare();
        try {
            startUI();
            System.out.println("=========================================");
            System.out.println(" 雪花已启动！在终端中按下 [Ctrl+C] 即可停止。");
            System.out.println("=========================================");
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
        // 启动消息循环，保持进程运行
        Looper.loop();
    }

    private static void startUI() throws Exception {
        // 1. 通过反射获取系统级 Context
        Class<?> activityThreadClass = Class.forName("android.app.ActivityThread");
        Method systemMainMethod = activityThreadClass.getMethod("systemMain");
        Object activityThread = systemMainMethod.invoke(null);
        Method getSystemContextMethod = activityThreadClass.getMethod("getSystemContext");
        Context context = (Context) getSystemContextMethod.invoke(activityThread);

        // 2. 获取 WindowManager
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        
        // 3. 配置悬浮窗参数
        int type = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O 
                ? WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY 
                : WindowManager.LayoutParams.TYPE_SYSTEM_ERROR;

        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.MATCH_PARENT,
                WindowManager.LayoutParams.MATCH_PARENT,
                type,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
                        | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE // 不拦截任何触摸事件
                        | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
                        | WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED,
                PixelFormat.TRANSLUCENT
        );
        params.gravity = Gravity.TOP | Gravity.LEFT;

        // 4. 添加自定义绘制的雪花 View
        SnowView snowView = new SnowView(context);
        wm.addView(snowView, params);
    }

    /**
     * 自定义的雪花视图
     */
    private static class SnowView extends View {
        private static final int FLAKE_COUNT = 80; // 雪花数量
        private final Flake[] flakes = new Flake[FLAKE_COUNT];
        private final Paint paint;
        private final String[] chars = {"❄", "❅", "❆", "*", "。"};

        public SnowView(Context context) {
            super(context);
            paint = new Paint(Paint.ANTI_ALIAS_FLAG);
            paint.setColor(Color.WHITE);
            // 加上黑色阴影，保证在白色背景的应用上也能看清
            paint.setShadowLayer(5, 0, 0, Color.BLACK);

            // 初始化雪花
            DisplayMetrics metrics = context.getResources().getDisplayMetrics();
            Random random = new Random();
            for (int i = 0; i < FLAKE_COUNT; i++) {
                flakes[i] = new Flake(metrics.widthPixels, metrics.heightPixels, random, chars);
            }
        }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            int height = getHeight();
            for (Flake flake : flakes) {
                paint.setTextSize(flake.size);
                canvas.drawText(flake.text, flake.x, flake.y, paint);
                flake.y += flake.speed;
                // 雪花落到底部后重置到顶部
                if (flake.y > height + flake.size) {
                    flake.y = -flake.size;
                }
            }
            // 每 16ms 刷新一次，大概 60 FPS
            postInvalidateDelayed(16); 
        }

        private static class Flake {
            float x, y, speed, size;
            String text;
            Flake(int width, int height, Random random, String[] chars) {
                x = random.nextInt(width);
                y = random.nextInt(height);
                speed = 2 + random.nextFloat() * 6; // 下落速度
                size = 30 + random.nextInt(50); // 字体大小
                text = chars[random.nextInt(chars.length)]; // 随机雪花样式
            }
        }
    }
}
