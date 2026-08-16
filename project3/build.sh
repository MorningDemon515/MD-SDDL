#!/bin/bash

# ==============================================================================
# 用户自定义路径配置 (请修改为你电脑上的实际绝对路径)
# ==============================================================================
# 1. Android SDK 的目录
SDK_PATH="${SDK_PATH:-/media/morningdemon/299D817A2D97AD94/android-sdk}"

# 2. Android Studio 的安装根目录 (例如 /media/xxx/android-studio)
AS_PATH="${AS_PATH:-/media/morningdemon/299D817A2D97AD94/android-studio}"
# ==============================================================================

# 检查 SDK 路径
if [ ! -d "$SDK_PATH" ]; then
    echo "❌ 找不到 Android SDK，请检查 SDK_PATH 路径是否正确: $SDK_PATH"
    exit 1
fi

# 检查并定位 Android Studio 中的 javac (兼容 jbr 和 jre)
JAVAC_CMD=""
if [ -f "$AS_PATH/jbr/bin/javac" ]; then
    JAVAC_CMD="$AS_PATH/jbr/bin/javac"
elif [ -f "$AS_PATH/jre/bin/javac" ]; then
    JAVAC_CMD="$AS_PATH/jre/bin/javac"
elif [ -f "$AS_PATH/bin/javac" ]; then
    JAVAC_CMD="$AS_PATH/bin/javac"
fi

if [ -z "$JAVAC_CMD" ]; then
    echo "❌ 无法在 Android Studio 路径中找到 javac，请检查 AS_PATH 路径是否正确: $AS_PATH"
    exit 1
fi

# 自动获取最新的 build-tools 和 platform
BUILD_TOOLS=$(ls "$SDK_PATH/build-tools" 2>/dev/null | sort -rV | head -n 1)
PLATFORM=$(ls "$SDK_PATH/platforms" 2>/dev/null | grep "android-" | sort -rV | head -n 1)

D8_TOOL="$SDK_PATH/build-tools/$BUILD_TOOLS/d8"
ANDROID_JAR="$SDK_PATH/platforms/$PLATFORM/android.jar"

if [ ! -f "$D8_TOOL" ] || [ ! -f "$ANDROID_JAR" ]; then
    echo "❌ 无法在 SDK 中找到 d8 或 android.jar，请检查 build-tools 和 platforms 安装。"
    exit 1
fi

echo "=== 编译环境信息 ==="
echo "SDK 路径:      $SDK_PATH"
echo "AS 路径:       $AS_PATH"
echo "使用 Javac:    $JAVAC_CMD"
echo "使用 D8 工具:   $D8_TOOL"
echo "使用 Android:  $ANDROID_JAR"
echo "===================="

# 1. 使用 AS 自带的 javac 编译 Java 文件
echo "[1/2] 正在编译 .java 为 .class ..."
"$JAVAC_CMD" -source 1.8 -target 1.8 -cp "$ANDROID_JAR" src/com/snow/SnowflakeApp.java

if [ $? -ne 0 ]; then
    echo "❌ 编译 Java 失败！"
    exit 1
fi

# 2. 使用 d8 打包成 dex
echo "[2/2] 正在将 .class 转换为 classes.dex ..."
"$D8_TOOL" --lib "$ANDROID_JAR" --output . src/com/snow/*.class

if [ $? -eq 0 ]; then
    echo "✅ 构建成功！已生成: classes.dex"
else
    echo "❌ 构建 Dex 失败！"
fi
